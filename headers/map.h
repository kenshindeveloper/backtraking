#ifndef MAP_H
#define MAP_H

typedef struct Map {
    int width;
    int height;
    int steps;
    int enemies;
    char **matrix;
}Map;

Map NewMap(const char *path);

void PrintMap(Map *map);

void DeleteMap(Map *map);

#endif //MAP_H
