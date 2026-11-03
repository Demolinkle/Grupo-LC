/* Detecta palabras reservadas de C en un archivo fuente cargado en memoria
   dinamica (malloc/realloc) y las traduce al espanol.
   Uso: palabras archivo.c [traducido.txt] [reporte.txt] */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct { const char *c; const char *es; int cont; } Palabra;

/* 32 palabras de C89 + las agregadas en C99 y C11 */
static Palabra tabla[] = {
    {"auto","automatico",0},{"break","romper",0},{"case","caso",0},{"char","caracter",0},
    {"const","constante",0},{"continue","continuar",0},{"default","pordefecto",0},{"do","hacer",0},
    {"double","doble",0},{"else","sino",0},{"enum","enumeracion",0},{"extern","externo",0},
    {"float","flotante",0},{"for","para",0},{"goto","irA",0},{"if","si",0},
    {"int","entero",0},{"long","largo",0},{"register","registro",0},{"return","retornar",0},
    {"short","corto",0},{"signed","consigno",0},{"sizeof","tamanoDe",0},{"static","estatico",0},
    {"struct","estructura",0},{"switch","segun",0},{"typedef","definirTipo",0},{"union","union",0},
    {"unsigned","sinsigno",0},{"void","vacio",0},{"volatile","volatil",0},{"while","mientras",0},
    /* C99 */
    {"inline","enlinea",0},{"restrict","restringido",0},{"_Bool","_Booleano",0},
    {"_Complex","_Complejo",0},{"_Imaginary","_Imaginario",0},
    /* C11 */
    {"_Alignas","_AlinearComo",0},{"_Alignof","_AlineacionDe",0},{"_Atomic","_Atomico",0},
    {"_Generic","_Generico",0},{"_Noreturn","_SinRetorno",0},{"_Static_assert","_AfirmacionEstatica",0},
    {"_Thread_local","_LocalDeHilo",0}
};
#define NP (sizeof tabla / sizeof tabla[0])

/* Carga todo el archivo en un bloque de memoria que crece con realloc */
static char *cargar(const char *ruta, size_t *len) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;
    size_t cap = 256, n = 0, r;
    char *buf = malloc(cap);
    if (!buf) { fclose(f); return NULL; }
    while ((r = fread(buf + n, 1, cap - n - 1, f)) > 0) {
        n += r;
        if (n + 1 >= cap) {
            cap *= 2;
            char *tmp = realloc(buf, cap);
            if (!tmp) { free(buf); fclose(f); return NULL; }
            buf = tmp;
        }
    }
    buf[n] = '\0';
    fclose(f);
    *len = n;
    return buf;
}

static Palabra *buscar(const char *w) {
    for (size_t i = 0; i < NP; i++) if (strcmp(tabla[i].c, w) == 0) return &tabla[i];
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Uso: %s archivo.c [traducido.txt] [reporte.txt]\n", argv[0]); return 1; }
    const char *rtrad = argc > 2 ? argv[2] : "traducido.txt";
    const char *rrep  = argc > 3 ? argv[3] : "reporte.txt";
    size_t n;
    char *src = cargar(argv[1], &n);
    if (!src) { fprintf(stderr, "No se pudo leer '%s'\n", argv[1]); return 1; }

    FILE *ft = fopen(rtrad, "w"), *fr = fopen(rrep, "w");
    if (!ft || !fr) { fprintf(stderr, "No se pudieron crear los archivos de salida\n"); free(src); return 1; }
    fprintf(fr, "Archivo analizado: %s (%zu bytes cargados en memoria dinamica)\n\nOcurrencias:\n", argv[1], n);

    int linea = 1, total = 0;
    size_t i = 0;
    while (i < n) {
        char c = src[i];
        if (c == '/' && i + 1 < n && src[i+1] == '/') {            /* comentario de linea */
            while (i < n && src[i] != '\n') fputc(src[i++], ft);
        } else if (c == '/' && i + 1 < n && src[i+1] == '*') {     /* comentario de bloque */
            fputc(src[i++], ft); fputc(src[i++], ft);
            while (i < n && !(src[i] == '*' && i + 1 < n && src[i+1] == '/')) {
                if (src[i] == '\n') linea++;
                fputc(src[i++], ft);
            }
            if (i < n) { fputc(src[i++], ft); fputc(src[i++], ft); }
        } else if (c == '"' || c == '\'') {                        /* cadena o caracter */
            char q = c;
            fputc(src[i++], ft);
            while (i < n && src[i] != q && src[i] != '\n') {
                if (src[i] == '\\' && i + 1 < n) fputc(src[i++], ft);
                fputc(src[i++], ft);
            }
            if (i < n && src[i] == q) fputc(src[i++], ft);
        } else if (isalpha((unsigned char)c) || c == '_') {        /* identificador */
            size_t ini = i;
            while (i < n && (isalnum((unsigned char)src[i]) || src[i] == '_')) i++;
            size_t l = i - ini;
            char *w = malloc(l + 1);                               /* palabra en memoria dinamica */
            memcpy(w, src + ini, l); w[l] = '\0';
            Palabra *p = buscar(w);
            if (p) {
                p->cont++; total++;
                fprintf(fr, "  linea %d: %s -> %s\n", linea, p->c, p->es);
                fputs(p->es, ft);
            } else fputs(w, ft);
            free(w);
        } else {
            if (c == '\n') linea++;
            fputc(c, ft); i++;
        }
    }

    fprintf(fr, "\nResumen (palabra reservada -> espanol : veces):\n");
    int distintas = 0;
    for (size_t k = 0; k < NP; k++)
        if (tabla[k].cont) { distintas++; fprintf(fr, "  %-14s -> %-20s : %d\n", tabla[k].c, tabla[k].es, tabla[k].cont); }
    if (!total) fprintf(fr, "  (no se encontraron palabras reservadas)\n");
    fprintf(fr, "\nTotal de ocurrencias: %d (%d palabras reservadas distintas)\n", total, distintas);

    fclose(ft); fclose(fr); free(src);
    FILE *r2 = fopen(rrep, "r"); int ch;
    while ((ch = fgetc(r2)) != EOF) putchar(ch);
    fclose(r2);
    printf("\nCodigo traducido guardado en: %s\nReporte guardado en: %s\n", rtrad, rrep);
    return 0;
}
