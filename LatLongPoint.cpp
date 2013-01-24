#include <LatLongPoint.h>
#include <cmath>

bool CLatLongPoint::_isAutoCorrectOn = false; //Initialize static auto correct boolean
bool CLatLongPoint::_isErrorCheckingOn = false; //Initialize static error checking boolean

/**************************************************/
CLatLongPoint::CLatLongPoint()
/**************************************************/
{
	Lat(0.0);
	Long(0.0);
	Height(0.0);
}

/**************************************************/
CLatLongPoint::CLatLongPoint(double lat, double lon, double height)
/**************************************************/
{
	Lat(lat);
	Long(lon);
	Height(height);
}

/**************************************************/
CLatLongPoint::CLatLongPoint(CDegMinSec lat, CDegMinSec lon, double height)
/**************************************************/
{
	Lat(lat);
	Long(lon);
	Height(height);
}

/**************************************************/
CLatLongPoint::~CLatLongPoint()
/**************************************************/
{
}

/**************************************************/
void CLatLongPoint::AdjustCoordinateSoNoLongerInvalid()
/**************************************************/
{
	//Wrap-around technique
	//TODO: Potentially loops could take forever if correction needed is large, change it to
	//something else, maybe modulo
	while (Long() < -180)
	{
		Long(Long()+360);
	}
	while (Long() > 180)
	{
		Long(Long()-360);
	}
	while (Lat() > 90)
	{
		Lat(Lat()-180);
	}
	while (Lat() <-90)
	{
		Lat(Lat()+180);
	}
}

/**************************************************/
double CLatLongPoint::ConvertDegMinSecToDouble(CDegMinSec d)
/**************************************************/
{
	return(CDegMinSec::ConvertDegMinSecToDouble(d));
}

/**************************************************/
CDegMinSec CLatLongPoint::ConvertDoubleToDegMinSec(double d)
/**************************************************/
{
	return(CDegMinSec::ConvertDoubleToDegMinSec(d));
}

/**************************************************/
double CLatLongPoint::Height() const
/**************************************************/
{
	return (_height);
}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::Height(double height)
/**************************************************/
{
	_height = height;
	return (VALID);
}

/**************************************************/
bool CLatLongPoint::IsAutoCorrectOn()
/**************************************************/
{
	return (_isAutoCorrectOn);
}

/**************************************************/
bool CLatLongPoint::IsErrorCheckingOn()
/**************************************************/
{
	return (_isErrorCheckingOn);
}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::IsLatitudeValid()
/**************************************************/
{
	if (IsErrorCheckingOn())
	{
		if ((Lat()<=-90) || (Lat()>=90))
		{
			if (IsAutoCorrectOn())
			{
				AdjustCoordinateSoNoLongerInvalid();
			}
			return (INVALID_LAT_LONG);
		}
	}

	return (VALID);


}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::IsLongitudeValid()
/**************************************************/
{
	if (IsErrorCheckingOn())
	{
		if ((Long()<=-180) || (Long()>=180))
		{
			if (IsAutoCorrectOn())
			{
				AdjustCoordinateSoNoLongerInvalid();
			}
			return (INVALID_LAT_LONG);
		}
	}

	return (VALID);
}

/**************************************************/
double CLatLongPoint::Lat() const
/**************************************************/
{
	return (_lat);
}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::Lat(double lat)
/**************************************************/
{
	_lat = lat;
	return (IsLatitudeValid());
}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::Lat(CDegMinSec lat)
/**************************************************/
{
	_lat = ConvertDegMinSecToDouble(lat);
	return (IsLatitudeValid());
}

/**************************************************/
CDegMinSec CLatLongPoint::LatDegMinSec()
/**************************************************/
{
	return (ConvertDoubleToDegMinSec(Lat()));
}

/**************************************************/
double CLatLongPoint::Long() const
/**************************************************/
{
	return (_long);
}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::Long(double lon)
/**************************************************/
{
	_long = lon;
	return (IsLongitudeValid());
}

/**************************************************/
MAP_SYSTEM_RC CLatLongPoint::Long(CDegMinSec lon)
/**************************************************/
{
	_long = ConvertDegMinSecToDouble(lon);
	return (IsLongitudeValid());
}

/**************************************************/
CDegMinSec CLatLongPoint::LongDegMinSec()
/**************************************************/
{
	return (ConvertDoubleToDegMinSec(Long()));
}

/**************************************************/
bool CLatLongPoint::operator == (const CLatLongPoint& p) const
/**************************************************/
{
	const double threshold = 0.0000000001;
	return ((fabs(p.Lat() - Lat()) <= threshold) && (fabs(p.Long() - Long()) <= threshold));
}

/**************************************************/
bool CLatLongPoint::operator != (const CLatLongPoint& p) const
/**************************************************/
{
    return (!((*this)==p));
}

/**************************************************/
void CLatLongPoint::TurnAutoCorrectOff()
/**************************************************/
{
	_isAutoCorrectOn = false;
}

/**************************************************/
void CLatLongPoint::TurnAutoCorrectOn()
/**************************************************/
{
	_isAutoCorrectOn = true;
}

/**************************************************/
void CLatLongPoint::TurnErrorCheckingOff()
/**************************************************/
{
	_isErrorCheckingOn = false;
}

/**************************************************/
void CLatLongPoint::TurnErrorCheckingOn()
/**************************************************/
{
	_isErrorCheckingOn = true;
}
