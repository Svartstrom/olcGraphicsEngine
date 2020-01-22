main: 3dCube 3dCube_v2 3dCube_v4

3dCube: 3dCube.cpp
	g++ -o 3dCube 3dCube.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs

3dCube_v2: 3dCube_v2.cpp
	g++ -o 3dCube_v2 3dCube_v2.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs

3dCube_v4: 3dCube_v4.cpp
	g++ -o 3dCube_v4 3dCube_v4.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs
