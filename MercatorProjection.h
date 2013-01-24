/*****************************************************************************/
// Filename: MercatorProjection.h
/*****************************************************************************/
// Description: This class represents probably the most basic of cylindrical
// projections, and even all map projections in general. A Mercator projection
// assumes the Earth is encapsulated in a cylinder where the equator runs around
// the circular cross section and that meridians runs along the length of the
// cylinder. This projection has heavy distortion as one moves away from the
// equator. Scale is only true within a few degrees north and south of the
// the equator. Distance is true with rumb lines, but distance does not indicate
// great circle path.
/*****************************************************************************/

#ifndef _MERCATOR_PROJECTION_H
#define _MERCATOR_PROJECTION_H

#include <CylindricalProjection.h>
#include <CoordinateTypes.h>

/**
* This class represents a standard Mercator projection
*/
class CMercatorProjection : public CCylindricalProjection
{
	public:

/**
* Default constructor
*/
		CMercatorProjection();

/**
* A destructor for this class
*/
		virtual ~CMercatorProjection();

/**
* This method inverts a Cartesian point on a Mercator projection back to a geographical coordinate
* Note - The meridian of origin and parallel of origin should be set back to what they were
* originally to what they were during the forward projection.
* @param p - Cartesian point
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return A CLatLongPoint referencing the passed ellipsoid
*/
		virtual CLatLongPoint         InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel);

/**
* This method projects a geographical point to an XY point using the Mercator projection.
* The origin of the projected grid is the intersection of the parallel of origin (x-axis)
* and meridian of origin (y-axis).
* @param p - Geographical point
* @param worldModel - Reference ellipsoid the coordinates are relative to
* @return A CXYPoint representing projected point (x and y in meters)
*/
		virtual CXYPoint             ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method gives the scale factor for specified point on Mercator projection
* @param p - Geographical point the coordinates are relative to
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return Scale factor for point as a double
*/
		virtual double               ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);


/**
* This method allows figuring out the type of projection this is using virtual lookup.
* @return A PROJECTED_MODEL_TYPE, in this case = MERCATOR
*/
		virtual PROJECTED_MODEL_TYPE WhichProjectionAmI() const;

	private:
		CXYPoint                     ProjectPointBasic(const CLatLongPoint& p, const CWorld3DModel& worldModel);

	private:


};

#endif
