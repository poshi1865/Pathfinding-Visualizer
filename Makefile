main : src/SpriteSheet.cpp src/main.cpp src/Node.cpp include/screen.h include/colors.h stb_image.h 
	g++ src/main.cpp src/Node.cpp src/SpriteSheet.cpp include/screen.h -lGL -lglut -lGLU -o main
