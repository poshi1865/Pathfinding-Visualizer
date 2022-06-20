#ifndef NODE_H_
#define NODE_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include "screen.h"

#define NODE_NORMAL 1
#define NODE_WALL 2
#define NODE_SOURCE 3
#define NODE_DESTINATION 4

class Node {
    private:
        float x;
        float y;
        float w;
        float h;

        float *color;

        int type;
    public:
        Node();
        Node(float x, float y, float w, float h, int type); 

        void drawNode(); 

        bool hasInside(int x, int y);

        void setType(int type); 

        int getType();

        bool isSame(Node* a); 
};

#endif
