/**
 * @file main.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 1.0
 * @date 2019-06-08
 * 
 */
#if !defined(BMAP_H)
#define BMAP_H

#include "raylib.h"

/**
 * @struct Map
 * @brief Estructura que permite almacenar el mapa y
 * la data vinculada (width, height, steps y la matrix).
 */
typedef struct Map {
    int width; // < Representa el ancho del mapa (#columnas).
    int height; // < Representa el alto del mapa (#filas). 
    int steps; // < Representa el numero de pasos que el player debe dar.
    char **matrix; // < Matriz logico del mapa a cargar.

    Image image;
    Texture2D texture;
    Vector2 position;

}Map;


/**
 * @fn NewMap 
 * @brief Funcion que inicializa la estructua Map.
 * @param path Parametro de la ruta del fichero a cargar.
 * @return Map Estructura inicializada.
 */
Map NewMap(const char *path);

/**
 * @fn DeleteMap
 * @brief Funcion que libera la memoria reservada en la inicializacion
 * de una estructura de tipo Map.
 * @param map Puntero a una estructura de tipo Map.
 * @return void.
 */
void DeleteMap(Map *map);

/**
 * @fn UpdateMap
 * @brief Funcion que permite detectar los eventos.
 * @param map Puntero a una estructura de tipo Map.
 * @return void.
 */
void UpdateMap(Map *map);

/**
 * @fn DrawMap
 * @brief Funcion que permite dibujar el mapa.
 * @param map Puntero a una estructura de tipo Map.
 * @return void.
 */
void DrawMap(Map *map);

/**
 * @fn ValidateMap
 * @brief Funcion que se encarga de la validaciones para verificar la 
 * carga correcta del mapa.
 * @param map Puntero a una estructura de tipo Map.
 * @return bool Retorna true si el resultado de la validacion
 * fue correcta, y false en caso contrario. 
 */
bool ValidateMap(Map *map);

#endif //BMAP_H