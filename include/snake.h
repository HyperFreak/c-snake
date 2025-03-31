#ifndef SNAKE_H
#define SNAKE_H

#include <raylib.h>

typedef struct Snake Snake;
typedef struct Tail Tail;

struct Snake {
    Vector2 position;
    Vector2 lastPos;
    Vector2 size;
    short int direction;    // 0 = right, 1 = left, 2 = down, 3 = up, (for bit operations, last bit = subdirection, second last = axis)
    unsigned short int tailSize;
    Tail* tail;
    Tail* tailEnd;
};

struct Tail {
    Vector2 position;
    Vector2 lastPos;
    Vector2 size;
    Snake* head;
    Tail* prev;
    Tail* next;
};

/**
 * render the snake and its tail
 * @param snake the snake which shall be rendered
 */
void renderSnake(Snake* snake);

/**
 * update snake position
 * @param snake the snake that shall be moved
 * @param step the amount of pixels which the snake shall move by
 */
void moveSnake(Snake* snake, short int step);

/**
 * update the snakes direction to only allow changes into another axis
 * @param snake the snake whose direction shall be changed
 * @param dir the new direction in which the snake shall move
 */
void setSnakeDirection(Snake* snake, short int dir);

/**
 * adds a tail segment to a snake
 * @param snake the snake which shall gain another tail segment
 */
void addTail(Snake* snake);

/**
 * checks whether the snake head collides with its own tail
 * @param snake the snake which shall be checked
 * @return true if the head collides with its own tail, false otherwise
 */
bool collidesWithTail(Snake* snake);

/**
 * checks whether the snake moved outside the bounds of the field
 * @param snake the snake whose position shall be checked
 * @param wallLeft the leftmost x coordinate of the field
 * @param wallRight the rightmost x coordinate of the field
 * @param wallTop the lowest y coordinate of the field
 * @param wallBottom the highest y coordinate of the field
 * @returns true if the head moves outside the walls, otherwise false
 */
bool collidesWithWall(Snake* snake, unsigned int wallLeft, unsigned int wallRight, unsigned int wallTop, unsigned int wallBottom);

/**
 * resets a snake to the last position it was at
 * @param snake the snake whose position shall be reset
 */
void resetToLastPosition(Snake* snake);

/**
 * removes the tail from a snake and frees up the memory in which the segments are stored
 * @param snake the snake whose tail shall be removed
 */
void deleteSnakeTail(Snake* snake);

#endif