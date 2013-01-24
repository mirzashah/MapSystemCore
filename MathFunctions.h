/*****************************************************************************/
// Filename: MathFunctions.h
/*****************************************************************************/
// Description: This is a simple library for performing basic math functions
// used by the MapSystem library.
/*****************************************************************************/

#ifndef _MATH_FUNCTIONS_H
#define _MATH_FUNCTIONS_H

//#include <math.h>
#include <cmath>
using namespace std;
const double PI = 3.14159265;

double cosDeg(double deg);
double sinDeg(double deg);
double tanDeg(double deg);
double toRad(double deg);
double toDeg(double rad);


#endif
