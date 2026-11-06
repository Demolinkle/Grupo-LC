
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

#define REPETICIONES 10
#define MAX_N 500
#define TERMINOS_POR_LINEA 5

/* Reserva memoria y copia una cadena */
char *copiar_cadena(const char *texto) {
    char *copia = (char *)malloc(strlen(texto) + 1);

    if (copia == NULL) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    strcpy(copia, texto);
    return copia;
}

/* Suma dos numeros enteros grandes representados como cadenas */
char *sumar(const char *a, const char *b) {
    int longitud_a = (int)strlen(a);
    int longitud_b = (int)strlen(b);

    int maximo = longitud_a > longitud_b ? longitud_a : longitud_b;

    char *resultado = (char *)malloc(maximo + 2);

    if (resultado == NULL) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    int i = longitud_a - 1;
    int j = longitud_b - 1;
    int k = maximo;
    int acarreo = 0;

    resultado[maximo + 1] = '\0';

    while (i >= 0 || j >= 0 || acarreo > 0) {
        int digito_a = (i >= 0) ? a[i] - '0' : 0;
        int digito_b = (j >= 0) ? b[j] - '0' : 0;

        int suma = digito_a + digito_b + acarreo;

        resultado[k] = (char)('0' + (suma % 10));
        acarreo = suma / 10;

        i--;
        j--;
        k--;
    }

    int inicio = k + 1;

    memmove(resultado, resultado + inicio,
            (size_t)(maximo + 2 - inicio));

    return resultado;
}

/* Genera los coeficientes mediante el triangulo de Pascal */
char **generar_pascal(int n) {
    char **fila = (char **)malloc((n + 1) * sizeof(char *));

    if (fila == NULL) {
        printf("Error al reservar memoria.\n");
        exit(1);
    }

    for (int i = 0; i <= n; i++) {
        fila[i] = copiar_cadena("0");
    }

    free(fila[0]);
    fila[0] = copiar_cadena("1");

    for (int i = 1; i <= n; i++) {
        for (int j = i; j >= 1; j--) {
            char *nuevo = sumar(fila[j], fila[j - 1]);

            free(fila[j]);
            fila[j] = nuevo;
        }
    }

    return fila;
}

/* Libera la memoria dinamica utilizada */
void liberar_pascal(char **fila, int n) {
    if (fila == NULL) {
        return;
    }

    for (int i = 0; i <= n; i++) {
        free(fila[i]);
    }

    free(fila);
}

/* Muestra el polinomio organizado en cinco terminos por linea */
void mostrar_polinomio(char **fila, int n) {
    printf("\nPOLINOMIO DESARROLLADO:\n\n");

    for (int i = 0; i <= n; i++) {
        int exponente = n - i;

        printf("%s", fila[i]);

        if (exponente > 1) {
            printf("x^%d", exponente);
        } else if (exponente == 1) {
            printf("x");
        }

        if (i < n) {
            printf(" + ");
        }

        if ((i + 1) % TERMINOS_POR_LINEA == 0 && i < n) {
            printf("\n");
        }
    }

    printf("\n");
}

/* Evalua el polinomio mostrando el calculo de cada termino */
double evaluar_polinomio(char **fila, int n, double x) {
    double resultado = 0.0;

    printf("\nEVALUACION DEL POLINOMIO:\n\n");

    for (int i = 0; i <= n; i++) {
        int exponente = n - i;

        double coeficiente = strtod(fila[i], NULL);
        double potencia = pow(x, exponente);
        double termino = coeficiente * potencia;

        printf("Termino %d: %s * %.10g^%d = %.10g\n",
               i + 1, fila[i], x, exponente, termino);

        resultado += termino;
    }

    printf("\nResultado de la suma de los terminos: %.10e\n",
           resultado);

    printf("Comprobacion directa (x+1)^n: %.10e\n",
           pow(x + 1.0, n));

    return resultado;
}

/* Mide el tiempo de generacion de los coeficientes */
double medir_tiempo(int n) {
    LARGE_INTEGER frecuencia;
    LARGE_INTEGER inicio;
    LARGE_INTEGER fin;

    QueryPerformanceFrequency(&frecuencia);

    QueryPerformanceCounter(&inicio);

    char **temporal = generar_pascal(n);

    QueryPerformanceCounter(&fin);

    liberar_pascal(temporal, n);

    return ((double)(fin.QuadPart - inicio.QuadPart) /
            frecuencia.QuadPart) * 1000.0;
}

/* Ejecuta diez mediciones y calcula el promedio */
void comparar_tiempos(int n) {
    double tiempos[REPETICIONES];
    double suma = 0.0;
    double promedio;

    printf("\nMEDICION DEL TIEMPO DE EJECUCION EN C:\n\n");

    for (int i = 0; i < REPETICIONES; i++) {
        tiempos[i] = medir_tiempo(n);
        suma += tiempos[i];

        printf("Ejecucion %d: %.8f ms\n",
               i + 1, tiempos[i]);
    }

    promedio = suma / REPETICIONES;

    printf("\nPromedio de las %d ejecuciones: %.8f ms\n",
           REPETICIONES, promedio);

    /* Guarda los resultados en un archivo TXT */
    FILE *archivo = fopen("resultados_c.txt", "w");

    if (archivo != NULL) {
        fprintf(archivo, "RESULTADOS DE EJECUCION EN C\n");
        fprintf(archivo, "Valor de n: %d\n", n);
        fprintf(archivo, "Repeticiones: %d\n\n", REPETICIONES);

        for (int i = 0; i < REPETICIONES; i++) {
            fprintf(archivo, "Ejecucion %d: %.8f ms\n",
                    i + 1, tiempos[i]);
        }

        fprintf(archivo, "\nPromedio: %.8f ms\n", promedio);

        fclose(archivo);

        printf("\nResultados guardados en resultados_c.txt\n");
    } else {
        printf("\nNo se pudo crear el archivo de resultados.\n");
    }
}

int main() {
    int n;
    double x;

    printf("========================================\n");
    printf("       TRIANGULO DE PASCAL Y BINOMIO\n");
    printf("========================================\n");

    printf("\nIngrese un numero entero no negativo: ");

    if (scanf("%d", &n) != 1 || n < 0 || n > MAX_N) {
        printf("\nError: ingrese un entero no negativo %d.\n",
               MAX_N);
        return 1;
    }

    printf("Ingrese el valor de x: ");

    if (scanf("%lf", &x) != 1) {
        printf("\nError: valor de x no valido.\n");
        return 1;
    }

    /* Generacion interna de los coeficientes */
    char **coeficientes = generar_pascal(n);

    /* Presentacion del polinomio */
    mostrar_polinomio(coeficientes, n);

    /* Evaluacion paso a paso */
    evaluar_polinomio(coeficientes, n, x);

    /* Medicion del tiempo de ejecucion */
    comparar_tiempos(n);

    /* Liberacion de memoria */
    liberar_pascal(coeficientes, n);

    printf("\n========================================\n");
    printf("          PROGRAMA FINALIZADO\n");
    printf("========================================\n");

    return 0;
}
