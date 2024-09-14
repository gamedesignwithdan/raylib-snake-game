#include "include/raylib.h"
#include <stdio.h>
#define TILE_SIZE 30
#define ROWS 30
#define COLS 20

static short food_position[2], snake_position[2], snake_direction = 0, snake_part_time[ROWS][COLS] = {0}, snake_length = 2, score = 0, game_over = 0;

void draw_grid()
{
    for (int i = 0; i < GetScreenWidth() / TILE_SIZE + 1; i++)
    {
        DrawLineV((Vector2){TILE_SIZE * i, 0}, (Vector2){TILE_SIZE * i, GetScreenHeight()}, LIGHTGRAY);
    }
    for (int i = 0; i < GetScreenHeight() / TILE_SIZE + 1; i++)
    {
        DrawLineV((Vector2){0, TILE_SIZE * i}, (Vector2){GetScreenWidth(), TILE_SIZE * i}, LIGHTGRAY);
    }
}

void draw_score()
{
    DrawText(TextFormat("SCORE: %i", score), TILE_SIZE, TILE_SIZE - 2, 20, BLACK);
}

void draw_snake()
{
    for (short i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (snake_part_time[i][j] > 0)
            {
                if (snake_part_time[i][j] == (snake_length - 1))
                {
                    // FRONT CELL
                    DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, BLUE);
                }
                else if (snake_part_time[i][j] == 1)
                {
                    // BACK CELL
                    DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, ORANGE);
                }
                else
                {
                    DrawRectangle(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
                }
            }
}

int get_x_direction()
{
    if (snake_direction == 0)
        return 1;
    else if (snake_direction == 1)
        return -1;
    return 0;
}

int get_y_direction()
{
    if (snake_direction == 2)
        return 1;
    else if (snake_direction == 3)
        return -1;
    return 0;
}

int main()
{
    // setup
    InitWindow(ROWS * TILE_SIZE, COLS * TILE_SIZE, "Snake");
    SetTargetFPS(8);

    // food starting position
    food_position[0] = GetRandomValue(5, ROWS - 5);
    food_position[1] = GetRandomValue(5, COLS - 5);

    // snake starting position
    snake_position[0] = (int)ROWS / 2;
    snake_position[1] = (int)COLS / 2;

    while (!WindowShouldClose())
    {
        if (game_over == 0)
        {
            snake_position[0] += get_x_direction();
            snake_position[1] += get_y_direction();

            // REMOVE FOR MORE OF A CHALLENGE - ENABLING KILLS SNAKE WHEN CROSSING ITSELF
            // if (snake_part_time[snake_position[0]][snake_position[1]] > 0)
            // {
            //     game_over = 1;
            // }

            snake_part_time[snake_position[0]][snake_position[1]] = snake_length;

            for (short i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    snake_part_time[i][j] -= (snake_part_time[i][j] > 0) ? 1 : 0;
                }
            }

            snake_direction = IsKeyDown(KEY_LEFT) ? 1 : (IsKeyDown(KEY_RIGHT) ? 0 : (IsKeyDown(KEY_UP) ? 3 : (IsKeyDown(KEY_DOWN) ? 2 : snake_direction)));

            if ((snake_position[0] < 0) || (snake_position[1] < 0) || (snake_position[0] >= ROWS) || (snake_position[1] >= COLS))
                game_over = 1;
            if ((snake_position[0] == food_position[0]) && (snake_position[1] == food_position[1]))
            {
                score += 100;
                snake_length++;
                food_position[0] = GetRandomValue(5, ROWS - 5);
                food_position[1] = GetRandomValue(5, COLS - 5);
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_grid();
        draw_score();

        if (game_over == 0)
        {
            // re-draw food
            DrawRectangle(food_position[0] * TILE_SIZE, food_position[1] * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);

            draw_snake();
        }
        else
        {
            // draw failure text
            DrawText("YOU LOST!", GetScreenWidth() / 2 - (MeasureText("YOU LOST!", 40) / 2), COLS * TILE_SIZE / 4, 40, RED);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
