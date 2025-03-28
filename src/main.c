#include <time.h>
#include <stdlib.h>

#include <stdio.h>

#include "raylib.h"
#include "../include/snake.h"

#define GRID_SIZE 16
#define TILES_X 60
#define TILES_Y 32
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
    posVec->x = FIELD_X + (randomUpTo(TILES_X) * GRID_SIZE);
    posVec->y = FIELD_Y + (randomUpTo(TILES_Y) * GRID_SIZE);
}

int main() {
    srand(time(NULL));
	
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Window");

    unsigned short int score = 0;
    const unsigned short int fieldWidth = TILES_X * GRID_SIZE;
    const unsigned short int fieldHeight = TILES_Y * GRID_SIZE;
    Vector2 fieldSize = { fieldWidth, fieldHeight };
    Vector2 fieldPos = { WINDOW_WIDTH / 2 - (fieldWidth / 2), WINDOW_HEIGHT / 2 - (fieldHeight / 2) };
    Vector2 fruitPos = { 0, 0 };
    setToRandomPosition(&fruitPos);

    Snake snake = { {WINDOW_WIDTH / 2 - GRID_SIZE, WINDOW_HEIGHT / 2 - GRID_SIZE}, {0, 0}, {GRID_SIZE, GRID_SIZE}, 0 };
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
            setSnakeDirection(&snake, direction);
            moveSnake(&snake, GRID_SIZE);

            if (samePosition(snake.position, fruitPos)) {
                score++;
                setToRandomPosition(&fruitPos);
                addTail(&snake);
            }
        }

		BeginDrawing();
		ClearBackground(RAYWHITE);
        DrawRectangleV(fieldPos, fieldSize, BLACK);

        renderSnake(&snake);
        DrawRectangleV(fruitPos, (Vector2){ GRID_SIZE, GRID_SIZE }, (Color){ 255, 0, 0, 255 });
        EndDrawing();
	}

	CloseWindow();

	return 0;
}
