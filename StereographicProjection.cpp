#include <StereographicProjection.h>
#include <MapSystemEnumerations.h>
#include <MathFunctions.h>

/****************************************************/
CStereographicProjection::CStereographicProjection()
/****************************************************/
{
	SetCentralScaleFactor(0.9999);
}


/****************************************************/
CStereographicProjection::~CStereographicProjection()
/****************************************************/
{
}

/****************************************************/
double CStereographicProjection::AdjustedLambdaValue(double lambda, double atanNumerator, double atanDenominator, bool isSouthPole)
/****************************************************/
{
	/*This is ugly, but from Snyder's examples, if denominator negative...*/
    if (atanDenominator < 0)
		if (atanNumerator>0)  //...and numerator positive...
			lambda = lambda + toRad(180); //...add 180 degrees
		else  //Numerator negative
			lambda = lambda - toRad(180); //...subtract 180 degrees

	if (isSouthPole)
		lambda = -lambda; //South pole adjustment

	if (lambda > toRad(180)) //Adjust values to make sure within bounds, contradiction between examples
		lambda = lambda - toRad(360); //and wrote, he said add/subtract 180, but examples use 360
	else if (lambda < toRad(-180))
		lambda = lambda + toRad(360);
	/*end Snyder adjustment*/

	return (lambda);
}

/****************************************************/
double CStereographicProjection::Calculate_A(const CLatLongPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a       = worldModel.SemiMajorAxis();
	double k0      = CentralScaleFactor();
	double phi     = toRad(p.Lat());
	double phi1    = toRad(OriginParallel());
	double lambda  = toRad(p.Long());
	double lambda0 = toRad(OriginMeridian());

	double m1      = Calculate_m(phi1, worldModel);
	double chi1    = Calculate_chi_Forward(phi1, worldModel);
	double chi     = Calculate_chi_Forward(phi, worldModel);

	double ANumeratorTerm = 2*a*k0*m1;
	double ADenominatorTerm = cos(chi1)*(1+sin(chi1)*sin(chi)+cos(chi1)*cos(chi)*cos(lambda-lambda0));

	double A = ANumeratorTerm/ADenominatorTerm;

	return (A);
}

/****************************************************/
double CStereographicProjection::Calculate_ce(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double a = worldModel.SemiMajorAxis();
	double k0   = CentralScaleFactor();
	double phi1 = toRad(OriginParallel());
	double m1   = Calculate_m(phi1, worldModel);
	double chi1 = Calculate_chi_Forward(phi1, worldModel);
	double rho  = Calculate_rho(p, worldModel);

	double ce = 2*atan((rho*cos(chi1))/(2*a*k0*m1));

	return (ce);
}

/****************************************************/
double CStereographicProjection::Calculate_chi_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double y = p.Y();
	double phi1 = toRad(OriginParallel());
	double chi1 = Calculate_chi_Forward(phi1, worldModel);
	double rho  = Calculate_rho(p, worldModel);
	double ce   = Calculate_ce(p, worldModel);

	double chiInverse = 0;
	if(rho==0)
	{
		return(chi1);
	}
	else
	{
		chiInverse = asin(cos(ce)*sin(chi1)+(y*sin(ce)*cos(chi1)/rho));
		return(chiInverse);
	}
}

/****************************************************/
double CStereographicProjection::Calculate_chi_Forward(double phi, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double e = worldModel.Eccentricity();
	double chi = (2 * atan(tan(PI/4+phi/2)*pow(((1-e*sin(phi))/(1+e*sin(phi))), e/2))) - (PI/2);
	return (chi);
}

/****************************************************/
double CStereographicProjection::Calculate_m(double phi, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double eSquared = worldModel.EccentricitySquared();
	double m = cos(phi)/(sqrt((1-eSquared*pow(sin(phi),2))));
	return (m);
}

/****************************************************/
double CStereographicProjection::Calculate_lambda(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double lambda = 0;

	if (IsOriginAtNorthPole())
	{
		lambda = Calculate_lambda_At90North(p, worldModel);
	}
	else if (IsOriginAtSouthPole())
	{
		lambda = Calculate_lambda_At90South(p, worldModel);
	}
	else
	{
		lambda = Calculate_lambda_AtNon90(p, worldModel);
	}

	return (lambda);

}
/****************************************************/
double CStereographicProjection::Calculate_lambda_At90North(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double x = p.X();
	double y = p.Y();
	double lambda0 = toRad(OriginMeridian());

	double lambda = lambda0 + atan2(x,(-y));

	lambda = AdjustedLambdaValue(lambda, x, (-y));

	return (lambda);
}

/****************************************************/
double CStereographicProjection::Calculate_lambda_At90South(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Notice negation of northern polar values
	double x = -(p.X());
	double y = -(p.Y());
	double lambda0 = -(toRad(OriginMeridian()));

	double lambda = lambda0 + atan2(x,(-y));

	lambda = (AdjustedLambdaValue(lambda, x, (-y), true)); //I'm sorry if this is so confusing...

	return (lambda);
}

/****************************************************/
double CStereographicProjection::Calculate_lambda_AtNon90(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double x = p.X();
	double y = p.Y();
	double ce = Calculate_ce(p, worldModel);
	double lambda0 = toRad(OriginMeridian());
	double rho = Calculate_rho(p, worldModel);
	double phi1 = toRad(OriginParallel());
	double chi1 = Calculate_chi_Forward(phi1, worldModel);

	double aTanNumerator = x*sin(ce);
	double aTanDenominator = (rho*cos(chi1)*cos(ce)-y*sin(chi1)*sin(ce));

	double lambda = lambda0 + atan2(aTanNumerator,aTanDenominator);

	lambda = AdjustedLambdaValue(lambda, aTanNumerator, aTanDenominator);

	return (lambda);
}

/****************************************************/
double CStereographicProjection::Calculate_phi(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi = 0;

	if (IsOriginAtNorthPole())
	{
		phi = Calculate_phi_At90North(p, worldModel);
	}
	else if (IsOriginAtSouthPole())
	{
		phi = Calculate_phi_At90South(p, worldModel);
	}
	else //-90 < phi1 < 90
	{
		phi = Calculate_phi_AtNon90(p, worldModel);
	}

	return (phi);
}

/****************************************************/
double CStereographicProjection::Calculate_phi_At90North(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double rho = Calculate_rho(p, worldModel);
	double e   = worldModel.Eccentricity();
	double k0  = CentralScaleFactor();
	double a   = worldModel.SemiMajorAxis();
	double t   = rho*(sqrt(pow(1+e,1+e)*pow(1-e,1-e))) / (2*a*k0); //TODO: I didn't understand what phiC was in Snyder

	double phiLast = 0;
	double phi = PI/2 - 2*atan(t); //First trial
	const double convergenceFactor = 0.0000004;

	while (abs(phi-phiLast)>convergenceFactor)
	{
		phiLast = phi;
		phi = PI/2-2*atan(t*(pow(((1-e*sin(phiLast))/(1+e*sin(phiLast))), e/2)));
	}

	return (phi);

}

/****************************************************/
double CStereographicProjection::Calculate_phi_At90South(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Notice call to north polar calculate phi method with negation of key terms
	CXYPoint pNeg(-p.X(), -p.Y());
	double lambda0 = OriginParallel();
	SetOriginParallel(-OriginParallel());
	double phi = -(Calculate_phi_At90North(pNeg, worldModel));
	SetOriginParallel(lambda0);
	return(phi);
}

/****************************************************/
double CStereographicProjection::Calculate_phi_AtNon90(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double chi = Calculate_chi_Inverse(p, worldModel);
	double phi = chi;
	double phiLast = 0;
	const double convergenceFactor = 0.0000004;
	double e = worldModel.Eccentricity();

	while (abs(phi-phiLast)>convergenceFactor)
	{
		phiLast = phi;
		phi = 2*atan(tan(PI/4 + chi/2)*(pow((1+e*sin(phiLast))/(1-e*sin(phiLast)), e/2))) - PI/2;
	}

	return (phi);
}

/****************************************************/
double CStereographicProjection::Calculate_rho(const double& phi, const CWorld3DModel& worldModel)
/****************************************************/
{
	double a  = worldModel.SemiMajorAxis();
	double k0 = CentralScaleFactor();
	double t  = Calculate_t(phi, worldModel);
	double e  = worldModel.Eccentricity();

	double rho = 2*a*k0*t/(sqrt((pow(1+e,1+e))*(pow(1-e,1-e))));
	return (rho);
}

/****************************************************/
double CStereographicProjection::Calculate_rho(const CXYPoint& p, const CWorld3DModel& worldModel) const
/****************************************************/
{
	double x = p.X();
	double y = p.Y();

	double rho = sqrt(pow(x,2)+pow(y,2));

	return (rho);
}

/****************************************************/
double CStereographicProjection::Calculate_t(const double& phi, const CWorld3DModel& worldModel)
/****************************************************/
{
	double e = worldModel.Eccentricity();

	double t = tan(PI/4-phi/2) / (pow(((1-e*sin(phi))/(1+e*sin(phi))), e/2.0));
	return (t);
}

/****************************************************/
double CStereographicProjection::Calculate_x(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	if (IsOriginAtNorthPole())
	{
		return(Calculate_x_At90North(p, worldModel));
	}
	else if (IsOriginAtSouthPole())
	{
		return(Calculate_x_At90South(p, worldModel));
	}
	else
	{
		return(Calculate_x_AtNon90(p, worldModel));
	}
}

/****************************************************/
double CStereographicProjection::Calculate_x_At90North(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi     = toRad(p.Lat());
	double rho     = Calculate_rho(phi, worldModel);
	double lambda  = toRad(p.Long());
	double lambda0 = toRad(OriginMeridian());
	double x = rho*sin(lambda-lambda0);
	return (x);

}

/****************************************************/
double CStereographicProjection::Calculate_x_At90South(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Notice same equations as north, but reverse signs
	double phi     = -(toRad(p.Lat()));
	double rho     = Calculate_rho(phi, worldModel);
	double lambda  = -(toRad(p.Long()));
	double lambda0 = -(toRad(OriginMeridian()));
	double x = rho*sin(lambda-lambda0);
	return (-x);
}

/****************************************************/
double CStereographicProjection::Calculate_x_AtNon90(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	const double A             = Calculate_A(p, worldModel);
	const double phi           = toRad(p.Lat());
	const double chi           = Calculate_chi_Forward(phi, worldModel);
	const double lambda        = toRad(p.Long());
	const double lambda0       = toRad(this->OriginMeridian());

	double x = A * cos(chi)*sin(lambda-lambda0);
	return (x);
}

/****************************************************/
double CStereographicProjection::Calculate_y(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	if (IsOriginAtNorthPole())
	{
		return(Calculate_y_At90North(p, worldModel));
	}
	else if (IsOriginAtSouthPole())
	{
		return(Calculate_y_At90South(p, worldModel));
	}
	else
	{
		return(Calculate_y_AtNon90(p, worldModel));
	}
}

/****************************************************/
double CStereographicProjection::Calculate_y_At90North(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	double phi     = toRad(p.Lat());
	double rho     = Calculate_rho(phi, worldModel);
	double lambda  = toRad(p.Long());
	double lambda0 = toRad(OriginMeridian());
	double y = (-rho)*cos(lambda-lambda0);
	return (y);
}

/****************************************************/
double CStereographicProjection::Calculate_y_At90South(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Notice same equations as north, but reverse signs
	double phi     = -(toRad(p.Lat()));
	double rho     = Calculate_rho(phi, worldModel);
	double lambda  = -toRad(p.Long());
	double lambda0 = -toRad(OriginMeridian());
	double y = (-rho)*cos(lambda-lambda0);
	return (-y);
}

/****************************************************/
double CStereographicProjection::Calculate_y_AtNon90(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	const double A             = Calculate_A(p, worldModel);
	const double phi           = toRad(p.Lat());
	const double phi1          = toRad(OriginParallel()); //NOTE: I found this strange, phi1 refers to origin parallel, but no phi0, whhich is usual notation
	const double chi           = Calculate_chi_Forward(phi, worldModel);
	const double chi1          = Calculate_chi_Forward(phi1, worldModel);
	const double lambda        = toRad(p.Long());
	const double lambda0       = toRad(this->OriginMeridian());

	double y = A * (cos(chi1)*sin(chi)-sin(chi1)*cos(chi)*cos(lambda-lambda0));
	return (y);
}

/****************************************************/
CLatLongPoint CStereographicProjection::InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	//Inverse  Stereographic Projection (from J. Snyder - Map Projections: A Working Manual)
	double lambda = Calculate_lambda(p, worldModel);
	double phi = Calculate_phi(p, worldModel);

	return (CLatLongPoint(toDeg(phi), toDeg(lambda)));
}

/****************************************************/
bool CStereographicProjection::IsOriginAtNorthPole()
/****************************************************/
{
	return((OriginParallel()==90));
}

/****************************************************/
bool CStereographicProjection::IsOriginAtSouthPole()
/****************************************************/
{
	return((OriginParallel()==-90));
}

/****************************************************/
CXYPoint CStereographicProjection::ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	// Stereographic Projection (from J. Snyder - Map Projections: A Working Manual)

	double x = Calculate_x(p, worldModel);
	double y = Calculate_y(p, worldModel);
	return (CXYPoint(x,y));
}

/****************************************************/
double CStereographicProjection::ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************/
{
	const double A   = Calculate_A(p, worldModel);
	const double phi = toRad(p.Lat());
	const double chi = Calculate_chi_Forward(phi, worldModel);
	double m         = Calculate_m(phi, worldModel);
	double a         = worldModel.SemiMajorAxis();

	double k = A * cos(chi)/(a*m);
	return (k);
}

/****************************************************/
PROJECTED_MODEL_TYPE CStereographicProjection::WhichProjectionAmI() const
/****************************************************/
{
	return (STEREOGRAPHIC);
}
