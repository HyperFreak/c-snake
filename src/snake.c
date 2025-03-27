#include "../include/snake.h"
#include <stdio.h>

void renderSnake(Snake* snake) {
    DrawRectangleV(snake->position, snake->size, (Color){ 0, 82, 172, 255 });
}

void moveSnake(Snake* snake, short int step) {
    short int subDir = (1 - (2 * (snake->direction & 1)));
    if (snake->direction & 2) {
        snake->position.y += subDir;
        return;
    }
    snake->position.x += subDir;
}