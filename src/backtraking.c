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

static bool __PutBomb(const Map *const map, const Point *const point);

static bool __IsPutBomb(Node *node);

static const Point *__IsExplotion(Node *node);

static bool __Explotion(Map **map, Point *point, const Point *bomb);

bool Backtraking(Map *map, Node **node, Node **prev, Point point, int step, char event) {
    AddPointNode(&(*node), ((prev != NULL)?(&(*prev)):(NULL)), point, step, event);
    if (map->enemys == 0 && map->steps >= step)
        return true;
    
    if (map->steps < step)
        return false;

    printf("%d, %d -- %d\n", point.x, point.y, step);
    // const Point *auxPoint = __IsExplotion((*node));
    // if (auxPoint != NULL && !__Explotion(&(map), &point, auxPoint)) 
    //     return false;

    int index = 0;

    // if (!__IsPutBomb((*node)) && __PutBomb(map, &point)) {
    //     Backtraking(&(*map), &(*node), &(*node)->prev, point, (step+1), EVENT_BOMB);
    // }
    // Move Up
    if ((point.y-1) >= 0 && map->matrix[(point.y-1)][point.x] == FLOOR) {
        Backtraking(&(*map), &(*node)->child[index++], &(*node), (Point){point.x, (point.y-1)}, (step+1), EVENT_UP);
    }
    // Move Right
    if ((point.x+1) < map->width && map->matrix[point.y][(point.x+1)] == FLOOR) {
        Backtraking(&(*map), &(*node)->child[index++], &(*node), (Point){(point.x+1), point.y}, (step+1), EVENT_RIGHT);
    }
    // Move Down
    if ((point.y+1) < map->height && map->matrix[(point.y+1)][point.x] == FLOOR) {
        Backtraking(&(*map), &(*node)->child[index++], &(*node), (Point){point.x, (point.y+1)}, (step+1), EVENT_DOWN);
    }
    // Move Left
    if ((point.x-1) < map->width && map->matrix[point.y][(point.x-1)] == FLOOR) {
        Backtraking(&(*map), &(*node)->child[index++], &(*node), (Point){(point.x-1), point.y}, (step+1), EVENT_LEFT);
    }

    // Backtraking(&(*map), &(*node), &(*node)->prev, point, (step+1), EVENT_WAIT);

    return false;
}

static bool __PutBomb(const Map *const map, const Point *const point) {
    // Vertical UP
    for (int i=point->y; i >= 0; i--) {
        if (map->matrix[i][point->x] == DESTRUCTIBLE_WALL || map->matrix[i][point->x] == ENEMY)
            return true;
    }
    // Vertical DOWN
    for (int i=point->y; i < map->height; i++) {
        if (map->matrix[i][point->x] == DESTRUCTIBLE_WALL || map->matrix[i][point->x] == ENEMY)
            return true;
    }

    // Horizontal RIGHT
    for (int j=point->x; j < map->width; j++) {
        if (map->matrix[point->y][j] == DESTRUCTIBLE_WALL || map->matrix[point->y][j] == ENEMY)
            return true;
    }
    // Vertical LEFT
    for (int j=point->x; j >= 0; j--) {
        if (map->matrix[point->y][j] == DESTRUCTIBLE_WALL || map->matrix[point->y][j] == ENEMY)
            return true;
    }

    return false;
}

static bool __IsPutBomb(Node *node) {
    Node *auxNode = node;
    int cont = 0;
    while (auxNode != NULL && cont < 3) {
        if (auxNode->event == EVENT_BOMB)
            return true;

        cont++;
        auxNode = auxNode->prev;
    }

    return false;
}

static const Point *__IsExplotion(Node *node) {
    Node *auxNode = node;
    int cont = 0;

    while (auxNode != NULL && cont < 3) {
        if (cont == 2 && auxNode->event == EVENT_BOMB)
            return &auxNode->point;

        cont++;
        auxNode = auxNode->prev;
    }

    return NULL;
}

static bool __Explotion(Map **map, Point *point, const Point *bomb) {
    if (((point->x- bomb->x) == 0) || ((point->y- bomb->y) == 0))
        return false;

    // Explotion UP
    for (int i=bomb->y; (i >= 0 && (*map)->matrix[i][point->x] != INDESTRUCTIBLE_WALL); i--) {
        if ((*map)->matrix[i][point->x] == ENEMY) {
            (*map)->matrix[i][point->x] = FLOOR;
            (*map)->enemys--;
        }

        if ((*map)->matrix[i][point->x] == DESTRUCTIBLE_WALL) {
            (*map)->matrix[i][point->x] = FLOOR;
            i = -1;
        }
    }    
    // Explotion DOWN
    for (int i=bomb->y; (i < (*map)->height && (*map)->matrix[i][point->x] != INDESTRUCTIBLE_WALL); i++) {
        if ((*map)->matrix[i][point->x] == ENEMY) {
            (*map)->matrix[i][point->x] = FLOOR;
            (*map)->enemys--;
        }

        if ((*map)->matrix[i][point->x] == DESTRUCTIBLE_WALL) {
            (*map)->matrix[i][point->x] = FLOOR;
            i = (*map)->height;
        }
    }  
    // Explotion RIGHT
    for (int j=bomb->x; (j < (*map)->width && (*map)->matrix[point->y][j] != INDESTRUCTIBLE_WALL); j++) {
        if ((*map)->matrix[point->y][j] == ENEMY) {
            (*map)->matrix[point->y][j] = FLOOR;
            (*map)->enemys--;
        }

        if ((*map)->matrix[point->y][j] == DESTRUCTIBLE_WALL) {
            (*map)->matrix[point->y][j] = FLOOR;
            j = (*map)->width;
        }
    } 
    // Explotion LEFT
    for (int j=bomb->x; (j >= 0 && (*map)->matrix[point->y][j] != INDESTRUCTIBLE_WALL); j--) {
        if ((*map)->matrix[point->y][j] == ENEMY) {
            (*map)->matrix[point->y][j] = FLOOR;
            (*map)->enemys--;
        }

        if ((*map)->matrix[point->y][j] == DESTRUCTIBLE_WALL) {
            (*map)->matrix[point->y][j] = FLOOR;
            j = -1;
        }
    } 


    return true;
}