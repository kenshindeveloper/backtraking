/**
 * @file point.h
 * @author Franklin Morales (Kenshin Urashima)
 * @brief
 * @version 1.0
 * @date 2019-16-08
 */
#if !defined(BPOINT_H)
#define BPOINT_H

#include "raylib.h"

/**
 * @struct Point
 * @brief Permite almacenar las posicion de la matriz.
 */
typedef struct Point {
    int x; // < fila.
    int y; // < columna.

}Point;

typedef struct Points {
    Point point;
    struct Points *prox;

}Points;

void AddPoints(Points **points, Point point);

bool DeletePoint(Points **points, Point point);

void DeleteAllPoints(Points **points);

int SizePoints(const Points *points);

#endif //BPOINT_H