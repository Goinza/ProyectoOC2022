#include <stdio.h>
#include <stdlib.h>
#include "colacp.h"

void swap(TNodo padre, TNodo hijo) {
    TNodo auxIzq;
    TNodo auxDer;
    auxIzq = hijo->hijo_izquierdo;
    auxDer = hijo->hijo_derecho;

    hijo->padre = padre->padre;
    if (padre->padre != NULL) {
        if (padre->padre->hijo_izquierdo == padre) {
            padre->padre->hijo_izquierdo = hijo;
        }
        else {
            padre->padre->hijo_derecho = hijo;
        }
    }

    if (padre->hijo_izquierdo == hijo) {
        hijo->hijo_izquierdo = padre;
        hijo->hijo_derecho = padre->hijo_derecho;
        if (padre->hijo_derecho != NULL) {
            padre->hijo_derecho->padre = hijo;
        }
    }
    else {
        hijo->hijo_derecho = padre;
        hijo->hijo_izquierdo = padre->hijo_izquierdo;
        if (padre->hijo_izquierdo != NULL) {
            padre->hijo_izquierdo->padre = hijo;
        }
    }

    padre->padre = hijo;
    padre->hijo_izquierdo = auxIzq;
    if (padre->hijo_izquierdo != NULL) {
        padre->hijo_izquierdo->padre = padre;
    }
    padre->hijo_derecho = auxDer;
    if (padre->hijo_derecho != NULL) {
        padre->hijo_derecho->padre = padre;
    }
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

TNodo crearNodo(TEntrada entr){
    TNodo nuevoNodo = (struct nodo *) malloc (sizeof(struct nodo));
    TEntrada entradaNueva = (struct entrada *) malloc (sizeof(struct entrada));
    *entradaNueva = *entr;
    nuevoNodo->entrada = entradaNueva;
    nuevoNodo->hijo_izquierdo = NULL;
    nuevoNodo->hijo_derecho = NULL;
    return nuevoNodo;
}

int cp_insertar(TColaCP cola, TEntrada entr) {
    if (cola == NULL) {
        exit(CCP_NO_INI);
    }

    if (entr == NULL){
        return FALSE;
    }

    int posicion = FALSE;

    if (cola->raiz->entrada == NULL){
        struct entrada *entradaNueva;
        entradaNueva = (struct entrada *) malloc(sizeof(struct entrada));
        *entradaNueva = *entr;
        cola->raiz->entrada = entradaNueva;
        posicion = TRUE;
    }
    else{
        TNodo nodoActual = cola->raiz;
        TEntrada entradaMayor = entr;
        int comparacion;
        while(posicion == FALSE){
            comparacion = cola->comparador(nodoActual->entrada , entradaMayor);
            //Comparo la entradaMayor con la del nodo si la entrada del nodo es mayor las intercambio
            if (comparacion == 1){
            TEntrada aux;
            *aux = *(nodoActual->entrada);
            *(nodoActual->entrada) = *entradaMayor;
            entradaMayor = aux;
            }
            //Si el hijo izquiero de la entrada actual es null incerto un hijo izquiero con la entrada
            if (nodoActual->hijo_izquierdo == NULL){
                nodoActual->hijo_izquierdo = crearNodo(entr);
                posicion = TRUE;
            }
            //Sino incerte la entrada y el hijo derecho del nodo actual esta null incerto un hijo derecho con la entrada
            if (posicion == FALSE && nodoActual->hijo_derecho == NULL){
                nodoActual->hijo_derecho = crearNodo(entr);
                posicion = TRUE;
            }

            if (posicion == FALSE)
                if (cola->comparador(nodoActual->hijo_izquierdo->entrada , entradaMayor) == 1)
                    nodoActual = nodoActual->hijo_izquierdo;
                else
                    nodoActual = nodoActual->hijo_derecho;

        }
    if (posicion == TRUE)
        cola->cantidad_elementos++;
    return posicion;
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
