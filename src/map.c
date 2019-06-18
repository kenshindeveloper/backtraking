/**
 * @file map.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 1.0
 * @date 2019-06-08
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/map.h"
#include "../headers/config.h"
#include "../headers/point.h"

extern Points *enemys;
extern Point posPlayer;

//----------------------------------------------------------------------------------
// Prototipos de las funciones estaticas.
//----------------------------------------------------------------------------------

/**
 * @fn __LoadDataMap
 * @brief Funcion estatica, se encargada de leer el fichero donde se almacena el
 * mapa a cargar.
 * @param map Puntero a una estructura de tipo Map.
 * @param path Puntero a una cadena de caracteres constantes donde 
 * se almacena la cadena de caracteres que representa la ruta al
 * fichero a leer.
 * @return void. 
 */
static void __LoadDataMap(Map *map, const char *path);

/**
 * @fn __SaveDataMap
 * @brief Funcion estatic, se encargada de recervar la memoria y almacenar
 * la data en la matriz de la estructura Map.
 * @param map Puntero a una estructura de tipo Map.
 * @param indexRow Indice de la fila.
 * @param data Puntero a una cadena de caracteres constantes donde 
 * se almacena la cadena de caracteres que representa la fila.
 * @return void. 
 */
static void __SaveDataMap(Map *map, int indexRow, char *data);

/**
 * @fn __InitMap
 * @brief Funcion estatica, se encargada de reservar la memoria
 * para la inicializacion de la matriz en la estructura Map.
 * @param map Puntero a una estructura de tipo Map.
 * @return void. 
 */
static void __InitMap(Map * map);

#ifdef BDEBUG
/**
 * @fn __PrintMap
 * @brief Funcion estatica, se encargada de imprimir la matrix en consola.
 * @param map Puntero a una estructura de tipo Map.
 * @return void. 
 */
static void __PrintMap(Map *map);
#endif //BDEBUG

/**
 * @fn __CharValidMap
 * @brief Funcion estatica, se encargada de verificar si el parametro
 * coincide con algun nodo de carga.
 * @param c Caracter a validar.
 * @return bool Si el parametro no coincide con algun node de carga
 * del map retorna false, en caso contrario retorna true.
 * @see config.h
 */
static bool __CharValidMap(char c);

//----------------------------------------------------------------------------------
// Implementacion de las funciones no estaticas.
//----------------------------------------------------------------------------------

Map NewMap(const char *path) {
    Map map = {0};
    map.image = LoadImage("resources/sprites/tiles.png");
    ImageResizeNN(
        &map.image, 
        2 * TILE_SIZE, 3 * TILE_SIZE
    );
    map.texture = LoadTextureFromImage(map.image);
    map.position = (Vector2) {INIT_X, INIT_Y};
    map.matrix = NULL;
    __LoadDataMap(&map, path);

#ifdef BDEBUG
    __PrintMap(&map);
#endif //BDEBUG

    return map;
}

void DeleteMap(Map *map) {
    if (map->matrix != NULL) {
        for (int i=0; i < map->height; i++) {
            free(map->matrix[i]);
#ifdef BDEBUG
            printf("(%d) fila eliminada.\n", i);
#endif //BDEBUG
        }

        UnloadImage(map->image);
        free(map->matrix);
        map->matrix = NULL;
#ifdef BDEBUG
    printf("matriz eliminada.\n");
#endif //BDEBUG
    }
}

void UpdateMap(Map *map) {
    
    if (IsKeyPressed(KEY_LEFT))
        map->position.x -= TILE_SIZE;
    
    else if (IsKeyPressed(KEY_RIGHT))
        map->position.x += TILE_SIZE;
    
    else if (IsKeyPressed(KEY_UP))
        map->position.y -= TILE_SIZE;

    else if (IsKeyPressed(KEY_DOWN))
        map->position.y += TILE_SIZE;
}

void DrawMap(Map *map) {
    Vector2 auxPosition = map->position;
    Rectangle rectangle = {0};
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {

            if (map->matrix[i][j] == PLAYER)
                rectangle = (Rectangle) {0, 0, TILE_SIZE, TILE_SIZE};
            
            else if (map->matrix[i][j] == ENEMY)
                rectangle = (Rectangle) {TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
            
            else if (map->matrix[i][j] == BOMB)
                rectangle = (Rectangle) {0, TILE_SIZE, TILE_SIZE, TILE_SIZE};
            
            else if (map->matrix[i][j] == FLOOR)
                rectangle = (Rectangle) {TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE};
            
            else if (map->matrix[i][j] == INDESTRUCTIBLE_WALL)
                rectangle = (Rectangle) {0, TILE_SIZE*2, TILE_SIZE, TILE_SIZE};
            
            else if (map->matrix[i][j] == DESTRUCTIBLE_WALL)
                rectangle = (Rectangle) {TILE_SIZE, TILE_SIZE*2, TILE_SIZE, TILE_SIZE};
            
            DrawTextureRec(
                map->texture,
                rectangle,
                auxPosition, 
                WHITE
            );
            auxPosition.x += TILE_SIZE+DIFF_TILE;
        }
        auxPosition.x = map->position.x;
        auxPosition.y += TILE_SIZE+DIFF_TILE;
    }
}

bool ValidateMap(Map *map) {
    int numEnemy = 0;
    int numPlayer = 0;
    
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            if (map->matrix[i][j] == PLAYER)
                numPlayer++;
            
            else if (map->matrix[i][j] == ENEMY)
                numEnemy++;

            else if (!__CharValidMap(map->matrix[i][j])) {
                printf("Caracter invalido: %c.\n", map->matrix[i][j]);
                return false;
            }
                
        }
    }

    if (numEnemy == 0) {
        printf("No existe enemigos.\n");
        return false;
    }
    else if (numPlayer == 0) {
        printf("No existe jugador.\n");
        return false;
    }
    else if (numPlayer > 1) {
        printf("Hay mas de un jugador.\n");
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------
// Implementacion de las funciones estaticas.
//----------------------------------------------------------------------------------

static void __LoadDataMap(Map *map, const char *path) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        char *string = (char *)malloc(sizeof(char)*128);
        int indexRow = -1;
        while (feof(file) == 0) {
            fgets(string, 128, file);
            if (indexRow == -1) {
                char *auxString = string;
                char *token = strtok(auxString, " ");
                map->steps = atoi(token);
                map->width = atoi(strtok(NULL, " "));
                map->height = atoi(strtok(NULL, " "));
            }
            else if (indexRow >= 0)
                __SaveDataMap(map, indexRow, string);

            indexRow++;
        }
#ifdef BDEBUG
        printf("Numero de enemigos: %d\n", map->enemys);
#endif

        free(string);
        fclose(file);
    }
}

static void __InitMap(Map *map) {
    map->matrix = (char **) malloc(sizeof(char *)*map->height);

    for (int i=0; i < map->height; i++) {
        map->matrix[i] = (char *) malloc(sizeof(char)*map->width);
        for (int j=0; j < map->width; j++) 
            map->matrix[i][j] = INDESTRUCTIBLE_WALL;
    }
}

static void __SaveDataMap(Map *map, int indexRow, char *data) {
    if (map->matrix == NULL)
        __InitMap(map);

    size_t lenData = strlen(data);
    for (int indexColumn=0; indexColumn < lenData; indexColumn++) {
        map->matrix[indexRow][indexColumn] = data[indexColumn];
        
        if (data[indexColumn] == ENEMY) {
            AddPoints(&enemys, (Point){indexColumn, indexRow}); // Almaceno la posicion del enemigo.
            map->enemys++; // Contador de enemigos.
        }
        else if (data[indexColumn] == PLAYER) 
            posPlayer = (Point){indexColumn, indexRow};
    }
}

#ifdef BDEBUG
static void __PrintMap(Map *map) {
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            printf("%c ", map->matrix[i][j]);
        }
        printf("\n");
    }
}
#endif //BDEBUG


static bool __CharValidMap(char c) {
    switch (c) {
        case PLAYER:
        case DESTRUCTIBLE_WALL:
        case INDESTRUCTIBLE_WALL:
        case FLOOR:
        case ENEMY:
        case BOMB:
            return true;
    } 

    return false;
}