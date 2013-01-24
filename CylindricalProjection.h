/*****************************************************************************/
// Filename: CylidrinicalProjection.h
/*****************************************************************************/
// Description: This class is a base class for all cylindrical projections.
// A cylindrical projection maps the Earth to a two dimension plane by envisioning
// the Earth inside a cylinder. Cylindrical projections tend to fail around
// the poles.
/*****************************************************************************/

#ifndef _CYLINDRICAL_PROJECTION_H
#define _CYLINDRICAL_PROJECTION_H

#include <Projected2DModel.h>

/**
* This class represents all cylindrical projections
*/
class CCylindricalProjection : public CProjected2DModel
{
	public:
/**
* Default constructor
*/
		CCylindricalProjection();

/**
* A destructor for this class
*/
		virtual ~CCylindricalProjection();

};


#endif
