#include "../include/snake.h"
#include <stdio.h>
#include <stdlib.h>

void renderSnake(Snake* snake) {
    if (snake == NULL) return;

    DrawRectangleV(snake->position, snake->size, (Color){ 0, 82, 172, 255 });                               // draw the snake head

    Tail* tail = snake->tail;                                                                               // iterate through all tail segments and draw them
    while (tail != NULL) {
        DrawRectangleV(tail->position, tail->size, (Color){ 0, 62, 152, 255});
        tail = tail->next;
    }
}

void moveSnake(Snake* snake, short int step) {
    if (snake == NULL) return;

    snake->lastPos = snake->position;                                                                       // update last position
    short int subDir = (1 - (2 * (snake->direction & 1)));                                                  // sub-direction = 1 if last bit is 0 and -1 if last bit is 1
    if (snake->direction & 2) {                                                                             // if second last bit is 1 direction is vertical
        snake->position.y += subDir * step;                                                                 //  else it's horizontal
    } else {
        snake->position.x += subDir * step;
    }

    if (snake->tail == NULL || snake->tailEnd == NULL) {                                                    // check if a tail exists
        return;
    }
    Tail* last = snake->tailEnd;
    Tail* first = snake->tail;

    if (last == first) {                                                                                    // if tailsize is 1, just update it's position
        first->lastPos = first->position;
        first->position = snake->lastPos;
        return;
    }
    Tail* nLast = last->prev;                                                                               // update lastPos of second last to be current pos of last
    nLast->lastPos = last->position;

    last->position = snake->lastPos;                                                                        // update last segments position to become the first segment
    
    first->prev = last;                                                                                     // put the last segment at the beginning of the list
    last->next = first;
    
    nLast->next = NULL;                                                                                     // remove circular list by removing references to last being 
    last->prev = NULL;                                                                                      //  the last
    
    snake->tailEnd = nLast;                                                                                 // update values in snake
    snake->tail = last;
}

void setSnakeDirection(Snake* snake, short int dir) {
    if (snake == NULL) return;

    if ((snake->direction & 2) == (dir & 2)) {                                                              // don't allow direction to change without changing axis
        return;
    }
    snake->direction = dir;
}

void addTail(Snake* snake) {
    if (snake == NULL) return;

    Tail* nTail = (Tail*)malloc(sizeof(Tail));                                                              // save new Tail segment into memory
    if (nTail == NULL) {
        printf("Error: FAILED TO ALLOCATE MEMORY FOR NEW TAIL SEGMENT\n");
    }

    nTail->next = NULL;
    nTail->size = snake->size;

    if (snake->tail == NULL) {                                                                              // if it's the first tail segment
        nTail->position = snake->lastPos;                                                                   //  attach to the snake directly
        snake->tail = nTail;
    } else {
        Tail* prevTail = snake->tailEnd;                                                                     // else, attach to the last tail segment
        nTail->position = prevTail->lastPos;
        prevTail->next = nTail;
        nTail->prev = prevTail;
    }
    nTail->lastPos = nTail->position;
    snake->tailEnd = nTail;                                                                                 // update snakes values
    snake->tailSize++;
}

bool collidesWithTail(Snake* snake) {
    if (snake == NULL) return false;
    if (snake->tail == NULL) return false;

    Tail* current = snake->tail;                                                                            // iterate through all tail segments and check
    while (current != NULL) {                                                                               //  if the snake head collides with it
        bool samePos = snake->position.x == current->position.x 
                        && snake->position.y == current->position.y;
        if (samePos) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool collidesWithWall(Snake* snake, unsigned int wallLeft, unsigned int wallRight, unsigned int wallTop, unsigned int wallBottom) {
    return (snake->position.x < wallLeft || snake->position.x > wallRight || snake->position.y < wallTop || snake->position.y > wallBottom);
}

void resetToLastPosition(Snake* snake) {
    snake->position.x = snake->lastPos.x;                                                                   // reset snake head position
    snake->position.y = snake->lastPos.y;

    Tail* current = snake->tail;                                                                            // iterate through all tail segments and reset position
    while (current != NULL) {
        current->position.x = current->lastPos.x;
        current->position.y = current->lastPos.y;
        current = current->next;
    }
}

void deleteSnakeTail(Snake* snake) {
    if (snake == NULL) return;

    Tail* current = snake->tail;                                                                            // iterate through all tail segments
    Tail* next;

    while (current != NULL) {
        next = current->next;
        free(current);                                                                                      // free up the allocated memory for the segment
        current = next;
    }

    snake->tail = NULL;                                                                                     // reset snake values
    snake->tailEnd = NULL;
    snake->tailSize = 0;
}