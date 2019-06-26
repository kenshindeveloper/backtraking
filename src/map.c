#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/map.h"

void __InitMatrix(Map *map, const char *path);

void __SaveInfoHeaderMap(Map *map, char *string);

void __SaveInfoDataMap(Map *map, char *string, int indexRow);

Map NewMap(const char *path) {
    Map map = {0};
    map.matrix = NULL;

    __InitMatrix(&map, path);

    return map;
}

void DeleteMap(Map *map) {
    for (int i=0; i < map->height; i++) {
        free(map->matrix[i]);
        map->matrix[i] = NULL;
    }

    if (map->matrix != NULL) {
        free(map->matrix);
        map->matrix = NULL;
    }
}

void __InitMatrix(Map *map, const char *path) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        char *string = (char *)malloc(sizeof(char)*128);
        int indexRow = -1;
        while(feof(file) == 0) {
            fgets(string, 128, file);
            if (indexRow == -1) {
                __SaveInfoHeaderMap(map, string);
                map->matrix = (char **)malloc(sizeof(char *)*map->height);
                for (int i=0; i < map->height; i++) {
                    map->matrix[i] = (char *)malloc(sizeof(char)*map->width);
                    for (int j=0; j < map->width; j++)
                    map->matrix[i][j] = '=';

                }
            }
            // else
            //     __SaveInfoDataMap(map, string, indexRow);

            indexRow++;
        }
        free(string);
        string = NULL;
        fclose(file);
    }
}

void PrintMap(Map *map) {
    printf("\n");
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            printf("%c ", map->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void __SaveInfoHeaderMap(Map *map, char *string) {
    char *auxString = string;

    char *token = strtok(auxString, " ");
    map->steps = atoi(token);

    token = strtok(NULL, " ");
    map->width = atoi(token);

    token = strtok(NULL, " ");
    map->height = atoi(token);
}

void __SaveInfoDataMap(Map *map, char *string, int indexRow) {
    char *auxString = string;
    int lenString = strlen(string);

    for (int indexColumn=0; indexColumn < lenString; indexColumn++)
        map->matrix[indexRow][indexColumn] = auxString[indexColumn];
        // printf("len: %d\n", lenString);
}
