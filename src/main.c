#include "raylib.h"
#include "../include/snake.h"

#define GRID_SIZE 16

const int controls[] = { KEY_D, KEY_A, KEY_S, KEY_W, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP };

unsigned short int handleInput(Snake* snake) {

    for (unsigned short int i = 0; i < 8; i++) {
        if (IsKeyPressed(controls[i])) {
            return i % 4;
        }
    }
    return snake->direction;
}

int main() {
	
	const int screenWidth = 800;
	const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Test Window");


    Snake snake = { {0, 0}, {0, 0}, {GRID_SIZE, GRID_SIZE}, 0 };

    SetTargetFPS(5);
	while (!WindowShouldClose()) {
        unsigned short int nDir = handleInput(&snake);
        setSnakeDirection(&snake, nDir);

        moveSnake(&snake, GRID_SIZE);
        addTail(&snake);

		BeginDrawing();
		ClearBackground(RAYWHITE);
        renderSnake(&snake);
		DrawText(TextFormat("Dir: %d, = {%d, %d}", snake.direction, snake.direction &2, snake.direction &1), 350, 200, 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
