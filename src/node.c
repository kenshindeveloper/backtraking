/**
 * @file node.c
 * @author Franklin Morales (Kenshin Urashima)
 * @brief
 * @version 1.0
 * @date 2019-16-08
 */

#include <stdio.h>
#include <stdlib.h>
#include "../headers/node.h"
#include "../headers/point.h"
#include "../headers/config.h"

extern Points *enemys;

void AddPointNode(Node **node, Node **prev, Point point, int cost, char event) {
    if ((*node) == NULL) {
        (*node) = (Node *)malloc(sizeof(Node));
        (*node)->point = point;
        (*node)->cost = cost;
        (*node)->event = event;
        (*node)->prev = (prev != NULL)?((*prev)):(NULL);
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
#ifdef BDEBUG
        // printf("eliminado nodo...\n");
#endif //BDEBUG
    }
}

bool ExistPointNode(Node *node, Point point) {
    Node *auxNode = node;
    while (auxNode != NULL) {
        if (auxNode->point.x == point.x && auxNode->point.y == point.y)
            return true;
            
        auxNode = auxNode->prev;
    }

    return false;
}

int SizeNode(Node *node) {
    int size = 0;

    if (node != NULL) {
        size += 1;
        size += SizeNode(node->child[0]);
        size += SizeNode(node->child[1]);
        size += SizeNode(node->child[2]);
        size += SizeNode(node->child[3]);
    }

    return size;
}

void PrintNode(Node *node) {
    if (node != NULL) {
        printf("(%d, %d)\n", node->point.x, node->point.y);
        PrintNode(node->child[0]);
        PrintNode(node->child[1]);
        PrintNode(node->child[2]);
        PrintNode(node->child[3]);
    }
}


int SearchCostNode(Node *node) {
    static int cost = 10000; // Valor muy alto para inicializar la variable static cost.

    if (node != NULL) {
        // for (int index=0; index < 4; index++)
        //     SearchCostNode(node->child[index]);
        SearchCostNode(node->child[0]);
        SearchCostNode(node->child[1]);
        SearchCostNode(node->child[2]);
        SearchCostNode(node->child[3]);

        if (node->cost < cost && node->point.x == 1 && node->point.y == 1)
            cost = node->cost;
    }

    return cost; 
}

Node *SearchPathNode(Node *node, int steps) {
    static Node *__node = NULL;
    if (node != NULL) {
        SearchPathNode(node->child[0], steps);
        SearchPathNode(node->child[1], steps);
        SearchPathNode(node->child[2], steps);
        SearchPathNode(node->child[3], steps);

        if (node->cost == steps && node->point.x == 1 && node->point.y == 1) {
            __node = node;
        }
    }

    return __node;
}

void ViewPathNode(const Node *const node) {
    const Node *auxNode = node;
    printf("\n");
    while (auxNode != NULL) {
        printf("%c ", auxNode->event);
        auxNode = auxNode->prev;
    }
    printf("\n");
}