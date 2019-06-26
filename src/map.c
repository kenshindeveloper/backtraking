#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/map.h"

void __InitMatrix(Map *map, const char *path);

Map NewMap(const char *path) {
    Map map = {0};

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
        int cont = 0;
        while(feof(file) == 0) {
            fgets(string, 128, file);
            if (cont == 0) {
                char *auxString = string;

                char *token = strtok(auxString, " ");
                map->steps = atoi(token);

                token = strtok(NULL, " ");
                map->height = atoi(token);

                token = strtok(NULL, " ");
                map->width = atoi(token);

                cont++;
            }
        }
        free(string);
        string = NULL;
        fclose(file);
    }
}
