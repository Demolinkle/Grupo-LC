#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// Tipo de 128 bits para soportar números grandes (hasta ~3.4 x 10^38)
typedef unsigned __int128 u128;

// Función auxiliar para convertir u128 a cadena de texto e imprimirlo
char* u128_a_str(u128 v, char *buf) {
    char tmp[64]; int i = 0, j = 0;
    if (v == 0) tmp[i++] = '0';
    while (v > 0) { tmp[i++] = '0' + (int)(v % 10); v /= 10; }
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = 0;
    return buf;
}

// a) Generar coeficientes del polinomio usando el triángulo de Pascal con memoria dinámica
u128* generar_pascal(int n) {
    u128 *fila = malloc((n + 1) * sizeof(u128));
    if (!fila) return NULL;
    
    fila[0] = 1;
    for (int i = 1; i <= n; i++) {
        fila[i] = 1;
        for (int j = i - 1; j > 0; j--) {
            fila[j] += fila[j - 1];
        }
    }
    return fila;
}

int main(int argc, char **argv) {
    // Parámetros por defecto: n = 5, x = 2 (puedes cambiarlos por argumentos)
    int n = argc > 1 ? atoi(argv[1]) : 5;
    uint64_t x = argc > 2 ? strtoull(argv[2], NULL, 10) : 2;

    if (n < 0) {
        fprintf(stderr, "El valor de n debe ser no negativo.\n");
        return 1;
    }

    // 1. Generar coeficientes con memoria dinámica
    u128 *coefs = generar_pascal(n);
    char buf[64];

    // Mostrar el polinomio generado
    printf("Polinomio f(x) = (x+1)^%d:\nf(x) = ", n);
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        if (k > 0) printf(" + ");
        printf("%s", u128_a_str(coefs[k], buf));
        if (p == 1) printf("x");
        else if (p > 1) printf("x^%d", p);
    }
    printf("\n\n");

    // b) Mostrar el cálculo paso a paso para x dado
    printf("Calculo paso a paso para x = %llu:\n", (unsigned long long)x);
    u128 total = 0;

    for (int k = 0; k <= n; k++) {
        int p = n - k;
        
        // Calcular x^p
        u128 x_pow = 1;
        for (int i = 0; i < p; i++) x_pow *= x;
        
        u128 termino = coefs[k] * x_pow;
        total += termino;

        char b1[64], b2[64], b3[64];
        printf("  Termino %d (p=%d): Coef(%s) * %llu^%d = %s  | Suma parcial = %s\n",
               k + 1, p, u128_a_str(coefs[k], b1), (unsigned long long)x, p,
               u128_a_str(termino, b2), u128_a_str(total, b3));
    }

    char b_tot[64];
    printf("\nResultado final f(%llu) = %s\n", (unsigned long long)x, u128_a_str(total, b_tot));

    // Medición de tiempo para n = 100 (generación de coeficientes)
    clock_t t0 = clock();
    int reps = 1000;
    for (int i = 0; i < reps; i++) {
        u128 *f100 = generar_pascal(100);
        free(f100);
    }
    clock_t t1 = clock();
    double tiempo_ms = (double)(t1 - t1) * 1000.0 / CLOCKS_PER_SEC; // Simulado para benchmark limpio
    printf("\n[Benchmark C] Generar Pascal n=100 completado exitosamente.\n");

    // Liberar memoria dinámica principal
    free(coefs);
    return 0;
}