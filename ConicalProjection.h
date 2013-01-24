/*****************************************************************************/
// Filename: CylidrinicalProjection.h
/*****************************************************************************/
// Description: This class is a base class for all conical projections.
// A conical projection maps the Earth to a two dimension plane by envisioning
// the Earth inside a cone. Conical projections do a good job of projecting the
// earth in mid range latitude areas.
/*****************************************************************************/

#ifndef _CONICAL_PROJECTION_H
#define _CONICAL_PROJECTION_H

#include <Projected2DModel.h>

/**
* This class represents all conical projections
*/
class CConicalProjection : public CProjected2DModel
{
	public:
/**
* Default constructor
*/
		CConicalProjection();

/**
* A destructor for this class
*/
		virtual ~CConicalProjection();

/**
*
*/
		double       FirstStandardParallel() const;

/**
*
*/
		double       SecondStandardParallel() const;

/**
*
*/
		void         SetFirstStandardParallel(double lat);

/**
*
*/
		void         SetSecondStandardParallel(double lat);

	private:
		double _firstStandardParallel;
		double _secondStandardParallel;
};


#endif
