#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <queue>
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
#define KEY_T 't'

#define INFINITY INT16_MAX

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
    //sourceNode = (Node*) malloc(sizeof(Node));
    //destinationNode = (Node*) malloc(sizeof(Node));
    //node[0] = Node(0, 0, nodeSideLength, nodeSideLength, NODE_NORMAL, 0);
    int x = 1;
    int y = 1;
    for (int i = 1; i < numberOfNodes; i++) {
        if (x >= WIDTH) {
            x = 1;
            y += nodeSideLength;
        }
        node[i] = Node(x, y, nodeSideLength, nodeSideLength, NODE_NORMAL, i);

        if (node[i].isBoundary()) {
            node[i].setType(NODE_BOUNDARY);
        }

        x += nodeSideLength;
    }
}

//The function for rendering objects on the screen
//This function just draws all the nodes 
void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);

    for (int i = 1; i < numberOfNodes; i++) {
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
            if (node[i].hasInside(mouseX, mouseY) && !node[i].isBoundary()) {
                if (mouseClick == MOUSE_LEFT) {
                    printf("%d\n", i);
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

    std::queue<Node> nodeQueue;
    //BREADTH FIRST 
    //Set distance of all nodes to infinity
    for (int i = 1; i < numberOfNodes; i++) {
        if (node[i].getType() == NODE_NORMAL) {
            node[i].setDistanceFromSource(INFINITY);
        }
    }

    //Set distance value of start node to 0
    sourceNode->setDistanceFromSource(0);

    //If start node is the end node, return 
    if (sourceNode == destinationNode) return;
    
    //Push start node on queue
    nodeQueue.push(*sourceNode);

    while(!nodeQueue.empty()) {
        Node temp = nodeQueue.front();
        nodeQueue.pop();
        //std::cout << temp.getCellNumber() << std::endl;
        int distanceOfTemp = temp.getDistanceFromSource();

        if (temp.getType() != NODE_VISITED) {
            temp.setType(NODE_VISITED);
            int* adjacentOfTempIndex = temp.getAdjacentNodeIndex();
            int* startOfAdjacentIndex = adjacentOfTempIndex;

            //for  all adjacent unvisited nodes of temp set distance=distanceOfTemp+1
            for (int i = 0; i < 4; i++) {
                if (node[*adjacentOfTempIndex].getType() == NODE_VISITED) {
                    adjacentOfTempIndex++;
                    continue;
                }
                //Set node to visited and distance of node from source
                node[*adjacentOfTempIndex].setType(NODE_VISITED);
                node[*adjacentOfTempIndex].setDistanceFromSource(distanceOfTemp + 1);

                std::cout << "adjacent cell: " << *adjacentOfTempIndex << std::endl;
                //Push into queue and render
                nodeQueue.push(node[*adjacentOfTempIndex]);
                render();

                //Go to the next adjacent node
                adjacentOfTempIndex++;

                //Check if destination node has been reached 
                if (node[*adjacentOfTempIndex].getCellNumber() == destinationNode->getCellNumber()) {
                    printf("Shortest distance: %d\n", distanceOfTemp);
                    return;
                }
            }
            free(startOfAdjacentIndex);
        }
    }
}

//Function for handling key presses
void handleKeyboardPress(unsigned char key, int x, int y) {
    if (key == KEY_R) {
        sourceNode = nullptr;
        destinationNode = nullptr;
        for (int i = 1; i < numberOfNodes; i++) {
            if (!node[i].isBoundary()) {
                node[i].setType(NODE_NORMAL);
            }
        }
    }

    else if (key == KEY_S) {
        for (int i = 1; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY) && !node[i].isBoundary()) {
                node[i].setType(NODE_SOURCE);
                sourceNode = &node[i];
                break;
            }
        }
    }

    else if (key == KEY_D) {
        for (int i = 1; i < numberOfNodes; i++) {
            if (node[i].hasInside(mouseX, mouseY) && !node[i].isBoundary()) {
                node[i].setType(NODE_DESTINATION);
                destinationNode = &node[i];
                break;
            }
        }
    }
    
    else if (key == KEY_A) {
        algo();
    }
    
    else if (key == KEY_T) {
        printf("T PRESSED\n");
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

















