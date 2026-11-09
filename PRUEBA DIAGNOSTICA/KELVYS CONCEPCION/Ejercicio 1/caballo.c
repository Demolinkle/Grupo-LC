// Prueba Diagnostica - Ejercicio 1: Movimientos del caballo
// Estudiante: Kelvys Concepcion

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 500 movimientos
char *MOVIMIENTOS_500 =
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
    "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
    "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 "
    "Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 "
    "Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 "
    "Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 "
    "Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
    "Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 "
    "Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
    "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 "
    "Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 "
    "Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 "
    "Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 "
    "Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 "
    "Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4";

// Función para verificar que la casilla exista.
int casilla_valida(char columna, char fila)
{
    return columna >= 'a' && columna <= 'h' && fila >= '1' && fila <= '8';
}

// Función para verificar si el movimiento del caballo es válido.
int salto_de_caballo(char columna1, char fila1, char columna2, char fila2)
{
    return abs(columna2 - columna1) * abs(fila2 - fila1) == 2;
}

// Función para verificar los movimientos, devuelve 0 o el numero del movimiento que falla.
int verificar(char *inicio, char *cadena, char *pieza)
{
    char columna = inicio[0];
    char fila = inicio[1];
    int numero = 0;
    int i = 0;

    while (cadena[i] != '\0')
    {
        if (cadena[i] == ' ')
        {
            i++;
            continue;
        }

        int largo = 0;

        while (cadena[i + largo] != ' ' && cadena[i + largo] != '\0')
        {
            largo++;
        }

        numero++;
        *pieza = cadena[i];

        if (largo != 3 || cadena[i] != 'K')
            return numero;

        if (!casilla_valida(cadena[i + 1], cadena[i + 2]))
            return numero;

        if (!salto_de_caballo(columna, fila, cadena[i + 1], cadena[i + 2]))
            return numero;

        columna = cadena[i + 1];
        fila = cadena[i + 2];
        i += largo;
    }

    return 0;
}

// Función para mostrar el resultado de la verificación.
void mostrar_resultado(char *inicio, char *cadena)
{
    char pieza;
    int fallo = verificar(inicio, cadena, &pieza);

    printf("Ak = %s  C = %s\n  salida: ", inicio, cadena);

    if (fallo == 0)
        printf("Movimiento correcto.\n");

    else if (pieza != 'K')
        printf("Movimiento incorrecto falla letra %c no pertenece a un caballo.\n", pieza);

    else
        printf("Incorrecto, falla en el movimiento: %d.\n", fallo);
}

// Función para medir el tiempo del algoritmo.
void medir_tiempo(void)
{
    char pieza;
    int fallo = 0;
    clock_t inicio = clock();

    for (int i = 0; i < 10; i++) 
    {
        fallo = verificar("a2", MOVIMIENTOS_500, &pieza);
    }

    double milisegundos = (double)(clock() - inicio) * 1000 / CLOCKS_PER_SEC;

    printf("\n500 movimientos desde a2, verificados 10 veces\n");
    if (fallo == 0)
        printf("Resultado: CORRECTO\n");

    else
        printf("Resultado: INCORRECTO\n");

    printf("Tiempo total: %.3f ms\n", milisegundos);
}

int main(int cantidad_argumentos, char *args[])
{
    if (cantidad_argumentos == 3)
    {
        mostrar_resultado(args[1], args[2]);
            return 0;
    }

    mostrar_resultado("a1", "Kc2 Kd4 Kb3 Kc5");
    mostrar_resultado("a1", "Kc2 Kd4 Kb4 Kc5");
    mostrar_resultado("a2", "Kc3 Ke4 Kf6 Ke8 Kg7");
    mostrar_resultado("a2", "Kc8 Ke4 Kf6 Ke8 Kg7");
    mostrar_resultado("a8", "Kb6 Kd7");
    mostrar_resultado("a8", "Kb6 Kb7");
    mostrar_resultado("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4");
    mostrar_resultado("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4");
    mostrar_resultado("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4");

    medir_tiempo();
        return 0;
}
