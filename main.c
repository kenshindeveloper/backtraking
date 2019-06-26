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
#include "headers/point.h"
#include "headers/backtraking.h"
#include "headers/node.h"

Points *enemys = NULL;
Point posPlayer = {0};

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

    // Map map = NewMap("resources/maps/Entrada.txt");
    Map map = NewMap("resources/maps/map_0.txt");
    if (!ValidateMap(&map))
        return EXIT_FAILURE;

#ifdef BDEBUG
    printf("POSITION PLAYER: (%d, %d)\n", posPlayer.x, posPlayer.y);
#endif //BDEBUG
    Node *node = (Node *)malloc(sizeof(Node));
    node->cost = 0;
    node->event = 0;
    node->point = posPlayer;
    node->prev = NULL;
    for (int i=0; i < 4; i++)
        node->child[i] = NULL;
    
    bool result = Backtraking(&map, &node, NULL, posPlayer, 0, 0);
    printf("*********************************\n");
    // PrintNode(node);
    int steps = SearchCostNode(node);
    printf("encontrado: %d\n", result);
    printf("num de move: %d\n", steps);
    printf("node num: %d\n", SizeNode(node));
    ViewPathNode(SearchPathNode(node, steps));
    printf("*********************************\n");


    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateMap(&map);
        BeginDrawing();
            // ClearBackground((Color) {43, 43, 38, 255});
            ClearBackground((Color) {224, 219, 205, 255});
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
    DeleteNode(&node);
    DeleteAllPoints(&enemys); // Libero la memoria por las posiciones de los enemigos creados.
    UnloadFont(font);
    DeleteMap(&map);
    CloseWindow();

    return EXIT_SUCCESS;
}
