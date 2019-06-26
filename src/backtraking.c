/**
 * @file backtraking.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 0.1
 * @date 2019-06-09
 */
#include <stdio.h>
#include <stdlib.h>
#include "../headers/backtraking.h"
#include "../headers/config.h"

bool Backtraking(Map *map, Node **node, Node **prev, Point point, int step, char event) {   
    if (step > 0) 
        AddPointNode(&(*node), &(*prev), point, step, event);   

    // Move Up
    if ((point.y-1) >= 0 && map->matrix[(point.y-1)][point.x] == FLOOR && !ExistPointNode((*node), (Point){point.x, (point.y-1)}))
        Backtraking(map, &(*node)->child[0], &(*prev), (Point){point.x, (point.y-1)}, (step+1), EVENT_UP);
    // Move Down
    if ((point.y+1) < map->height && map->matrix[(point.y+1)][point.x] == FLOOR && !ExistPointNode((*node), (Point){point.x, (point.y+1)}))
        Backtraking(map, &(*node)->child[2], &(*node), (Point){point.x, (point.y+1)}, (step+1), EVENT_DOWN);
    // Move Right
    if ((point.x+1) < map->width && map->matrix[point.y][(point.x+1)] == FLOOR && !ExistPointNode((*node), (Point){(point.x+1), point.y}))
        Backtraking(map, &(*node)->child[1], &(*node), (Point){(point.x+1), point.y}, (step+1), EVENT_RIGHT);
    // Move Left
    if ((point.x-1) >= 0 && map->matrix[point.y][(point.x-1)] == FLOOR && !ExistPointNode((*node), (Point){(point.x-1), point.y}))
        Backtraking(map, &(*node)->child[3], &(*node), (Point){(point.x-1), point.y}, (step+1), EVENT_LEFT);

    if (step > map->steps)
        return false;
    // TEST 
    if ((*node)->point.x == 1 && (*node)->point.y == 1 && step < map->steps)
        return true; 
    
    return false;
}