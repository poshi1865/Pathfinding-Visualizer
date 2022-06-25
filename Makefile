main : src/main.cpp src/Node.cpp include/screen.hpp include/colors.hpp 
	g++ src/main.cpp src/Node.cpp include/screen.hpp -lGL -lglut -lGLU -o main
