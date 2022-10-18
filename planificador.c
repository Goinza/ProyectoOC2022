#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    //Declaro variables
    ciudades = (TCiudad *) malloc(sizeof(TCiudad) * 4);
    const char limiter[2] = ";";
    int x, y;
    char * str = (char *) malloc(sizeof(char)*20);

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
        (*ciudades)->nombre = (strtok(str, limiter));
        (*ciudades)->pos_x = (float) string_a_entero((strtok(NULL, limiter)));
        (*ciudades)->pos_y = (float) string_a_entero((strtok(NULL, limiter)));
        //print usado para testeo
        printf("Ciudad: %s - X: %.0f - Y: %.0f \n", (*ciudades)->nombre, (*ciudades)->pos_x, (*ciudades)->pos_y);
    }

    //Inicio de programa en la pantalla
    printf("\nEliga una opcion\n");
    printf("1. Mostrar ascendente\n");
    printf("2. Mostrar descendente\n");
    printf("3. Reducir hojas de manejo\n");
    printf("4. Salir\n");

    //Leo lo ingresado por el usuario
    scanf("%s", str);
    x = string_a_entero(str);
    switch(x) {
        case 1:
            printf("Ascendente");
            break;
        case 2:
            printf("Descendente");
            break;
        case 3:
            printf("Reducir");
            break;
        case 4:
            printf("Salir");
            break;
        default:
            printf("Error");
    }

    return 0;
}
