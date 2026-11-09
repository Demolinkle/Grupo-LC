// Prueba Diagnostica - Ejercicio 2: Triangulo de Pascal (x+1)^n
// Estudiante: Kelvys Concepcion

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Cada casilla del numero guarda 9 digitos
#define BASE 1000000000

// Función para calcular cuantas casillas necesita cada numero.
int calcular_ancho(int n, int x)
{
    return (int)(n * log10(x + 2.0)) / 9 + 2;
}

// Función para crear un numero en memoria dinamica.
int *crear_numero(int ancho, int valor)
{
    int *numero = calloc(ancho, sizeof(int));
    numero[0] = valor;

    return numero;
}

// Función para sumar dos numeros grandes con acarreo.
void sumar(int *destino, int *origen, int ancho)
{
    int acarreo = 0;

    for (int i = 0; i < ancho; i++)
    {
        int suma = destino[i] + origen[i] + acarreo;
        destino[i] = suma % BASE;
        acarreo = suma / BASE;
    }
}

// Función para multiplicar un numero grande por un entero.
void multiplicar(int *numero, int factor, int ancho)
{
    long long acarreo = 0;

    for (int i = 0; i < ancho; i++)
    {
        long long producto = (long long)numero[i] * factor + acarreo;
        numero[i] = producto % BASE;
        acarreo = producto / BASE;
    }
}

// Función para imprimir un numero grande.
void imprimir(int *numero, int ancho)
{
    int primero = ancho - 1;

    while (primero > 0 && numero[primero] == 0)
    {
        primero--;
    }

    printf("%d", numero[primero]);

    for (int i = primero - 1; i >= 0; i--)
    {
        printf("%09d", numero[i]);
    }
}

// Función para generar los coeficientes con el triangulo de Pascal.
int **generar_coeficientes(int n, int ancho)
{
    int **fila = malloc((n + 1) * sizeof(int *));

    for (int k = 0; k <= n; k++)
    {
        fila[k] = crear_numero(ancho, 1);
    }

    for (int k = 2; k <= n; k++)
    {
        for (int i = k - 1; i >= 1; i--)
        {
            sumar(fila[i], fila[i - 1], ancho);
        }
    }

    return fila;
}

// Función para liberar la memoria de los coeficientes.
void liberar(int **fila, int n)
{
    for (int k = 0; k <= n; k++)
    {
        free(fila[k]);
    }

    free(fila);
}

// Función para mostrar el polinomio.
void mostrar_polinomio(int **coef, int n, int ancho)
{
    printf("(x+1)^%d = ", n);

    for (int i = 0; i <= n; i++)
    {
        int grado = n - i;

        if (i > 0)
            printf(" + ");

        if (i > 0 || grado == 0)
            imprimir(coef[i], ancho);

        if (grado == 1)
            printf("x");

        else if (grado > 1)
            printf("x^%d", grado);
    }

    printf("\n");
}

// Función para evaluar f(x) paso a paso.
void evaluar_por_pasos(int **coef, int n, int x, int ancho)
{
    int *total = crear_numero(ancho, 0);
    int *termino = malloc(ancho * sizeof(int));

    for (int i = 0; i <= n; i++)
    {
        int grado = n - i;

        for (int k = 0; k < ancho; k++)
        {
            termino[k] = coef[i][k];
        }

        for (int k = 0; k < grado; k++)
        {
            multiplicar(termino, x, ancho);
        }

        sumar(total, termino, ancho);

        printf("Paso %d: ", i + 1);
        imprimir(coef[i], ancho);
        printf(" * %d^%d = ", x, grado);
        imprimir(termino, ancho);
        printf("   acumulado = ");
        imprimir(total, ancho);
        printf("\n");
    }

    printf("f(%d) = ", x);
    imprimir(total, ancho);
    printf("\n");

    free(termino);
    free(total);
}

// Función para medir el tiempo del algoritmo.
void medir_tiempo(int n, int repeticiones)
{
    int ancho = calcular_ancho(n, 0);
    clock_t inicio = clock();

    for (int r = 0; r < repeticiones; r++)
    {
        int **coef = generar_coeficientes(n, ancho);
        liberar(coef, n);
    }

    double milisegundos = (double)(clock() - inicio) * 1000 / CLOCKS_PER_SEC;
    double promedio = milisegundos / repeticiones;

    printf("\nTiempo promedio para generar n = %d: %.4f ms (%d repeticiones)\n", n, promedio, repeticiones);
}

int main(void)
{
    int n, x;

    printf("Ingrese n: ");
        scanf("%d", &n);
        
    printf("Ingrese x (entero >= 0): ");
        scanf("%d", &x);

    if (n < 0 || x < 0)
    {
        printf("n y x deben ser enteros no negativos\n");
        return 1;
    }

    int ancho = calcular_ancho(n, x);
    int **coef = generar_coeficientes(n, ancho);

    printf("\nCoeficientes:");

    for (int i = 0; i <= n; i++)
    {
        printf(" ");
        imprimir(coef[i], ancho);
    }

    printf("\n");
    mostrar_polinomio(coef, n, ancho);

    printf("\n");
    evaluar_por_pasos(coef, n, x, ancho);
    liberar(coef, n);

    medir_tiempo(100, 1000);
    return 0;
}
