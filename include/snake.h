#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>

typedef struct Snake Snake;
typedef struct Tail Tail;

struct Snake {
    Vector2 position;
    Vector2 lastPos;
    Vector2 size;
    short int direction;    // 0 = right, 1 = left, 2 = down, 3 = up
    unsigned short int tailSize;
    Tail* tail;
    Tail* tailEnd;
};

struct Tail {
    Vector2 position;
    Vector2 lastPos;
    Vector2 size;
    Snake* head;
    Tail* next;
};

void renderSnake(Snake* snake);

void moveSnake(Snake* snake, short int step);

void setSnakeDirection(Snake* snake, short int dir);

void addTail(Snake* snake);

bool collidesWithTail(Snake* snake);

bool collidesWithWall(Snake* snake, unsigned int wallLeft, unsigned int wallRight, unsigned int wallTop, unsigned int wallBottom);

void resetToLastPosition(Snake* snake);

void deleteSnakeTail(Snake* snake);

#endif