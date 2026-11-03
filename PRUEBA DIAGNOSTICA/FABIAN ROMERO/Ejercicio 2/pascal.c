/* (x+1)^n con el triangulo de Pascal y memoria dinamica (malloc/realloc/free).
   Uso: pascal [n] [x]   (n entre 0 y 130, x entero no negativo; por defecto n=7, x=2)
   Los coeficientes se guardan en enteros de 128 bits (C(130,65) ~ 9.5e37 cabe).
   La evaluacion usa un entero grande propio (base 1e9) para obtener el valor EXACTO. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

typedef unsigned __int128 u128;
#define BASE 1000000000u

/* ---------- entero grande (arreglo dinamico de digitos en base 1e9) ---------- */
typedef struct { uint32_t *d; size_t n; } Big;

static void big_norm(Big *a) { while (a->n > 1 && a->d[a->n - 1] == 0) a->n--; }
static void big_free(Big *a) { free(a->d); a->d = NULL; a->n = 0; }

static Big big_from_u128(u128 v) {
    Big r; r.d = calloc(5, sizeof(uint32_t)); r.n = 0;     /* 5 digitos base 1e9 = 1e45 > 2^128 */
    do { r.d[r.n++] = (uint32_t)(v % BASE); v /= BASE; } while (v > 0);
    return r;
}
static Big big_mul(const Big *a, const Big *b) {
    Big r; r.n = a->n + b->n; r.d = calloc(r.n, sizeof(uint32_t));
    for (size_t i = 0; i < a->n; i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b->n; j++) {
            uint64_t cur = r.d[i + j] + (uint64_t)a->d[i] * b->d[j] + carry;
            r.d[i + j] = (uint32_t)(cur % BASE); carry = cur / BASE;
        }
        size_t k = i + b->n;
        while (carry) { uint64_t cur = r.d[k] + carry; r.d[k] = (uint32_t)(cur % BASE); carry = cur / BASE; k++; }
    }
    big_norm(&r); return r;
}
static Big big_add(const Big *a, const Big *b) {
    size_t m = a->n > b->n ? a->n : b->n;
    Big r; r.n = m + 1; r.d = calloc(r.n, sizeof(uint32_t));
    uint64_t carry = 0;
    for (size_t i = 0; i < m; i++) {
        uint64_t cur = carry + (i < a->n ? a->d[i] : 0) + (i < b->n ? b->d[i] : 0);
        r.d[i] = (uint32_t)(cur % BASE); carry = cur / BASE;
    }
    r.d[m] = (uint32_t)carry; big_norm(&r); return r;
}
static char *big_str(const Big *a) {                       /* cadena nueva (liberar con free) */
    char *s = malloc(a->n * 9 + 1);
    int pos = sprintf(s, "%u", a->d[a->n - 1]);
    for (size_t i = a->n - 1; i-- > 0;) pos += sprintf(s + pos, "%09u", a->d[i]);
    return s;
}

static char *u128s(u128 v, char *buf) {                    /* u128 -> texto */
    char tmp[64]; int i = 0, j = 0;
    if (v == 0) tmp[i++] = '0';
    while (v > 0) { tmp[i++] = '0' + (int)(v % 10); v /= 10; }
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = 0; return buf;
}

/* ---------- Pascal con programacion dinamica: UNA fila en memoria dinamica ---------- */
static u128 *pascal(int n, FILE *tri) {
    u128 *fila = malloc((n + 1) * sizeof(u128));
    char b[64];
    fila[0] = 1;
    if (tri) fprintf(tri, "  n=0: 1\n");
    for (int i = 1; i <= n; i++) {
        fila[i] = 1;
        for (int j = i - 1; j > 0; j--) fila[j] += fila[j - 1];
        if (tri) {
            fprintf(tri, "  n=%d:", i);
            for (int k = 0; k <= i; k++) fprintf(tri, " %s", u128s(fila[k], b));
            fprintf(tri, "\n");
        }
    }
    return fila;
}

/* f(x) = suma de C * x^p, exacto; si o != NULL muestra el calculo paso a paso */
static Big evaluar(int n, const u128 *fila, uint64_t x, FILE *o) {
    Big xb = big_from_u128(x);
    Big *pw = malloc((n + 1) * sizeof(Big));                /* x^0 ... x^n */
    pw[0] = big_from_u128(1);
    for (int p = 1; p <= n; p++) pw[p] = big_mul(&pw[p - 1], &xb);
    Big total = big_from_u128(0);
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        Big c = big_from_u128(fila[k]);
        Big term = big_mul(&c, &pw[p]);
        Big nt = big_add(&total, &term);
        if (o) {
            char cb[64], *st = big_str(&term), *sp = big_str(&nt);
            fprintf(o, "  termino %d: %s * %llu^%d = %s   (suma parcial = %s)\n",
                    k + 1, u128s(fila[k], cb), (unsigned long long)x, p, st, sp);
            free(st); free(sp);
        }
        big_free(&c); big_free(&term); big_free(&total); total = nt;
    }
    for (int p = 0; p <= n; p++) big_free(&pw[p]);
    free(pw); big_free(&xb);
    return total;
}

static double ahora(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec * 1e6 + t.tv_nsec / 1e3; }

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 7;
    if (argc > 2 && argv[2][0] == '-') { fprintf(stderr, "x debe ser no negativo\n"); return 1; }
    uint64_t x = argc > 2 ? strtoull(argv[2], NULL, 10) : 2;
    if (n < 0 || n > 130) { fprintf(stderr, "n debe estar entre 0 y 130\n"); return 1; }

    FILE *o = fopen("resultados_pascal_c.txt", "w");
    char b[64];
    fprintf(o, "n = %d, x = %llu\n", n, (unsigned long long)x);
    if (n <= 12) fprintf(o, "Triangulo de Pascal:\n");
    u128 *fila = pascal(n, n <= 12 ? o : NULL);

    fprintf(o, "Coeficientes:");
    for (int k = 0; k <= n; k++) fprintf(o, " %s", u128s(fila[k], b));
    fprintf(o, "\nPolinomio: f(x) = ");
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        if (k) fprintf(o, " + ");
        if (!(fila[k] == 1 && p > 0)) fprintf(o, "%s", u128s(fila[k], b));
        if (p == 1) fputs("x", o); else if (p > 1) fprintf(o, "x^%d", p);
    }
    fprintf(o, "\nCalculo paso a paso:\n");
    Big r = evaluar(n, fila, x, o);
    char *rs = big_str(&r);

    Big base = big_from_u128((u128)x + 1), v = big_from_u128(1);   /* (x+1)^n por multiplicacion directa */
    for (int i = 0; i < n; i++) { Big t = big_mul(&v, &base); big_free(&v); v = t; }
    char *vs = big_str(&v);
    fprintf(o, "f(%llu) = %s\nVerificacion (%llu+1)^%d = %s\nCoinciden: %s\n",
            (unsigned long long)x, rs, (unsigned long long)x, n, vs, strcmp(rs, vs) == 0 ? "SI" : "NO");
    free(rs); free(vs); big_free(&r); big_free(&base); big_free(&v); free(fila);

    /* Tiempo para n=100 (promedio de REPS repeticiones) */
    enum { REPS = 1000 };
    volatile uint64_t sink = 0;
    double t0 = ahora();
    for (int i = 0; i < REPS; i++) { u128 *f = pascal(100, NULL); sink ^= (uint64_t)f[50]; free(f); }
    double tg = (ahora() - t0) / REPS;
    t0 = ahora();
    for (int i = 0; i < REPS; i++) {
        u128 *f = pascal(100, NULL); Big e = evaluar(100, f, x, NULL);
        sink ^= e.d[0]; big_free(&e); free(f);
    }
    double tge = (ahora() - t0) / REPS;
    fprintf(o, "\n[C] n=100: generar coeficientes = %.2f us (promedio de %d repeticiones)\n", tg, REPS);
    fprintf(o, "[C] n=100, x=%llu: generar + evaluar = %.2f us (promedio de %d repeticiones)\n",
            (unsigned long long)x, tge, REPS);
    fclose(o);

    FILE *r2 = fopen("resultados_pascal_c.txt", "r"); int c;
    while ((c = fgetc(r2)) != EOF) putchar(c);
    fclose(r2);
    return 0;
}
