#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef unsigned __int128 u128;   /* C(100,50) ~ 1.0e29 cabe en 128 bits */

static char *u128s(u128 v, char *buf) {   /* imprime un entero de 128 bits */
    char tmp[64]; int i = 0, j = 0;
    if (v == 0) tmp[i++] = '0';
    while (v > 0) { tmp[i++] = '0' + (int)(v % 10); v /= 10; }
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = 0; return buf;
}
/* Programacion dinamica: una sola fila actualizada de derecha a izquierda */
static void pascal(int n, u128 *fila) {
    fila[0] = 1;
    for (int i = 1; i <= n; i++) {
        fila[i] = 1;
        for (int j = i - 1; j > 0; j--) fila[j] += fila[j - 1];
    }
}
static long double evaluar(int n, u128 *fila, long double x, FILE *o) {
    long double total = 0; char b[64];
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        long double term = (long double)fila[k] * powl(x, p);
        total += term;
        if (o) fprintf(o, "  termino %d: %s * %.0Lf^%d = %.10Lg   (suma parcial = %.10Lg)\n",
                       k + 1, u128s(fila[k], b), x, p, term, total);
    }
    return total;
}
static double ahora(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec * 1e6 + t.tv_nsec / 1e3; }

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 7;
    long double x = argc > 2 ? atof(argv[2]) : 2;
    if (n > 100) { fprintf(stderr, "n maximo 100\n"); return 1; }
    u128 fila[101]; char b[64];
    FILE *o = fopen("resultados_pascal_c.txt", "w");
    pascal(n, fila);
    fprintf(o, "n = %d, x = %.0Lf\nCoeficientes:", n, x);
    for (int k = 0; k <= n; k++) fprintf(o, " %s", u128s(fila[k], b));
    fprintf(o, "\nCalculo paso a paso:\n");
    long double r = evaluar(n, fila, x, o);
    fprintf(o, "f(%.0Lf) = %.10Lg\nVerificacion (%.0Lf+1)^%d = %.10Lg\n", x, r, x, n, powl(x + 1, n));
    double tot = 0; u128 f2[101];
    for (int i = 0; i < 10; i++) {
        double t = ahora(); pascal(100, f2); volatile long double v = evaluar(100, f2, x, NULL); (void)v;
        tot += ahora() - t;
    }
    fprintf(o, "\n[C] n=100, x=%.0Lf: 10 repeticiones, total=%.2f us, promedio=%.2f us\n", x, tot, tot / 10);
    fclose(o);
    FILE *r2 = fopen("resultados_pascal_c.txt", "r"); int c;
    while ((c = fgetc(r2)) != EOF) putchar(c);
    fclose(r2); return 0;
}
