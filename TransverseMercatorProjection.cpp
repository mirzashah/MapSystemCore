#include <TransverseMercatorProjection.h>
#include <World3DModel.h>
#include <MathFunctions.h>

/****************************************************/
CTransverseMercatorProjection::CTransverseMercatorProjection()
/****************************************************/
{
	SetCentralScaleFactor(0.9996); //This is standard for UTM
}

/****************************************************/
CTransverseMercatorProjection::~CTransverseMercatorProjection()
/****************************************************/
{
}

/****************************************************/
double CTransverseMercatorProjection::CalculateA(const CWorld3DModel& worldModel, const CLatLongPoint& p) const
/****************************************************/
{
	double lambda        = toRad(p.Long());
	double phi           = toRad(p.Lat());
	double const lambda0 = toRad(this->OriginMeridian());
	double A             = (cos(phi)) * (lambda-lambda0); //eq (8-15)
	return (A);
}
/****************************************************/
double CTransverseMercatorProjection::CalculateC(const CWorld3DModel& worldModel, double phi) const
/****************************************************/
{
	double ePrimeSquared = CalculateEPrimeSquared(worldModel);
	double C = ePrimeSquared * (pow(cos(phi), 2.0));      //eq (8-14)
	return (C);
}

/****************************************************/
double CTransverseMercatorProjection::CalculateEPrimeSquared(const CWorld3DModel& worldModel) const
/****************************************************/
{
	double eSquared = worldModel.EccentricitySquared();
	double ePrimeSquared = eSquared / (1-eSquared);       //eq (8-12)

	return (ePrimeSquared);
}

/****************************************************/
double CTransverseMercatorProjection::CalculateM(const CWorld3DModel& worldModel, double phi) const
/****************************************************/
{
	//Can't use simplified equations for M because they are for Clarke 1866...
	//Use eq(3-21) instead of eq(3-22)
	double a        = worldModel.SemiMajorAxis();
	double e2	    = worldModel.EccentricitySquared();
	double e4       = pow(e2, 2.0);
	double e6       = pow(e2, 3.0);

	//TODO: Is infinite power series, only first few terms taken, might want to expand further...
	double term1 = (1 - e2/4.0 - (3*e4)/64.0 - (5.0*e6)/256.0)*phi;
	double term2 = ((3.0*e2)/8.0 + (3*e4)/32.0 + (45.0*e6)/1024.0)*sin(2.0*phi);
	double term3 = ((15.0*e4)/256.0 + (45.0*e6)/1024.0) * sin(4.0*phi);
	double term4 = ((35*e6)/3072.0 * sin(6*phi));

	double M = a * (term1 - term2 + term3 - term4);       //eq (3-21)

	return M;
}

/****************************************************/
double CTransverseMercatorProjection::CalculateN(const CWorld3DModel& worldModel, double phi) const
/****************************************************/
{
	double a        = worldModel.SemiMajorAxis();
	double eSquared = worldModel.EccentricitySquared();
	double N = a / (sqrt((1 - (eSquared * pow(sin(phi), 2.0))))); //eq (4-20)
	return (N);
}

/****************************************************/
double CTransverseMercatorProjection::CalculateR(const CWorld3DModel& worldModel, double phi) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double eSquared = worldModel.EccentricitySquared();
	double RTerm1 = a*(1-eSquared);
	double RTerm2a = (eSquared*(pow(sin(phi),2.0)));
	double RTerm2 = 1 - RTerm2a;
		   RTerm2 = pow(RTerm2, 1.5);
	double R       = RTerm1/RTerm2;
	return (R);
}

/****************************************************/
double CTransverseMercatorProjection::CalculateT(const CWorld3DModel& worldModel, double phi) const
/****************************************************/
{
	double T = pow(tan(phi), 2.0); //eq (8-13)
	return (T);
}

/****************************************************/
double CTransverseMercatorProjection::CentralScaleFactor() const
/****************************************************/
{
	return (_centralScaleFactor);
}

/****************************************************/
CLatLongPoint CTransverseMercatorProjection::InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Transverse Mercator Inverse Projection (from J. Snyder - Map Projections: A Working Manual)
	//NOTE!!! - Not very accurate as you get farther from central meridian
	const double a        = worldModel.SemiMajorAxis();
	const double eSquared = worldModel.EccentricitySquared();
	const double phi0     = toRad(this->OriginParallel());
	const double k0       = this->CentralScaleFactor();
	const double lambda0  = toRad(this->OriginMeridian());
	const double x        = p.X();
	const double y        = p.Y();

	const double e1       = (1-(sqrt(1-eSquared))) / (1+sqrt(1-eSquared)); //eq (3-24)
	const double M0       = CalculateM(worldModel, phi0);                  //eq (3-21)
	const double M        = M0 + y/k0;                                     //eq (8-20)

	//TODO: mu is infinite power series, only first few terms taken, might want to expand further...
	const double mu = M/((a*(1 - (eSquared/4) - (3*pow(eSquared, 2.0)/64) - (5*pow(eSquared, 3.0)))));  //eq (7-19)

	//TODO: phi1 terms are multiple infinite power series, only first few termst taken, might want to expand further...
	const double phi1Term1 = mu + (((3*e1/2) - (27*pow(e1,3.0)/32))*sin(2*mu));
	const double phi1Term2 = ((21*pow(e1,2.0)/16) - (55*pow(e1, 4.0)/32))*sin(4*mu);
	const double phi1Term3 = ((151*pow(e1,3.0))/96)*sin(6*mu);
	const double phi1Term4 = ((1097*pow(e1,4.0))/512)*sin(8*mu);
	const double phi1      = phi1Term1 + phi1Term2 + phi1Term3 + phi1Term4; //eq (3-26)

	const double ePrimeSquared = CalculateEPrimeSquared(worldModel); //eq (8-12)
	const double C1 = CalculateC(worldModel, phi1); //eq (8-21)
	const double T1 = CalculateT(worldModel, phi1); //eq (8-22)
	const double N1 = CalculateN(worldModel, phi1); //eq (8-23)
	const double R1 = CalculateR(worldModel, phi1); //eq (8-24)
	const double D  = x / (N1*k0);                  //eq (8-25)

	const double phiTerm1a = N1*tan(phi1)/R1;
	const double phiTerm1b = (pow(D,2.0)/2) - (5 + 3*T1 + 10*C1 - 4*pow(C1,2.0) - 9*ePrimeSquared)*(pow(D,4.0)/24);
	const double phiTerm1  = phi1 - phiTerm1a*phiTerm1b;
	const double phiTerm2 = (61 + 90*T1 + 298*C1 + 45*pow(T1,2.0) - 252*ePrimeSquared - 3*pow(C1,2.0))* (pow(D,6.0)/720);
	const double phi = phiTerm1 + phiTerm2; //eq (8-17)

	const double lambdaTerm1a = D - (1 + 2*T1 + C1)*pow(D,3.0)/6.0;
	const double lambdaTerm1b = (5 - 2*C1 + 28*T1 - 3*pow(C1, 2.0) + 8*ePrimeSquared + 24*pow(T1,2.0))*(pow(D,5.0)/120);
	const double lambda = lambda0 + (lambdaTerm1a + lambdaTerm1b)/(cos(phi1)); //eq (8-18)

	CLatLongPoint toBeReturned;
	toBeReturned.Lat(toDeg(phi));
	toBeReturned.Long(toDeg(lambda));

	return (toBeReturned);
}

/****************************************************/
bool CTransverseMercatorProjection::IsPointPlottable(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//TODO: This isn't completely correct
	if ((p.Long()>OriginMeridian()+90) || (p.Long()<OriginMeridian()-90))
		return false;
	else
		return true;
}

/****************************************************/
CXYPoint CTransverseMercatorProjection::ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Transverse Mercator Projection (from J. Snyder - Map Projections: A Working Manual)

	//Given parameters
	double const a            = worldModel.SemiMajorAxis();
	double const e            = worldModel.Eccentricity();
	double const eSquared     = worldModel.EccentricitySquared();
	double const phi0         = toRad(this->OriginParallel());
	double const lambda0      = toRad(this->OriginMeridian());
	double const k0           = this->CentralScaleFactor();
	double const lambda		  = toRad(p.Long());
	double const phi          = toRad(p.Lat());

	//First set of calculated parameters
	double ePrimeSquared = CalculateEPrimeSquared(worldModel);
	double N             = CalculateN(worldModel, phi);
	double T             = CalculateT(worldModel, phi);
	double A             = CalculateA(worldModel, p);
	double C             = CalculateC(worldModel, phi);
	double M             = CalculateM(worldModel, phi);
	double M0            = CalculateM(worldModel, phi0);

	//Calculate x
	double xTerm1 = A + ((1-T + C) * (pow(A, 3.0)/6));
	double xTerm2 = ((5-18*T + pow(T,2.0) + 72*C -58*ePrimeSquared)*(pow(A,5.0)/120));
	double x      = (k0*N)*(xTerm1 + xTerm2); //eq (8-9)

	//Calculate y
	double yTerm1 = M-M0;
	double yTerm2 = N*tan(phi)*((pow(A,2.0)/2.0) + ((5.0-T+(9.0*C)+(4.0*pow(C,2.0)))*(pow(A,4.0)/24.0)));
	double yTerm3 = ((61 - 58*T + (pow(T,2.0)) + 600*C - 330*ePrimeSquared)*(pow(A,6.0)/720));
	double y = k0 * (yTerm1+yTerm2+yTerm3); //eq (8-10)

	CXYPoint toBeReturned;
	toBeReturned.X(x);
	toBeReturned.Y(y);

	return (toBeReturned);
}

/****************************************************/
double CTransverseMercatorProjection::ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Given parameters
	double const eSquared     = worldModel.EccentricitySquared();
	double const lambda0      = toRad(this->OriginMeridian());
	double const k0           = this->CentralScaleFactor();

	double const lambda		  = toRad(p.Long());
	double const phi          = toRad(p.Lat());

	//First set of calculated parameters
	double ePrimeSquared = this->CalculateEPrimeSquared(worldModel);
	double A             = this->CalculateA(worldModel, p);
	double C             = this->CalculateC(worldModel, phi);
	double T             = this->CalculateT(worldModel, phi);

	//Calculate k
	double kTerm1 = 1.0+((1.0+C)*(pow(A,2.0)/2.0));
	double kTerm2 = (5.0-4.0*T+42.0*C+13.0*pow(C,2.0)-28.0*ePrimeSquared)*(pow(A,4.0)/24.0);
	double kTerm3 = (61.0-148.0*T+16*pow(T,2.0))*(pow(A,6.0)/720.0);
	double k = k0*(kTerm1 + kTerm2 + kTerm3); //eq (8-11)
	return k;
}

/****************************************************/
void CTransverseMercatorProjection::SetCentralScaleFactor(double centralScaleFactor)
/****************************************************/
{
	_centralScaleFactor = centralScaleFactor;
}

/****************************************************/
PROJECTED_MODEL_TYPE CTransverseMercatorProjection::WhichProjectionAmI() const
/****************************************************/
{
	return (TRANSVERSE_MERCATOR);
}
