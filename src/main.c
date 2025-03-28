#include "raylib.h"
#include "../include/snake.h"

#define GRID_SIZE 16
#define TILES_X 60
#define TILES_Y 32
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

const int controls[] = { KEY_D, KEY_A, KEY_S, KEY_W, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP };

unsigned short int handleInput(unsigned short int current) {

    for (unsigned short int i = 0; i < 8; i++) {
        if (IsKeyPressed(controls[i])) {
            return i % 4;
        }
    }
    return current;
}

int main() {
	
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test Window");

    unsigned short int score = 0;
    const unsigned short int fieldWidth = TILES_X * GRID_SIZE;
    const unsigned short int fieldHeight = TILES_Y * GRID_SIZE;
    Vector2 fieldSize = { fieldWidth, fieldHeight };
    Vector2 fieldPos = { WINDOW_WIDTH / 2 - (fieldWidth / 2), WINDOW_HEIGHT / 2 - (fieldHeight / 2) };
    Vector2 fruitPos = { 0, 0 };

    Snake snake = { {WINDOW_WIDTH / 2 - GRID_SIZE, WINDOW_HEIGHT / 2 - GRID_SIZE}, {0, 0}, {GRID_SIZE, GRID_SIZE}, 0 };
    snake.lastPos = snake.position;
    snake.lastPos.x -= GRID_SIZE;
    addTail(&snake);

    SetTargetFPS(60);
    unsigned short int movementTimer = 0;
    const unsigned short int movementTime = 12; // 60 / 12 = 5 steps per second
    unsigned short int direction = 0;
	while (!WindowShouldClose()) {
        direction = handleInput(direction);

        movementTimer++;
        if (movementTimer >= movementTime) {
            movementTimer = 0;
            setSnakeDirection(&snake, direction);
            moveSnake(&snake, GRID_SIZE);    
        }

		BeginDrawing();
		ClearBackground(RAYWHITE);
        DrawRectangleV(fieldPos, fieldSize, BLACK);

        DrawRectangleV(fruitPos, (Vector2){ GRID_SIZE, GRID_SIZE }, (Color){ 255, 0, 0, 255 });
        renderSnake(&snake);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
