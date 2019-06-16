/**
 * @file main.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief
 * @version 1.0
 * @date 2019-06-08
 */
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "headers/config.h"
#include "headers/map.h"

/**
 * @fn main
 * @brief Entrada principal del programa.
 */
int main(int argc, char *argv[]) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Don Pepe y sus globos!");
    SetExitKey(KEY_F8);
    HideCursor();

    Font font = LoadFont("resources/fonts/04B_03__.TTF");
    const char *msg = "Push key space to start.";

    Map map = NewMap("resources/maps/Entrada.txt");
    if (!ValidateMap(&map))
        return EXIT_FAILURE;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateMap(&map);
        BeginDrawing();
            ClearBackground((Color) {43, 43, 38, 255});
            // ClearBackground((Color) {224, 219, 205, 255});
            DrawMap(&map);
            DrawRectangleRec(
                (Rectangle) {0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40},
                (Color) {43, 43, 38, 255}
            );
            DrawTextEx(
                font,
                msg,
                (Vector2) {28, SCREEN_HEIGHT - 32},
                24,
                1,
                (Color) {168, 159, 148, 255}
            );

        EndDrawing();
    }
    UnloadFont(font);
    DeleteMap(&map);
    CloseWindow();

    return EXIT_SUCCESS;
}
