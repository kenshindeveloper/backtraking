gcc -c main.c -o main.o -std=c11
gcc -c src/map.c -o map.o -std=c11
gcc main.o map.o -o bomberman -std=c11
del *.o
