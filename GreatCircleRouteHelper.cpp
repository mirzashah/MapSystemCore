#include <GreatCircleRouteHelper.h>
#include <GnomonicProjection.h>
#include <WGS84Model.h>

/****************************************************************/
CGreatCircleRouteHelper::CGreatCircleRouteHelper()
/****************************************************************/
{
}

/****************************************************************/
CGreatCircleRouteHelper::~CGreatCircleRouteHelper()
/****************************************************************/
{
}

/****************************************************************/
vector<CLatLongPoint> CGreatCircleRouteHelper::GreatCircleRoute(const CLatLongPoint& p1, const CLatLongPoint& p2)
/****************************************************************/
{
	CGnomonicProjection gnomonicProj;
	gnomonicProj.SetOriginParallel(p1.Lat());
	gnomonicProj.SetOriginMeridian(p1.Long());
	
	CWGS84Model worldModel;
	CXYPoint xyPnt1 = gnomonicProj.ProjectPoint(p1, worldModel);
	CXYPoint xyPnt2 = gnomonicProj.ProjectPoint(p2, worldModel);

	vector<CLatLongPoint> toReturn;

	CXYPoint p = xyPnt1;
	double res = 1000;
	double dY = (xyPnt2.Y() - xyPnt1.Y()) / res;
	double dX = (xyPnt2.X() - xyPnt1.X()) / res;
	for (int ctr = 0; ctr<=res; ctr++)
	{
		p.X(p.X()+dX);
		p.Y(p.Y()+dY);
		toReturn.push_back(gnomonicProj.InverseProjectPoint(p, worldModel));	
	}
	

	return (toReturn);
}


