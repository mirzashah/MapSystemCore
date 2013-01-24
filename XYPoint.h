/*****************************************************************************/
// Filename: XYPoint.h
/*****************************************************************************/
// Description: This class represents a Cartesian XY point, corresponds with
// CLatLongPoint
/*****************************************************************************/

#ifndef _XY_POINT_H
#define _XY_POINT_H

#include <MapSystemEnumerations.h>

/**
* This class represents an XY (Cartesian) point.
*/
class CXYPoint
{
	public:
/**
* Default constructor for this class
*/
		CXYPoint();
/**
* Constructor for this class
* @param x - x value
* @param y - y value
*/
		CXYPoint(double x, double y, double z = 0);

/**
* A destructor for this class
*/
		~CXYPoint();

/**
* Equality test operator overload
*/
        bool operator == (const CXYPoint& p) const;

/**
* Inequality test operator overload
*/
        bool operator != (const CXYPoint& p) const;

/**
* Returns x value
* @return X value
*/
		double         X() const;

/**
* Sets x value
* @param x - new value of x
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC  X(double x);

/**
* Returns y value
* @return Y value
*/
		double         Y() const;

/**
* Sets y value
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC  Y(double y);

/**
* Returns z value
* @return z value
*/
		double         Z() const;

/**
* Sets z value
* @return MAP_SYSTEM_RC (Enumerated Error Code)
*/
		MAP_SYSTEM_RC  Z(double z);


	private:
		double _x;
		double _y;
		double _z;
};

#endif


