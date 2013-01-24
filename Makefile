# Makefile for libMapSystemCore
# Mirza A. Shah

CC = g++
CFLAGS = -O0 -fPIC -g -I. -c -DLINUX
OUT = ../Debug/libMapSystemCore.a

OBJS =	AzimuthalProjection.o \
		DegMinSec.o \
		InternationalModel.o \
		Projected2DModel.o \
		WGS84Model.o \
		Clarke1866Model.o \
		EquidistantConicProjection.o \
		LatLongPoint.o \
		StereographicProjection.o \
		World3DModel.o \
		ConicalProjection.o \
		GnomonicProjection.o \
		MathFunctions.o \
		TransverseMercatorProjection.o \
		XYPoint.o \
		CylindricalProjection.o \
		GreatCircleRouteHelper.o \
		MercatorProjection.o \
		VerticalPerspectiveProjection.o

all:	createLibrary

createLibrary:	$(OBJS)
		ar rcs $(OUT) $(OBJS) 
.cpp.o:
	$(CC) $(CFLAGS) -o $*.o $*.cpp

clean:
	rm -f $(OBJS) $(OUT) *~
