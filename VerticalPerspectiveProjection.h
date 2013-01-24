/*****************************************************************************/
// Filename: VerticalPerspectiveProjection.h
/*****************************************************************************/
// Description: This class represents a vertical perspective projection of
// the Azimuthal projection family. This projection is a true three-dimensional
// projection, hence it's significantly more complex and slower in its projection equations.
// The inverse projection method is even slower than the forward equations.
// The vertical perspective projection is a fixed form of the general perspective
// projection, is a representation of Earth as if viewed from outer space. This class
// allows adjusting the perspective distance as well a the height of projected points
// in respect to sea level (very useful for showing 3D surfaces like terrain)
/*****************************************************************************/

#ifndef _VERTICAL_PERSPECTIVE_PROJECTION_H
#define _VERTICAL_PERSPECTIVE_PROJECTION_H

#include <AzimuthalProjection.h>

/**
* This class represents a Vertical Perspective Projection
*/

class CVerticalPerspectiveProjection : public CAzimuthalProjection
{
	public:

/**
* A constructor for this class
*/
		CVerticalPerspectiveProjection();

/**
* A destructor for this class
*/
		virtual ~CVerticalPerspectiveProjection();

/**
* Height of central point (intersection of meridiana and parallel of origin) above sea level
* @return height above sea level of central point in meters as double
*/
		double                 HeightOfCenterAboveSeaLevel() const; //h0

/**
* The distance of the "eye" perpendicular to the surface of the Earth.
* @return perspective distance in meters as a double
*/
		double                 HeightOfPerspectivePoint() const; //H

/**
* The distance of potential future projected points above sea level (state value)
* @return height in meters of potential projected points above sea level as double
*/
		double                 HeightOfPotentialProjectedPointsAboveSeaLevel() const; //h

/**
* This method inverts a Cartesian point on a Vertical Perspective projection back to a geographical coordinate
* Note - The meridian of origin and parallel of origin should be set back to what they were
* originally to what they were during the forward projection.
* @param p - Cartesian point
* @param worldModel - Reference ellipsoid the point was originally generated with
* @return A CLatLongPoint referencing the passed ellipsoid
*/
		virtual CLatLongPoint  InverseProjectPoint(const CXYPoint& p, const CWorld3DModel& worldModel);

/**
* Provides a method of culling \when rasterizing points. This method will return false
* if a point is beyond line of sight (i.e. on other side of the world) and true otherwise.
* If this method is NOT used when drawing, the result is a wireframe model of the earth
* where one can see through.
* @return boolean true if point is visible and can be plotted, else false
*/
		virtual bool           IsPointPlottable (const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method projects a geographical point to an XY point using the Vertical Perspective projection.
* The origin of the projected grid is the intersection of the parallel of origin (x-axis)
* and meridian of origin (y-axis).
* @param p - Geographical point
* @param worldModel - Reference ellipsoid the coordinates are relative to
* @return A CXYPoint representing projected point (x and y in meters)
*/
		virtual CXYPoint       ProjectPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* This method returns 1 everywhere as scale cannot be calculated on this projection.
* @return scale factor as double
*/
		virtual double         ScaleFactorForPoint(const CLatLongPoint& p, const CWorld3DModel& worldModel);

/**
* Set method which sets the height of the central point (intersection of origin meridian
* and parallel) above sea level
* @param meters - height of central point in meters above sea level
*/
		void                   SetHeightOfCenterAboveSeaLevel(double meters);

/**
* Set method which sets the distance of the "eye" perpendicular to the surface of the
* Earth.
* @param meters - height perpendicular to surface of the Earth of viewing point as meters
*/
		void                   SetHeightOfPerspectivePoint(double meters);
/**
* Set method which sets height of any potentially projected points above sea level. By
* default, all points are projected on surface
* @param meters - height of potential projected points above sea level in meters
*/
		void                   SetHeightOfPotentialProjectedPointsAboveSeaLevel(double meters);
/**
* This method allows figuring out the type of projection this is using virtual lookup.
* @return A PROJECTED_MODEL_TYPE, in this case = VERTICAL_PERSPECTIVE
*/
		virtual PROJECTED_MODEL_TYPE WhichProjectionAmI() const;

	private:

		//Forward Calculation Methods
		double Calculate_N(double phi, const CWorld3DModel& worldModel) const;
		double Calculate_C(const CLatLongPoint& p, const CWorld3DModel& worldModel) const;
		double Calculate_S(const CLatLongPoint& p, const CWorld3DModel& worldModel) const;
		double Calculate_phiG_SingleIteration(double P, const CWorld3DModel& worldModel) const;
		void   CalculateBoth_phiG_AND_P(double& phiG, double& P, const CWorld3DModel& worldModel);
		double Calculate_P_SingleIteration(double phiG, const CWorld3DModel& worldModel) const;
		double Calculate_K(const CLatLongPoint& p, const CWorld3DModel& worldModel);

		//Inverse Calculation Methods
		double Calculate_B(const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_D(const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_u(const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_v(const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_G(const CWorld3DModel& worldModel);
		double Calculate_L(const CWorld3DModel& worldModel);
		double Calculate_J(const CWorld3DModel& worldModel);
		double Calculate_t(double E, const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_KPrime(double u, double v, double t, const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_X(double KPrime, const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_Y(double KPrime, const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_S_Inverse(double KPrime, const CXYPoint& p, const CWorld3DModel& worldModel);
		double Calculate_lambda(double X, double Y, const CWorld3DModel& worldModel);
		double Calculate_phi(double phiLast, double S, const CWorld3DModel& worldModel);
		double Calculate_E(double phi, const CWorld3DModel& worldModel);
		void   CalculateBoth_phi_AND_lambda(double& phi, double& lambda, const CXYPoint& p, const CWorld3DModel& worldModel);

	private:
		double _heightOfCenterAboveSeaLevel;
		double _heightOfPerspectivePoint;
		double _heightofPotentialProjectedPointsAboveSeaLevel;

};

#endif
