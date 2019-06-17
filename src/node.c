/**
 * @file node.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief
 * @version 1.0
 * @date 2019-16-08
 */

#include<stdlib.h>
#include "../headers/node.h"
#include "../headers/point.h"

extern Points *enemys;

void AddPointNode(Node **node, Node **prev, Point point, int cost) {
    if ((*node) == NULL) {
        (*node) = (Node *)malloc(sizeof(Node));
        (*node)->point = point;
        (*node)->cost = cost;
        (*node)->prev = (prev == NULL)?(NULL):((*prev));
        (*node)->child[0] = NULL;
        (*node)->child[1] = NULL;
        (*node)->child[2] = NULL;
        (*node)->child[3] = NULL;
    }
}

void DeleteNode(Node **node) {
    if ((*node) != NULL) {
        DeleteNode(&(*node)->child[0]);
        DeleteNode(&(*node)->child[1]);
        DeleteNode(&(*node)->child[2]);
        DeleteNode(&(*node)->child[3]);
        free((*node));
        (*node) = NULL;
    }
}

bool ExistPointNode(Node *node, Point point) {
    Node *auxNode = node;
    while (auxNode != NULL) {
        if (node->point.x == point.x && node->point.y == point.y)
            return true;
            
        auxNode = auxNode->prev;
    }

    return false;
}