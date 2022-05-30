main : src/main.cpp src/Node.cpp include/screen.h include/colors.h 
	g++ src/main.cpp src/Node.cpp include/screen.h -lGL -lglut -lGLU -o main
