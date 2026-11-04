#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 600
#define MSG 120

int col(char c) { return (c >= 'a' && c <= 'h') ? c - 'a' : -1; }
int fil(char f) { return (f >= '1' && f <= '8') ? f - '1' : -1; }

int salto_valido(int c1, int f1, int c2, int f2) {
    int dc = abs(c2 - c1), df = abs(f2 - f1);
    return (dc == 1 && df == 2) || (dc == 2 && df == 1);
}

void verificar(char *inicio, char movs[][16], int n, char *msg) {
    if (strlen(inicio) != 2 || col(inicio[0]) == -1 || fil(inicio[1]) == -1) {
        snprintf(msg, MSG, "Posicion inicial invalida: %s", inicio);
        return;
    }
    int ca = col(inicio[0]), fa = fil(inicio[1]);

    for (int i = 0; i < n; i++) {
        if (movs[i][0] != 'K') {
            snprintf(msg, MSG, "movimiento INCORRECTO falla letra %c no pertenece a un caballo.", movs[i][0]);
            return;
        }
        int cn = col(movs[i][1]), fn = fil(movs[i][2]);
        if (strlen(movs[i]) != 3 || cn == -1 || fn == -1 || !salto_valido(ca, fa, cn, fn)) {
            snprintf(msg, MSG, "INCORRECTO falla en el movimiento %d", i + 1);
            return;
        }
        ca = cn; fa = fn;
    }
    strcpy(msg, "movimiento CORRECTO");
}

double medir(char *inicio, char movs[][16], int n, int veces) {
    char msg[MSG];
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (int v = 0; v < veces; v++) {
        verificar(inicio, movs, n, msg);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);
    return (t2.tv_sec - t1.tv_sec) * 1e6 + (t2.tv_nsec - t1.tv_nsec) / 1e3; // microsegundos
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <archivo_movimientos> <posicion_inicial>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("No se pudo abrir el archivo %s\n", argv[1]);
        return 1;
    }

    char movs[MAX][16];
    int total = 0;
    while (total < MAX && fscanf(f, "%15s", movs[total]) == 1) {
        total++;
    }
    fclose(f);

    char *inicio = argv[2];
    char mensaje[MSG];
    verificar(inicio, movs, total, mensaje);

    // Medición para n=100 (o el total si son menos) y para el total (500)
    int n1 = total < 100 ? total : 100;
    double t_100 = medir(inicio, movs, n1, 10);
    double t_total = medir(inicio, movs, total, 10);

    // Guardar en archivo txt tal como pide la consigna
    FILE *out = fopen("resultados_caballo_c.txt", "w");
    if (out) {
        fprintf(out, "Archivo: %s | Total movimientos: %d | Inicio: %s\n", argv[1], total, inicio);
        fprintf(out, "Resultado: %s\n", mensaje);
        fprintf(out, "n=%d (10 pruebas): %.2f us\n", n1, t_100);
        fprintf(out, "n=%d (10 pruebas): %.2f us\n", total, t_total);
        fclose(out);
        printf("Resultados guardados en 'resultados_caballo_c.txt'\n");
    }

    return 0;
}