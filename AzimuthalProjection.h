/*****************************************************************************/
// Filename: AzimuthalProjection.h
/*****************************************************************************/
// Description: This class is a base class for all Azimuthal projections.
// Azimuthal projections (also known as Zenithal projections) and are formed
// by mapping the Earth to a plane which is tangent to the Earth, in most cases
// where the tangent represents the zenith of the north or south poles. The azimuth
// or distance from the tangent point of the plane(at center of map) to any point
// on the map is always true and is the great circle distance,
// hence the name azimuthal.
/*****************************************************************************/

#ifndef _AZIMUTHAL_PROJECTION_H
#define _AZIMUTHAL_PROJECTION_H

#include <Projected2DModel.h>

/**
* This class represents all azimuthal projections
*/
class CAzimuthalProjection : public CProjected2DModel
{
	public:

/**
* Default constructor
*/
		CAzimuthalProjection();

/**
* A destructor for this class
*/
		virtual ~CAzimuthalProjection();

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
* This method sets the current scale factor of the projection
* @param centralScaleFactor - value to be set
*/
		void                         SetCentralScaleFactor(double centralScaleFactor);

	private:
		double _centralScaleFactor; //k0
};


#endif
