/*****************************************************************************/
// Filename: StereographicProjection.h
/*****************************************************************************/
// Description: This class represents a stereographic projection. A stereographic
// projection is one of the most common types of azimuthal projections. If a
// sphere is used for the projection (in contrast to some reference ellipsoid like
// WGS-84), the lines from the center outward are azimuthal, or the correct
// great circle distance. This is not the case with a reference ellipsoid, it's a
// little off, but still quite accurate. A stereographic projection is generated
// by projection outward from the point antipodal to the tangent point for the plane
// on which the map is projected...kind of like looking through a crystal ball
// from one side and looking across to the other. A stereographic projection can
// be oblique, equatorial, or polar, this class can cover all 3 depending on what
// the parallel of origin is chosen as.
/*****************************************************************************/

#ifndef _STEREOGRAPHIC_PROJECTION_H
#define _STEREOGRAPHIC_PROJECTION_H

#include <AzimuthalProjection.h>
#include <World3DModel.h>

class CStereographicProjection : public CAzimuthalProjection
{
	public:

/**
* Default constructor
*/
		CStereographicProjection();

/**
* A destructor for this class
*/
		virtual ~CStereographicProjection();

/**
* This method inverts a Cartesian point on a Stereographic projection back to a geographical coordinate
* Note - The meridian of origin and parallel of origin should be set back to what they were
* originally to what they were during the forward projection.
* @param p - Cartesian point
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return A CLatLongPoint referencing the passed ellipsoid
*/
		virtual CLatLongPoint  InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel);

/**
* This method projects a geographical point to an XY point using the Stereographic projection.
* The origin of the projected grid is the intersection of the parallel of origin (x-axis)
* and meridian of origin (y-axis).
* @param p - Geographical point
* @param worldModel - Reference ellipsoid the coordinates are relative to
* @return A CXYPoint representing projected point (x and y in meters)
*/
		virtual CXYPoint       ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method gives the scale factor for specified point on Stereographic projection
* @param p - Geographical point the coordinates are relative to
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return Scale factor for point as a double
*/
		virtual double         ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method allows figuring out the type of projection this is using virtual lookup.
* @return A PROJECTED_MODEL_TYPE, in this case = STEREOGRAPHIC
*/
		virtual PROJECTED_MODEL_TYPE WhichProjectionAmI() const;

	private:
		double         AdjustedLambdaValue(double lambda, double atanNumerator, double atanDenominator, bool isSouthPole = false);

		double         Calculate_A(const CLatLongPoint& p, const CWorld3DModel& worldModel) const;
		double         Calculate_ce(const CXYPoint& p, const CWorld3DModel& worldModel) const;
		double         Calculate_chi_Forward(double phi, const CWorld3DModel& worldModel) const;
		double         Calculate_chi_Inverse(const CXYPoint& p, const CWorld3DModel& worldModel) const;
		double         Calculate_lambda(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_lambda_At90North(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_lambda_At90South(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_lambda_AtNon90(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_m(double phi, const CWorld3DModel& worldModel) const;
		double         Calculate_phi(const CXYPoint& p, const CWorld3DModel& worldModel);
        double         Calculate_phi_At90North(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_phi_At90South(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_phi_AtNon90(const CXYPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_rho(const double& phi, const CWorld3DModel& worldModel); //forward
		double         Calculate_rho(const CXYPoint& p, const CWorld3DModel& worldModel) const; //inverse
		double         Calculate_t(const double& phi, const CWorld3DModel& worldModel);
		double         Calculate_x(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_x_At90North(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_x_At90South(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_x_AtNon90(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_y(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_y_At90North(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_y_At90South(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		double         Calculate_y_AtNon90(const CLatLongPoint& p, const CWorld3DModel& worldModel);
		bool           IsOriginAtNorthPole();
		bool           IsOriginAtSouthPole();

};

#endif
