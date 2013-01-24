/*****************************************************************************/
// Filename: WGS84Model.h
/*****************************************************************************/
// Description: This class represents the WGS-84 ellipsoid used to approximate
// the Earth geoid. WGS-84 is a DoD standard that is now used worldwide for
// geodetic datums. WGS-84 coordinates are used by GPS and in worst case is only
// off by about 110 meters.
/*****************************************************************************/

#ifndef _WGS84_MODEL_H
#define _WGS84_MODEL_H

#include <World3DModel.h>

/**
* This class describes a the WGS-84 reference ellipsoid specification
*/
class CWGS84Model : public CWorld3DModel
{
	public:

/**
* Default constructor
*/
		CWGS84Model();

/**
* A destructor for this class.
*/
		virtual ~CWGS84Model();

/**
* Indicates type of reference ellipsoid (or CWorld3DModel) current instance belongs to,
* in this case WGS-84.
* @return Enumerated value WGS_84 (of type WORLD_MODEL_TYPE)
*/
		WORLD_MODEL_TYPE WhichWorldModelAmI() const;
};

#endif
