#include "../include/Node.hpp"
#include <string>
#include "../include/screen.hpp"
#include "../include/colors.hpp"

Node::Node() {
}

Node::Node(float x, float y, float w, float h, int type, int cellNumber) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
    this->cellNumber = cellNumber;
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
    if (type == NODE_NORMAL ||  type == NODE_WALL || type == NODE_SOURCE || type == NODE_DESTINATION || type == NODE_VISITED || type == NODE_BOUNDARY || type == NODE_TRACE) {
        this->type = type;

        if (this->type == NODE_NORMAL) {
            this->color = COLOR_WHITE;
        }
        else if (this->type == NODE_WALL) {
            this->color = COLOR_BROWN;
        }
        else if (this->type == NODE_SOURCE) {
            this->color = COLOR_BLUE;
        }
        else if (this->type == NODE_DESTINATION) {
            this->color = COLOR_GREEN;
        }
        else if (this->type == NODE_VISITED) {
            this->color = COLOR_CYAN;
        }
        else if (this->type == NODE_BOUNDARY) {
            this->color = COLOR_BLACK;
        }
        else if (this->type == NODE_TRACE) {
            this->color = COLOR_YELLOW;
        }
    }
}

int Node::getType() {
    return this->type;
}

int Node::getCellNumber() {
    return this->cellNumber;
}

int* Node::getAdjacentNodeIndex() {
    int n = this->getCellNumber() + 1;
    int* ar = (int*)malloc(sizeof(int) * 4);
    int* result = ar;

    *ar = n;

    ar++;
    int temp = this->getCellNumber() - 1;
    *ar = temp;

    ar++;
    temp = this->getCellNumber() + 42;
    *ar = temp;

    ar++;
    temp = this->getCellNumber() - 42;
    *ar = temp;

    return result;
}

int Node::getDistanceFromSource() {
    return this->distanceFromSource;
}

void Node::setDistanceFromSource(int n) {
    this->distanceFromSource = n;
}

bool Node::isSame(Node* a) {
    if (this->x == a->x && this->y == a->y) {
        return true;
    }
    return false;
}

bool Node::isVisited() {
    if (this->type == NODE_VISITED) {
        return true;
    }
    return false;
}

bool Node::isBoundary() {
    if (this->getCellNumber() >= 1 && this->getCellNumber() <= 42) {
        return true;
    }
    else if (this->getCellNumber() >= 1135 && this->getCellNumber() <= 1176) {
        return true;
    }
    else if (this->getCellNumber() % 42 == 1) {
        return true;
    }
    else if (this->getCellNumber() % 42 == 0) {
        return true;
    }
    return false;
}
