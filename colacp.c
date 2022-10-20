#include <stdio.h>
#include <stdlib.h>
#include "colacp.h"

void swap(TNodo padre, TNodo hijo) {
    TEntrada aux = padre->entrada;
    padre->entrada = hijo->entrada;
    hijo->entrada = aux;
}

void heapify(TNodo raiz, int (*comparador)(TEntrada, TEntrada)) {
    TClave entradaRaiz = raiz->entrada;
    TEntrada izquierda;
    TEntrada derecha;
    if (raiz->hijo_izquierdo != NULL) {
        izquierda = raiz->hijo_izquierdo->entrada;
        if (raiz->hijo_derecho != NULL) {
            derecha = raiz->hijo_derecho->entrada;
            if (comparador(izquierda, derecha) == 1) {
                if (comparador(izquierda, entradaRaiz) == 1) {
                    swap(raiz, raiz->hijo_izquierdo);
                    heapify(raiz->hijo_izquierdo, comparador);
                }
            }
            else if (comparador(derecha, entradaRaiz) == 1) {
                swap(raiz, raiz->hijo_derecho);
                heapify(raiz->hijo_derecho, comparador);
            }
        }
        else if (comparador(izquierda, entradaRaiz) == 1) {
            swap(raiz, raiz->hijo_izquierdo);
            heapify(raiz->hijo_izquierdo, comparador);
        }
    }
}

/*
Obtiene el nodo hoja que seria un nodo padre si se fuera a agregar un nuevo elemento a la cola
Este es la funcion que se utiliza para saber donde insertar una nueva entrada a la cola
n es el numero de orden que se le asignaria a un nuevo nodo si se lo fuera a agregar (cantidad_elementos)
*/
TNodo obtener_padre_ultimo_nodo(TNodo nodo, int n) {
    /* Find the largest power of two no greater than n. */
    int bitIndex = 0;
    while (1 << (bitIndex + 1) <= n) {
        /* See if the next power of two is greater than n. */
        bitIndex++;
    }

    /* Back off the bit index by one.  We're going to use this to find the
     * path down.
     */
    bitIndex--;

    /* Read off the directions to take from the bits of n. */
    for (; bitIndex > 0; bitIndex--) {
        int mask = (1 << bitIndex);
        if (n & mask)
            nodo = nodo->hijo_derecho;
        else
            nodo = nodo->hijo_izquierdo;
    }
    return nodo;
}

/*
Obtiene el ultimo nodo hoja que fue agregado a la cola
Esta es la funcion que se utiliza para encontrar el ultimo elemento de la cola,
lo cual es necesario en el proceso de la funcion cp_eliminar
n es el numero del ultimo nodo que fue agregado a la cola (cantidad_elementos)
*/
TNodo obtener_ultimo_nodo(TNodo nodo, int n) {
    /* Find the largest power of two no greater than n. */
    int bitIndex = 0;
    while (1 << (bitIndex + 1) <= n) {
        /* See if the next power of two is greater than n. */
        bitIndex++;
    }

    /* Back off the bit index by one.  We're going to use this to find the
     * path down.
     */
    bitIndex--;

    /* Read off the directions to take from the bits of n. */
    for (; bitIndex >= 0; bitIndex--) {
        int mask = (1 << bitIndex);
        if (n & mask)
            nodo = nodo->hijo_derecho;
        else
            nodo = nodo->hijo_izquierdo;
    }
    return nodo;
}

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)) {
    TColaCP cola = (TColaCP) malloc(sizeof(struct cola_con_prioridad));
    cola->cantidad_elementos = 0;
    cola->comparador = f;
    cola->raiz = NULL;
    return cola;
}

TNodo crear_nodo(TEntrada entr, TNodo padre){
    TNodo nuevo_nodo = (TNodo) malloc(sizeof(struct nodo));
    nuevo_nodo->entrada = entr;
    nuevo_nodo->padre = padre;
    nuevo_nodo->hijo_izquierdo = NULL;
    nuevo_nodo->hijo_derecho = NULL;
    return nuevo_nodo;
}

void reordenar(TNodo nodo, int (*comparador)(TEntrada , TEntrada)){
    // Si el nodo no es la raiz
    if (nodo->padre != NULL) {
        if (comparador(nodo->entrada , nodo->padre->entrada) == 1){
                swap(nodo->padre , nodo);
                reordenar(nodo->padre , comparador);
        }
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
        cola->raiz = crear_nodo(entr, NULL);
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
            nodo_actual->hijo_derecho = crear_nodo(entr, nodo_actual);
            nodo_actual = nodo_actual->hijo_derecho;
        }
        else {
            nodo_actual->hijo_izquierdo = crear_nodo(entr, nodo_actual);
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
    TNodo ultimo;

    if (cola->cantidad_elementos > 1) {
        ultimo = obtener_ultimo_nodo(cola->raiz, cola->cantidad_elementos);
        swap(cola->raiz, ultimo);
        //Ahora que el elemento a eliminar es el ultimo nodo de la cola, antes de borrarlo hay que eliminar el enlace con su nodo padre
        if (cola->cantidad_elementos % 2 == 1) {
            //Hay cantidad impar de elementos, el ultimo nodo es un hijo derecho
            ultimo->padre->hijo_derecho = NULL;
        }
        else {
            //Hay cantidad par de elementos, el ultimo nodo es un hijo izquierdo
            ultimo->padre->hijo_izquierdo = NULL;
        }
    }
    else {
        ultimo = cola->raiz;
    }

    free(ultimo);
    cola->cantidad_elementos--;
    if (cola->cantidad_elementos >= 2) {
        //Solo hace faltar verificar el orden del heap si hay al menos 2 elementos
        heapify(cola->raiz, cola->comparador);
    }

    return entrada;
}

int cp_cantidad(TColaCP cola) {
    if (cola == NULL) {
        exit(CCP_NO_INI);
    }
    return cola->cantidad_elementos;
}

// Recorre el arbol usando el algoritmo postorden y elimina el nodo visitado
void eliminar_nodo (TNodo nodo, void (*fEliminar)(TEntrada)){
    if (nodo != NULL){
        eliminar_nodo(nodo->hijo_izquierdo, fEliminar);
        eliminar_nodo(nodo->hijo_derecho, fEliminar);
        fEliminar(nodo->entrada);
        free(nodo);
    }
}

void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)) {
    if (cola == NULL) {
        exit(CCP_NO_INI);
    }

    eliminar_nodo(cola->raiz, fEliminar);
    free(cola);
}
