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

#define FIELD_X WINDOW_WIDTH  / 2 - (TILES_X * GRID_SIZE) / 2
#define FIELD_Y WINDOW_HEIGHT / 2 - (TILES_Y * GRID_SIZE) / 2

const int controls[] = { KEY_D, KEY_A, KEY_S, KEY_W, KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP };               // keys indices i%4: 0: right, 1: left, 2: down, 3: up

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
    setSnakeDirection(snake, *directionInput);                                                              // update direction
    moveSnake(snake, GRID_SIZE);                                                                            // update snake position

    if (collidesWithTail(snake)                                                                             // check for game-over
        || collidesWithWall(snake, FIELD_X, FIELD_X + GRID_SIZE * TILES_X,
             FIELD_Y, FIELD_Y + GRID_SIZE * TILES_Y)) {
        resetToLastPosition(snake);
        return -1;
    }

    if (samePosition(snake->position, *fruitPos)) {                                                         // check for scoring
        score++;
        setToRandomPosition(fruitPos);                                                                      // reposition fruit
        addTail(snake);                                                                                     // increase snake length
        return 1;
    }
    return 0;
}

void drawFieldTiles() {
    unsigned int fieldX = FIELD_X;                                                                          // init field positions once to not calculate
    unsigned int fieldY = FIELD_Y;                                                                          //  the variable every iteration

    for (unsigned short int i = 0; i < TILES_X; i++) {                                                      // render 2D grid
        for (unsigned short int j = 0; j < TILES_Y; j++) {
            DrawRectangle(1 + fieldX + i * GRID_SIZE, 1 + fieldY + j * GRID_SIZE, GRID_SIZE - 2, GRID_SIZE - 2, GRAY);
        }
    }
}

void render(Snake* snake, bool gameOver, unsigned short int score, Vector2 fieldPos, Vector2 fieldSize, Vector2 fruitPos) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText(TextFormat("Score: %d", score), FIELD_X, FIELD_Y - 42, 32, BLACK);                             // draw score
    
    DrawRectangleV(fieldPos, fieldSize, DARKGRAY);                                                          // draw background field
    drawFieldTiles();

    renderSnake(snake);                                                                                     // draw snake and fruit
    DrawRectangleV(fruitPos, (Vector2){ GRID_SIZE - 2, GRID_SIZE - 2 }, (Color){ 255, 0, 0, 255 });

    if (gameOver) {                                                                                         // if game-over
        const char* restartText = "Press Enter to restart";                                                 //  darken the background and render
        const char* gameOverText = "Game Over";                                                             //  a game over screen with final score
        const char* finalScore = TextFormat("Final Score: %d", score);
        DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, (Color){ 0, 0, 0, 120 });
        int gOWidth = MeasureText(gameOverText, 72);
        int scoreWidth = MeasureText(finalScore, 32);
        int restartWidth = MeasureText(restartText, 32);
        DrawText(gameOverText, WINDOW_WIDTH / 2 - gOWidth / 2, WINDOW_HEIGHT / 2 - 100, 72, WHITE);
        DrawText(finalScore, WINDOW_WIDTH / 2 - scoreWidth / 2, WINDOW_HEIGHT / 2 - 16, 32, WHITE);
        DrawText(restartText, WINDOW_WIDTH / 2 - restartWidth / 2, WINDOW_HEIGHT / 2 + 256, 32, WHITE);
    }
    EndDrawing();
}

int main() {
    srand(time(NULL));                                                                                      // init random seed
	
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");                                                       // open window

    Vector2 fieldSize = { TILES_X * GRID_SIZE, TILES_Y * GRID_SIZE };                                       // field position and size
    Vector2 fieldPos = { WINDOW_WIDTH / 2 - (fieldSize.x / 2), WINDOW_HEIGHT / 2 - (fieldSize.y / 2) };

    Vector2 fruitPos = { 0, 0 };                                                                            // init fruit position
    setToRandomPosition(&fruitPos);

    bool gameOver = false;                                                                                  // init score and gameover
    unsigned short int score = 0;

    Snake snake = { {WINDOW_WIDTH / 2 - GRID_SIZE + 1, WINDOW_HEIGHT / 2 - GRID_SIZE + 1}, {0, 0}, {GRID_SIZE-2, GRID_SIZE-2}, 0 };
    snake.lastPos = snake.position;                                                                         // set lastPos one tile to the snake's left
    snake.lastPos.x -= GRID_SIZE;                                                                           //  in order to have the tail be behind the
    addTail(&snake);                                                                                        //  in the start
    unsigned short int direction = 0;

    SetTargetFPS(60);                                                                                       // 60 FPS for rendering and input
    unsigned short int movementTimer = 0;                                                                   // movement timer to reduce movements per second
    const unsigned short int movementTime = 6;                                                              //  without reducing input FPS

    while (!WindowShouldClose()) {                                                                          // Loop keeping the window open
        direction = handleInput(direction);                                                                 // get the direction input

        if (!gameOver) {                                                                                    // if the game is running update the position
            movementTimer++;                                                                                //  every #movementTime# frames
            if (movementTimer >= movementTime) {
                movementTimer = 0;
                switch(gameLoop(&snake, &fruitPos, &direction, score)) {                                    // run game loop and check result
                    case 1:
                        score++;
                        break;
                    case -1:
                        gameOver = true;
                        break;
                }
            }
        } else {                                                                                            // if gameOver wait for player to press Enter
            if (IsKeyPressed(KEY_ENTER)) {                                                                  // reset to start values
                score = 0;
                deleteSnakeTail(&snake);
                snake.position.x = WINDOW_WIDTH / 2 - GRID_SIZE + 1;
                snake.position.y = WINDOW_HEIGHT / 2 - GRID_SIZE + 1;
                snake.lastPos = snake.position;
                snake.lastPos.x -= GRID_SIZE;
                snake.direction = 0;
                direction = 0;
                addTail(&snake);
                gameOver = false;
                movementTimer = 0;
            }
        }
        render(&snake, gameOver, score, fieldPos, fieldSize, fruitPos);                                     // render the game
    }
    CloseWindow();                                                                                          // close window after game loop ends
    return 0;
}
