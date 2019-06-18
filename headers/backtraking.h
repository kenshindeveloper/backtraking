/**
 * @file backtraking.h
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 1.0
 * @date 2019-06-09
 */
#if !defined(BBACKTRAKING_H)
#define BBACKTRAKING_H

#include "node.h"
#include "point.h"
#include "map.h"

bool Backtraking(Map *map, Node **node, Node **prev, Point point, int step, char event);

#endif //BBACKTRAKING_H