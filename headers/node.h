/**
 * @file node.h
 * @author Franklin Morales (Kenshin Urashima)
 * @brief
 * @version 1.0
 * @date 2019-16-08
 */

#if !defined(BNODE_H)
#define BNODE_H

#include "raylib.h"
#include "point.h"

/**
 * @struct Node
 * @brief Estructura que permite construir el arbol de soluciones
 * para encontrar los posibles eventos a ejecutar.
 */
typedef struct Node {
    Point point; // < Posicion.
    char event; // < evento a ejecutar.
    int cost; // < costo de ejecuacion.
    struct Node *prev; // < Puntero al nodo anterior.
    struct Node *child[4]; // < Puntero a posibles nodos (caminos).

}Node;

/**
 * @fn AddPointNode
 * @brief Funcion que crea un nodo y almacena la informacion relacionada
 * con el mismo.
 * @param node Recibe la direcion de memoria del puntero Node root. a partir
 * del el se crea todo el arbol.
 * @param prev Recibe la direcion de memoria del puntero previo al root.
 * @param point Recibe la posicion a almacenar.
 * @param cost Almacena el costo del movimiento hacia el nuevo nodo.
 * @return void.
 */
void AddPointNode(Node **node, Node **prev, Point point, int cost);

void DeleteNode(Node **node);

bool ExistPointNode(Node *node, Point point);

#endif //BNODE_H