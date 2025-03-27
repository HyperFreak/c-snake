#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>

typedef struct {
    Vector2 position;
    Vector2 size;
    short int direction;    // 0 = right, 1 = left, 2 = down, 3 = up
} Snake;

void renderSnake(Snake* snake);

void moveSnake(Snake* snake, short int step);


#endif