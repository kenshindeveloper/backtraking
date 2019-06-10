/**
 * @file point.h
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 0.1
 * @date 2019-06-10
 */

#include "raylib.h"

typedef struct Point {
    int x;
    int y;
    char event;
    struct Point *prox;

}Point;

Point NewPoint(void);

void AddPoint(Point **point, int x, int y, char event);

bool ExistPoint(Point *const point, int x, int y);

void DeletePoint(Point **point);