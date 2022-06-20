#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "../include/Node.h"
#include "../include/SpriteSheet.h"

void render(void);
void resize(int width, int height);
void handleMouseMotion(int x, int y);
void handleMouseClick(int button, int state, int x, int y);
void handleKeyboardPress(unsigned char key, int x, int y);
void update(int a);
void init();

int mouseClick ;

int mouseX;
int mouseY;
int keyPressed;
bool mouseDown = false;

const int nodeSideLength = 30;
const int numberOfNodes = (WIDTH / nodeSideLength) * (HEIGHT / nodeSideLength) + 1;

Node node[numberOfNodes];

void init() {
    node[0] = Node(1, 1, nodeSideLength, nodeSideLength, "normal");
    int x = 1;
    int y = 1;
    for (int i = 1; i < numberOfNodes; i++) {
        if (x >= WIDTH) {
            x = 1;
            y += nodeSideLength;
        }
        node[i] = Node(x, y, nodeSideLength, nodeSideLength, "normal");
        x += nodeSideLength;
    }
}

void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    for (int i = 0; i < numberOfNodes; i++) {
        node[i].drawNode();
    }
    
    glutSwapBuffers();
}

void update(int a) {
    glutTimerFunc(1000 / 120, update, 0);
    
    if (mouseDown) {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                if (mouseClick == 1) {
                    node[i].setType("wall");
                }
                else if (mouseClick == 2) {
                    node[i].setType("normal");
                }
            }
        }
    }

    //Repaint the display
    glutPostRedisplay();
}

void handleMouseMotion(int x, int y) {
    mouseX = x;
    mouseY = y;
}

void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseClick = 1;
        mouseDown = true;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        mouseClick = 2;
        mouseDown = true;
    }
    if (state == GLUT_UP) {
        mouseClick = 0;
        mouseDown = false;
    }
}

void handleKeyboardPress(unsigned char key, int x, int y) {
    keyPressed = key;
    if (keyPressed == 'r') {
        for (int i = 0; i < numberOfNodes; i++) {
            node[i].setType("normal");
        }
    }

    else if (keyPressed == 's') {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                node[i].setType("source");
                break;
            }
        }
    }

    else if (keyPressed == 'd') {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                node[i].setType("destination");
                break;
            }
        }
    }
}

void resize(int width, int height) {
    glutReshapeWindow(WIDTH, HEIGHT);
}

void tempAnimation() {

}

int main(int argc, char** argv) {
    //TEST CODE

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowSize(WIDTH, HEIGHT);
    glutReshapeFunc(resize);
    glutCreateWindow("Pathfinder");

    glutDisplayFunc(render);
    glutPassiveMotionFunc(handleMouseMotion);
    glutMotionFunc(handleMouseMotion);
    glutMouseFunc(handleMouseClick);
    glutKeyboardFunc(handleKeyboardPress);
    update(0);

    glClearColor(1.0, 1.0, 1.0, 0.0);
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);

    init();

    glutMainLoop();

    return 0;
}

















