#include "raylib.h"

int main() {
	
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Test Window");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("TESTTESTTEST", 350, 200, 20, DARKGRAY);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
