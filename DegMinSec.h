/*****************************************************************************/
// Filename: DegMinSec.h
/*****************************************************************************/
// Description: This class represents a degrees/minutes/seconds value. This
// class is used primarily by CLatLongPoint for holding coordinates in deg/min/sec
// form. Users of the MapSystem library might find it convenient
// to fill up CDegMinSec instances and pass them back and forth to CLatLongPoint
// instances rather than use doubles.
/*****************************************************************************/

#ifndef _DEG_MIN_SEC_H
#define _DEG_MIN_SEC_H

#include <MapSystemEnumerations.h>

/**
*This class represents a coordinate in terms of degrees, minutes, and seconds
*/
class CDegMinSec
{
	public:
/**
* Default constructor for this class
*/
		CDegMinSec();

/**
* Constructor for this class
* @param degMinSec - double floating point value representing degrees, minutes, seconds
*/
		CDegMinSec(double degMinSec);

/**
* Constructor for this class
* @param deg - degrees as integer
* @param min - minutes as integer
* @param sec - seconds as double floating point
*/
		CDegMinSec(int deg, int min, double sec);

/**
* Destructor for this class
*/
		~CDegMinSec();

/**
* Static method to retrive a double floating point equivalent of
* the current object in contrast to a CDegMinSec, useful for calculations.
* @param degMinSec - A CDegMinSec whose 'double' equivalent is requested
* @return double representing double precision floating point equivalent of
* current instance
*/
		static double     ConvertDegMinSecToDouble(CDegMinSec degMinSec);

/**
* Static method to retrive a CDegMinSec equivalent of
* the a double representing a degrees/minutes/seconds value.
* @param degMinSec - A double representing degrees/minutes/seconds
* @return A CDegMinSec representing the passed double value
*/
		static CDegMinSec ConvertDoubleToDegMinSec(double degMinSec);

/**
* Returns 'degrees' field of current instance
* @return degrees as integer
*/
		int        Deg();

/**
* Set method to set 'degrees' field of current instance
* @param deg - degrees as integer
*/
		void       Deg(int deg);

/**
* Returns 'minutes' field of current instance
* @return minutes as integer
*/
		int        Min();

/**
* Set method to set 'minutes' field of current instance
* @param min - minutes as integer
*/
		void       Min(int min);

/**
* Returns 'seconds' field of current instance
* @return seconds as double
*/
		double     Sec();

/**
* Set method to set 'seconds' field of current instance
* @param sec - seconds as double
*/
		void       Sec(double sec);

	private:
		int       _deg;
		int       _min;
		double    _sec;

};

#endif
