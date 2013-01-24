/*****************************************************************************/
// Filename: EquidistantConicProjection.h
/*****************************************************************************/
// Description: This class represent an equidistant conic projection, which
// belongs to the conical projection family.
/*****************************************************************************/

#ifndef _EQUIDISTANT_CONIC_PROJECTION_H
#define _EQUIDISTANT_CONIC_PROJECTION_H

#include <ConicalProjection.h>

/**
* This class represents an equidistant conic projection.
*/
class CEquidistantConicProjection : public CConicalProjection
{
	public:
		CEquidistantConicProjection();
		virtual ~CEquidistantConicProjection();

/**
* This method inverts a Cartesian point on a Equidistant Conic projection back to a geographical coordinate
* Note - The meridian of origin and parallel of origin should be set back to what they were
* originally to what they were during the forward projection.
* @param p - Cartesian point
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return A CLatLongPoint referencing the passed ellipsoid
*/
		virtual CLatLongPoint       InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel);

/**
* This method projects a geographical point to an XY point using the Equidistant Conic projection.
* The origin of the projected grid is the intersection of the parallel of origin (x-axis)
* and meridian of origin (y-axis).
* @param p - Geographical point
* @param worldModel - Reference ellipsoid the coordinates are relative to
* @return A CXYPoint representing projected point (x and y in meters)
*/
		virtual CXYPoint             ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method gives the scale factor for specified point on Equidistant Conic projection
* @param p - Geographical point the coordinates are relative to
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return Scale factor for point as a double
*/
		virtual double               ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method allows figuring out the type of projection this is using virtual lookup.
* @return A PROJECTED_MODEL_TYPE, in this case = EQUIDISTANT_CONIC
*/
		virtual PROJECTED_MODEL_TYPE WhichProjectionAmI() const;

private:
	double Calculate_G(const CWorld3DModel& worldModel) const;
	double Calculate_m(double phi, const CWorld3DModel& worldModel) const;
	double Calculate_M(double phi, const CWorld3DModel& worldModel) const;
	double Calculate_n(const CWorld3DModel& worldModel) const;
	double Calculate_rho(double M, const CWorld3DModel& worldModel) const;
	double Calculate_theta(const CLatLongPoint& p, const CWorld3DModel& worldModel) const;

	double Calculate_e1(const CWorld3DModel& worldModel) const;
	double Calculate_mu(const CXYPoint& p, const CWorld3DModel& worldModel) const;
	double Calculate_M_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const;
	double Calculate_rho_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const;
	double Calculate_theta_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const;
};

#endif
