#include <stdio.h>
#include <stdlib.h>

typedef struct cola_con_prioridad {
    unsigned int cantidad_elementos;
    TNodo raiz;
} * TColaCP;
typedef struct nodo {
    TEntrada entrada;
    struct nodo * padre;
    struct nodo * hijo_izquierdo;
    struct nodo * hijo_derecho;
} * TNodo;
typedef struct entrada {
    TClave clave;
    TValor valor;
} * TEntrada;

typedef void * TClave;
typedef void * TValor;