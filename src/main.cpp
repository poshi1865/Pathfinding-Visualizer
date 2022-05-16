#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "../include/Node.h"

#define WIDTH 840
#define HEIGHT 840

void render(void);
void handleMouse(int button, int state, int x, int y);
void update(int a);
void init();

const int nodeSideLength = 30;
const int numberOfNodes = (WIDTH / nodeSideLength) * (HEIGHT / nodeSideLength);

Node n[numberOfNodes];

int mouseHold = 0;

void init() {
    n[0] = Node(1, 1, nodeSideLength, nodeSideLength);
    int x = 1;
    int y = 1;
    for (int i = 1; i < numberOfNodes; i++) {
        if (x >= WIDTH) {
            x = 1;
            y += nodeSideLength;
        }
        n[i] = Node(x, y, nodeSideLength, nodeSideLength);
        x += nodeSideLength;
    }
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Pathfinder");

    glutDisplayFunc(render);
    glutMouseFunc(handleMouse);
    update(0);

    glClearColor(1.0, 1.0, 1.0, 0.0);
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);

    init();

    glutMainLoop();

    return 0;
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    for (int i = 0; i < numberOfNodes; i++) {
        n[i].drawNode();
    }
    
    glutSwapBuffers();
}
void update(int a) {
    glutTimerFunc(1000 / 60, update, 0);
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseHold = 1;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        mouseHold = 1;
    }
    if (state == GLUT_UP) {
        mouseHold = 0;
    }
}
