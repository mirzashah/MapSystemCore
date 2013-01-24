#include <ConicalProjection.h>

/****************************************************/
CConicalProjection::CConicalProjection()
/****************************************************/
{
	SetSecondStandardParallel(60.0);
	SetFirstStandardParallel(20.0);
}

/****************************************************/
CConicalProjection::~CConicalProjection()
/****************************************************/
{
}

/****************************************************/
double CConicalProjection::FirstStandardParallel() const
/****************************************************/
{
	return (_firstStandardParallel);
}

/****************************************************/
double CConicalProjection::SecondStandardParallel() const
/****************************************************/
{
	return (_secondStandardParallel);
}

/****************************************************/
void CConicalProjection::SetFirstStandardParallel(double lat)
/****************************************************/
{
	_firstStandardParallel = lat;
}

/****************************************************/
void CConicalProjection::SetSecondStandardParallel(double lat)
/****************************************************/
{
	_secondStandardParallel = lat;
}
