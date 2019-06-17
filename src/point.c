/**
 * @file point.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief
 * @version 1.0
 * @date 2019-16-08
 */
#include <stdlib.h>
#include "../headers/point.h"

static bool __EqualPoints(const Point *const p0, const Point *const p1);

void AddPoints(Points **points, Point point) {
    if ((*points) != NULL) {
        (*points) = (Points *)malloc(sizeof(Points));
        (*points)->point = point;
        (*points)->prox = NULL;
    }
    else {
        Points *auxPoints = (*points);
        while (auxPoints->prox != NULL)
            auxPoints = auxPoints->prox;
        
        auxPoints->prox = (Points *)malloc(sizeof(Points));
        auxPoints->prox->point = point;
        auxPoints->prox->prox = NULL;
    }
}

bool DeletePoint(Points **points, Point point) {
    if ((*points) != NULL) {
        if (__EqualPoints(&(*points)->point, &point)) {
            Points *auxPoints = (*points);
            (*points) = (*points)->prox;
            free(auxPoints);
            auxPoints = NULL;
            return true;
        }
        else {
            Points *auxPoints = (*points);
            while (auxPoints->prox != NULL) {
                if (__EqualPoints(&auxPoints->prox->point, &point)) {
                    Points *auxPoints1 = auxPoints->prox;
                    auxPoints->prox = auxPoints1->prox;
                    free(auxPoints1);
                    auxPoints1 = NULL;
                    return true;
                }
                auxPoints = auxPoints->prox;
            }
        }
    }
    return false;
}

void DeleteAllPoints(Points **points) {
    Points *auxPoints = NULL;
    while ((*points) != NULL) {
        auxPoints = (*points);
        (*points) = (*points)->prox;
        free(auxPoints);
        auxPoints = NULL;
    }
}

static bool __EqualPoints(const Point *const p0, const Point *const p1) {
    if (p0->x == p1->x && p0->y == p1->y)
        return true;

    return false;
}