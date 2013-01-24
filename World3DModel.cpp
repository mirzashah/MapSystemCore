#include <World3DModel.h>
#include <MathFunctions.h>
#include <stdio.h>

/****************************************************/
CWorld3DModel::CWorld3DModel(double semiMajorAxis, double semiMinorAxis)
/****************************************************/
{
	_semiMajorAxis = semiMajorAxis;
	_semiMinorAxis = semiMinorAxis;
	PreCalculateFlattening();
	PreCalculateEccentricityAndEccentrictySquared();
}

/****************************************************/
CWorld3DModel::~CWorld3DModel()
/****************************************************/
{
}

/****************************************************/
double CWorld3DModel::DistanceBetweenTwoPoints(const CLatLongPoint& p1, const CLatLongPoint& p2)
/****************************************************/
{
    if(p1==p2)
        return(0.0f);

	//Vincenty's Algorithm (taken from http://www.movable-type.co.uk/scripts/LatLongVincenty.html)
	//See also http://www.ga.gov.au/nmd/geodesy/datums/vincenty_inverse.jsp
	//and http://www.ga.gov.au/nmd/geodesy/datums/vincenty_direct.jsp
	//also read paper: Vincenty, T. "Direct and Inverse Solutions of Geodesics on the Ellipsoid
	//with Application of Nested Equations", DMAAC Geodetic Survey Squadron, F.E.
	//Warren AFB, Wyoming 82001, Survey Review XXII, 176, April 1975.
	//If performance is an issue, do a search for Haversine's Algorithm, gives less accurate
	//but significantly faster value

	double s = -1;
	int    iterationLimit = 20;
	double a = SemiMajorAxis();
	double b = SemiMinorAxis();
	double f = Flattening();
	double phi1 = toRad(p1.Lat());
	double phi2 = toRad(p2.Lat());
	double L = toRad(p2.Long() - p1.Long());
	double U1 = atan((1-f)*tan(phi1));
	double U2 = atan((1-f)*tan(phi2));
	double lambda = L;
	double lambdaP = 2 * PI;
	double sinU1 = sin(U1);
	double sinU2 = sin(U2);
	double cosU1 = cos(U1);
	double cosU2 = cos(U2);
	double sinLambda = 0.0;
	double cosLambda = 0.0;
	double sinSigma = 0.0;
	double cosSigma = 0.0;
	double sigma = 0.0;
	double alpha =0.0;
	double cosSqAlpha = 0.0;
	double cos2SigmaM = 0.0;
	double C = 0.0;

	while (((fabs(lambda - lambdaP)) > (pow(10.0, -12.0)))&&(iterationLimit >=0))
	{
		sinLambda = sin(lambda);
		cosLambda = cos(lambda);

		sinSigma = sqrt(((cosU2*sinLambda) * (cosU2*sinLambda)) + (((cosU1*sinU2)-(sinU1*cosU2*cosLambda)) * ((cosU1*sinU2)-(sinU1*cosU2*cosLambda))));
		cosSigma = (sinU1*sinU2) + (cosU1*cosU2*cosLambda);

		sigma = atan2(sinSigma, cosSigma);
		alpha = asin(cosU1 * cosU2 * sinLambda / sinSigma);
		cosSqAlpha = cos(alpha) * cos(alpha);
		cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;
		C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
		lambdaP = lambda;
		lambda = L + (1-C) * f * sin(alpha) * (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));

		iterationLimit--;
	}

	if (iterationLimit==0)
	{
		printf("Convergence failure.");
		return -1;  // formula failed to converge
	}

	double uSq = cosSqAlpha*(a*a-b*b)/(b*b);
	double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
	double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
	double deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));

	s = b*A*(sigma-deltaSigma);

	return s;

}

/****************************************************/
double CWorld3DModel::Eccentricity() const
/****************************************************/
{
	return (_eccentricity);
}

/****************************************************/
double CWorld3DModel::EccentricitySquared() const
/****************************************************/
{
	return (_eccentricitySquared);
}

/****************************************************/
double CWorld3DModel::Flattening() const
/****************************************************/
{
	return (_flattening);

}

/****************************************************/
void CWorld3DModel::PreCalculateEccentricityAndEccentrictySquared()
/****************************************************/
{
	double a = SemiMajorAxis();
	double b = SemiMinorAxis();
	double aSquared = pow(a, 2);
	double bSquared = pow(b, 2);
	_eccentricitySquared = (aSquared - bSquared)/aSquared;
	_eccentricity = sqrt(_eccentricitySquared);
}

/****************************************************/
void CWorld3DModel::PreCalculateFlattening()
/****************************************************/
{
	double a = SemiMajorAxis();
	double b = SemiMinorAxis();
	_flattening = (a-b)/a;
}

/****************************************************/
double CWorld3DModel::SemiMajorAxis() const
/****************************************************/
{
	return (_semiMajorAxis);
}

/****************************************************/
double CWorld3DModel::SemiMinorAxis() const
/****************************************************/
{
	return (_semiMinorAxis);
}
