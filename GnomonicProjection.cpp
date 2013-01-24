#include <GnomonicProjection.h>
#include <MapSystemEnumerations.h>
#include <MathFunctions.h>

/*************************************************/
CGnomonicProjection::CGnomonicProjection()
/*************************************************/
{
}

/*************************************************/
CGnomonicProjection::~CGnomonicProjection()
/*************************************************/
{
}

/****************************************************/
double CGnomonicProjection::Calculate_c(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double R = worldModel.SemiMajorAxis(); //TODO: Got to fix this, this model only for sphere
	double rho = Calculate_rho(p);

	double c = atan(rho/R);
	return (c);
}

/****************************************************/
double CGnomonicProjection::Calculate_cosc(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double const lambda0      = toRad(OriginMeridian());
	double const phi1         = toRad(OriginParallel());
	double const lambda		  = toRad(p.Long());
	double const phi          = toRad(p.Lat());

	double cosc = sin(phi1)*sin(phi) + cos(phi1)*cos(phi)*cos(lambda-lambda0);

	return (cosc);

}

/****************************************************/
double CGnomonicProjection::Calculate_kPrime(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double cosc = Calculate_cosc(p, worldModel);
	double kPrime = 1/cosc;

	return (kPrime);
}

/****************************************************/
double CGnomonicProjection::Calculate_rho(const CXYPoint& p)
/****************************************************/
{
	double x = p.X();
	double y = p.Y();
	double rho = sqrt(pow(x,2.0)+pow(y,2.0));
	return (rho);
}

/****************************************************/
CLatLongPoint CGnomonicProjection::InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Inverse Gnomonic Projection (from J. Snyder - Map Projections: A Working Manual)
	double const a            = worldModel.SemiMajorAxis();
	double const lambda0      = toRad(OriginMeridian());
	double const phi1         = toRad(OriginParallel());
	double const x            = p.X();
	double const y            = p.Y();

	double rho = Calculate_rho(p);
	double c = Calculate_c(p, worldModel);
	double phi = asin(cos(c)*sin(phi1)+(y*sin(c)*cos(phi1)/rho));
	
	double lambda = 0.0;
	if ((phi1<90.0)&&(phi1>-90.0))
	{
		lambda = lambda0 + atan(x*sin(c)/(rho*cos(phi1)*cos(c)-y*sin(phi1)*sin(c)));

	}
	else if (phi1==90)
	{
		lambda = lambda0 + atan(x/(-y));
	}
	else if (phi1==-90)
	{
		lambda = lambda0 + atan(x/y);
	}

	return CLatLongPoint(toDeg(phi), toDeg(lambda));
}

/****************************************************/
CXYPoint CGnomonicProjection::ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Gnomonic Projection (from J. Snyder - Map Projections: A Working Manual)
	double const lambda0      = toRad(OriginMeridian());
	double const lambda		  = toRad(p.Long());
	double const phi          = toRad(p.Lat());
	double const phi1         = toRad(OriginParallel());

	double R = worldModel.SemiMajorAxis(); //Approximation, fix this, force only sphere
	double kPrime = Calculate_kPrime(p, worldModel);

	double x = R*kPrime*cos(phi)*sin(lambda-lambda0);
	double y = R*kPrime*(cos(phi1)*sin(phi)-sin(phi1)*cos(phi)*cos(lambda-lambda0));

	return (CXYPoint(x,y));
}

/****************************************************/
double CGnomonicProjection::ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double k = 0;
	return k;
}

/****************************************************/
PROJECTED_MODEL_TYPE CGnomonicProjection::WhichProjectionAmI() const
/****************************************************/
{
	return (GNOMONIC);
}
