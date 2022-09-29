#include <stdio.h>
#include <stdlib.h>
#include "colacp.h"

void swap(TNodo padre, TNodo hijo) {
    TEntrada aux = padre->entrada;
    padre->entrada = hijo->entrada;
    hijo->entrada = aux;
}

void heapify(TNodo raiz, int (*comparador)(TEntrada, TEntrada)) {
    TClave claveRaiz = raiz->entrada->clave;
    TClave izquierda;
    TClave derecha;
    if (raiz->hijo_izquierdo != NULL) {
        izquierda = raiz->hijo_izquierdo->entrada->clave;
        if (raiz->hijo_derecho != NULL) {
            derecha = raiz->hijo_derecho->entrada->clave;
            if (comparador(izquierda, derecha) == 1) {
                if (comparador(izquierda, claveRaiz) == 1) {
                    swap(raiz, raiz->hijo_izquierdo);
                    heapify(raiz->hijo_izquierdo, comparador);
                }
            }
            else if (comparador(derecha, claveRaiz) == 1) {
                swap(raiz, raiz->hijo_derecho);
                heapify(raiz->hijo_derecho, comparador);
            }
        }
        else if (comparador(izquierda, claveRaiz) == 1) {
            swap(raiz, raiz->hijo_izquierdo);
            heapify(raiz->hijo_izquierdo, comparador);
        }
    }
}

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)) {
    //TO-DO
    return NULL;
}

TNodo crearNodo(TEntrada entr, TNodo padre){
    TNodo nuevoNodo = (TNodo) malloc(sizeof(struct nodo));
    nuevoNodo->entrada = entr;
    nuevoNodo->padre = padre;
    nuevoNodo->hijo_izquierdo = NULL;
    nuevoNodo->hijo_derecho = NULL;
    return nuevoNodo;
}

void reordenar(TNodo nodo, int (*comparador)(TEntrada, TEntrada)){
    int seguir = TRUE;
    TNodo nodo_actual = nodo;
    TNodo nodo_padre;

    while (seguir == TRUE) {
        nodo_padre = nodo_actual->padre;

        if (comparador(nodo_actual -> entrada, nodo_padre -> entrada) == 1) {
            TNodo aux = nodo_actual;
            nodo_actual = nodo_padre;
            nodo_padre = aux;
        }
        else
            seguir = FALSE;
        if (nodo_padre == NULL)
            seguir = FALSE;
    }
}

int cp_insertar(TColaCP cola, TEntrada entr){
    if (cola == NULL) {
        exit(CCP_NO_INI);
    }

    if (entr == NULL) {
        return FALSE;
    }

    if (cola->raiz == NULL) {
        cola->raiz = crearNodo(entr, NULL);
    }
    else {
        int direccion = 1;
        // Numero del nodo padre de nuevo nodo
        int cantidad_nodos = (cola-> cantidad_elementos + 1) / 2;
        TNodo nodo_actual = cola->raiz;

        while (cantidad_nodos != 1) {
            direccion = direccion * 10 + (cantidad_nodos % 2);
            cantidad_nodos = cantidad_nodos / 2;
        }

        while(direccion > 1) {
            if (direccion % 10 == 1)
                nodo_actual = nodo_actual->hijo_derecho;
            else
                nodo_actual = nodo_actual->hijo_izquierdo;
            direccion = direccion / 10;
        }

        if (nodo_actual->hijo_izquierdo != NULL) {
            nodo_actual->hijo_derecho = crearNodo(entr, nodo_actual);
            nodo_actual = nodo_actual->hijo_derecho;
        }
        else {
            nodo_actual->hijo_izquierdo = crearNodo(entr, nodo_actual);
            nodo_actual = nodo_actual->hijo_izquierdo;
        }
        reordenar(nodo_actual , cola->comparador);
    }
    cola->cantidad_elementos++;
    return TRUE;
}

TEntrada cp_eliminar(TColaCP cola) {
    if (cola == NULL) {
        exit(CCP_NO_INI);
    }
    if (cola->cantidad_elementos == 0) {
        return ELE_NULO;
    }

    TEntrada entrada = cola->raiz->entrada;
    TNodo ultimo = cola->raiz;
    int found = FALSE;
    //Asumo que si un nodo tiene un solo hijo lo tendra como hijo izquierdo
    while (ultimo->hijo_izquierdo != NULL && found == FALSE) {
        if (ultimo->hijo_derecho != NULL) {
            ultimo = ultimo->hijo_derecho;
        }
        else {
            ultimo = ultimo->hijo_izquierdo;
        }
    }
    free(cola->raiz);
    cola->raiz = ultimo;
    heapify(cola->raiz, cola->comparador);
    cola->cantidad_elementos--;

    return entrada;
}

int cp_cantidad(TColaCP cola) {
    if (cola == NULL) {
        exit(CCP_NO_INI);
    }
    return cola->cantidad_elementos;
}

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)) {
    //TO-DO
}
