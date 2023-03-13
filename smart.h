#ifndef SMART_H
#define SMART_H

#include <vector>

struct Node
{
    int x, y;
    int g, h;
    Node *parent;

    Node(int x, int y, int g, int h, Node *parent);

    int f() const;
};

bool comp(Node *a, Node *b);

int manhattanDistance(int x1, int y1, int x2, int y2);

bool isTail(int x, int y);

bool isWall(int x, int y);

std::vector<Node> aStar(int startX, int startY, int goalX, int goalY);

void smartprintBoard();
void smartgenerateFood();
void smartupdateSnake();
int smartcheckCollision();

void runSmart();

#endif
