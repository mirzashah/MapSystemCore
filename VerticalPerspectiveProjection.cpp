#include <VerticalPerspectiveProjection.h>
#include <MapSystemEnumerations.h>
#include <MathFunctions.h>
#include <World3DModel.h>

/****************************************************/
CVerticalPerspectiveProjection::CVerticalPerspectiveProjection()
/****************************************************/
{
	SetHeightOfCenterAboveSeaLevel(0);
	SetHeightOfPotentialProjectedPointsAboveSeaLevel(0);
	SetHeightOfPerspectivePoint(70000000);
}


/****************************************************/
CVerticalPerspectiveProjection::~CVerticalPerspectiveProjection()
/****************************************************/
{
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_C(const CLatLongPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double h = HeightOfPotentialProjectedPointsAboveSeaLevel();
	double phi = toRad(p.Lat());
	double N = Calculate_N(phi, worldModel);
	double C = ((N+h)/a)*cos(phi);
	return(C);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_K(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double C = Calculate_C(p, worldModel);
	double H = HeightOfPerspectivePoint();
	double S = Calculate_S(p, worldModel);
	double P = 0.0;
	double phiG = 0.0;
	CalculateBoth_phiG_AND_P(phiG, P, worldModel);
	double phi1 = toRad(OriginParallel());
	double lambda        = toRad(p.Long());
	double lambda0       = toRad(this->OriginMeridian());

	double K = H /(P*cos(phi1-phiG)-S*sin(phi1)-C*cos(phi1)*cos(lambda-lambda0));
	return (K);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_N(double phi, const CWorld3DModel& worldModel) const
/****************************************************/
{

	double a = worldModel.SemiMajorAxis();
	double eSquared = worldModel.EccentricitySquared();

	double N = a / sqrt((1-eSquared*pow(sin(phi),2)));
	return (N);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_P_SingleIteration(double phiG, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double phi1 = toRad(OriginParallel());
	double H = HeightOfPerspectivePoint();
	double N1 = Calculate_N(phi1, worldModel);
	double a = worldModel.SemiMajorAxis();
	double h0 = HeightOfCenterAboveSeaLevel();
	double P = (cos(phi1)/cos(phiG))*(H+N1+h0)/a;
	return (P);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_phiG_SingleIteration(double P, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double phi1 = toRad(OriginParallel());
	double N1 = Calculate_N(phi1, worldModel);
	double eSquared = worldModel.EccentricitySquared();
	double phiG = phi1 - asin(N1*eSquared*sin(phi1)*cos(phi1)/(P*a));
	return (phiG);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_S(const CLatLongPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double phi = toRad(p.Lat());
	double eSquared = worldModel.EccentricitySquared();
	double N = Calculate_N(phi, worldModel);
	double h = HeightOfPotentialProjectedPointsAboveSeaLevel();

	double S = ((N*(1-eSquared)+h)/a)*sin(phi);
	return (S);
}

/****************************************************/
void CVerticalPerspectiveProjection::CalculateBoth_phiG_AND_P(double& phiG, double& P, const CWorld3DModel& worldModel)
/****************************************************/
{
	//NOTE: Both must be calculated at same time as iterative process with both variables
	//dependent on each other
	double convergenceFactor = 0.00000004;
	double phiGLast = 0.0;
	double phi1 = toRad(OriginParallel());
	phiG = phi1; //First trial phiG

	while ((abs(phiG-phiGLast)>convergenceFactor))
	{
		phiGLast = phiG;
		P = Calculate_P_SingleIteration(phiG, worldModel);
		phiG = Calculate_phiG_SingleIteration(P, worldModel);
	}
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_lambda(double X, double Y, const CWorld3DModel& worldModel)
/****************************************************/
{
	double lambda0 = toRad(OriginMeridian());
	double lambda = lambda0 + atan2(Y,X);
	return (lambda);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_phi(double phiLast, double S, const CWorld3DModel& worldModel)
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double h = HeightOfPotentialProjectedPointsAboveSeaLevel();
	double sinSquarePhi = pow(sin(phiLast),2.0) ;
	double eSquared = worldModel.EccentricitySquared();
	double phi = asin(S/((1-eSquared)/(sqrt((1-eSquared*sinSquarePhi))) + h/a));
	return (phi);
}

/****************************************************/
void CVerticalPerspectiveProjection::CalculateBoth_phi_AND_lambda(double& phi, double& lambda, const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	phi = 0;
	lambda = 0;
	double E = 1; //First Trial E = 1
	double t = Calculate_t(E, p, worldModel);
	double u = Calculate_u(p, worldModel);
	double v = Calculate_v(p, worldModel);
	double KPrime = Calculate_KPrime(u, v, t, p, worldModel);
	double X = Calculate_X(KPrime, p, worldModel);
	double Y = Calculate_Y(KPrime, p, worldModel);
	double S = Calculate_S_Inverse(KPrime, p, worldModel);
	double phiLast = 0;
	double lambdaLast = 0;
	lambda = Calculate_lambda(X,Y, worldModel);
	phi = asin(S); //First trial

	double convergenceFactor = 0.00004;

	while ((abs(phi-phiLast)>convergenceFactor)||(abs(lambda-lambdaLast)>convergenceFactor))
	{
		phiLast = phi;
		lambdaLast = lambda;
		phi = Calculate_phi(phiLast, S, worldModel);
		E = Calculate_E(phi, worldModel);
	    t = Calculate_t(E, p, worldModel);
		u = Calculate_u(p, worldModel);
		v = Calculate_v(p, worldModel);
		KPrime = Calculate_KPrime(u, v, t, p, worldModel);
		X = Calculate_X(KPrime, p, worldModel);
		Y = Calculate_Y(KPrime, p, worldModel);
		S = Calculate_S_Inverse(KPrime, p, worldModel);
		lambda = Calculate_lambda(X,Y, worldModel);
	}

	//TODO: Something wrong with inverse equation, this is my quick fix hack
	if (lambda<toRad(-180))
		lambda+=toRad(360);
	if (lambda>toRad(180))
		lambda-=toRad(360);
	//END QUICK HACK ;)
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_E(double phi, const CWorld3DModel& worldModel)
/****************************************************/
{
	double h = HeightOfPotentialProjectedPointsAboveSeaLevel();

/*	if (HeightOfPotentialProjectedPointsAboveSeaLevel()==0)
		return h;*/ //Uncomment after testing of this part

	double a = worldModel.SemiMajorAxis();
	double sinSquarePhi = pow(sin(phi),2.0) ;
	double eSquared = worldModel.EccentricitySquared();

	double E = (pow((1/(sqrt(1-eSquared*sinSquarePhi))) + h/a, 2.0)) - eSquared*sinSquarePhi*(1/(1-eSquared*sinSquarePhi)-h*h/(a*a-a*a*eSquared));
	return(E);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_X(double KPrime, const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double B = Calculate_B(p, worldModel);
	double H = HeightOfPerspectivePoint();
	double phi1          = toRad(OriginParallel());
	double D = Calculate_D(p, worldModel);
	double y = p.Y();

	double X = a*((B-H/KPrime)*cos(phi1)-(y/KPrime-D)*sin(phi1));
	return X;
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_B(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi1 = toRad(OriginParallel());
	double phiG, P;
	CalculateBoth_phiG_AND_P(phiG, P, worldModel);

	double B = P*cos(phi1-phiG);
	return(B);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_D(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi1 = toRad(OriginParallel());
	double phiG, P;
	CalculateBoth_phiG_AND_P(phiG, P, worldModel);

	double D = P*sin(phi1-phiG);
	return(D);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_KPrime(double u, double v, double t, const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double KPrime = (-u+(sqrt(u*u-4*t*v)))/ (2*t);
	return (KPrime);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_u(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double B = Calculate_B(p, worldModel);
	double L = Calculate_L(worldModel);
	double J = Calculate_J(worldModel);
	double H = HeightOfPerspectivePoint();
	double y = p.Y();
	double G = Calculate_G(worldModel);
	double D = Calculate_D(p, worldModel);

	double u = -2*B*L*H - 2*D*G*y + B*J*y + D*H*J;
	return (u);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_v(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double L = Calculate_L(worldModel);
	double H =  HeightOfPerspectivePoint();
	double G = Calculate_G(worldModel);
	double x = p.X();
	double y = p.Y();
	double eSquared = worldModel.EccentricitySquared();
	double J = Calculate_J(worldModel);
	double v = L*H*H + G*y*y - H*J*y + (1-eSquared)*x*x;
	return v;
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_G(const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi1 = toRad(OriginParallel());
	double eSquared = worldModel.EccentricitySquared();

	double G = 1 - eSquared*(pow(sin(phi1),2.0));
	return(G);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_L(const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi1 = toRad(OriginParallel());
	double eSquared = worldModel.EccentricitySquared();
	double L = 1 - eSquared*(pow(cos(phi1),2.0));
	return(L);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_J(const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi1 = toRad(OriginParallel());
	double eSquared = worldModel.EccentricitySquared();
	double J = 2*eSquared*sin(phi1)*cos(phi1);
	return(J);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_S_Inverse(double KPrime, const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double y = p.Y();
	double D = Calculate_D(p, worldModel);
	double B = Calculate_B(p, worldModel);
	double phi1 = toRad(OriginParallel());
	double H =  HeightOfPerspectivePoint();

	double S = (y/KPrime-D)*cos(phi1) + (B-H/KPrime)*sin(phi1);
	return(S);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_t(double E, const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double P, phiG;
	CalculateBoth_phiG_AND_P(phiG, P, worldModel);
	double eSquared = worldModel.EccentricitySquared();

	double t = P*P*(1-eSquared*pow(cos(phiG),2.0))-E*(1-eSquared);
	return (t);
}

/****************************************************/
double CVerticalPerspectiveProjection::Calculate_Y(double KPrime, const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double x = p.X();
	double Y = a*x/KPrime;

	return (Y);
}

/****************************************************/
bool CVerticalPerspectiveProjection::IsPointPlottable(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi = toRad(p.Lat());
	double phi1 = toRad(OriginParallel());
	double lambda = toRad(p.Long());
	double lambda0 = toRad(OriginMeridian());

	double cosc = sin(phi1)*sin(phi) + cos(phi1)*cos(phi)*cos(lambda-lambda0);

	double P = 0.0;
	double phiG = 0.0;
	CalculateBoth_phiG_AND_P(phiG, P, worldModel);

	if (cosc < (1/P))
		return false;
	else
		return true;
}

/****************************************************/
double CVerticalPerspectiveProjection::HeightOfCenterAboveSeaLevel() const
/****************************************************/
{
	return (_heightOfCenterAboveSeaLevel);
}

/****************************************************/
double CVerticalPerspectiveProjection::HeightOfPerspectivePoint() const
/****************************************************/
{
	return (_heightOfPerspectivePoint);
}

/****************************************************/
double CVerticalPerspectiveProjection::HeightOfPotentialProjectedPointsAboveSeaLevel() const
/****************************************************/
{
	return (_heightofPotentialProjectedPointsAboveSeaLevel);
}

/****************************************************/
CLatLongPoint CVerticalPerspectiveProjection::InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Vertical Perspective Inverse Projection (from J. Snyder - Map Projections: A Working Manual)
	//TODO: Implement

	double lambda, phi;
	this->CalculateBoth_phi_AND_lambda(phi, lambda, p, worldModel);
	return (CLatLongPoint(toDeg(phi), toDeg(lambda)));
}


/****************************************************/
CXYPoint CVerticalPerspectiveProjection::ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Vertical Perspective Projection (from J. Snyder - Map Projections: A Working Manual)

	const double phi           = toRad(p.Lat());
	const double phi1          = toRad(OriginParallel()); //NOTE: I found this strange, phi1 refers to origin parallel, but no phi0, whhich is usual notation
	const double lambda        = toRad(p.Long());
	const double lambda0       = toRad(this->OriginMeridian());

	double K = Calculate_K(p, worldModel);
	double C = Calculate_C(p, worldModel);
	double P = 0.0;
	double phiG = 0.0;
	this->CalculateBoth_phiG_AND_P(phiG, P, worldModel);
	double S = Calculate_S(p, worldModel);

	double x = K * C * sin(lambda-lambda0);
	double y = K*(P*sin(phi1-phiG)+S*cos(phi1)-C*sin(phi1)*cos(lambda-lambda0));

	CXYPoint toReturn(x,y);
	return (toReturn);
}

/****************************************************/
double CVerticalPerspectiveProjection::ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//TODO: Implement
	double k = 1;
	return (k);
}

/****************************************************/
void CVerticalPerspectiveProjection::SetHeightOfCenterAboveSeaLevel(double meters)
/****************************************************/
{
	_heightOfCenterAboveSeaLevel = meters;
}

/****************************************************/
void CVerticalPerspectiveProjection::SetHeightOfPerspectivePoint(double meters)
/****************************************************/
{
	_heightOfPerspectivePoint = meters;
}

/****************************************************/
void CVerticalPerspectiveProjection::SetHeightOfPotentialProjectedPointsAboveSeaLevel(double meters)
/****************************************************/
{
	_heightofPotentialProjectedPointsAboveSeaLevel = meters;
}

/****************************************************/
PROJECTED_MODEL_TYPE CVerticalPerspectiveProjection::WhichProjectionAmI() const
/****************************************************/
{
	return (VERTICAL_PERSPECTIVE);
}
