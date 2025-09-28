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

typedef enum
{
    PLAY = 0,
    PAUSE = 1,
    VICTORY = 2,
    DEAD = 3
} State;

typedef struct SnakeBody
{
    bool apple;
    Direction direction;
    Vector2 position;
} SnakeBody;

typedef struct Snake
{
    Direction direction;
    Direction prevDirection;
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
static int boardSize = 576;
const static int minMarginPercent = 10;
static float tileSize = 28.8f;

static Snake snake;
static Apple apple;
static State state = PLAY;
static int score = 0;
static float timer = 0.0f;

static void Init(void);
static void InitGame(void);
static void Update(void);
static void Draw(void);
static void Pause(void);
static void Restart(void);
static void Move(void);
static void MoveHead(void);
static void MoveBody(void);
static void Die(void);
static void Victory(void);
static void Eat(void);
static void InputDirection(void);

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
    InitGame();
}

void InitGame(void)
{
    score = 0;
    timer = 0.0f;
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
    Snake snake = {RIGHT, RIGHT, initBodySize};

    for (int i = 0; i < initBodySize; i++)
    {
        snake.body[i].apple = false;
        snake.body[i].direction = RIGHT;
        snake.body[i].position = (Vector2){10.0f - i, 10.0f};
    }
    for (int i = initBodySize; i < maxBodySize; i++)
    {
        snake.body[i].apple = false;
        snake.body[i].position = (Vector2){0.0f, 0.0f};
    }
    return snake;
}

void Update(void)
{
    Pause();
    Restart();
    if (state != PLAY)
        return;
    Move();
    Victory();
    Die();
    Eat();
    InputDirection();
}

void Restart(void)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        if ((state == VICTORY) || (state == DEAD))
        {
            state = PLAY;
            InitGame();
        }
    }
}

void Pause(void)
{
    if (IsKeyPressed(KEY_P))
    {
        if (state == PLAY)
            state = PAUSE;
        else if (state == PAUSE)
            state = PLAY;
    }
}

void Move(void)
{
    timer += GetFrameTime();

    if (timer >= stepTime)
    {
        timer = 0.0f;
        snake.prevDirection = snake.direction;
        MoveBody();
        MoveHead();
    }
}

void MoveBody(void)
{
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
        snake.body[i].direction = snake.body[i - 1].direction;
        snake.body[i].position.x = snake.body[i - 1].position.x;
        snake.body[i].position.y = snake.body[i - 1].position.y;
    }
}

void MoveHead(void)
{
    snake.body[HEAD].direction = snake.direction;
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

void Victory(void)
{
    if (snake.bodySize == (ROWS * COLUMNS))
        state = VICTORY;
}

void Die(void)
{
    if (state == VICTORY)
        return;
    for (int i = 4; i < snake.bodySize; i++)
        if ((snake.body[HEAD].position.x == snake.body[i].position.x) && (snake.body[HEAD].position.y == snake.body[i].position.y))
            state = DEAD;
}

void Eat(void)
{
    if (state == VICTORY)
        return;
    if ((snake.body[HEAD].position.x == apple.position.x) && (snake.body[HEAD].position.y == apple.position.y))
    {
        snake.body[HEAD].apple = true;
        apple.position = GetValidApplePosition(snake);
        score++;
    }
}

void InputDirection(void)
{
    if (IsKeyDown(KEY_RIGHT) && (snake.prevDirection != LEFT))
        snake.direction = RIGHT;
    else if (IsKeyDown(KEY_LEFT) && (snake.prevDirection != RIGHT))
        snake.direction = LEFT;
    else if (IsKeyDown(KEY_UP) && (snake.prevDirection != DOWN))
        snake.direction = UP;
    else if (IsKeyDown(KEY_DOWN) && (snake.prevDirection != UP))
        snake.direction = DOWN;
}

void Draw(void)
{
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    Vector2 margin = (Vector2){72.0f, 72.0f};
    if (screenHeight <= screenWidth)
    {
        margin.y = (float)(screenHeight * minMarginPercent) / 100;
        boardSize = screenHeight - margin.y - margin.y;
        margin.x = (float)(screenWidth - boardSize) / 2;
    }
    else
    {
        margin.x = (float)(screenWidth * minMarginPercent) / 100;
        boardSize = screenWidth - margin.x - margin.x;
        margin.y = (float)(screenHeight - boardSize) / 2;
    }

    tileSize = (float)boardSize / ROWS;

    BeginDrawing();

    ClearBackground(GREEN);

    for (int i = 0; i <= COLUMNS; i++)
    {
        int x = margin.x + (i * tileSize);
        DrawLine(x, margin.y, x, screenHeight - margin.y, RED);
    }
    for (int i = 0; i <= ROWS; i++)
    {
        int y = margin.y + (i * tileSize);
        DrawLine(margin.x, y, screenWidth - margin.x, y, RED);
    }

    Vector2 size = (Vector2){tileSize, tileSize};
    DrawRectangleV(Vector2Add(Vector2Scale(apple.position, tileSize), margin), size, RED);
    for (int i = 0; i < snake.bodySize; i++)
    {
        Color color = BLACK;
        if (i == 0)
            color = DARKGRAY;
        else if (snake.body[i].apple)
            color = BROWN;
        DrawRectangleV(Vector2Add(Vector2Scale(snake.body[i].position, tileSize), margin), size, color);
    }

    DrawText(TextFormat("Score: %i", score), 10, 10, 20, BLACK);

    DrawText("SNAKE", (screenWidth / 2) - (MeasureText("SNAKE", 60) / 2) + 2, (margin.y / 2) - 30 + 2, 60, BLACK);
    DrawText("SNAKE", (screenWidth / 2) - (MeasureText("SNAKE", 60) / 2), (margin.y / 2) - 30, 60, RED);

    switch (state)
    {
    case VICTORY:
        DrawText("VICTORY", (screenWidth / 2) - (MeasureText("VICTORY", 100) / 2) + 2, (screenHeight / 2) - 50 + 2, 100, BLACK);
        DrawText("VICTORY", (screenWidth / 2) - (MeasureText("VICTORY", 100) / 2), (screenHeight / 2) - 50, 100, GOLD);
        DrawText("Press ENTER to play again.", (screenWidth / 2) - (MeasureText("Press ENTER to play again.", 20) / 2), screenHeight - (margin.y / 2) - 10, 20, BLACK);
        break;
    case PAUSE:
        DrawText("GAME PAUSED", (screenWidth / 2) - (MeasureText("GAME PAUSED", 50) / 2) + 2, (screenHeight / 2) - 25 + 2, 50, BLACK);
        DrawText("GAME PAUSED", (screenWidth / 2) - (MeasureText("GAME PAUSED", 50) / 2), (screenHeight / 2) - 25, 50, GRAY);
        DrawText("Press P to resume.", (screenWidth / 2) - (MeasureText("Press P to resume.", 20) / 2), screenHeight - (margin.y / 2) - 10, 20, BLACK);
        break;
    case DEAD:
        DrawText("UPS", (screenWidth / 2) - (MeasureText("UPS", 100) / 2) + 2, (screenHeight / 2) - 50 + 2, 100, BLACK);
        DrawText("UPS", (screenWidth / 2) - (MeasureText("UPS", 100) / 2), (screenHeight / 2) - 50, 100, GRAY);
        DrawText("Press ENTER to play again.", (screenWidth / 2) - (MeasureText("Press ENTER to play again.", 20) / 2), screenHeight - (margin.y / 2) - 10, 20, BLACK);
        break;
    case PLAY:
        break;
    }

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