
#include "../include/Node.h"
#include <string>
#include "../include/screen.h"
#include "../include/colors.h"


Node::Node() {
}

Node::Node(float x, float y, float w, float h, int type) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
    setType(type);
}

void Node::drawNode() {
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

bool Node::hasInside(int x, int y) {
    if (x >= this->x && x <= this->x + this->w) {
        if (HEIGHT - y >= this->y && HEIGHT - y <= this->y + this->h) 
            return true;
    }
    return false;
}

void Node::setType(int type) {
    if (type == NODE_NORMAL ||  type == NODE_WALL || type == NODE_SOURCE || type == NODE_DESTINATION) {
        this->type = type;

        if (this->type == NODE_NORMAL) {
            this->color = COLOR_WHITE;
        }
        else if (this->type == NODE_WALL) {
            this->color = COLOR_BLACK;
        }
        else if (this->type == NODE_SOURCE) {
            this->color = COLOR_BLUE;
        }
        else if (this->type == NODE_DESTINATION) {
            this->color = COLOR_GREEN;
        }
    }
}

int Node::getType() {
    return this->type;
}

bool Node::isSame(Node* a) {
    if (this->x == a->x && this->y == a->y) {
        return true;
    }
    return false;
}

