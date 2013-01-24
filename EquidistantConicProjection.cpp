#include <EquidistantConicProjection.h>
#include <MapSystemEnumerations.h>
#include <MathFunctions.h>
#include <World3DModel.h>

/****************************************************/
CEquidistantConicProjection::CEquidistantConicProjection()
/****************************************************/
{
}

/****************************************************/
CEquidistantConicProjection::~CEquidistantConicProjection()
/****************************************************/
{
}

/****************************************************/
double CEquidistantConicProjection::Calculate_e1(const CWorld3DModel& worldModel) const
/****************************************************/
{
	double eSquared = worldModel.EccentricitySquared();
	double e1 = (1 - (sqrt(1-eSquared)))/(1+sqrt(1-eSquared));
	return (e1);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_G(const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();

	double phi1 = toRad(FirstStandardParallel());
	double phi2 = toRad(SecondStandardParallel());

	double m1 = Calculate_m(phi1, worldModel);
	double M1 = Calculate_M(phi1, worldModel);
	double n = Calculate_n(worldModel);

	double G = m1/n + M1/a;
	return(G);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_m(double phi, const CWorld3DModel& worldModel) const 
/****************************************************/
{
	double eSquared = worldModel.EccentricitySquared();
	double m = cos(phi)/(sqrt((1-eSquared*pow(sin(phi),2.0))));
	return (m);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_M(double phi, const CWorld3DModel& worldModel) const
/****************************************************/
{
	//TODO: Expand power series if necessary...

	double a = worldModel.SemiMajorAxis();

	double e2 = worldModel.EccentricitySquared();
	double e4 = pow(e2,2.0);
	double e6 = pow(e2,3.0);

	double Mterm1 = (1 - e2/4 - 3*e4/64 - 5*e6/256)*phi;
	double Mterm2 = (3*e2/8 + 3*e4/32 + 45*e6/1024)*sin(2*phi);
	double Mterm3 = (15*e4/256 + 45*e6/1024)*sin(4*phi);
	double Mterm4 = (35*e6/3072)*sin(6*phi);

	double M = a*(Mterm1 - Mterm2 + Mterm3 - Mterm4);

	return (M);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_M_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double G = Calculate_G(worldModel);
	double rho = Calculate_rho_Inverse(p, worldModel);
	double M = a*G - rho;

	return(M);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_mu(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double M = Calculate_M_Inverse(p, worldModel);
	double e2 = worldModel.EccentricitySquared();
	double e4 = pow(e2,2.0);
	double e6 = pow(e2,3.0);

	double mu = M/(a*(1 - e2/4 - 3*e4/64 - 5*e6/256));
	return (mu);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_n(const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();

	double phi1 = toRad(FirstStandardParallel());
	double phi2 = toRad(SecondStandardParallel());

	double m1 = Calculate_m(phi1, worldModel);
	double M1 = Calculate_M(phi1, worldModel);
	double m2 = Calculate_m(phi2, worldModel);
	double M2 = Calculate_M(phi2, worldModel);

	double n = a*(m1-m2)/(M2-M1);
	return (n);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_rho(double M, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double G = Calculate_G(worldModel);

	double rho = a*G-M;
	return (rho);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_rho_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double n = Calculate_n(worldModel);
	double sign = n/abs(n);
	double phi0 = toRad(OriginParallel());
	double M0 = Calculate_M(phi0, worldModel);
	double rho0 = Calculate_rho(M0, worldModel);
	double x = p.X();
	double y = p.Y();


	double rho = sqrt((pow(x,2.0)+pow(rho0-y, 2)));
	rho = abs(rho) * sign; //Assign appropriate sign

	return(rho);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_theta(const CLatLongPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double n = Calculate_n(worldModel);
	double lambda = toRad(p.Long());
	double lambda0 = toRad(OriginMeridian());

	double theta = n*(lambda-lambda0);
	return (theta);
}

/****************************************************/
double CEquidistantConicProjection::Calculate_theta_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double x = p.X();
	double y = p.Y();
	
	double phi0 = toRad(OriginParallel());
	double M0 = Calculate_M(phi0, worldModel);
	double rho0 = Calculate_rho(M0, worldModel);
	double theta = atan((x/(rho0-y)));

	return (theta);
}

/****************************************************/
CLatLongPoint CEquidistantConicProjection::InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Equidistant Conic Inverse Projection (from J. Snyder - Map Projections: A Working Manual)

	double mu = Calculate_mu(p, worldModel);
	double e1 = Calculate_e1(worldModel);
	double n = Calculate_n(worldModel);
	double lambda0 = toRad(OriginMeridian());
	double theta = Calculate_theta_Inverse(p, worldModel);

	double phiTerm1 = (3*e1/2 - 27*pow(e1,3)/32)*sin(2*mu);
	double phiTerm2 = (21*pow(e1,2)/16 - 55*pow(e1,4)/32) * sin(4*mu);
	double phiTerm3 = (151*pow(e1,3)/96)*sin(6*mu);
	double phiTerm4 = (1097*pow(e1,4)/512)*sin(8*mu);

	double phi = mu + phiTerm1 + phiTerm2 + phiTerm3 + phiTerm4;
	double lambda = lambda0 + theta/n;

	return (CLatLongPoint(toDeg(phi), toDeg(lambda)));
}

/****************************************************/
CXYPoint CEquidistantConicProjection::ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Equidistant Conic Projection (from J. Snyder - Map Projections: A Working Manual)
	double phi = toRad(p.Lat());
	double phi0 = toRad(OriginParallel());
	double M = Calculate_M(phi, worldModel);
	double M0 = Calculate_M(phi0, worldModel);

	double rho = Calculate_rho(M, worldModel);
	double theta = Calculate_theta(p, worldModel);
	double rho0 = Calculate_rho(M0, worldModel);

	double x = rho*sin(theta);
	double y = rho0 - rho*cos(theta);

	return (CXYPoint(x,y));

}

/****************************************************/
double CEquidistantConicProjection::ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double phi = toRad(p.Lat());
	double M = Calculate_M(phi, worldModel);
	double rho = Calculate_rho(M, worldModel);
	double n = Calculate_n(worldModel);
	double m = Calculate_m(phi, worldModel);

	double k = rho*n/(a*m);
	return (k);
}

/****************************************************/
PROJECTED_MODEL_TYPE CEquidistantConicProjection::WhichProjectionAmI() const
/****************************************************/
{
	return (EQUIDISTANT_CONIC);
}

