#!/usr/bin/make -f

CXXFLAGS=-lX11 -lGL -lpthread -lpng -lstdc++fs
CXX=g++

MAIN_OFILES=besier.o point2d.o besiercurve.o
#main: 3dCube_v4# 3dCube_v2 3dCube_v4
main: $(MAIN_OFILES)
	$(CXX) -o besier $(MAIN_OFILES) $(CXXFLAGS)

besies.o: besier.cpp
point2d.o: point2d.cpp
besiercurve.o: besiercurve.cpp point2d.cpp
