#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colacp.h"

typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

TCiudad * ciudades;

void eliminar_entrada_ciudad(TEntrada entrada) {
    free(entrada->clave);
    TCiudad ciudad = (TCiudad) entrada->valor;
    free(ciudad->nombre);
    free(ciudad);
}

//Asumo que el parametro solo contiene caracteres de digitos (y posiblemente un caracter de nueva linea)
//Como admite negativos, tambien puede contener el caracter '-' al principio de la cadena
int string_a_entero(char * string) {
    int resultado = 0;
    int digito;
    int i = 0;
    int esNegativo = FALSE;
    if (string[0] == '-') {
        i = 1;
        esNegativo = TRUE;
    }
    for (; i<strlen(string); i++) {
        if (string[i] != 10) { //Ignora caracter de nueva linea
            digito = (int)string[i]-48;
            resultado = resultado * 10 + digito;
        }
    }
    if (esNegativo) {
        resultado = resultado * (-1);
    }

    return resultado;
}

int comparador(TEntrada e1, TEntrada e2){
    int clave1 = *((int *)e1->clave);
    int clave2 = *((int *)e2->clave);
    if (clave1 > clave2)
        return -1;
    if (clave1 < clave2)
        return 1;
    return 0;
}

int comparador_invertido(TEntrada e1, TEntrada e2){
    return comparador(e2,e1);
}

float distancia_de_manhattan(int x, int y, TCiudad c){
    int dx = abs(c->pos_x - x);
    int dy = abs(c->pos_y - y);
    float distancia = dx + dy;

    return distancia;
}

void mostrar_ordenado(TColaCP cola, int x, int y, int cant) {
    //Insertar en la cola
    float * d;
    for (int i = 0; i < cant ; i++){
        d = (float *) malloc(sizeof(float));
        TEntrada entrada = (TEntrada) malloc(sizeof(struct entrada));
        entrada->clave = d;
        entrada->valor = *(ciudades + i);
        *d = distancia_de_manhattan(x,y, *(ciudades + i));
        if (cp_insertar(cola,entrada) == FALSE)
            printf("Ocurrio un error al insertar");
    }

    //Mostramos por pantalla el contenido de la cola
    int cantidad = cola->cantidad_elementos;
    for (int i = 1; i <= cantidad; i ++){
        TEntrada entr = cp_eliminar(cola);
        TCiudad ciudad = entr->valor;
        printf("%d %s\n",i,ciudad->nombre);
        eliminar_entrada_ciudad(entr);
    }
}

//Retorna 1 si ciudad esta en el arreglo visitadas
int fue_visitada(TCiudad ciudad, TCiudad * visitadas, int cantidad) {
    int visitada = 0;
    int i = 0;
    while (visitada == 0 && i < cantidad) {
        if (ciudad == visitadas[i]) {
            visitada = 1;
        }
        i++;
    }

    return visitada;
}

void mostrar_reducido(TColaCP cola, int x, int y, int cant) {
    //Insertar en la cola
    float * d;
    TCiudad ciudad;
    TEntrada entrada;
    int horas = 0;
    TCiudad * visitadas = (TCiudad *) malloc(sizeof(TCiudad) * cant);
    for (int j= 0; j < cant; j++) {
        for (int i = 0; i < cant; i++){
            if (fue_visitada(*(ciudades + i), visitadas,j) == 0) {
                d = (float *) malloc(sizeof(float));
                entrada = (TEntrada) malloc(sizeof(struct entrada));
                entrada->clave = d;
                entrada->valor = *(ciudades + i);
                *d = distancia_de_manhattan(x,y, *(ciudades + i));
                if (cp_insertar(cola,entrada) == FALSE)
                    printf("Ocurrio un error al insertar");
            }
        }

        entrada = cp_eliminar(cola);
        ciudad = (TCiudad) entrada->valor;
        visitadas[j] = ciudad;
        d = (float *) entrada->clave;
        horas += (int) *d;
        x = (int) ciudad->pos_x;
        y = (int) ciudad->pos_y;
        printf("%d %s\n", j + 1, ciudad->nombre);
        eliminar_entrada_ciudad(entrada);
        while (cp_cantidad(cola) > 0) {
            entrada = cp_eliminar(cola);
            eliminar_entrada_ciudad(entrada);
        }
    }
    printf("Total recorrido: %d\n", horas);
    free(visitadas);
}

int contar_lineas(char * arch){
    FILE * file = fopen(arch,"rb");
    int cantidad = 0;

    if (file == NULL) {
        printf("Error. El archivo no existe");
        exit(-1);
    }

    while (!feof(file)){
        if (fgetc(file) == '\n')
            cantidad++;
    }
    fclose(file);
    return cantidad;
}

int main(int argc, char *argv[]) {

    int cantidad = contar_lineas(argv[1]);

    if (cantidad < 1){
        printf("El archivo no contiene ciudades");
        exit(0);
    }

    //Declaro variables
    ciudades = (TCiudad *) malloc(sizeof(TCiudad) * cantidad);
    const char limiter[2] = ";";
    int x, y, aux;
    int i = 0;
    char * str = (char *) malloc(sizeof(char)*20);
    char * aux_nombre;

    //Decalor variable para la cola
    TColaCP cola = NULL;

    //Abro archivo para lectura
    FILE * file;
    file = fopen(argv[1], "r");

    //Leo la primera linea con la posicion inicial
    //fscanf(file, "%s", str);
    fgets(str, 20, file);
    x = (int) string_a_entero((strtok(str, limiter)));
    y = (int) string_a_entero((strtok(NULL, limiter)));

    //Leo el resto del archivo que contiene las ciudades y sus posiciones
    while (fgets(str, 20, file) != NULL) {
        ciudades[i] = (TCiudad) malloc(sizeof(struct ciudad));
        ciudades[i]->nombre = (char *) malloc(sizeof(char)*20);
        aux_nombre = (strtok(str, limiter));
        strcpy(ciudades[i]->nombre, aux_nombre);
        ciudades[i]->pos_x = (float) string_a_entero((strtok(NULL, limiter)));
        ciudades[i]->pos_y = (float) string_a_entero((strtok(NULL, limiter)));
        i++;
    }

    fclose(file);

    while (TRUE) {
        //Inicio de programa en la pantalla
        printf("\nEliga una opcion\n");
        printf("1. Mostrar ascendente\n");
        printf("2. Mostrar descendente\n");
        printf("3. Reducir hojas de manejo\n");
        printf("4. Salir\n");

        //Leo lo ingresado por el usuario
        scanf("%s", str);
        aux = string_a_entero(str);
        switch(aux) {
            case 1:
                cola = crear_cola_cp(comparador);
                mostrar_ordenado(cola, x, y,cantidad);
                break;
            case 2:
                cola = crear_cola_cp(comparador_invertido);
                mostrar_ordenado(cola, x, y,cantidad);
                break;
            case 3:
                cola = crear_cola_cp(comparador);
                mostrar_reducido(cola, x, y,cantidad);
                break;
            case 4:
                free(ciudades);
                free(str);
                if (cola != NULL)
                    cp_destruir(cola, eliminar_entrada_ciudad);
                exit(0);
                break;
            default:
                printf("Error\n");
        }
    }

    return 0;
}
