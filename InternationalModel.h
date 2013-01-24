/*****************************************************************************/
// Filename: InternationalModel.h
/*****************************************************************************/
// Description: This class represents the International ellipsoid used to approximate
// the Earth geoid. Same as Hayford 1909 ellipsoid.
/*****************************************************************************/

#ifndef _INTERNATIONAL_MODEL_H
#define _INTERNATIONAL_MODEL_H

#include <World3DModel.h>

/**
* This class describes a the International reference ellipsoid specification
*/
class CInternationalModel : public CWorld3DModel
{
	public:

/**
* Default constructor
*/
		CInternationalModel();

/**
* A destructor for this class.
*/
		virtual ~CInternationalModel();

/**
* Indicates type of reference ellipsoid (or CWorld3DModel) current instance belongs to,
* in this case International.
* @return Enumerated value INTERNATIONAL (of type WORLD_MODEL_TYPE)
*/
		WORLD_MODEL_TYPE WhichWorldModelAmI() const;
};

#endif
