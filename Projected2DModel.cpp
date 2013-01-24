#include <Projected2DModel.h>
#include <cmath>
using namespace std;
#include <stdio.h>

/****************************************************************************/
CProjected2DModel::CProjected2DModel()
/****************************************************************************/
{
	SetOriginMeridian(0.0);
	SetOriginParallel(0.0);
}

/****************************************************************************/
CProjected2DModel::~CProjected2DModel()
/****************************************************************************/
{
}

/****************************************************************************/
double CProjected2DModel::DistanceBetweenTwoCXYPoints(const CXYPoint& p1, const CXYPoint& p2) const
/****************************************************************************/
{
	double deltaXSq = abs(pow(p1.X()-p2.X(), 2.0));
	double deltaYSq = abs(pow(p1.Y()-p2.Y(), 2.0));
	printf("DeltaX = %lf \n", abs(p1.X()-p2.X()));
	printf("DeltaY = %lf \n", abs(p1.Y()-p2.Y()));
	double dis = sqrt(deltaXSq + deltaYSq);
	return dis;
}

/****************************************************/
double CProjected2DModel::DistanceBetweenTwoProjectedCLatLongPoints(const CLatLongPoint& p1, const CLatLongPoint& p2, const CWorld3DModel& worldModel)
/****************************************************/
{
	CXYPoint xyP1      = ProjectPoint(p1, worldModel); //This should call appropriate
	CXYPoint xyP2      = ProjectPoint(p2, worldModel); //virtual method...
	printf("XYP1 %lf, %lf\n", xyP1.X(), xyP1.Y());
	printf("XYP2 %lf, %lf\n", xyP2.X(), xyP2.Y());
	double distance   = CProjected2DModel::DistanceBetweenTwoCXYPoints(xyP1, xyP2);

	return (distance);
}

/****************************************************************************/
bool CProjected2DModel::IsPointPlottable(const CLatLongPoint& p, const CWorld3DModel& worldModel)
/****************************************************************************/
{
	return (true);
}

/****************************************************************************/
double CProjected2DModel::OriginMeridian() const
/****************************************************************************/
{
	return (_originMeridian);
}

/****************************************************************************/
double CProjected2DModel::OriginParallel() const
/****************************************************************************/
{
	return (_originParallel);
}

/****************************************************************************/
void CProjected2DModel::SetOriginMeridian(double originMeridian)
/****************************************************************************/
{
	_originMeridian = originMeridian;

}

/****************************************************************************/
void CProjected2DModel::SetOriginParallel(double originParallel)
/****************************************************************************/
{
	_originParallel = originParallel;

}
