/**
 * @file point.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief 
 * @version 0.1
 * @date 2019-06-10
 */
#include <stdio.h>
#include <stdlib.h>
#include "../headers/point.h"

Point NewPoint(void) {
    Point point = {0};
    return point;
}

void AddPoint(Point **point, int x, int y, char event) {
    if ((*point) == NULL) {
        (*point) = (Point *) malloc(sizeof(Point));
        (*point)->x = x;
        (*point)->y = y;
        (*point)->event = event;
        (*point)->prox = NULL;
    }
    else {
        Point *auxPoint = (*point);

        while (auxPoint->prox != NULL) 
            auxPoint = auxPoint->prox;
        
        auxPoint->prox = (Point *) malloc(sizeof(Point));
        auxPoint->prox->x = x;
        auxPoint->prox->y = y;
        auxPoint->prox->event = event;
        auxPoint->prox->prox = NULL;
    }
}

bool ExistPoint(Point *const point, int x, int y) {
    Point *auxPoint = point;

    while (auxPoint != NULL) {
        if (auxPoint->x == x && auxPoint->y == y)
            return true;
        
        auxPoint = auxPoint->prox;
    }

    return false;
}

void DeletePoint(Point **point) {
    Point *auxPoint = NULL;
    
    while ((*point) != NULL) {
        auxPoint = (*point);
        (*point) = (*point)->prox;
        free(auxPoint);
        auxPoint = NULL;
    }
}