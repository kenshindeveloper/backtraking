#include <stdio.h>
#include <stdlib.h>
#include "headers/map.h"

int main(int argc, char *argv[]) {
    Map map = NewMap("resources/maps/map.txt");

    printf("steps: %d\n", map.steps);
    printf("width: %d\n", map.width);
    printf("height: %d\n", map.height);

    PrintMap(&map);

    DeleteMap(&map);

    return EXIT_SUCCESS;
}
