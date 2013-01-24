/*****************************************************************************/
// Filename: World3DModel.h
/*****************************************************************************/
// Description: This class is a base class representing ellipsoids that model
// the Earth geoid. A method has been included to find great circle distance
// between any two points on the ellipsoid using Vicenty's algorithm.
/*****************************************************************************/

#ifndef _WORLD_3D_MODEL
#define _WORLD_3D_MODEL

#include <CoordinateTypes.h>
#include <MapSystemEnumerations.h>

/**
* This class describes a reference ellipsoid used to model the Earth geoid.
*/
class CWorld3DModel
{
	public:
/**
* The one and only constructor, requires two parameters.
* @param semiMajorAxis Defines the semi-major axis of the ellipsoid (also
* commonly denoted as <I>a</I> in mathematical equations).
* @param semiMinorAxis Defines the semi-minor axis of the ellipsoid (also
* commonly denoted as <I>b</I> in mathematic equations).
*/
		CWorld3DModel(double semiMajorAxis, double semiMinorAxis);

/**
* The destructor for the class.
*/
		virtual ~CWorld3DModel();

/**
* Calculates true "great circle" distance between a pair of geographical coordinates using
* algorithm from T. Vincenty's paper "Direct and Inverse Solutions of Geodesics on the
* Ellipsoid with Application of Nested Equations"
* @return True "great circle" distance between two geographical points in meters.
*/
		double                   DistanceBetweenTwoPoints(const CLatLongPoint& p1, const CLatLongPoint& p2);

/**
* Eccentricity is a mathematical property of an ellipsoid, refered to in mathematical equations
* simply as <I>e</I>, not to be confused with the constant <I>e</I> used in
* natural logarithms. <I>e</I> is used frequently in map projection equations.
* @return The eccentricity of the ellipsoid.
*/
		double				     Eccentricity() const;

/**
* Eccentricity squared (or <I>e^2</I>
* as it's referred to in mathematical equations), is a property of an ellipsoid which is
* used frequently in map projection equations.
* @return The square of eccentricity of the ellipsoid.
*/
		double				     EccentricitySquared() const;

/**
* Flattening is a mathematical property of an
* ellipsoid, referred to in mathematic equations simply as <I>f</I>. <I>f</I> is used
* frequently in map projection equations.
* @return The flattening of the ellipsoid.
*/
		double				     Flattening() const;

/**
* The semi-major axis of an ellipsoid specifies the longer of its two semi-axes.
* Also called the equatorial radius.
* @return The semi-major axis in meters of the ellipsoid
*/
		double				     SemiMajorAxis() const;

/**
* The semi-minor axis of an ellipsoid specifies the shorter of its two semi-axes.
* Also called the polar radius.
* @return The semi-minor axis in meters of the ellipsoid
*/
		double				     SemiMinorAxis() const;

/**
* This method can determine the type of ellipsoid we're using run-time dynamic dispatch.
* @return A WORLD_MODEL_TYPE indicating which subclass the current instance belongs
* to.
*/
		virtual WORLD_MODEL_TYPE WhichWorldModelAmI() const = 0;

	private:
		void                     PreCalculateEccentricityAndEccentrictySquared();
		void                     PreCalculateFlattening();

	private:

		double              _eccentricity;
		double              _eccentricitySquared;
		double              _flattening;

/** Member variable that holds the ellipsoid semi-major axis in meters. */
		double              _semiMajorAxis;

/** Member variable that holds the ellipsoid semi-major axis in meters. */
		double              _semiMinorAxis;


};

#endif
