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

//Asumo que el parametro solo contiene caracteres de digitos (y posiblemente un caracter de nueva linea)
int string_a_entero(char * string) {
    int resultado = 0;
    int digito;
    for (int i=0; i<strlen(string); i++) {
        if (string[i] != 10) { //Ignora caracter de nueva linea
            digito = (int)string[i]-48;
            resultado = resultado * 10 + digito;
        }
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
    printf("Ciudad: %s - X: %.0f - Y: %.0f \n", c->nombre, c->pos_x, c->pos_y);
    int dx = abs(c->pos_x - x);
    int dy = abs(c->pos_y - y);
    float distancia = dx + dy;

    return distancia;
}

int main(int argc, char *argv[]) {
    //Declaro variables
    ciudades = (TCiudad *) malloc(sizeof(TCiudad) * 4);
    const char limiter[2] = ";";
    int x, y, aux;
    int i = 0;
    char * str = (char *) malloc(sizeof(char)*20);
    char * aux_nombre;

    //Decalor variable para la cola
    TColaCP cola;

    //Abro archivo para lectura
    FILE * file;
    file = fopen(argv[1], "r");
    if (file==NULL) {
        printf("Error. El archivo no existe");
        exit(-1);
    }

    //Leo la primera linea con la posicion inicial
    //fscanf(file, "%s", str);
    fgets(str, 20, file);
    x = (int) string_a_entero((strtok(str, limiter)));;
    y = (int) string_a_entero((strtok(NULL, limiter)));;
    //print usado para testeo
    printf("X: %d - Y: %d\n", x, y);

    //Leo el resto del archivo que contiene las ciudades y sus posiciones
    while (fgets(str, 20, file) != NULL) {
        ciudades[i] = (TCiudad) malloc(sizeof(struct ciudad));
        ciudades[i]->nombre = (char *) malloc(sizeof(char)*20);
        aux_nombre = (strtok(str, limiter));
        strcpy(ciudades[i]->nombre, aux_nombre);
        ciudades[i]->pos_x = (float) string_a_entero((strtok(NULL, limiter)));
        ciudades[i]->pos_y = (float) string_a_entero((strtok(NULL, limiter)));
        //print usado para testeo
        printf("Ciudad: %s - X: %.0f - Y: %.0f \n", ciudades[i]->nombre, ciudades[i]->pos_x, ciudades[i]->pos_y);
        i++;
    }

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
        case 1:{
            printf("Ascendente\n");
            cola = crear_cola_cp(comparador);
        }
            break;
        case 2:{
            printf("Descendente\n");
            cola = crear_cola_cp(comparador_invertido);
        }
            break;
        case 3:
            printf("Reducir\n");
            cola = crear_cola_cp(comparador);
            break;
        case 4:
            printf("Salir\n");
            break;
        default:
            printf("Error\n");
    }

    //Insertar en la cola
    float * d;
    for (int i = 0; i < 4 ; i++){
        d = (float *) malloc(sizeof(float));
        TEntrada entrada = (TEntrada) malloc(sizeof(struct entrada));
        entrada->clave = d;
        entrada->valor = *(ciudades + i);
        if (aux == 3){
            if (cola->raiz != NULL){
                x = ((TCiudad)cola->raiz->entrada->valor)->pos_x;
                y = ((TCiudad)cola->raiz->entrada->valor)->pos_y;
            }
        }
        *d = distancia_de_manhattan(x,y, *(ciudades + i));
        printf("%f\n", *d);
        if (cp_insertar(cola,entrada) == FALSE)
            printf("Ocurrio un error al insertar");
    }

    //Mostramos por pantalla el contenido de la cola
    int cantidad = cola->cantidad_elementos;
    for (int i = 0; i < cantidad; i ++){
        TEntrada entr = cp_eliminar(cola);
        TCiudad ciudad = entr->valor;
        printf("%d %s\n",i,ciudad->nombre);
    }

    return 0;
}
