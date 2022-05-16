#ifndef NODE_H_
#define NODE_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
using namespace std;

class Node {
    public:
        float x;
        float y;
        float w;
        float h;

        float *color;

    public:
        Node() {
        }
        Node(float x, float y, float w, float h, GLfloat color[]) {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->color = color;
        }

        void drawNode() {
            glLineWidth(3);

            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y);
                glVertex2f(x + w, y);
                glVertex2f(x + w, y + h);
                glVertex2f(x, y + h);
            glEnd();

            glColor3fv(color);
            glBegin(GL_POINTS);
                for (int i = x; i < x + w - 1; i++) {
                    for (int j = y+1; j < y + h; j++) {
                        glVertex2f(i, j);
                    }
                }
            glEnd();
        }
};

#endif
