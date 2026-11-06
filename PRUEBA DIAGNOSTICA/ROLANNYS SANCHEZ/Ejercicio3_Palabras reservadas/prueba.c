#include <stdio.h>
#include <stdlib.h>

/* Programa de prueba para el ejercicio 3 */

int main() {
    int edad = 20;
    float promedio = 15.5;
    char inicial = 'A';

    if (edad >= 18) {
        printf("La persona es mayor de edad\n");
    } else {
        printf("La persona es menor de edad\n");
    }

    for (int i = 0; i < 3; i++) {
        promedio = promedio + i;
    }

    return 0;
}