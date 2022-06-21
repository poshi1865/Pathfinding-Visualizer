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
#define NODE_VISITED 5

class Node {
    private:
        float x;
        float y;
        float w;
        float h;

        float *color;

        int type;

        int cellNumber;

    public:
        Node();
        Node(float x, float y, float w, float h, int type, int cellNumber); 

        void drawNode(); 

        bool hasInside(int x, int y);

        void setType(int type); 

        int getType();

        bool isSame(Node* a); 

        int getCellNumber();

        bool isVisited();

        int* getAdjacentNodeIndex();
};

#endif
