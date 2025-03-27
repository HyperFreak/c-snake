#include "raylib.h"
#include "../include/snake.h"

const int controls[] = { KEY_D, KEY_A, KEY_S, KEY_W, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP };

void handleInput(Snake* snake) {

    for (unsigned short int i = 0; i < 8; i++) {
        if (IsKeyPressed(controls[i])) {
            snake->direction = i % 4;
        }
    }

}

int main() {
	
	const int screenWidth = 800;
	const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Test Window");


    Snake snake = { {0, 0}, {20, 20}, 0 };

    SetTargetFPS(30);
	while (!WindowShouldClose()) {
        handleInput(&snake);

        moveSnake(&snake, 5);

		BeginDrawing();
		ClearBackground(RAYWHITE);
        renderSnake(&snake);
		DrawText(TextFormat("Dir: %d, = {%d, %d}", snake.direction, snake.direction &2, snake.direction &1), 350, 200, 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
