#ifndef NODE_H_
#define NODE_H_

#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include "screen.h"
#include "colors.h"

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
        Node() {
        }
        Node(float x, float y, float w, float h, string type) {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
            this->type = type;
            setType(type);
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
        bool hasInside(int x, int y) {
            if (x >= this->x && x <= this->x + this->w) {
                if (HEIGHT - y >= this->y && HEIGHT - y <= this->y + this->h) 
                    return true;
            }
            return false;
        }
        void setColor(GLfloat color[]) {
            this->color = color;
        }

        void setType(string type) {
            if (type == "normal" ||  type == "wall" || type == "source" || type == "destination") {
                this->type = type;

                if (this->type == "normal") {
                    this->color = COLOR_WHITE;
                }
                else if (this->type == "wall") {
                    this->color = COLOR_BLACK;
                }
                else if (this->type == "source") {
                    this->color = COLOR_BLUE;
                }
                else if (this->type == "destination") {
                    this->color = COLOR_GREEN;
                }
            }
        }
};

#endif
