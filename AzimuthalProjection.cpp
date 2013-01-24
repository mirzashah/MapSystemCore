#include <AzimuthalProjection.h>

/****************************************************/
CAzimuthalProjection::CAzimuthalProjection()
/****************************************************/
{
}

/****************************************************/
CAzimuthalProjection::~CAzimuthalProjection()
/****************************************************/
{
}

/**************************************************************/
double CAzimuthalProjection::CentralScaleFactor() const
/**************************************************************/
{
	return (_centralScaleFactor);
}

/**************************************************************/
void CAzimuthalProjection::SetCentralScaleFactor(double centralScaleFactor)
/**************************************************************/
{
	_centralScaleFactor = centralScaleFactor;
}
