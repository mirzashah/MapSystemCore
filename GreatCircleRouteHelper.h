/*****************************************************************************/
// Filename: GreatCircleRouteHelper.h
/*****************************************************************************/
// Description: This class helps the user plot a "great circle" path between
// a pair of geographical coordinates. This is useful in showing the shortest distance
// on maps where straight lines aren't the shortest path and giving an indicaion of
// distortion in a projection.
/*****************************************************************************/

#ifndef _GREAT_CIRCLE_ROUTE_HELPER
#define _GREAT_CIRCLE_ROUTE_HELPER

#include <CoordinateTypes.h>
#include <GnomonicProjection.h>

#include <vector>
using namespace std;

/**
* This class helps generate great circle routes.
*/
class CGreatCircleRouteHelper
{
	public:
/**
* A constructor for this class
*/
		CGreatCircleRouteHelper();
/**
* A destructor for this class
*/
		virtual ~CGreatCircleRouteHelper();

/**
* This method generates the Great Circle route between two points
* @p1 First lat/long point
* @p2 Second lat/long point
* @return STL vector of lat/long points that make up path
*/
		vector<CLatLongPoint> GreatCircleRoute(const CLatLongPoint& p1, const CLatLongPoint& p2);

};

#endif
