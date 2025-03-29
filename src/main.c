#include <time.h>
#include <stdlib.h>

#include <stdio.h>

#include "raylib.h"
#include "../include/snake.h"

#define GRID_SIZE 20
#define TILES_X 30
#define TILES_Y 30
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

#define FIELD_X WINDOW_WIDTH / 2 - (TILES_X * GRID_SIZE) / 2
#define FIELD_Y WINDOW_HEIGHT / 2 - (TILES_Y * GRID_SIZE) / 2

const int controls[] = { KEY_D, KEY_A, KEY_S, KEY_W, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP };

unsigned short int handleInput(unsigned short int current) {

    for (unsigned short int i = 0; i < 8; i++) {
        if (IsKeyPressed(controls[i])) {
            return i % 4;
        }
    }
    return current;
}

unsigned short int randomUpTo(unsigned short int max) {
    return rand() % max;
}

bool samePosition(Vector2 a, Vector2 b) {
    return a.x == b.x && a.y == b.y;
}

void setToRandomPosition(Vector2* posVec) {
    posVec->x = FIELD_X + (randomUpTo(TILES_X) * GRID_SIZE) + 1;
    posVec->y = FIELD_Y + (randomUpTo(TILES_Y) * GRID_SIZE) + 1;
}

int gameLoop(Snake* snake, Vector2* fruitPos, unsigned short int* directionInput, unsigned short int score) {
    setSnakeDirection(snake, *directionInput);
    moveSnake(snake, GRID_SIZE);

    if (samePosition(snake->position, *fruitPos)) {
        score++;
        setToRandomPosition(fruitPos);
        addTail(snake);
    }
    return score;
}

void drawFieldTiles() {
    unsigned int fieldX = FIELD_X;
    unsigned int fieldY = FIELD_Y;
    for (unsigned short int i = 0; i < TILES_X; i++) {
        for (unsigned short int j = 0; j < TILES_Y; j++) {
            DrawRectangle(1 + fieldX + i * GRID_SIZE, 1 + fieldY + j * GRID_SIZE, GRID_SIZE - 2, GRID_SIZE - 2, GRAY);
        }
    }
}

int main() {
    srand(time(NULL));
	
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");

    unsigned short int score = 0;
    const unsigned short int fieldWidth = TILES_X * GRID_SIZE;
    const unsigned short int fieldHeight = TILES_Y * GRID_SIZE;
    Vector2 fieldSize = { fieldWidth, fieldHeight };
    Vector2 fieldPos = { WINDOW_WIDTH / 2 - (fieldWidth / 2), WINDOW_HEIGHT / 2 - (fieldHeight / 2) };
    Vector2 fruitPos = { 0, 0 };
    setToRandomPosition(&fruitPos);

    Snake snake = { {WINDOW_WIDTH / 2 - GRID_SIZE + 1, WINDOW_HEIGHT / 2 - GRID_SIZE + 1}, {0, 0}, {GRID_SIZE-2, GRID_SIZE-2}, 0 };
    snake.lastPos = snake.position;
    snake.lastPos.x -= GRID_SIZE;
    addTail(&snake);

    SetTargetFPS(60);
    unsigned short int movementTimer = 0;
    const unsigned short int movementTime = 6;
    unsigned short int direction = 0;
    while (!WindowShouldClose()) {
        direction = handleInput(direction);

        movementTimer++;
        if (movementTimer >= movementTime) {
            movementTimer = 0;
            score = gameLoop(&snake, &fruitPos, &direction, score);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Score: %d", score), FIELD_X, FIELD_Y - 42, 32, BLACK);

        DrawRectangleV(fieldPos, fieldSize, DARKGRAY);

        drawFieldTiles();

        renderSnake(&snake);
        DrawRectangleV(fruitPos, (Vector2){ GRID_SIZE - 2, GRID_SIZE - 2 }, (Color){ 255, 0, 0, 255 });
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
