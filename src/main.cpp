#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "../include/Node.h"
#include "../include/SpriteSheet.h"

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2

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

//This function creates objects for all of the nodes on the grid
void init() {
    node[0] = Node(1, 1, nodeSideLength, nodeSideLength, NODE_NORMAL);
    int x = 1;
    int y = 1;
    for (int i = 1; i < numberOfNodes; i++) {
        if (x >= WIDTH) {
            x = 1;
            y += nodeSideLength;
        }
        node[i] = Node(x, y, nodeSideLength, nodeSideLength, NODE_NORMAL);
        x += nodeSideLength;
    }
}

//The function for rendering objects on the screen
void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    for (int i = 0; i < numberOfNodes; i++) {
        node[i].drawNode();
    }
    
    glutSwapBuffers();
}

//Updates different objects
void update(int a) {
    glutTimerFunc(1000 / 120, update, 0);
    
    if (mouseDown) {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                if (mouseClick == MOUSE_LEFT) {
                    node[i].setType(NODE_WALL);
                }
                else if (mouseClick == MOUSE_RIGHT) {
                    node[i].setType(NODE_NORMAL);
                }
            }
        }
    }

    //Repaint the display
    glutPostRedisplay();
}

//assigns the current mouse position to global variables mouseX and mouseY
void handleMouseMotion(int x, int y) {
    mouseX = x;
    mouseY = y;
}

//assigns values to global variables mouseClick and mouseDown
void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseClick = MOUSE_LEFT;
        mouseDown = true;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        mouseClick = MOUSE_RIGHT;
        mouseDown = true;
    }
    if (state == GLUT_UP) {
        mouseClick = 0;
        mouseDown = false;
    }
}

//Function for handling key presses
void handleKeyboardPress(unsigned char key, int x, int y) {
    keyPressed = key;
    if (keyPressed == 'r') {
        for (int i = 0; i < numberOfNodes; i++) {
            node[i].setType(NODE_NORMAL);
        }
    }

    else if (keyPressed == 's') {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                node[i].setType(NODE_SOURCE);
                break;
            }
        }
    }

    else if (keyPressed == 'd') {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                node[i].setType(NODE_DESTINATION);
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

















