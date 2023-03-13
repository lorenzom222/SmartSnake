#include <iostream>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <unistd.h>
#include <queue>
#include <vector>

#define WIDTH 40
#define HEIGHT 20

// A class to represent a node in the grid
struct Node
{
    int x, y;
    int g, h;
    Node *parent;

    Node(int x, int y, int g, int h, Node *parent) : x(x), y(y), g(g), h(h), parent(parent) {}

    int f()
    {
        return g + h;
    }
};
bool comp(Node *a, Node *b)
{
    return a->f() > b->f();
}

int smartsnakeX[100], smartsnakeY[100];
int smartfoodX, smartfoodY;
int smartsnakeSize = 3;
int smartscore = 0;
int smartdirection = 2; // 1 = up, 2 = right, 3 = down, 4 = left

// A function to calculate the manhattan distance between two points
int manhattanDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}
bool isTail(int x, int y)
{
    for (int i = 0; i < smartsnakeSize; i++)
    {
        if (smartsnakeX[i] == x && smartsnakeY[i] == y)
        {
            return true;
        }
    }
    return false;
}

bool isWall(int x, int y)
{
    if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
    {
        return true;
    }
    return false;
}

// A function to generate the path using A* algorithm
std::vector<Node> aStar(int startX, int startY, int goalX, int goalY)
{
    std::vector<Node> path;
    std::priority_queue<Node *, std::vector<Node *>, decltype(&comp)> open(comp);
    bool closed[WIDTH][HEIGHT];

    Node *start = new Node(startX, startY, 0, manhattanDistance(startX, startY, goalX, goalY), nullptr);
    open.push(start);

    while (!open.empty())
    {
        Node *current = open.top();
        open.pop();

        if (current->x == goalX && current->y == goalY)
        {
            while (current->parent != nullptr)
            {
                path.push_back(*current);
                current = current->parent;
            }
            break;
        }

        closed[current->x][current->y] = true;

        if (current->y > 0 && !closed[current->x][current->y - 1] && !isWall(current->x, current->y - 1) && !isTail(current->x, current->y - 1))
        {
            int g = current->g + 1;
            int h = manhattanDistance(current->x, current->y - 1, goalX, goalY);
            Node *neighbor = new Node(current->x, current->y - 1, g, h, current);
            open.push(neighbor);
        }

        // Check east
        if (current->x < WIDTH - 1 && !closed[current->x + 1][current->y] && !isWall(current->x + 1, current->y) && !isTail(current->x + 1, current->y))
        {
            int g = current->g + 1;
            int h = manhattanDistance(current->x + 1, current->y, goalX, goalY);
            Node *neighbor = new Node(current->x + 1, current->y, g, h, current);
            open.push(neighbor);
        }

        // Check south
        if (current->y < HEIGHT - 1 && !closed[current->x][current->y + 1] && !isWall(current->x, current->y + 1) && !isTail(current->x, current->y + 1))
        {
            int g = current->g + 1;
            int h = manhattanDistance(current->x, current->y + 1, goalX, goalY);
            Node *neighbor = new Node(current->x, current->y + 1, g, h, current);
            open.push(neighbor);
        }

        // Check west
        if (current->x > 0 && !closed[current->x - 1][current->y] && !isWall(current->x - 1, current->y) && !isTail(current->x - 1, current->y))
        {
            int g = current->g + 1;
            int h = manhattanDistance(current->x - 1, current->y, goalX, goalY);
            Node *neighbor = new Node(current->x - 1, current->y, g, h, current);
            open.push(neighbor);
        }
    }

    while (!open.empty())
    {
        Node *n = open.top();
        open.pop();
        delete n;
    }

    return path;
}

void smartprintBoard()
{
    clear();
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
            {
                addch('#');
            }
            else if (i == smartfoodY && j == smartfoodX)
            {
                addch('F');
            }
            else
            {
                int snakePart = 0;
                for (int k = 0; k < smartsnakeSize; k++)
                {
                    if (smartsnakeX[k] == j && smartsnakeY[k] == i)
                    {
                        snakePart = 1;
                        break;
                    }
                }
                if (snakePart)
                {
                    addch('S');
                }
                else
                {
                    addch(' ');
                }
            }
        }
        addch('\n');
    }
    printw("smartScore: %d\n", smartscore);
    refresh();
}

void smartgenerateFood()
{
    srand(time(NULL));
    smartfoodX = rand() % (WIDTH - 2) + 1;
    smartfoodY = rand() % (HEIGHT - 2) + 1;
}

void smartupdateSnake()
{
    std::vector<Node> path = aStar(smartsnakeX[0], smartsnakeY[0], smartfoodX, smartfoodY);
    if (!path.empty())
    {
        Node next = path.back();
        if (next.x < smartsnakeX[0])
        {
            smartdirection = 4;
        }
        else if (next.x > smartsnakeX[0])
        {
            smartdirection = 2;
        }
        else if (next.y < smartsnakeY[0])
        {
            smartdirection = 1;
        }
        else if (next.y > smartsnakeY[0])
        {
            smartdirection = 3;
        }
    }

    for (int i = smartsnakeSize - 1; i > 0; i--)
    {
        smartsnakeX[i] = smartsnakeX[i - 1];
        smartsnakeY[i] = smartsnakeY[i - 1];
    }

    if (smartdirection == 1)
    {
        smartsnakeY[0]--;
    }
    else if (smartdirection == 2)
    {
        smartsnakeX[0]++;
    }
    else if (smartdirection == 3)
    {
        smartsnakeY[0]++;
    }
    else if (smartdirection == 4)
    {
        smartsnakeX[0]--;
    }

    if (smartsnakeX[0] == smartfoodX && smartsnakeY[0] == smartfoodY)
    {
        smartsnakeSize++;
        smartscore++;
        smartgenerateFood();
    }
}

int smartcheckCollision()
{
    if (smartsnakeX[0] == 0 || smartsnakeX[0] == WIDTH - 1 || smartsnakeY[0] == 0 || smartsnakeY[0] == HEIGHT - 1)
    {
        return 1;
    }
    for (int i = 1; i < smartsnakeSize; i++)
    {
        if (smartsnakeX[i] == smartsnakeX[0] && smartsnakeY[i] == smartsnakeY[0])
        {
            return 1;
        }
    }
    return 0;
}

void runSmart()

{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    timeout(100);

    while (1)
    {
        printw("smartScore: %d\n", smartscore);

        smartsnakeX[0] = WIDTH / 2;
        smartsnakeY[0] = HEIGHT / 2;
        smartsnakeX[1] = WIDTH / 2 - 1;
        smartsnakeY[1] = HEIGHT / 2;
        smartsnakeX[2] = WIDTH / 2 - 2;
        smartsnakeY[2] = HEIGHT / 2;
        smartgenerateFood();
        smartsnakeSize = 3;
        smartscore = 0;
        smartdirection = 2;

        while (1)
        {

            smartupdateSnake();

            smartprintBoard();

            if (smartcheckCollision())
            {
                break;
            }
            usleep(100000);
        }
        getch();
    }
    endwin();
    // return 0;
}
