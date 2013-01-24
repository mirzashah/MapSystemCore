/*****************************************************************************/
// Filename: Clarke1866Model.h
/*****************************************************************************/
// Description: This class represents the Clarke 1866 ellipsoid used to approximate
// the Earth geoid. Even though a little dated, the Clarke 1866 ellispoid is
// excellent for representing the Americas. It is still used for generating maps
// of North America.
/*****************************************************************************/

#ifndef _CLARKE1866_MODEL_H
#define _CLARKE1866_MODEL_H

#include <World3DModel.h>

/**
* This class describes a the Clarke 1866 reference ellipsoid specification
*/
class CClarke1866Model : public CWorld3DModel
{
	public:

/**
* Default constructor
*/
		CClarke1866Model();

/**
* A destructor for this class.
*/
		virtual ~CClarke1866Model();

/**
* Indicates type of reference ellipsoid (or CWorld3DModel) current instance belongs to,
* in this case Clarke 1866.
* @return Enumerated value CLARKE_1866 (of type WORLD_MODEL_TYPE)
*/
		WORLD_MODEL_TYPE WhichWorldModelAmI() const;
};

#endif
