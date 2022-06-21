#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "../include/Node.h"
#include "../include/SpriteSheet.h"

#define MOUSE_LEFT 1
#define MOUSE_RIGHT 2

#define KEY_S 's'
#define KEY_D 'd'
#define KEY_R 'r'
#define KEY_A 'a'

void render(void);
void resize(int width, int height);
void handleMouseMotion(int x, int y);
void handleMouseClick(int button, int state, int x, int y);
void handleKeyboardPress(unsigned char key, int x, int y);
void update(int a);
void init();

//THERE IS A DIFFERENCE OF 32 b/w node and its adjacent node on top and bottom

//MOUSE VARIABLES
int mouseClick ;
int mouseX;
int mouseY;
bool mouseDown = false;

//NODE VARIABLES
const int nodeSideLength = 30;
const int numberOfNodes = (WIDTH / nodeSideLength) * (HEIGHT / nodeSideLength) + 1;
Node node[numberOfNodes];

//Variables for the algorithm
Node* sourceNode;
Node* destinationNode;
//This function creates objects for all of the nodes on the grid
void init() {
    node[0] = Node(1, 1, nodeSideLength, nodeSideLength, NODE_NORMAL, 0);
    int x = 1;
    int y = 1;
    for (int i = 1; i < numberOfNodes; i++) {
        if (x >= WIDTH) {
            x = 1;
            y += nodeSideLength;
        }
        node[i] = Node(x, y, nodeSideLength, nodeSideLength, NODE_NORMAL, i);
        x += nodeSideLength;
    }
}

//The function for rendering objects on the screen
//This function just draws all the nodes 
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
    
    //If mouse is pressed, go through all the nodes and check
    //which node has to be changed
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

//ALGORITHM
void algo() {
    if (sourceNode == nullptr || destinationNode == nullptr) return;

    int *adjacentNodeI = sourceNode->getAdjacentNodeIndex();
    for (int i = 0; i < 4; i++) {
        //node[*adjacentNodeI].setType(NODE_VISITED);
        if (node[*adjacentNodeI].getType() != NODE_WALL) {
            node[*adjacentNodeI].setType(NODE_VISITED);
        }
        adjacentNodeI++;
    }

    //while (currNode != destinationNode) {
    //    for (int i = 0; i < numberOfNodes; i++) {
    //        if (abs(currNode->getCellNumber() - node[i].getCellNumber()) == 32) { 
    //            if (node[i].getType() == NODE_VISITED) continue;
    //            node[i].setType(NODE_VISITED);
    //            currNode = &node[i];

    //            //call render function to draw all nodes
    //            render();
    //        }
    //    }
    //}
}

//Function for handling key presses
void handleKeyboardPress(unsigned char key, int x, int y) {
    if (key == KEY_R) {
        sourceNode = nullptr;
        destinationNode = nullptr;
        for (int i = 0; i < numberOfNodes; i++) {
            node[i].setType(NODE_NORMAL);
        }
    }

    else if (key == KEY_S) {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                node[i].setType(NODE_SOURCE);
                sourceNode = &node[i];
                break;
            }
        }
    }

    else if (key == KEY_D) {
        for (int i = 0; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY)) {
                node[i].setType(NODE_DESTINATION);
                destinationNode = &node[i];
                break;
            }
        }
    }
    
    if (key == KEY_A) {
        algo();
    }
}

void resize(int width, int height) {
    glutReshapeWindow(WIDTH, HEIGHT);
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

















