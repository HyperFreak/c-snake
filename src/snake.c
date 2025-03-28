#include "../include/snake.h"
#include <stdio.h>
#include <stdlib.h>

void renderSnake(Snake* snake) {
    if (snake == NULL) return;

    DrawRectangleV(snake->position, snake->size, (Color){ 0, 82, 172, 255 });
    Tail* tail = snake->tail;
    while (tail != NULL) {
        DrawRectangleV(tail->position, tail->size, (Color){ 0, 62, 152, 255});
        tail = tail->next;
    }
}

void moveSnake(Snake* snake, short int step) {
    if (snake == NULL) return;
    snake->lastPos = snake->position;

    short int subDir = (1 - (2 * (snake->direction & 1)));
    if (snake->direction & 2) {
        snake->position.y += subDir * step;
    } else {
        snake->position.x += subDir * step;
    }

    Tail* tail = snake->tail;
    Tail* next;
    if (tail != NULL) {
        tail->lastPos = tail->position;
        tail->position = snake->lastPos;
        while (tail->next != NULL) {
            next = tail->next;
            next->lastPos = next->position;
            next->position = tail->lastPos;
            tail = next;
        }
    }
}

void setSnakeDirection(Snake* snake, short int dir) {
    if (snake == NULL) return;

    if ((snake->direction & 2) == (dir & 2)) {
        return;
    }
    snake->direction = dir;
}

void addTail(Snake* snake) {
    if (snake == NULL) return;

    Tail* nTail = (Tail*)malloc(sizeof(Tail));
    if (nTail == NULL) {
        printf("Error: FAILED TO ALLOCATE MEMORY FOR NEW TAIL SEGMENT\n");
    }

    nTail->next = NULL;
    nTail->size = snake->size;

    if (snake->tail == NULL) {
        nTail->position = snake->lastPos;
        snake->tail = nTail;
    } else {
        Tail* prevTal = snake->tailEnd;
        nTail->position = prevTal->lastPos;
        prevTal->next = nTail;
    }
    snake->tailEnd = nTail;
    snake->tailSize++;
}


void deleteSnakeTail(Snake* snake) {
    if (snake == NULL) return;

    Tail* current = snake->tail;
    Tail* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    snake->tail = NULL;
    snake->tailEnd = NULL;
    snake->tailSize = 0;
}