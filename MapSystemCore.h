/*****************************************************************************/
// Filename: MapSystem.h
/*****************************************************************************/
// Description: This header file does a mass include of everything in the library
//for convenience
/*****************************************************************************/

#ifndef _MAP_SYSTEM_CORE_H
#define _MAP_SYSTEM_CORE_H

//Coordinate Systems
#include <CoordinateTypes.h>

//Reference Ellipsoids
#include <WGS84Model.h>
#include <Clarke1866Model.h>
#include <InternationalModel.h>

//Projections
#include <TransverseMercatorProjection.h>
#include <MercatorProjection.h>
#include <StereographicProjection.h>
#include <VerticalPerspectiveProjection.h>
#include <EquidistantConicProjection.h>

//Greater Circle Route Helper
#include <GreatCircleRouteHelper.h>

//Math Functions
#include <MathFunctions.h>

//Enumerated Types
#include <MapSystemEnumerations.h>



#endif
