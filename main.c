#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1

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
    int stepsBomb;
    int steps;
    int numEnemies;
    int numPlayer;
    char **matrix;
    char *events;
    bool isLoaded;
    bool isPutBomb;
    Point posBomb;
}Map;

typedef struct ArrayMap {
    Map *map;
    struct ArrayMap *prox;
}ArrayMap;

typedef struct ListArray {
    Array *array;
    struct ListArray *prox;
}ListArray;

/**************************************************************
 *-----------------------GLOBALES------------------------------
 **************************************************************/
static Point POS_PLAYER = {0};
static ArrayMap *COPY_MAPS = NULL;
static ListArray *COPY_ARRAYS = NULL;
static char *TOTAL_EVENTS = NULL;
static int TOTAL_STEPS = 10000; // se inicializa con un valor grande.

/**************************************************************
 *-----------------------PROTOTIPOS----------------------------
 **************************************************************/
Map NewMap(const char *path);

void DeleteMap(Map *map);

Map *CopyMap(Map *map); 

void AddArrayMap(ArrayMap **arrayMap, Map *map);

void DeleteArrayMap(ArrayMap **arrayMap);

void AddPointArray(Array **array, Point point);

void DeleteArray(Array **array);

Array *CopyArray(Array *array);

void PrintArray(Array *const array);

char **CopyMatrix(Map *map);

bool Backtraking(Map *map, Point posPlayer, Array *visitBomb, Array *visitNoBomb);

char *CopyEventArray(Map *map);

void AddListArray(ListArray **listArray, Array *array);

void DeleteListArray(ListArray **listArray);

/**************************************************************
 *-----------------PROTOTIPOS-ESTATICOS------------------------
 **************************************************************/
static bool __InitMatrixMap(Map *map, const char *path);

static void __PrintMatrixMap(const Map *map);

static bool __SaveMatrixMap(Map *map, const char *rowData, int iRow);

static void __SaveInfoDataMap(Map *map, char *rowData);

static void __InitMatrix(Map *map);

static bool __ValidateDataMatrix(Map *map);

static bool __ValidateCharacter(char c);

static bool __EvaluateExplotionVertical(Map *map);

static bool __EvaluateExplotionHorizontal(Map *map);

static bool __ValidateExplotion(Map *map);

static bool __ValidatePutBomb(Map *map, Point posPlayer);

static void __ValidateMovePlayer(Map *map, Point posPlayer, Array *visitBomb, Array *visitNoBomb);

static bool __ValidatePointPath(Array *path, Point point);

static void __CopyEventsToEventsTotal(Map *map);

static void __PrintArray(Array *array);

static bool __IsSavedPlayer(Map *map);

static bool __IsSavedPlayerVertical(Map *map);

static bool __IsSavedPlayerHorizontal(Map *map);
/*************************************************************
*-----------------------FUNC--MAIN----------------------------
**************************************************************/
int main(int argc, char *argv[]) {
    system("cls");
    const char *path = "maps/map1.txt";
    if (argc == 2)
        path = argv[1];

    Map map = NewMap(path);
    if (map.isLoaded) {
        Backtraking(&map, POS_PLAYER, NULL, NULL);
        printf("solucion: ");
        printf("%s\n", TOTAL_EVENTS);
        DeleteListArray(&COPY_ARRAYS);
        DeleteArrayMap(&COPY_MAPS);
    }

    DeleteMap(&map);
    system("pause");
    return EXIT_SUCCESS;
}

/**************************************************************
 *-----------------------IMPLEMENTACION------------------------
 **************************************************************/
void AddListArray(ListArray **listArray, Array *array) {
    if ((*listArray) == NULL) {
        (*listArray) = (ListArray *)malloc(sizeof(ListArray));
        (*listArray)->array = &(*array);
        (*listArray)->prox = NULL;
    }
    else {
        ListArray *auxListArray = (*listArray);
        while (auxListArray->prox != NULL)
            auxListArray = auxListArray->prox;
        
        auxListArray->prox = (ListArray *)malloc(sizeof(ListArray));
        auxListArray->prox->array = &(*array);
        auxListArray->prox->prox = NULL;
    }
}

void DeleteListArray(ListArray **listArray) {
    ListArray *auxListArray = NULL;
    while ((*listArray) != NULL) {
        auxListArray = (*listArray);
        (*listArray) = (*listArray)->prox;
        DeleteArray(&(auxListArray->array));
        free(auxListArray);
        auxListArray = NULL;
    }
}

char *CopyEventArray(Map *map) {
    char *copyEvents = (char *)malloc(sizeof(char)*(map->maxSteps+2));
    memset(copyEvents, ' ', (map->maxSteps+2));
    copyEvents[map->maxSteps+1] = '\0';
    for (int i=0; i < map->maxSteps; i++)
        copyEvents[i] = map->events[i];

    return copyEvents;
}

char **CopyMatrix(Map *map) {
    char **copyMatrix = (char **)malloc(sizeof(char *)*map->height);

    for (int i=0; i < map->height; i++) {
        copyMatrix[i] = (char *)malloc(sizeof(char)*map->width);
        for (int j=0; j < map->width; j++)
            copyMatrix[i][j] = map->matrix[i][j];
    }
    return copyMatrix;
}

void AddArrayMap(ArrayMap **arrayMap, Map *map) {
    if ((*arrayMap) == NULL) {
        (*arrayMap) = (ArrayMap *)malloc(sizeof(ArrayMap));
        (*arrayMap)->map = &(*map);
        (*arrayMap)->prox = NULL;
    }
    else {
        ArrayMap *auxArrayMap = (*arrayMap);
        while (auxArrayMap->prox != NULL)
            auxArrayMap = auxArrayMap->prox;
        
        auxArrayMap->prox = (ArrayMap *)malloc(sizeof(ArrayMap));
        auxArrayMap->prox->map = &(*map);
        auxArrayMap->prox->prox = NULL;
    }
}

Map *CopyMap(Map *map) {
    Map *copyMap = (Map *)malloc(sizeof(Map));
    copyMap->width = map->width;
    copyMap->height = map->height;
    copyMap->maxSteps = map->maxSteps;
    copyMap->stepsBomb = map->stepsBomb;
    copyMap->steps = map->steps;
    copyMap->numEnemies = map->numEnemies;
    copyMap->numPlayer = map->numPlayer;
    copyMap->isLoaded = map->isLoaded;
    copyMap->isPutBomb = map->isPutBomb;
    copyMap->posBomb = map->posBomb;
    copyMap->matrix = CopyMatrix(map);
    copyMap->events = CopyEventArray(map);

    AddArrayMap(&COPY_MAPS, &(*copyMap));
    return copyMap;
}

void DeleteArrayMap(ArrayMap **arrayMap) {
    ArrayMap *auxArrayMap = NULL;

    while ((*arrayMap) != NULL) {
        auxArrayMap = (*arrayMap);
        (*arrayMap) = (*arrayMap)->prox;
        DeleteMap(auxArrayMap->map);
        free(auxArrayMap);
        auxArrayMap = NULL;
    }

    if (arrayMap != NULL) {
        free(arrayMap);
        arrayMap = NULL;
    }
}

Map NewMap(const char *path) {
    Map map = {0};
    map.steps = -1;
    map.isLoaded = __InitMatrixMap(&map, path);
    if (map.isLoaded) {
        map.events = (char *)malloc(sizeof(char)*(map.maxSteps+2));
        memset(map.events, ' ', (map.maxSteps+2));
        map.events[map.maxSteps+1] = '\0';
    }
    return map;
}

void DeleteMap(Map *map) {
    if (map != NULL) {
        if (map->events != NULL) {
            free(map->events);
            map->events = NULL;
        }

        for (int i=0; i < map->height; i++) {
            free(map->matrix[i]);
            map->matrix[i] = NULL;
        }

        if (map->matrix != NULL) {
            free(map->matrix);
            map->matrix = NULL;
        }
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

Array *CopyArray(Array *array) {
    Array *copyArray = NULL;
    Array *auxArray = array;

    while (auxArray != NULL) {
        AddPointArray(&copyArray, auxArray->point);
        auxArray = auxArray->prox;
    }

    if (copyArray != NULL)
        AddListArray(&COPY_ARRAYS, &(*copyArray));
    
    return copyArray;
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

bool Backtraking(Map *map, Point posPlayer, Array *visitBomb, Array *visitNoBomb) {
    if (map->numEnemies <= 0 || (map->steps > (map->maxSteps-1)))
        return false;
        
    if (map->isPutBomb && map->stepsBomb >= 3) {
        if (!__ValidateExplotion(map)) {
#ifdef DEBUG
            printf("MUERTO!!!!!!!!!\n");
#endif //DEBUG
            return false;
        }
        map->matrix[map->posBomb.y][map->posBomb.x] = FLOOR;
#ifdef DEBUG
        printf("EXPLOTA!!!!!!!!!!!! posBom: (%d, %d)\n", map->posBomb.x, map->posBomb.y);
#endif
        // DeleteArray(&visitBomb);
        // DeleteArray(&visitNoBomb);
        visitBomb = NULL;
        visitNoBomb = NULL;
        map->stepsBomb = 0;
        map->isPutBomb = false;
    }

    Array *auxVisitBomb = NULL;
    Array *auxVisitNoBomb = NULL;
    if (map->isPutBomb) {
        AddPointArray(&visitBomb, posPlayer);
        auxVisitBomb = CopyArray(visitBomb);
        map->stepsBomb++;
    }
    else {
        AddPointArray(&visitNoBomb, posPlayer);
        auxVisitNoBomb = CopyArray(visitNoBomb);
    }
    map->steps++;
    if (map->steps < map->maxSteps) {
#ifdef DEBUG
        __PrintMatrixMap(map);
        printf("enemies: %d, Bomb: %d, steps: %d, stepsBomb: %d\n", map->numEnemies, map->isPutBomb, map->steps, map->stepsBomb);
        system("pause");
#endif //DEBUG
        if (!map->isPutBomb && (map->steps < (map->maxSteps-2)) && __ValidatePutBomb(map, posPlayer)) {
            Map *copyMap = CopyMap(map);
            copyMap->events[map->steps] = EVENT_BOMB;
            copyMap->posBomb = (Point) {posPlayer.x, posPlayer.y};
#ifdef DEBUG
            printf("colocando bomba...\n");
#endif //DEBUG
            copyMap->isPutBomb = true;
            Backtraking(copyMap, posPlayer, auxVisitBomb, auxVisitBomb);
        }
        __ValidateMovePlayer(map, posPlayer, auxVisitBomb, auxVisitNoBomb);
        if(map->numEnemies == 0 && (map->steps <= (map->maxSteps-1)) && (TOTAL_STEPS > map->steps)) {
#ifdef DEBUG
            printf(">>>>>>>solucion hallada...\n");
#endif //DEBUG
            
            __CopyEventsToEventsTotal(map);
            TOTAL_STEPS = map->steps;
            return true;
        }
    }
    return false;
}

/**************************************************************
 *--------------IMPLEMENTACION--FUNCIONES-STATIC---------------
 **************************************************************/

static bool __IsSavedPlayerVertical(Map *map) {
    bool evalUp = true;
    bool evalDown = true;
    for (int i=0; ((evalUp || evalDown) && i < map->height); i++) {
        if (evalUp && (map->posBomb.y-i) >=0) {
            if (map->matrix[(map->posBomb.y-i)][map->posBomb.x] == WALL) 
                evalUp = false;
            else if (map->matrix[(map->posBomb.y-i)][map->posBomb.x] == PLAYER)
                return false;
        }

        if (evalDown && (map->posBomb.y+i) < map->height) {
            if (map->matrix[(map->posBomb.y+i)][map->posBomb.x] == WALL) 
                evalDown = false;
            else if (map->matrix[(map->posBomb.y+i)][map->posBomb.x] == PLAYER) 
                return false;
        }
    }
    return true;
}

static bool __IsSavedPlayerHorizontal(Map *map) {
    bool evalLeft = true;
    bool evalRight = true;
    for (int j=0; ((evalLeft || evalRight) && (j < map->width)); j++) {
        if (evalLeft && (map->posBomb.x-j) >=0) {
            if (map->matrix[map->posBomb.y][(map->posBomb.x-j)] == WALL) 
                evalLeft = false;
            else if (map->matrix[map->posBomb.y][(map->posBomb.x-j)] == PLAYER) 
                return false;
        }

        if (evalRight && (map->posBomb.x+j) < map->width) {
             if (map->matrix[map->posBomb.y][(map->posBomb.x+j)] == WALL) 
                evalLeft = false;
            else if (map->matrix[map->posBomb.y][(map->posBomb.x+j)] == PLAYER) 
                return false;
        }
    }
    return true;
}

static bool __IsSavedPlayer(Map *map) {
    return (__IsSavedPlayerVertical(map) && __IsSavedPlayerHorizontal(map));
}

static void __PrintArray(Array *array) {
    Array *auxArray = array;
    while (auxArray != NULL) {
        auxArray = auxArray->prox;
    } 
}

static void __CopyEventsToEventsTotal(Map *map) {
    if (TOTAL_EVENTS == NULL) {
        TOTAL_EVENTS = (char *)malloc(sizeof(char)*(map->maxSteps+1));
        memset(TOTAL_EVENTS, ' ', (map->maxSteps+1));
    }

    for (int i=0; i < map->maxSteps; i++)
        TOTAL_EVENTS[i] = map->events[i];         
}

static bool __ValidatePointPath(Array *path, Point point) {
    if (path != NULL) {
        Array *auxPath = path;
        while (auxPath->prox != NULL) {
            if (auxPath->point.x == point.x && auxPath->point.y == point.y)
                return true;
            auxPath = auxPath->prox;
        }
    }
    return false;
}

static bool __PutExplotionVertical(Map *map, Point posPlayer) {
    bool evalUp = true;
    bool evalDown = true;
    for (int i=0; ((evalUp || evalDown) && (i < map->height)); i++) {
        if (evalUp && (posPlayer.y-i) >=0) {
            if (map->matrix[(posPlayer.y-i)][posPlayer.x] == WALL_DEST ||
                map->matrix[(posPlayer.y-i)][posPlayer.x] == ENEMY) {
                return true;
            }
            else if (map->matrix[(posPlayer.y-i)][posPlayer.x] == WALL)
                evalUp = false;
        }
        if (evalDown && (posPlayer.y+i) < map->height) {
            if (map->matrix[(posPlayer.y+i)][posPlayer.x] == WALL_DEST || 
                map->matrix[(posPlayer.y+i)][posPlayer.x] == ENEMY) {
                return true;
            }
            else if (map->matrix[(posPlayer.y+i)][posPlayer.x] == WALL)
                evalDown = false; 
        }
    }
    return false;
}

static bool __PutExplotionHorizontal(Map *map, Point posPlayer) {
    bool evalLeft = true;
    bool evalRight = true;
    for (int j=0; ((evalLeft || evalRight) && (j < map->width)); j++) {
        if (evalLeft && (posPlayer.x-j) >=0) {
            if (map->matrix[posPlayer.y][(posPlayer.x-j)] == WALL_DEST ||
                map->matrix[posPlayer.y][(posPlayer.x-j)] == ENEMY) {
                return true;
            }
            else if (map->matrix[posPlayer.y][(posPlayer.x-j)] == WALL) {
                evalLeft = false;
            }
        }
        if (evalRight && (posPlayer.x+j) < map->width) {
            if (map->matrix[posPlayer.y][(posPlayer.x+j)] == WALL_DEST ||
                map->matrix[posPlayer.y][(posPlayer.x+j)] == ENEMY) {
                return true;
            }
            else if (map->matrix[posPlayer.y][(posPlayer.x+j)] == WALL)
                evalLeft = false;
        }
    }
    return false;
}

static bool __EvaluateExplotionVertical(Map *map) {
    bool evalUp = true;
    bool evalDown = true;
    for (int i=0; ((evalUp || evalDown) && i < map->height); i++) {
        if (evalUp && (map->posBomb.y-i) >=0) {
            if (map->matrix[(map->posBomb.y-i)][map->posBomb.x] == WALL_DEST) {
                map->matrix[(map->posBomb.y-i)][map->posBomb.x] = '.';
                evalUp = false;
            }
            else if (map->matrix[(map->posBomb.y-i)][map->posBomb.x] == ENEMY) {
                map->matrix[(map->posBomb.y-i)][map->posBomb.x] = '.';
                map->numEnemies--;
            }
            else if (map->matrix[(map->posBomb.y-i)][map->posBomb.x] == WALL) {
                evalUp = false;
            }
            else if (map->matrix[(map->posBomb.y-i)][map->posBomb.x] == PLAYER) {
                return false;
            }
        }

        if (evalDown && (map->posBomb.y+i) < map->height) {
            if (map->matrix[(map->posBomb.y+i)][map->posBomb.x] == WALL_DEST) {
                map->matrix[(map->posBomb.y+i)][map->posBomb.x] = '.';
                evalDown = false;
            }
            
            else if (map->matrix[(map->posBomb.y+i)][map->posBomb.x] == ENEMY) {
                map->matrix[(map->posBomb.y+i)][map->posBomb.x] = '.';
                map->numEnemies--;
            }

            else if (map->matrix[(map->posBomb.y+i)][map->posBomb.x] == WALL) {
                evalDown = false;
            }

            else if (map->matrix[(map->posBomb.y+i)][map->posBomb.x] == PLAYER) {
                return false;
            }
        }
    }
    return true;
}

static bool __EvaluateExplotionHorizontal(Map *map) {
    bool evalLeft = true;
    bool evalRight = true;
    for (int j=0; ((evalLeft || evalRight) && (j < map->width)); j++) {
        if (evalLeft && (map->posBomb.x-j) >=0) {
            if (map->matrix[map->posBomb.y][(map->posBomb.x-j)] == WALL_DEST) {
                map->matrix[map->posBomb.y][(map->posBomb.x-j)] = '.';
                evalLeft = false;
            }
            else if (map->matrix[map->posBomb.y][(map->posBomb.x-j)] == ENEMY) {
                map->matrix[map->posBomb.y][(map->posBomb.x-j)] = '.';
                map->numEnemies--;
            }
            else if (map->matrix[map->posBomb.y][(map->posBomb.x-j)] == WALL) {
                evalLeft = false;
            }
            else if (map->matrix[map->posBomb.y][(map->posBomb.x-j)] == PLAYER) {
                return false;
            }
        }

        if (evalRight && (map->posBomb.x+j) < map->width) {
             if (map->matrix[map->posBomb.y][(map->posBomb.x+j)] == WALL_DEST) {
                map->matrix[map->posBomb.y][(map->posBomb.x+j)] = '.';
                evalRight = false;
            }
            else if (map->matrix[map->posBomb.y][(map->posBomb.x+j)] == ENEMY) {
                map->matrix[map->posBomb.y][(map->posBomb.x+j)] = '.';
                map->numEnemies--;
            }
            else if (map->matrix[map->posBomb.y][(map->posBomb.x+j)] == WALL) {
                evalRight = false;
            }
            else if (map->matrix[map->posBomb.y][(map->posBomb.x+j)] == PLAYER) {
                return false;
            }
        }
    }
    return true;
}

static bool __ValidateExplotion(Map *map) {
    if (!__EvaluateExplotionVertical(map) || !__EvaluateExplotionHorizontal(map))
        return false;   

    return true;
}

static bool __ValidatePutBomb(Map *map, Point posPlayer) {
    return  (__PutExplotionVertical(map, posPlayer) || __PutExplotionHorizontal(map, posPlayer));
}

static void __ValidateMovePlayer(Map *map, Point posPlayer, Array *visitBomb, Array *visitNoBomb) {
    Point movePoint = {0};
    // Move Wait
    if (map->numEnemies > 0 && map->isPutBomb && map->stepsBomb <= 3 && __IsSavedPlayer(map)) {
        Map *copyMoveMap = CopyMap(map);
        copyMoveMap->events[map->steps] = EVENT_WAIT;
        Backtraking(copyMoveMap, posPlayer, visitBomb, visitNoBomb);
        // visitBomb = NULL;
        // visitNoBomb = NULL;
    } 
    // Move Up
    if (map->numEnemies > 0 && (posPlayer.y-1) >= 0 && !__ValidatePointPath((map->isPutBomb)?(visitBomb):(visitNoBomb), (Point) {posPlayer.x, (posPlayer.y-1)}) &&
        map->matrix[posPlayer.y-1][posPlayer.x] == FLOOR) {
        movePoint = (Point) {posPlayer.x, (posPlayer.y-1)};
        Map *copyMoveMap = CopyMap(map);
        copyMoveMap->events[map->steps] = EVENT_UP;
        copyMoveMap->matrix[posPlayer.y][posPlayer.x] = (copyMoveMap->isPutBomb && copyMoveMap->stepsBomb == 1)?(EVENT_BOMB):(FLOOR);
        copyMoveMap->matrix[posPlayer.y-1][posPlayer.x] = PLAYER;
        Backtraking(copyMoveMap, movePoint, visitBomb, visitNoBomb);
    }
    // Move Right
    if (map->numEnemies > 0 && (posPlayer.x+1) < map->width && !__ValidatePointPath((map->isPutBomb)?(visitBomb):(visitNoBomb), (Point) {(posPlayer.x+1), posPlayer.y}) &&
        map->matrix[posPlayer.y][posPlayer.x+1] == FLOOR) {
        movePoint = (Point) {(posPlayer.x+1), posPlayer.y};
        Map *copyMoveMap = CopyMap(map);
        copyMoveMap->events[map->steps] = EVENT_RIGHT;
        copyMoveMap->matrix[posPlayer.y][posPlayer.x] = (copyMoveMap->isPutBomb && copyMoveMap->stepsBomb == 1)?(EVENT_BOMB):(FLOOR);
        copyMoveMap->matrix[posPlayer.y][posPlayer.x+1] = PLAYER;
        Backtraking(copyMoveMap, movePoint, visitBomb, visitNoBomb);
    }
    // Move Down
    if (map->numEnemies > 0 && (posPlayer.y+1) < map->height && !__ValidatePointPath((map->isPutBomb)?(visitBomb):(visitNoBomb), (Point) {posPlayer.x, (posPlayer.y+1)}) &&
        map->matrix[posPlayer.y+1][posPlayer.x] == FLOOR) {
        movePoint = (Point) {posPlayer.x, (posPlayer.y+1)};
        Map *copyMoveMap = CopyMap(map);
        copyMoveMap->events[map->steps] = EVENT_DOWN;
        copyMoveMap->matrix[posPlayer.y][posPlayer.x] = (copyMoveMap->isPutBomb && copyMoveMap->stepsBomb == 1)?(EVENT_BOMB):(FLOOR);
        copyMoveMap->matrix[posPlayer.y+1][posPlayer.x] = PLAYER;
        Backtraking(copyMoveMap, movePoint, visitBomb, visitNoBomb);
    }
    // Move Left
    if (map->numEnemies > 0 && (posPlayer.x-1) >= 0 && !__ValidatePointPath((map->isPutBomb)?(visitBomb):(visitNoBomb), (Point) {(posPlayer.x-1), posPlayer.y}) &&
        map->matrix[posPlayer.y][posPlayer.x-1] == FLOOR) {
        movePoint = (Point) {(posPlayer.x-1), posPlayer.y};
        Map *copyMoveMap = CopyMap(map);
        copyMoveMap->events[map->steps] = EVENT_LEFT;
        copyMoveMap->matrix[posPlayer.y][posPlayer.x] = (copyMoveMap->isPutBomb && copyMoveMap->stepsBomb == 1)?(EVENT_BOMB):(FLOOR);
        copyMoveMap->matrix[posPlayer.y][posPlayer.x-1] = PLAYER;
        Backtraking(copyMoveMap, movePoint, visitBomb, visitNoBomb);
    }
}

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

        if (rowData[iColumn] == PLAYER) {
            POS_PLAYER = (Point) {iColumn, iRow};
            map->numPlayer++;
        }
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
