// Prueba Diagnostica - Ejercicio 2: (x+1)^n con el triangulo de Pascal
// Estudiante: Kelvys Concepcion

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BASE 1000000000

// Bloques suficientes: los numeros no superan (x+2)^n
int calcular_ancho(int n, int x)
{
    return (int)(n * log10(x + 2.0)) / 9 + 2;
}

// Reserva un numero en cero y le asigna un valor inicial pequeno
int *crear_numero(int ancho, int valor)
{
    int *numero = calloc(ancho, sizeof(int));
    numero[0] = valor;
    return numero;
}

// destino = destino + origen, bloque a bloque con acarreo
void sumar(int *destino, const int *origen, int ancho)
{
    int acarreo = 0;
    for (int i = 0; i < ancho; i++) {
        int suma = destino[i] + origen[i] + acarreo;
        destino[i] = suma % BASE;
        acarreo = suma / BASE;
    }
}

// numero = numero * factor
void multiplicar(int *numero, int factor, int ancho)
{
    long long acarreo = 0;
    for (int i = 0; i < ancho; i++) {
        long long producto = (long long)numero[i] * factor + acarreo;
        numero[i] = producto % BASE;
        acarreo = producto / BASE;
    }
}

// Imprime el numero desde el bloque mas significativo
void imprimir(const int *numero, int ancho)
{
    int i = ancho - 1;
    while (i > 0 && numero[i] == 0) i--;   // salta los ceros a la izquierda

    printf("%d", numero[i]);
    for (i--; i >= 0; i--)
        printf("%09d", numero[i]);         // los demas bloques llevan 9 digitos
}

// Fila n del triangulo: se actualiza de derecha a izquierda sobre si misma
int **generar_coeficientes(int n, int ancho)
{
    int **fila = malloc((n + 1) * sizeof(int *));
    for (int k = 0; k <= n; k++)
        fila[k] = crear_numero(ancho, 1);

    // fila[i] = fila[i] + fila[i-1] para cada nueva fila k
    for (int k = 2; k <= n; k++)
        for (int i = k - 1; i >= 1; i--)
            sumar(fila[i], fila[i - 1], ancho);

    return fila;
}

// Libera cada coeficiente y luego el arreglo
void liberar(int **fila, int n)
{
    for (int k = 0; k <= n; k++)
        free(fila[k]);
    free(fila);
}

void mostrar_polinomio(int **coef, int n, int ancho)
{
    printf("(x+1)^%d = ", n);
    for (int i = 0; i <= n; i++) {
        int grado = n - i;
        if (i > 0) printf(" + ");
        if (i > 0 || grado == 0) imprimir(coef[i], ancho);   // el 1 de x^n no se escribe
        if (grado == 1) printf("x");
        if (grado > 1) printf("x^%d", grado);
    }
    printf("\n");
}

// Calcula cada termino coef * x^grado y lo va sumando al total
void evaluar_por_pasos(int **coef, int n, int x, int ancho)
{
    int *total = crear_numero(ancho, 0);
    int *termino = malloc(ancho * sizeof(int));

    for (int i = 0; i <= n; i++) {
        int grado = n - i;
        for (int k = 0; k < ancho; k++)      // termino = coeficiente
            termino[k] = coef[i][k];
        for (int k = 0; k < grado; k++)      // termino = termino * x^grado
            multiplicar(termino, x, ancho);
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

// Tiempo promedio de generar los coeficientes de (x+1)^n
void medir_tiempo(int n, int repeticiones)
{
    int ancho = calcular_ancho(n, 0);
    clock_t inicio = clock();
    for (int r = 0; r < repeticiones; r++)
        liberar(generar_coeficientes(n, ancho), n);
    double promedio = (double)(clock() - inicio) * 1000 / CLOCKS_PER_SEC / repeticiones;

    printf("\nTiempo promedio para generar n = %d: %.4f ms (%d repeticiones)\n",
           n, promedio, repeticiones);
}

int main(void)
{
    int n, x;
    printf("Ingrese n: ");
    scanf("%d", &n);
    printf("Ingrese x (entero >= 0): ");
    scanf("%d", &x);
    if (n < 0 || x < 0) {
        printf("n y x deben ser enteros no negativos\n");
        return 1;
    }

    int ancho = calcular_ancho(n, x);
    int **coef = generar_coeficientes(n, ancho);

    // a) Coeficientes y polinomio
    printf("\nCoeficientes:");
    for (int i = 0; i <= n; i++) {
        printf(" ");
        imprimir(coef[i], ancho);
    }
    printf("\n");
    mostrar_polinomio(coef, n, ancho);

    // b) Evaluacion paso a paso
    printf("\n");
    evaluar_por_pasos(coef, n, x, ancho);
    liberar(coef, n);

    medir_tiempo(100, 1000);
    return 0;
}
