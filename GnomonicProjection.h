/*****************************************************************************/
// Filename: GnomonicProjection.h
/*****************************************************************************/
// Description: This class represents a gnomonic projection, DO NOT USE IT
// ONLY MEANT FOR A SPHERICAL REPRESENTATION OF THE WORLD. NEEDS TO BE FIXED.
// The only use of this projection is that all straight lines emerging from the center
// point (i.e. all azimuths) represent great circle route and distance.
/*****************************************************************************/

#ifndef _GNOMONIC_PROJECTION_H
#define _GNOMONIC_PROJECTION_H

#include <AzimuthalProjection.h>
#include <World3DModel.h>

/**
* This class is a representation of a gnomonic projection.
*/
class CGnomonicProjection : public CAzimuthalProjection
{
	public:
/**
* A constructor for this class
*/
		CGnomonicProjection();

/**
* A destructor for this class
*/
		virtual ~CGnomonicProjection();

/**
* This method inverts a Cartesian point on a Gnomonic projection back to a geographical coordinate
* Note - The meridian of origin and parallel of origin should be set back to what they were
* originally to what they were during the forward projection.
* @param p - Cartesian point
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return A CLatLongPoint referencing the passed ellipsoid
*/
		virtual CLatLongPoint         InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel);

/**
* This method projects a geographical point to an XY point using the Gnomonic projection.
* The origin of the projected grid is the intersection of the parallel of origin (x-axis)
* and meridian of origin (y-axis).
* @param p - Geographical point
* @param worldModel - Reference ellipsoid the coordinates are relative to
* @return A CXYPoint representing projected point (x and y in meters)
*/
		virtual CXYPoint             ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method gives the scale factor for specified point on Gnomonic projection
* @param p - Geographical point the coordinates are relative to
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return Scale factor for point as a double
*/
		virtual double               ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method allows figuring out the type of projection this is using virtual lookup.
* @return A PROJECTED_MODEL_TYPE, in this case = GNOMONIC
*/
		virtual PROJECTED_MODEL_TYPE WhichProjectionAmI() const;

	private:
		double Calculate_c(const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_cosc(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double Calculate_kPrime(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double Calculate_rho(const CXYPoint& p);
};

#endif
