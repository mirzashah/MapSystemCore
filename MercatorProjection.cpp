#include <MercatorProjection.h>
#include <World3DModel.h>
#include <MathFunctions.h>
#include <assert.h>

/****************************************************/
CMercatorProjection::CMercatorProjection()
/****************************************************/
{
}

/****************************************************/
CMercatorProjection::~CMercatorProjection()
/****************************************************/
{
}

/****************************************************/
CLatLongPoint CMercatorProjection::InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Inverse Mercator Projection (from J. Snyder - Map Projections: A Working Manual)
	double const a            = worldModel.SemiMajorAxis();
	double const e            = worldModel.Eccentricity();
	double const lambda0      = toRad(this->OriginMeridian());
	double  x            = p.X();
	double  y            = p.Y();

	/*This is a hack added in to center around origin set by user*/
	CXYPoint originXY = ProjectPointBasic(CLatLongPoint(OriginParallel(), OriginMeridian()), worldModel);
	y = y + originXY.Y();
	//assert(originXY.Y()==0);
	/*end hack*/

	double t   = pow(exp(1.0), (-y)/a); //eq (7-10)


	//Inverse requires converging series
	double convergenceFactor = 0.00000001;
	double phi = (PI/2) - (2*atan(t)); //eq (7-11) [first trial]
	double phiLast = 0.0;
	while (abs(phi - phiLast)>convergenceFactor)
	{
		phiLast = phi;
		double phiTerm1 = PI/2.0; //Start eq (7-9)
		double phiTerm2a = (1-e*sin(phi));
		double phiTerm2b = (1+ e*sin(phi));
		double phiTerm2 = t*pow(phiTerm2a/phiTerm2b, e/2.0);
		phi = phiTerm1 - 2.0*atan(phiTerm2); //End eq (7-9)
	}

	double lambda = x/a + lambda0; //eq (7-12)

    return(CLatLongPoint(toDeg(phi), toDeg(lambda), p.Z()));
}

/****************************************************/
CXYPoint CMercatorProjection::ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Mercator Projection (from J. Snyder - Map Projections: A Working Manual)
	CXYPoint toReturn  = ProjectPointBasic(p, worldModel);

	/*This is a hack added in to center around origin set by user*/
	CXYPoint originXY = ProjectPointBasic(CLatLongPoint(OriginParallel(), OriginMeridian()), worldModel);
	toReturn.Y(toReturn.Y() - originXY.Y());
	/*end hack*/

	return (toReturn);
}

/****************************************************/
CXYPoint CMercatorProjection::ProjectPointBasic(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double const a            = worldModel.SemiMajorAxis();
	double const e            = worldModel.Eccentricity();
	double const eSquared     = worldModel.EccentricitySquared();
	double const lambdaOrigin = toRad(this->OriginMeridian());
	double const lambda		  = toRad(p.Long());
	double const phi          = toRad(p.Lat());

	//Calculate x
	double x = a*(lambda - lambdaOrigin); //eq (7-6)

	//Calculate y (this is ugly)
	double temp1 = a/2;
	double temp2 = tan((PI/4) + (phi/2));
	double temp3 = 1 - (e*sin(phi));
	double temp4 = 1 + (e*sin(phi));
	double temp5 = pow((temp3 / temp4), (e/2));
	double temp6 = temp5 * temp2;
	double temp7 = log(temp6);
	double y = a * temp7; //eq (7-7)

	CXYPoint toBeReturned(x,y);

	return (toBeReturned);
}

/****************************************************/
double CMercatorProjection::ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double eSquared = pow(worldModel.Eccentricity(), 2);
	double phi = toRad(p.Lat());

	//Calculate scale along meridian
	double k = (pow(1 - (eSquared * pow(sin(phi),2)), 0.5)) / (cos(phi)); //eq (7-8)
	return k;
}

/****************************************************/
PROJECTED_MODEL_TYPE CMercatorProjection::WhichProjectionAmI() const
/****************************************************/
{
	return (MERCATOR);
}
