#include <DegMinSec.h>

/***************************************************/
CDegMinSec::CDegMinSec()
/***************************************************/
{
	Deg(0);
	Min(0);
	Sec(0);
}

/***************************************************/
CDegMinSec::CDegMinSec(double degMinSec)
/***************************************************/
{
	(*this) = CDegMinSec::ConvertDoubleToDegMinSec(degMinSec);
}

/***************************************************/
CDegMinSec::CDegMinSec(int deg, int min, double sec)
/***************************************************/
{
	Deg(deg);
	Min(min);
	Sec(sec);
}

/***************************************************/
CDegMinSec::~CDegMinSec()
/***************************************************/
{
}

/***************************************************/
double CDegMinSec::ConvertDegMinSecToDouble(CDegMinSec degMinSec)
/***************************************************/
{
	//TODO: Test this method
	double toReturn = (double)degMinSec.Deg() + (((double)(degMinSec.Min()))/60.0) + (((double)(degMinSec.Sec()))/3600.0);
	return (toReturn);
}

/***************************************************/
CDegMinSec CDegMinSec::ConvertDoubleToDegMinSec(double degMinSec)
/***************************************************/
{
	//TODO: Test this method
	CDegMinSec toReturn;
	double degDouble;
	double minDouble;
	double secDouble;

	degDouble = degMinSec;                          //Degrees (decimal)
	toReturn.Deg((int)degDouble);                   //Truncate off decimal to get degrees whole number
	double remainder1 = degDouble - toReturn.Deg(); //Get remainder decimal
	
	minDouble = (60.0*remainder1);                  //Get minutes (decimal)
	toReturn.Min((int)minDouble);                   //Truncate to get minutes whole number
	double remainder2 = minDouble - toReturn.Min();

	secDouble = remainder2*60.0;                    //Get seconds (decimals)
	toReturn.Sec(secDouble);                        //Truncate to get whole number

	return (toReturn);
}

/***************************************************/
int CDegMinSec::Deg()
/***************************************************/
{
	return (_deg);
}

/***************************************************/
void CDegMinSec::Deg(int deg)
/***************************************************/
{
	_deg = deg;
}

/***************************************************/
int CDegMinSec::Min()
/***************************************************/
{
	return (_min);
}

/***************************************************/
void CDegMinSec::Min(int min)
/***************************************************/
{
	_min = min;
}

/***************************************************/
double CDegMinSec::Sec()
/***************************************************/
{
	return (_sec);
}

/***************************************************/
void CDegMinSec::Sec(double sec)
/***************************************************/
{
	_sec = sec;
}
