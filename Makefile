main : src/main.cpp src/Node.cpp include/screen.hpp include/colors.hpp stb_image.h 
	g++ src/main.cpp src/Node.cpp include/screen.hpp -lGL -lglut -lGLU -o main
