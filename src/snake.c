#include "raylib.h"

#define HEAD 0
#define ROWS 20
#define COLUMNS 20

typedef enum
{
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
} Direction;

typedef struct SnakeBody
{
    bool apple;
    Rectangle rectangle;
} SnakeBody;

typedef struct Snake
{
    Direction direction;
    int bodySize;
    SnakeBody body[ROWS * COLUMNS];
} Snake;

typedef struct Apple
{
    Rectangle rectangle;
} Apple;

Snake CreateSnake(int initBodySize, int maxBodySize, int tileSize)
{
    Snake snake = {RIGHT, initBodySize};

    for (int i = 0; i < initBodySize; i++)
    {
        snake.body[i].apple = false;
        snake.body[i].rectangle.x = (10 * tileSize) - (i * tileSize);
        snake.body[i].rectangle.y = 10 * tileSize;
        snake.body[i].rectangle.width = tileSize;
        snake.body[i].rectangle.height = tileSize;
    }
    for (int i = initBodySize; i < maxBodySize; i++)
    {
        snake.body[i].apple = false;
        snake.body[i].rectangle.x = 0;
        snake.body[i].rectangle.y = 0;
        snake.body[i].rectangle.width = 0;
        snake.body[i].rectangle.height = 0;
    }
    return snake;
}

Apple CreateApple(int tileSize)
{
    Apple apple = {
        {GetRandomValue(0, ROWS - 1) * tileSize,
         GetRandomValue(0, COLUMNS - 1) * tileSize,
         tileSize,
         tileSize}};
    return apple;
}

int main(void)
{
    const int screenWidth = 720;
    const int screenHeight = 720;

    const int tileSize = screenWidth / ROWS;

    const int initBodySize = 3;
    const int maxBodySize = ROWS * COLUMNS;

    const int stepSize = tileSize;
    const float stepTime = 1.0f;

    InitWindow(screenWidth, screenHeight, "Snake 🐍");

    Snake snake = CreateSnake(initBodySize, maxBodySize, tileSize);
    Apple apple = CreateApple(tileSize);

    float timer = 0.0f;
    int score = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        timer += GetFrameTime();

        if (timer >= stepTime)
        {
            timer = 0.0f;
            for (int i = maxBodySize; i > HEAD; i--)
            {
                if ((i == snake.bodySize) && (snake.body[i - 1].apple))
                {
                    snake.body[i - 1].apple = false;

                    snake.body[snake.bodySize].apple = false;
                    snake.body[snake.bodySize].rectangle.x = snake.body[i].rectangle.x;
                    snake.body[snake.bodySize].rectangle.y = snake.body[i].rectangle.y;
                    snake.body[snake.bodySize].rectangle.width = tileSize;
                    snake.body[snake.bodySize].rectangle.height = tileSize;
                    snake.bodySize++;
                }
                snake.body[i].apple = snake.body[i - 1].apple;
                snake.body[i - 1].apple = false;
                snake.body[i].rectangle.x = snake.body[i - 1].rectangle.x;
                snake.body[i].rectangle.y = snake.body[i - 1].rectangle.y;
            }
            switch (snake.direction)
            {
            case UP:
                snake.body[HEAD].rectangle.y -= stepSize;
                if (snake.body[HEAD].rectangle.y < 0)
                    snake.body[HEAD].rectangle.y = (ROWS - 1) * tileSize;
                break;
            case RIGHT:
                snake.body[HEAD].rectangle.x += stepSize;
                if (snake.body[HEAD].rectangle.x > (COLUMNS * tileSize))
                    snake.body[HEAD].rectangle.x = 0;
                break;
            case DOWN:
                snake.body[HEAD].rectangle.y += stepSize;
                if (snake.body[HEAD].rectangle.y > (ROWS * tileSize))
                    snake.body[HEAD].rectangle.y = 0;
                break;
            case LEFT:
                snake.body[HEAD].rectangle.x -= stepSize;
                if (snake.body[HEAD].rectangle.x < 0)
                    snake.body[HEAD].rectangle.x = (COLUMNS - 1) * tileSize;
                break;
            default:
                snake.body[HEAD].rectangle.x += stepSize;
                if (snake.body[HEAD].rectangle.x > (COLUMNS * tileSize))
                    snake.body[HEAD].rectangle.x = 0;
                break;
            }
        }

        if ((snake.body[HEAD].rectangle.x == apple.rectangle.x) && (snake.body[HEAD].rectangle.y == apple.rectangle.y))
        {
            snake.body[HEAD].apple = true;
            apple.rectangle.x = GetRandomValue(0, ROWS - 1) * tileSize;
            apple.rectangle.y = GetRandomValue(0, COLUMNS - 1) * tileSize;
            score++;
        }

        if (IsKeyDown(KEY_RIGHT) && (snake.direction != LEFT))
            snake.direction = RIGHT;
        else if (IsKeyDown(KEY_LEFT) && (snake.direction != RIGHT))
            snake.direction = LEFT;
        else if (IsKeyDown(KEY_UP) && (snake.direction != DOWN))
            snake.direction = UP;
        else if (IsKeyDown(KEY_DOWN) && (snake.direction != UP))
            snake.direction = DOWN;

        BeginDrawing();

        ClearBackground(GREEN);

        for (int i = 0; i < screenHeight / tileSize; i++)
            DrawLine(i * tileSize, 0, i * tileSize, screenHeight, RED);
        for (int i = 0; i < screenWidth / tileSize; i++)
            DrawLine(0, i * tileSize, screenWidth, i * tileSize, RED);

        DrawRectangleRec(apple.rectangle, RED);
        for (int i = 0; i < snake.bodySize; i++)
        {
            if (snake.body[i].apple)
                DrawRectangleRec(snake.body[i].rectangle, BROWN);
            else
                DrawRectangleRec(snake.body[i].rectangle, BLACK);
        }

        DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);
        DrawText(TextFormat("Body size: %i", snake.bodySize), 10, 40, 20, BLACK);
        DrawText(TextFormat("Head: [%i, %i]", (int)(snake.body[HEAD].rectangle.x / tileSize), (int)(snake.body[HEAD].rectangle.y / tileSize)), 10, 70, 20, BLACK);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}