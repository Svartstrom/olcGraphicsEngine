#!/usr/bin/make -f

CXXFLAGS=-lX11 -lGL -lpthread -lpng -lstdc++fs
CXX=g++

#main: 3dCube_v4# 3dCube_v2 3dCube_v4
main: 3dCube_v4.o 3d_triangle.o 3d_vec3d.o 3d_mat4x4.o 3d_camera.o 3d_utilities.o
	$(CXX) -o 3dCube_v4 3dCube_v4.o 3d_triangle.o 3d_vec3d.o 3d_mat4x4.o 3d_camera.o 3d_utilities.o $(CXXFLAGS)

4d: 4dCube.o 3d_triangle.o 3d_vec3d.o 3d_mat4x4.o 3d_camera.o 3d_utilities.o 
	$(CXX) -o 4dCube 4dCube.o 3d_triangle.o 3d_vec3d.o 3d_mat4x4.o 3d_camera.o 3d_utilities.o $(CXXFLAGS)

3dCube_v4.o: 3dCube_v4.cpp 3d_vec3d.hpp 3d_mesh.hpp 3d_mat4x4.hpp 3d_triangle.hpp 3d_camera.hpp
4dCube.o: 4dCube.cpp 3d_vec3d.hpp 3d_mesh.hpp 3d_mat4x4.hpp 3d_triangle.hpp 3d_camera.hpp 3d_utilities.hpp
3d_camera.o: 3d_camera.hpp 3d_vec3d.hpp olcPixelGameEngine.h
3d_triangle.o: 3d_triangle.hpp 3d_vec3d.hpp
3d_vec3d.o: 3d_vec3d.hpp 3d_mat4x4.hpp
3d_mat4x4.o: 3d_mat4x4.hpp
3d_utilities.o: 3d_utilities.hpp 3d_mat4x4.hpp 3d_vec3d.hpp
