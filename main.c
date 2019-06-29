#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EVENT_UP 'N'
#define EVENT_RIGHT 'E'
#define EVENT_DOWN 'S'
#define EVENT_LEFT 'O'
#define EVENT_WAIT '_'
#define EVENT_BOMB 'Q'

#define FLOOR '.'
#define WALL '='
#define WALL_DEST '#'
#define ENEMY 'x'
#define PLAYER '@'

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

typedef struct Array {
    Point point;
    struct Array *prox;

}Array;

typedef struct Map {
    int width;
    int height;
    int maxSteps;
    int numEnemies;
    int numPlayer;

    bool isLoaded;

    Point posBomb;

    char **matrix;

}Map;


/**************************************************************
 *-----------------------PROTOTIPOS----------------------------
 **************************************************************/

Map NewMap(const char *path);

void DeleteMap(Map *map);

static bool __InitMatrixMap(Map *map, const char *path);

static void __PrintMatrixMap(const Map *map);

static bool __SaveMatrixMap(Map *map, const char *rowData, int iRow);

static void __SaveInfoDataMap(Map *map, char *rowData);

static void __InitMatrix(Map *map);

static bool __ValidateDataMatrix(Map *map);

static bool __ValidateCharacter(char c);

static bool __ValidateExplotion(Map map);

bool Bactraking(Map *map, Array *visitBomb, Array *visitNoBomb);

/*************************************************************
*-----------------------FUNC--MAIN----------------------------
**************************************************************/


int main(int argc, char *argv[]) {
    const char *path = "maps/map.txt";
    if (argc == 2)
        path = argv[1];

    Map map = NewMap(path);

    if (map.isLoaded)
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
     map.isLoaded = __InitMatrixMap(&map, path);
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

bool Bactraking(Map *map, Array *visitBomb, Array *visitNoBomb) {

    return false;
}

/**************************************************************
 *--------------IMPLEMENTACION--FUNCIONES-STATIC---------------
 **************************************************************/

static bool __ValidateCharacter(char c) {
    switch (c) {
        case FLOOR:
        case WALL:
        case WALL_DEST:
        case ENEMY:
        case PLAYER:
            return true;
    }

    return false;
}

static void __SaveInfoDataMap(Map *map, char *rowData) {
     char *auxRowData = (char *)malloc(sizeof(char)*strlen(rowData)+1);
     strcpy(auxRowData, rowData);

     map->maxSteps = atoi(strtok(auxRowData, " "));
     map->width = atoi(strtok(NULL, " "));
     map->height = atoi(strtok(NULL, " "));

     free(auxRowData);
 }

static bool __SaveMatrixMap(Map *map, const char *rowData, int iRow) {
    if (map->matrix == NULL)
        __InitMatrix(map);

    size_t sizeRowData = (strlen(rowData)-1);
    for (int iColumn=0; iColumn < sizeRowData; iColumn++) {

        if (!__ValidateCharacter(rowData[iColumn])) {
            printf("Caracter invalido: %c", rowData[iColumn]);
            return false;
        }

        map->matrix[iRow][iColumn] = rowData[iColumn];

        if (rowData[iColumn] == ENEMY)
            map->numEnemies++;

        if (rowData[iColumn] == PLAYER)
            map->numPlayer++;
    }

    return true;
}

static bool __ValidateDataMatrix(Map *map) {

    if (map->numEnemies == 0) {
        printf("No existen enemigos.\n");
        return false;
    }
    else if (map->numPlayer == 0) {
        printf("No existe jugador.");
        return false;
    }
    else if (map->numPlayer > 1) {
        printf("Hay mas de un jugador.");
        return false;
    }

    return true;
}

static bool __InitMatrixMap(Map *map, const char *path) {
     FILE *file = fopen(path, "r");
     if (file == NULL) {
         printf("\nNo se ha podido abrir el fichero.\n\n");
         return false;
     }

     int indexRow = -1;
     char *rowData = (char *)malloc(sizeof(char)*128);
     while (feof(file) == 0) {
         fgets(rowData, 128, file);
         if (indexRow == -1)
             __SaveInfoDataMap(map, rowData);
         else if (indexRow > -1 && indexRow < map->height) {
             if (!__SaveMatrixMap(map, rowData, indexRow))
                return false;
         }

         indexRow++;
     }

     free(rowData);
     rowData = NULL;
     fclose(file);

     return __ValidateDataMatrix(map);
 }

static void __PrintMatrixMap(const Map *map) {
    if (map->matrix != NULL) {
        printf("\n");
        for (int i=0; i < map->height; i++) {
         for (int j=0; j < map->width; j++) {
             printf("%c ", map->matrix[i][j]);
         }
         printf("\n");
        }
        printf("\n");
    }
}

static void __InitMatrix(Map *map) {
    map->matrix = (char **)malloc(sizeof(char *)*map->height);
    for (int iRow=0; iRow < map->height; iRow++) {
        map->matrix[iRow] = (char *)malloc(sizeof(char)*map->width);
        for (int iColumn=0; iColumn < map->width; iColumn++)
            map->matrix[iRow][iColumn] = WALL;
    }
}
