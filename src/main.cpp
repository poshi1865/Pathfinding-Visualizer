#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <queue>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <algorithm>

#include "../include/Node.hpp"
//#include "../include/SpriteSheet.hpp"

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
void trace_back_path(int *adjacentOfTempIndex);

//THERE IS A DIFFERENCE OF 42 b/w node and its adjacent node on top and bottom

//MOUSE VARIABLES
int mouseClick ;
int mouseX;
int mouseY;
bool mouseDown = false;

//NODE VARIABLES
const int nodeSideLength = 30;
const int numberOfNodes = (WIDTH / nodeSideLength) * ((HEIGHT - 100) / nodeSideLength) + 1;
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
char shortestDistanceString[200];
void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    //DRAWING STRINGS
    glColor3f(0.0f, 0.3f, 1.0f);

    glRasterPos2f(WIDTH - 950, HEIGHT - 30);
    const unsigned char* title = reinterpret_cast<const unsigned char *>("PATHFINDING ALGORITHM VISUALIZER");
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, title);

    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(WIDTH - 950, HEIGHT - 105);
    const unsigned char* shortest = reinterpret_cast<const unsigned char *>(shortestDistanceString);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, shortest);
                    
    glColor3f(0.0f, 0.3f, 1.0f);
    
    glRasterPos2f(WIDTH - 950, HEIGHT - 65);
    const unsigned char* aToRun = reinterpret_cast<const unsigned char *>("A to run breadth first search");
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, aToRun);

    glRasterPos2f(WIDTH - 350, HEIGHT - 30);
    const unsigned char* sToPlaceSource = reinterpret_cast<const unsigned char *>("S to place source node");
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, sToPlaceSource);

    glRasterPos2f(WIDTH - 350, HEIGHT - 65);
    const unsigned char* dToPlaceDestination = reinterpret_cast<const unsigned char *>("D to place destination node");
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, dToPlaceDestination);

    glRasterPos2f(WIDTH - 350, HEIGHT - 100);
    const unsigned char* rToReset = reinterpret_cast<const unsigned char *>("R to reset");
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, rToReset);

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
                    printf("x: %d\n", node[i].getX());
                    printf("y: %d\n", node[i].getY());
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

//Heuristic for A* search
int calculate_manhattan_distance(Node& current, Node* dest) {
    return (abs(current.getX() - dest->getX()) + abs(current.getY() - dest->getY()));
}

void a_star_search() {
    if (sourceNode == nullptr || destinationNode == nullptr) return;

    std::queue<Node> nodeQueue;
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
    Node temp = *sourceNode;

    while(!nodeQueue.empty()) {
        nodeQueue.pop();
        int distanceOfTemp = temp.getDistanceFromSource();

        if (temp.getType() == NODE_VISITED || temp.getType() == NODE_SOURCE) {
            int* adjacentOfTempIndex = temp.getAdjacentNodeIndex();
            int* startOfAdjacentIndex = adjacentOfTempIndex;

            //variables for calculating path to take
            //Go through all adjacent nodes and find the one with smallest f
            Node* selectedNode;
            int f = INFINITY;
            for (int i = 0; i < 4; i++) {
                if (node[*adjacentOfTempIndex].getType() == NODE_VISITED || node[*adjacentOfTempIndex].getType() == NODE_BOUNDARY || node[*adjacentOfTempIndex].getType() == NODE_WALL) {
                    adjacentOfTempIndex++;
                    continue;
                }
                //Set node to visited and distance of node from source
                node[*adjacentOfTempIndex].setDistanceFromSource(distanceOfTemp + 1);
                if (node[*adjacentOfTempIndex].getType() != NODE_SOURCE && node[*adjacentOfTempIndex].getType() != NODE_DESTINATION) {
                    //Set distance from source

                    //calculate heuristic
                    int tempHeuristic = calculate_manhattan_distance(node[*adjacentOfTempIndex], destinationNode);
                    if (tempHeuristic + node[*adjacentOfTempIndex].getDistanceFromSource() < f) {
                        f = tempHeuristic + node[*adjacentOfTempIndex].getDistanceFromSource();
                        selectedNode = &node[*adjacentOfTempIndex];
                    }
                }

                //Check if destination node has been reached 
                if (node[*adjacentOfTempIndex].getCellNumber() == destinationNode->getCellNumber()) {
                    printf("Shortest distance from source to destination: %d\n", node[*adjacentOfTempIndex].getDistanceFromSource());
                    glColor3f(0.0f, 0.3f, 1.0f);
                    
                    sprintf(shortestDistanceString, "Shortest distance from source to destination: %d", node[*adjacentOfTempIndex].getDistanceFromSource());
                    //TRACE BACK THE PATH
                    trace_back_path(adjacentOfTempIndex);
                    return;
                }
                //set node with lowest f as visited node 
                selectedNode->setType(NODE_VISITED);
                temp = *selectedNode;
                nodeQueue.push(*selectedNode);
                render();
                //Go to the next adjacent node
                adjacentOfTempIndex++;
            }
            free(startOfAdjacentIndex);
        }
    }
    //Set shortestDistanceString to "NO PATH" for displaying to user
    sprintf(shortestDistanceString, "NO PATH");
}

//ALGORITHM
void start_breadth_first_search() {
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
        int distanceOfTemp = temp.getDistanceFromSource();

        if (temp.getType() == NODE_VISITED || temp.getType() == NODE_SOURCE) {
            int* adjacentOfTempIndex = temp.getAdjacentNodeIndex();
            int* startOfAdjacentIndex = adjacentOfTempIndex;

            //for  all adjacent unvisited nodes of temp set distance=distanceOfTemp+1
            for (int i = 0; i < 4; i++) {
                if (node[*adjacentOfTempIndex].getType() == NODE_VISITED || node[*adjacentOfTempIndex].getType() == NODE_BOUNDARY || node[*adjacentOfTempIndex].getType() == NODE_WALL) {
                    adjacentOfTempIndex++;
                    continue;
                }
                //Set node to visited and distance of node from source
                if (node[*adjacentOfTempIndex].getType() != NODE_SOURCE && node[*adjacentOfTempIndex].getType() != NODE_DESTINATION) {
                    node[*adjacentOfTempIndex].setType(NODE_VISITED);
                }
                node[*adjacentOfTempIndex].setDistanceFromSource(distanceOfTemp + 1);

                //Push into queue and render
                nodeQueue.push(node[*adjacentOfTempIndex]);
                render();

                //Check if destination node has been reached 
                if (node[*adjacentOfTempIndex].getCellNumber() == destinationNode->getCellNumber()) {
                    printf("Shortest distance from source to destination: %d\n", node[*adjacentOfTempIndex].getDistanceFromSource());
                    glColor3f(0.0f, 0.3f, 1.0f);
                    
                    sprintf(shortestDistanceString, "Shortest distance from source to destination: %d", node[*adjacentOfTempIndex].getDistanceFromSource());
                    //TRACE BACK THE PATH
                    trace_back_path(adjacentOfTempIndex);
                    return;
                }
                //Go to the next adjacent node
                adjacentOfTempIndex++;
            }
            free(startOfAdjacentIndex);
        }
    }
    //Set shortestDistanceString to "NO PATH" for displaying to user
    sprintf(shortestDistanceString, "NO PATH");
}

void trace_back_path(int* adjacentOfTempIndex) {
    int shortestPathIndex = *adjacentOfTempIndex;
    for (int i = 0; i < node[*adjacentOfTempIndex].getDistanceFromSource() - 1; i++) {

        //get adjacent nodes
        int* adjacentNodes = node[shortestPathIndex].getAdjacentNodeIndex();

        //get least distance 
        int smallestDistance = INFINITY;
        for (int i = 0; i < 4; i++) {
            if (node[*adjacentNodes].getDistanceFromSource() <= smallestDistance) {
                if (node[*adjacentNodes].getType() == NODE_VISITED) {
                    smallestDistance = node[*adjacentNodes].getDistanceFromSource();
                }
            }
            adjacentNodes += 1;
        }
        //reset adjacentNodes
        adjacentNodes -= 4;

        //get index of node with smallest distance
        int indexOfNodeSmallestDistance;
        if (node[*adjacentNodes].getDistanceFromSource() == smallestDistance) {
            if (node[*adjacentNodes].getType() == NODE_VISITED)
                indexOfNodeSmallestDistance = node[*adjacentNodes].getCellNumber();
        }

        else if (node[*(adjacentNodes + 1)].getDistanceFromSource() == smallestDistance) {
            if (node[*(adjacentNodes + 1)].getType() == NODE_VISITED)
                indexOfNodeSmallestDistance = node[*(adjacentNodes+1)].getCellNumber();
        }

        else if (node[*(adjacentNodes + 2)].getDistanceFromSource() == smallestDistance) {
            if (node[*(adjacentNodes + 2)].getType() == NODE_VISITED)
                indexOfNodeSmallestDistance = node[*(adjacentNodes + 2)].getCellNumber();
        }

        else if (node[*(adjacentNodes + 3)].getDistanceFromSource() == smallestDistance) {
            if (node[*(adjacentNodes + 3)].getType() == NODE_VISITED)
                indexOfNodeSmallestDistance = node[*(adjacentNodes + 3)].getCellNumber();
        }

        shortestPathIndex = indexOfNodeSmallestDistance;

        //set type of node with smallest distance value as trace
        node[shortestPathIndex].setType(NODE_TRACE);
        usleep(10000);
        render();

        free(adjacentNodes);
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
        a_star_search();
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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);

    init();

    glutMainLoop();

    return 0;
}

















