/* Verificador de movimientos de un caballo de ajedrez.
   Uso:
     caballo                          modo interactivo (pide Ak, n y C)
     caballo a1 4 Kc2 Kd4 Kb3 Kc5     verifica: Ak, n y los movimientos
     caballo --ejemplos               corre los 9 ejemplos del enunciado
     caballo --tiempo [moves.txt] [Ak] mide el tiempo y escribe un txt   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *EJ[][2] = {
    {"a1","Kc2 Kd4 Kb3 Kc5"}, {"a1","Kc2 Kd4 Kb4 Kc5"},
    {"a2","Kc3 Ke4 Kf6 Ke8 Kg7"}, {"a2","Kc8 Ke4 Kf6 Ke8 Kg7"},
    {"a8","Kb6 Kd7"}, {"a8","Kb6 Kb7"},
    {"a1","Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"},
    {"a1","Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"},
    {"a1","Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"}};

/* 'a1' -> columna y fila (0..7). Devuelve 0 si no es una casilla valida. */
static int casilla(const char *s, int *c, int *f) {
    if (strlen(s) == 2 && s[0] >= 'a' && s[0] <= 'h' && s[1] >= '1' && s[1] <= '8') {
        *c = s[0] - 'a'; *f = s[1] - '1'; return 1;
    }
    return 0;
}

/* Devuelve 1 si todos son correctos; si no, 0 y el mensaje en msg.
   Se detiene en el primer movimiento incorrecto. */
static int verificar(const char *ak, char **m, int n, char *msg) {
    int c, f;
    if (!casilla(ak, &c, &f)) { sprintf(msg, "Posicion inicial invalida: %s", ak); return 0; }
    for (int i = 0; i < n; i++) {
        if (m[i][0] != 'K') {
            sprintf(msg, "INCORRECTO falla letra %c no pertenece a un caballo.", m[i][0]); return 0;
        }
        int nc, nf;
        if (!casilla(m[i] + 1, &nc, &nf) ||
            !((abs(nc-c) == 1 && abs(nf-f) == 2) || (abs(nc-c) == 2 && abs(nf-f) == 1))) {
            sprintf(msg, "INCORRECTO falla en el movimiento %d", i + 1); return 0;
        }
        c = nc; f = nf;
    }
    strcpy(msg, "movimiento CORRECTO"); return 1;
}

/* Separa la cadena en palabras; el arreglo de punteros crece con realloc */
static char **tokenizar(char *s, int *n) {
    int cap = 16; *n = 0;
    char **mv = malloc(cap * sizeof *mv);
    for (char *t = strtok(s, " \t\r\n"); t; t = strtok(NULL, " \t\r\n")) {
        if (*n == cap) { cap *= 2; mv = realloc(mv, cap * sizeof *mv); }
        mv[(*n)++] = t;
    }
    return mv;
}

static char *cargar(const char *ruta) {   /* archivo completo en memoria dinamica */
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;
    size_t cap = 256, n = 0, r;
    char *buf = malloc(cap);
    while ((r = fread(buf + n, 1, cap - n - 1, f)) > 0) {
        n += r;
        if (n + 1 >= cap) { cap *= 2; buf = realloc(buf, cap); }
    }
    buf[n] = '\0'; fclose(f); return buf;
}

static double ahora(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec * 1e6 + t.tv_nsec / 1e3; }

static void resolver(const char *ak, int n, char **mv, int cuenta) {
    char msg[128];
    if (n != cuenta)
        printf("Aviso: n=%d pero la cadena tiene %d movimientos; se verifican los %d recibidos.\n", n, cuenta, cuenta);
    verificar(ak, mv, cuenta, msg);
    printf("%s\n", msg);
}

static void modo_ejemplos(void) {
    char msg[128];
    for (int i = 0; i < 9; i++) {
        char *copia = strdup(EJ[i][1]); int n; char **mv = tokenizar(copia, &n);
        verificar(EJ[i][0], mv, n, msg);
        printf("Ak = %s n= %d C= %s  ->  %s\n", EJ[i][0], n, EJ[i][1], msg);
        free(mv); free(copia);
    }
}

static void modo_tiempo(const char *ruta, const char *ak) {
    char *txt = cargar(ruta), msg[128];
    if (!txt) { fprintf(stderr, "No se pudo leer '%s'\n", ruta); return; }
    int n; char **mv = tokenizar(txt, &n);
    FILE *o = fopen("resultados_caballo_c.txt", "w");
    verificar(ak, mv, n, msg);
    fprintf(o, "Archivo: %s | movimientos: %d | Ak inicial: %s\nResultado de la lista completa: %s\n", ruta, n, ak, msg);
    int tam[2] = {n < 100 ? n : 100, n};
    const char *et[2] = {"n=100", "n=total"};
    for (int k = 0; k < 2; k++) {
        double t = ahora();
        for (int r = 0; r < 10; r++) verificar(ak, mv, tam[k], msg);
        double t10 = ahora() - t;
        t = ahora();
        for (int r = 0; r < 1000; r++) verificar(ak, mv, tam[k], msg);
        double t1000 = (ahora() - t) / 1000;
        fprintf(o, "[C] %s (%d movs): 10 verificaciones = %.2f us en total | promedio de 1000 repeticiones = %.2f us por verificacion\n",
                et[k], tam[k], t10, t1000);
    }
    fclose(o);
    FILE *r2 = fopen("resultados_caballo_c.txt", "r"); int ch;
    while ((ch = fgetc(r2)) != EOF) putchar(ch);
    fclose(r2); free(mv); free(txt);
}

int main(int argc, char **argv) {
    if (argc == 1) {                               /* modo interactivo */
        char ak[64], num[64], linea[8192];
        printf("Posicion inicial del caballo Ak (ej. a1): "); if (!fgets(ak, sizeof ak, stdin)) return 1;
        ak[strcspn(ak, "\r\n")] = 0;
        printf("Cantidad de movimientos n: "); if (!fgets(num, sizeof num, stdin)) return 1;
        printf("Cadena C (ej. Kc2 Kd4 Kb3 Kc5): "); if (!fgets(linea, sizeof linea, stdin)) return 1;
        int cuenta; char **mv = tokenizar(linea, &cuenta);
        resolver(ak, atoi(num), mv, cuenta); free(mv);
    } else if (!strcmp(argv[1], "--ejemplos")) {
        modo_ejemplos();
    } else if (!strcmp(argv[1], "--tiempo")) {
        modo_tiempo(argc > 2 ? argv[2] : "moves.txt", argc > 3 ? argv[3] : "a2");
    } else if (argc >= 4) {
        size_t tam = 1;
        for (int i = 3; i < argc; i++) tam += strlen(argv[i]) + 1;
        char *cadena = malloc(tam); cadena[0] = 0;
        for (int i = 3; i < argc; i++) { strcat(cadena, argv[i]); strcat(cadena, " "); }
        int cuenta; char **mv = tokenizar(cadena, &cuenta);
        resolver(argv[1], atoi(argv[2]), mv, cuenta); free(mv); free(cadena);
    } else {
        printf("Uso: %s | %s Ak n m1 m2 ... | %s --ejemplos | %s --tiempo [moves.txt] [Ak]\n", argv[0], argv[0], argv[0], argv[0]);
    }
    return 0;
}
