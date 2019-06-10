/**
 * @file backtraking.h
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 1.0
 * @date 2019-06-09
 */
#if !defined(BBACKTRAKING_H)
#define BBACKTRAKING_H

#include "raylib.h"
#include "map.h"
#include "point.h"


typedef struct Data{
    int contEnemy;
    int contStep;
    Point *points;

}Data;

bool Backtraking(Map *map, Data data);

#endif //BBACKTRAKING_H