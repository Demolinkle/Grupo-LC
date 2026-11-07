#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void imprimir_polinomio(int n, FILE *archivo) {
    long long **pascal = (long long **)malloc((n + 1) * sizeof(long long *));
    for (int i = 0; i <= n; i++) {
        pascal[i] = (long long *)malloc((i + 1) * sizeof(long long));
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (int j = 1; j < i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }
    
    fprintf(archivo, "(x+1)^%d = ", n);
    for (int i = 0; i <= n; i++) {
        int potencia = n - i;
        long long coef = pascal[n][i];
        
        if (i > 0 && coef > 0) fprintf(archivo, "+");
        
        if (potencia == 0) {
            fprintf(archivo, "%lld", coef);
        } else if (potencia == 1) {
            if (coef != 1) fprintf(archivo, "%lld", coef);
            fprintf(archivo, "x");
        } else {
            if (coef != 1) fprintf(archivo, "%lld", coef);
            fprintf(archivo, "x^%d", potencia);
        }
    }
    fprintf(archivo, "\n\n");

    for (int i = 0; i <= n; i++) free(pascal[i]);
    free(pascal);
}

double calcular_polinomio_pasos(int n, double x, FILE *archivo) {
    long long **pascal = (long long **)malloc((n + 1) * sizeof(long long *));
    for (int i = 0; i <= n; i++) {
        pascal[i] = (long long *)malloc((i + 1) * sizeof(long long));
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (int j = 1; j < i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }

    fprintf(archivo, "Cálculo paso a paso para f(x) = (x+1)^%d con x = %.2f:\n", n, x);
    double resultado = 0;
    
    for (int i = 0; i <= n; i++) {
        int potencia = n - i;
        double valor_potencia = 1;
        for(int k = 0; k < potencia; k++) valor_potencia *= x;
        
        double valor_termino = (double)pascal[n][i] * valor_potencia;
        resultado += valor_termino;
        
        fprintf(archivo, "  Término %d: %lld * (%.2f)^%d = %.2f\n", 
                i + 1, pascal[n][i], x, potencia, valor_termino);
    }
    
    fprintf(archivo, "Resultado final f(%.2f) = %.2f\n\n", x, resultado);

    for (int i = 0; i <= n; i++) free(pascal[i]);
    free(pascal);

    return resultado;
}

double calcular_polinomio_tiempo(int n, double x) {
    long long **pascal = (long long **)malloc((n + 1) * sizeof(long long *));
    for (int i = 0; i <= n; i++) {
        pascal[i] = (long long *)malloc((i + 1) * sizeof(long long));
        pascal[i][0] = 1;
        pascal[i][i] = 1;
        for (int j = 1; j < i; j++) {
            pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
        }
    }

    double resultado = 0;
    for (int i = 0; i <= n; i++) {
        double potencia = 1;
        for (int k = 0; k < (n - i); k++) potencia *= x;
        resultado += (double)pascal[n][i] * potencia;
    }

    for (int i = 0; i <= n; i++) free(pascal[i]);
    free(pascal);

    return resultado;
}

int main() {
    clock_t inicio, fin;
    double tiempo;

    FILE *archivo = fopen("resultados-c.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir archivo.\n");
        return 1;
    }

    fprintf(archivo, "--- Resultados en C ---\n\n");
    
    fprintf(archivo, "a) Genere los coeficientes del polinomio (x+1)^n, muestre el resultado del polinomio:\n");
    imprimir_polinomio(2, archivo);

    fprintf(archivo, "b) Calcule f(x) = (x+1)^n según el polinomio generado (mostrando pasos):\n");
    calcular_polinomio_pasos(2, 2.0, archivo);

    fprintf(archivo, "c) Calcule el tiempo necesario para generar n=100:\n");
    inicio = clock();
    double resultado_c = calcular_polinomio_tiempo(100, 2.0);
    fin = clock();
    tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;

    fprintf(archivo, "Para x = 2.00 y n = 100\n");
    fprintf(archivo, "Resultado f(x): %.6e\n", resultado_c);
    fprintf(archivo, "Tiempo de ejecucion: %.6f segundos\n\n", tiempo);

    fclose(archivo);
    printf("C: Proceso terminado. Revisar resultados.txt\n");

    return 0;
}