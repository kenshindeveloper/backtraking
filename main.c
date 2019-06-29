#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus

typedef enum bool {
    false = 0,
    true
}bool;

#endif //__cplusplus

/**************************************************************
 *-----------------------ESTRUCTURAS---------------------------
 **************************************************************/

typedef struct Point {
    int x;
    int y;
}Point;

typedef struct List {
    Point point;
    struct List *prox;
}List;

typedef struct Map {
    int width;
    int height;
    int maxSteps;
    int numEnemies;
    char **matrix;
}Map;


/**************************************************************
 *-----------------------PROTOTIPOS----------------------------
 **************************************************************/

Map NewMap(const char *path);

void DeleteMap(Map *map);

static bool __InitMatrixMap(Map *map, const char *path);

static void __PrintMatrixMap(const Map *map);

/*************************************************************
*-----------------------FUNC--MAIN----------------------------
**************************************************************/


int main(int argc, char *argv[]) {
    const char *path = "entrada.txt";
    if (argc == 2)
        path = argv[1];

    Map map = NewMap(path);

    __PrintMatrixMap(&map);

    DeleteMap(&map);

    return EXIT_SUCCESS;
}

/**************************************************************
 *-----------------------IMPLEMENTACION------------------------
 **************************************************************/
 Map NewMap(const char *path) {
     Map map = {0};
     map.matrix = NULL;
     __InitMatrixMap(&map, path);
     return map;
 }

 void DeleteMap(Map *map) {
     for (int i=0; i < map->height; i++) {
         free(map->matrix);
         map->matrix = NULL;
     }

     if (map->matrix != NULL) {
         free(map->matrix);
         map->matrix = NULL;
     }
 }

 static bool __InitMatrixMap(Map *map, const char *path) {
     FILE *file = fopen(path, "r");
     if (file == NULL) {
         printf("No se ha podido abrir el fichero.\n");
         return false;
     }

     int indexRow = -1;
     char *row = (char *)malloc(sizeof(char)*128);
     while (feof(file) == 0) {
         fgets(row, 128, file);
         if (indexRow == -1) {
             char *auxRow = (char *)malloc(sizeof(char)*128);
             strcpy(auxRow, row);
             
             char *token = strtok(auxRow, " ");
             map->maxSteps = atoi(token);

             token = strtok(NULL, " ");
             map->width = atoi(token);

             token = strtok(NULL, " ");
             map->height = atoi(token);

             free(auxRow);
         }
         else if (indexRow > -1) {
         }

         printf("%s", row);
         indexRow++;
     }

     if (row != NULL) {
         free(row);
         row = NULL;
     }
     fclose(file);
     return true;
 }

 static void __PrintMatrixMap(const Map *map) {
     printf("\n");
     for (int i=0; i < map->height; i++) {
         for (int j=0; j < map->width; j++) {
             printf("%c ", map->matrix[i][j]);
         }
         printf("\n");
     }
     printf("\n");
 }
