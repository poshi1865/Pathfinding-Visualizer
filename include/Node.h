#ifndef NODE_H_
#define NODE_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include "screen.h"

using namespace std;

class Node {
    private:
        float x;
        float y;
        float w;
        float h;

        float *color;

        string type;
    public:
        Node();
        Node(float x, float y, float w, float h, string type); 

        void drawNode(); 

        bool hasInside(int x, int y);

        void setType(string type); 

        string getType();

        bool isSame(Node* a); 
};

#endif
