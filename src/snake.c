#include "raylib.h"
#include "raymath.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ONE 1

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
    Vector2 position;
} SnakeBody;

typedef struct Snake
{
    Direction direction;
    int bodySize;
    SnakeBody body[ROWS * COLUMNS];
} Snake;

typedef struct Apple
{
    Vector2 position;
} Apple;

const static int initBodySize = 3;
const static int maxBodySize = ROWS * COLUMNS;
const static float stepTime = 0.25f;
static int screenWidth = 720;
static int screenHeight = 720;
static float tileSize = 36;

static Snake snake;
static Apple apple;
static int score = 0;
static float timer = 0.0f;

static void Init(void);
static void Update(void);
static void Draw(void);

Snake CreateSnake(int initBodySize, int maxBodySize);
Apple CreateApple(Snake snake);
Vector2 GetValidApplePosition(Snake snake);

int main(void)
{
    Init();
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
    CloseWindow();
    return 0;
}

void Init(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Snake 🐍");
    SetWindowMinSize(480, 480);
    SetTargetFPS(60);
    tileSize = (screenWidth, screenHeight) / ROWS;
    snake = CreateSnake(initBodySize, maxBodySize);
    apple = CreateApple(snake);
}

Apple CreateApple(Snake snake)
{
    Vector2 position = GetValidApplePosition(snake);
    return (Apple){position};
}

Snake CreateSnake(int initBodySize, int maxBodySize)
{
    Snake snake = {RIGHT, initBodySize};

    for (int i = 0; i < initBodySize; i++)
    {
        snake.body[i].apple = false;
        snake.body[i].position.x = 10 - i;
        snake.body[i].position.y = 10;
    }
    for (int i = initBodySize; i < maxBodySize; i++)
    {
        snake.body[i].apple = false;
        snake.body[i].position.x = 0;
        snake.body[i].position.y = 0;
    }
    return snake;
}

void Update(void)
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
                snake.body[snake.bodySize].position.x = snake.body[i].position.x;
                snake.body[snake.bodySize].position.y = snake.body[i].position.y;
                snake.bodySize++;
            }
            snake.body[i].apple = snake.body[i - 1].apple;
            snake.body[i - 1].apple = false;
            snake.body[i].position.x = snake.body[i - 1].position.x;
            snake.body[i].position.y = snake.body[i - 1].position.y;
        }
        switch (snake.direction)
        {
        case UP:
            snake.body[HEAD].position.y -= ONE;
            if (snake.body[HEAD].position.y < 0)
                snake.body[HEAD].position.y = ROWS - 1;
            break;
        case RIGHT:
            snake.body[HEAD].position.x += ONE;
            if (snake.body[HEAD].position.x >= COLUMNS)
                snake.body[HEAD].position.x = 0;
            break;
        case DOWN:
            snake.body[HEAD].position.y += ONE;
            if (snake.body[HEAD].position.y >= ROWS)
                snake.body[HEAD].position.y = 0;
            break;
        case LEFT:
            snake.body[HEAD].position.x -= ONE;
            if (snake.body[HEAD].position.x < 0)
                snake.body[HEAD].position.x = COLUMNS - 1;
            break;
        default:
            snake.body[HEAD].position.x += ONE;
            if (snake.body[HEAD].position.x >= COLUMNS)
                snake.body[HEAD].position.x = 0;
            break;
        }
    }

    if ((snake.body[HEAD].position.x == apple.position.x) && (snake.body[HEAD].position.y == apple.position.y))
    {
        snake.body[HEAD].apple = true;
        apple.position = GetValidApplePosition(snake);
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
}

void Draw(void)
{
    BeginDrawing();

    ClearBackground(GREEN);

    for (int i = 0; i < screenHeight / tileSize; i++)
        DrawLine(i * tileSize, 0, i * tileSize, screenHeight, RED);
    for (int i = 0; i < screenWidth / tileSize; i++)
        DrawLine(0, i * tileSize, screenWidth, i * tileSize, RED);

    Vector2 size = (Vector2){tileSize, tileSize};
    DrawRectangleV(Vector2Scale(apple.position, tileSize), size, RED);
    for (int i = 0; i < snake.bodySize; i++)
    {
        if (snake.body[i].apple)
            DrawRectangleV(Vector2Scale(snake.body[i].position, tileSize), size, BROWN);
        else
            DrawRectangleV(Vector2Scale(snake.body[i].position, tileSize), size, BLACK);
    }

    DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);

    EndDrawing();
}

Vector2 GetValidApplePosition(Snake snake)
{
    Vector2 position = {GetRandomValue(0, ROWS - 1), GetRandomValue(0, COLUMNS - 1)};
    for (int i = 0; i < snake.bodySize; i++)
        if ((position.x == snake.body[i].position.x) && (position.y == snake.body[i].position.y))
            return GetValidApplePosition(snake);
    return position;
}