#include <iostream>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "../include/Node.h"
#include "../include/colors.h"

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
bool mouseDown = false;

const int nodeSideLength = 30;
const int numberOfNodes = (WIDTH / nodeSideLength) * (HEIGHT / nodeSideLength) + 1;

Node node[numberOfNodes];

void init() {
    printf("%d", numberOfNodes);
    node[0] = Node(1, 1, nodeSideLength, nodeSideLength, COLOR_WHITE);
    int x = 1;
    int y = 1;
    for (int i = 1; i < numberOfNodes; i++) {
        if (x >= WIDTH) {
            x = 1;
            y += nodeSideLength;
        }
        node[i] = Node(x, y, nodeSideLength, nodeSideLength, COLOR_WHITE);
        x += nodeSideLength;
    }
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
                    node[i].color = COLOR_RED;
                }
                else if (mouseClick == 2) {
                    node[i].color = COLOR_WHITE;
                }
            }
        }
    }

    //Repaint the display
    glutPostRedisplay();
}

void handleMouseMotion(int x, int y) {
    //Left click
    if (mouseClick == 1) {
        //printf("LEFT\n");
    }
    if (mouseClick == 2) {
        //printf("RIGHT\n");
    }
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
    if (key == 'r') {
        for (int i = 0; i < numberOfNodes; i++) {
            node[i].color = COLOR_WHITE;
        }
    }
}

void resize(int width, int height) {
    glutReshapeWindow(WIDTH, HEIGHT);
}























