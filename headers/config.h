/**
 * @file config.h
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 1.0
 * @date 2019-06-08
 * 
 */
#if !defined(BCONFIG_H)
#define BCONFIG_H

// Macro que permite activar las impresiones
// de las alertas en consola.
#define BDEBUG 1

// Define el ancho de la pantalla.
#define SCREEN_WIDTH 640

// Define el alto de la pantalla.
#define SCREEN_HEIGHT 480

// Nodos de carga del mapa.
#define PLAYER '@'
#define DESTRUCTIBLE_WALL '#'
#define INDESTRUCTIBLE_WALL '='
#define FLOOR '.'
#define ENEMY 'x'
#define BOMB 'Q'

// Tamano del tile NxN.
#define TILE_SIZE 64 

// Separacion entre tiles.
#define DIFF_TILE 6

//Posicion de inicio del dibujado (x, y).
#define INIT_X 0
#define INIT_Y 0

// Eventos que ejecutara el player.
#define EVENT_WAIT '_'
#define EVENT_UP 'N'
#define EVENT_RIGHT 'E'
#define EVENT_DOWN 'S'
#define EVENT_LEFT 'O'
#define EVENT_BOMB 'Q'

#endif //BCONFIG_H