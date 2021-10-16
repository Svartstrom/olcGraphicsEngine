#!/usr/bin/make -f

CXXFLAGS=-lX11 -lGL -lpthread -lpng -lstdc++fs
CXX=g++

#main: 3dCube_v4# 3dCube_v2 3dCube_v4
main: besier.o point2d.o 
	$(CXX) -o besier besier.o point2d.o $(CXXFLAGS)

besies.o: besier.cpp
point2d.o: point2d.cpp
