/*****************************************************************************/
// Filename: TransverseMercatorProjection.h
/*****************************************************************************/
// Description: This class represents the Transverse Mercator Projection (TMP)
// A TMP is similar to a standard Mercator projection, but rotates the cylinder
// 90 degrees so that the meridians run along the breadth of the cylinder. This
// projection has less scale variation than a regular Mercator, but is not as
// easy to read as the grid lines are no longer straight and perpendicular. TMP is
// used to implement the Universal Transverse Mercator grid system (see
// CModifiedUTMProjection) which used for the Military Grid System.
//
// Note: The central scale factor is a state variable for the TMP. By default
// it is set to 0.9996 which corresponds with UTM configuration, but it can be
// changed on the fly.
/*****************************************************************************/

#ifndef _TRANSVERSE_PROJECTION_H
#define _TRANSVERSE_PROJECTION_H

#include <CylindricalProjection.h>
#include <CoordinateTypes.h>

/**
* This class represents a Transverse Mercator projection
*/
class CTransverseMercatorProjection : public CCylindricalProjection
{
	public:
/**
* Default constructor
*/
		CTransverseMercatorProjection();

/**
* A destructor for this class
*/
		virtual ~CTransverseMercatorProjection();

/**
* Returns <I>central scale factor</I>, also known in projection equations as
* <I>k</I>0, (the 0 being a subscript). Allows one to fix the scale of the central
* meridian (i.e. meridian of origin). Fixing it to 1.0 would mean ideal scale, but
* in most cases reduce scale. This creates two adjacent meridians of true scale and
* reduces over scale variation throughout projection. By default set to 0.996, as
* used in Universal Transverse Mercator projection.
* @return Current central scale factor
*/
		double                       CentralScaleFactor() const;

/**
* This method inverts a Cartesian point on a Transverse Mercator projection back to a geographical coordinate
* Note - The meridian of origin and parallel of origin should be set back to what they were
* originally to what they were during the forward projection.
* @param p - Cartesian point
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return A CLatLongPoint referencing the passed ellipsoid
*/
		virtual CLatLongPoint        InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel);

/**
*
*/
		virtual bool                 IsPointPlottable(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method projects a geographical point to an XY point using the  Transverse Mercator projection.
* The origin of the projected grid is the intersection of the parallel of origin (x-axis)
* and meridian of origin (y-axis).
* @param p - Geographical point
* @param worldModel - Reference ellipsoid the coordinates are relative to
* @return A CXYPoint representing projected point (x and y in meters)
*/
		virtual CXYPoint             ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method gives the scale factor for specified point on Transverse Mercator projection
* @param p - Geographical point the coordinates are relative to
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return Scale factor for point as a double
*/
		virtual double               ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method sets the current scale factor of the projection
* @param centralScaleFactor - value to be set
*/
		void                         SetCentralScaleFactor(double centralScaleFactor);

/**
* This method allows figuring out the type of projection this is using virtual lookup.
* @return A PROJECTED_MODEL_TYPE, in this case = TRANSVERSE_MERCATOR
*/
		virtual PROJECTED_MODEL_TYPE WhichProjectionAmI() const;

	private:
		double CalculateA(const CWorld3DModel& worldModel, const CLatLongPoint& p) const;
		double CalculateC(const CWorld3DModel& worldModel, double phi) const;
		double CalculateEPrimeSquared(const CWorld3DModel& worldModel) const;
		double CalculateM(const CWorld3DModel& worldModel, double phi) const; //True distance along the central meridian from the Equator (phi) eq 3-21
		double CalculateN(const CWorld3DModel& worldModel, double phi) const; //Radius of curvature eq 4-20
		double CalculateR(const CWorld3DModel& worldModel, double phi) const;
		double CalculateT(const CWorld3DModel& worldModel, double phi) const;

	private:
		double _centralScaleFactor; //k0

};

#endif
