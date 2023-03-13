#include <iostream>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20

int snakeX[100], snakeY[100];
int foodX, foodY;
int snakeSize = 3;
int score = 0;
int direction = 2; // 1 = up, 2 = right, 3 = down, 4 = left

void printBoard()
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
            else if (i == foodY && j == foodX)
            {
                addch('F');
            }
            else
            {
                int snakePart = 0;
                for (int k = 0; k < snakeSize; k++)
                {
                    if (snakeX[k] == j && snakeY[k] == i)
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
    printw("Score: %d\n", score);
    refresh();
}

void generateFood()
{
    srand(time(NULL));
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
}

void updateSnake()
{
    for (int i = snakeSize - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }

    if (direction == 1)
    {
        snakeY[0]--;
    }
    else if (direction == 2)
    {
        snakeX[0]++;
    }
    else if (direction == 3)
    {
        snakeY[0]++;
    }
    else if (direction == 4)
    {
        snakeX[0]--;
    }

    if (snakeX[0] == foodX && snakeY[0] == foodY)
    {
        snakeSize++;
        score++;
        generateFood();
    }
}

int checkCollision()
{
    if (snakeX[0] == 0 || snakeX[0] == WIDTH - 1 || snakeY[0] == 0 || snakeY[0] == HEIGHT - 1)
    {
        return 1;
    }
    for (int i = 1; i < snakeSize; i++)
    {
        if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0])
        {
            return 1;
        }
    }
    return 0;
}

void runSnake()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    timeout(100);

    while (1)
    {
        snakeX[0] = WIDTH / 2;
        snakeY[0] = HEIGHT / 2;
        snakeX[1] = WIDTH / 2 - 1;
        snakeY[1] = HEIGHT / 2;
        snakeX[2] = WIDTH / 2 - 2;
        snakeY[2] = HEIGHT / 2;
        generateFood();
        snakeSize = 3;
        score = 0;
        direction = 2;

        while (1)
        {
            int c = getch();
            if (c == 'w' && direction != 3)
            {
                direction = 1;
            }
            else if (c == 'd' && direction != 4)
            {
                direction = 2;
            }
            else if (c == 's' && direction != 1)
            {
                direction = 3;
            }
            else if (c == 'a' && direction != 2)
            {
                direction = 4;
            }
            else if (c == 'q')
            {
                endwin();
                // return 0;
            }

            updateSnake();
            if (checkCollision())
            {
                break;
            }
            printBoard();
        }
    }
    endwin();
    // return 0;
}
