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
    bool isPutBomb;

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


void AddPointArray(Array **array, Point point);

void DeleteArray(Array **array);

Array *CopyArray(Array *const array);

void PrintArray(Array *const array);


bool Backtraking(Map *map, Array *visitBomb, Array *visitNoBomb);

static bool __ValidateExplotion(Map map, Point point, Array *auxVisitNoBomb, Array *auxVisitBomb);

static bool __ValidatePutBomb(Map map, Point point);

static Point __ValidateMovePlayer(Map map, Point point, Array path);


/*************************************************************
*-----------------------FUNC--MAIN----------------------------
**************************************************************/


int main(int argc, char *argv[]) {
    system("cls");
    const char *path = "maps/map.txt";
    if (argc == 2)
        path = argv[1];

    Array *array_0 = NULL;
    AddPointArray(&array_0, (Point) {0, 0});
    AddPointArray(&array_0, (Point) {1, 0});
    AddPointArray(&array_0, (Point) {2, 0});

    Array *array_1 = CopyArray(array_0);
    AddPointArray(&array_1, (Point) {5, 6});
    AddPointArray(&array_1, (Point) {6, 6});
    AddPointArray(&array_1, (Point) {7, 6});

    PrintArray(array_0);
    PrintArray(array_1);

    DeleteArray(&array_0);
    DeleteArray(&array_1);

    if (array_0 == NULL)
        printf("array_0 es NULL.\n");

    if (array_1 == NULL)
        printf("array_1 es NULL.\n");

    // Map map = NewMap(path);
    //
    // if (map.isLoaded)
    //     __PrintMatrixMap(&map);
    //
    // DeleteMap(&map);

    return EXIT_SUCCESS;
}

/**************************************************************
 *-----------------------IMPLEMENTACION------------------------
 **************************************************************/
Map NewMap(const char *path) {
     Map map = {0};
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

 void AddPointArray(Array **array, Point point) {
     if ((*array) == NULL) {
         (*array) = (Array *)malloc(sizeof(Array));
         (*array)->point = point;
         (*array)->prox = NULL;
     }
     else {
         Array *auxArray = (*array);
         while (auxArray->prox != NULL)
            auxArray = auxArray->prox;

        auxArray->prox = (Array *)malloc(sizeof(Array));
        auxArray->prox->point = point;
        auxArray->prox->prox = NULL;
     }
 }

 void DeleteArray(Array **array) {
     Array *auxArray = NULL;
     while ((*array) != NULL) {
         auxArray = (*array);
         (*array) = (*array)->prox;
         free(auxArray);
         auxArray = NULL;
     }
 }

 Array *CopyArray(Array *const array) {
     Array *newArray = NULL;
     const Array *auxArray = array;

     while (auxArray != NULL) {
         AddPointArray(&newArray, auxArray->point);
         auxArray = auxArray->prox;
     }

     return newArray;
 }

 void PrintArray(Array *const array) {
     printf("\n");
     Array *auxArray = array;
     while (auxArray != NULL) {
         printf("(%d, %d) ", auxArray->point.x, auxArray->point.y);
         auxArray = auxArray->prox;
     }
     printf("\n");
 }

bool Backtraking(Map *map, Array *visitBomb, Array *visitNoBomb) {
    // Array *auxVisitNoBomb = visitNoBomb;
    // Array *auxVisitBomb = visitBomb;
    // if (map.isPutBomb && !__ValidateExplotion(map, point, auxVisitNoBomb, auxVisitBomb))
    //     return false;
    //
    // if (map.isPutBomb)
    //     AddPointArray(auxVisitBomb, point);
    // else
    //     AddPointArray(auxVisitBomb, point);
    //
    // Map auxMap = map;
    // if (__ValidatePutBomb(auxMap, point))
    //     Backtraking(auxMap, point, auxVisitNoBomb, auxVisitBomb);
    //
    // auxMap = map;
    // Point movePoint = __ValidateMovePlayer(auxMap, point, ((auxMap.isPutBomb)?(auxVisitBomb):(auxVisitNoBomb)));
    // if (movePoint.x > -1)
    //     Backtraking(auxMap, movePoint, auxVisitNoBomb, auxVisitBomb);
    //
    // if(map.enemies == 0 && map.steps <= map.maxSteps)
    //     return true;

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
