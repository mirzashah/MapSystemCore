/*****************************************************************************/
// Filename: LatLongPoint.h
/*****************************************************************************/
// Description: This class represents a geograpical coordinate, corresponds with
// CXYPoint
/*****************************************************************************/

#ifndef _LAT_LONG_POINT_H
#define _LAT_LONG_POINT_H

#include <MapSystemEnumerations.h>
#include <DegMinSec.h>


/**
* This class represents a latitude/longitude point. Class handles invalid input as
* well.
*/
class CLatLongPoint
{
	public:
/**
* Default constructor
*/
		CLatLongPoint();

/**
* A constructor for this class allowing one to set lat/long with two doubles
*/
		CLatLongPoint(double lat, double lon, double height = 0);

/**
* A constructor for this class allowing one to set lat/long with two CDegMinSec
*/
		CLatLongPoint(CDegMinSec lat, CDegMinSec lon, double height = 0);

/**
* A destructor for this class
*/
		~CLatLongPoint();

/**
*
*/
		static void     TurnAutoCorrectOn();

/**
*
*/
		static void     TurnAutoCorrectOff();

/**
*
*/
		static void     TurnErrorCheckingOn();

/**
*
*/
		static void     TurnErrorCheckingOff();

/**
* Returns of height of coordinate as double
* @return height as double
*/
		double          Height() const;

/**
* Sets height of the point
* @param height - height
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC   Height(double height);

/**
*
*/

		static bool     IsAutoCorrectOn();

/**
*
*/
		static bool     IsErrorCheckingOn();

/**
* Returns latitude of coordinate as double
* @return latitude as double
*/
		double           Lat() const;

/**
* Sets latitude using a double
* @param  lat - latitude
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC    Lat(double lat);

/**
* Sets latitude using a CDegMinSec struc
* @param  lat - latitude
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC    Lat(CDegMinSec lat);

/**
* Returns latitude of coordinate as CDegMinSec struc
* @return latitude as CDegMinSec
*/
		CDegMinSec       LatDegMinSec();

/**
* Returns longitutde of coordinate as dobulee
* @return longitude as doublee
*/
		double           Long() const;

/**
* Sets longitude using a double
* @param lon - longitude as double
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC    Long(double lon);

/**
* Sets longitude using a CDegMinSec struc
* @param lon - longitude as a CDegMinSec struc
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC    Long(CDegMinSec lon);

/**
* Returns longitude of coordinate as CDegMinSec struc
* @return longitude as CDegMinSec
*/
		CDegMinSec       LongDegMinSec();

/**
* Equality test operator overload
*/
        bool operator == (const CLatLongPoint& p) const;

/**
* Inequality test operator overload
*/
        bool operator != (const CLatLongPoint& p) const;


	private:
	    void             AdjustCoordinateSoNoLongerInvalid();
		double           ConvertDegMinSecToDouble(CDegMinSec d);
		CDegMinSec       ConvertDoubleToDegMinSec(double d);
		MAP_SYSTEM_RC    IsLatitudeValid();
		MAP_SYSTEM_RC    IsLongitudeValid();

	private:

		double      _height;
		static bool _isAutoCorrectOn;
		static bool _isErrorCheckingOn;
		double      _lat;
		double      _long;

};

#endif
