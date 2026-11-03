#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* Devuelve 1 si es correcto; si no, 0 y escribe el mensaje en msg */
int verificar(const char *ak, char **m, int n, char *msg) {
    int c = ak[0]-'a', f = ak[1]-'1';
    for (int i = 0; i < n; i++) {
        if (m[i][0] != 'K') { sprintf(msg, "INCORRECTO falla letra %c no pertenece a un caballo.", m[i][0]); return 0; }
        int nc = m[i][1]-'a', nf = m[i][2]-'1';
        int dc = abs(nc-c), df = abs(nf-f);
        if (strlen(m[i]) != 3 || nc<0 || nc>7 || nf<0 || nf>7 || !((dc==1&&df==2)||(dc==2&&df==1))) {
            sprintf(msg, "INCORRECTO falla en el movimiento %d", i+1); return 0; }
        c = nc; f = nf;
    }
    strcpy(msg, "movimiento CORRECTO"); return 1;
}
double ahora(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC,&t); return t.tv_sec*1e6+t.tv_nsec/1e3; }

int main(void) {
    static char buf[20000]; static char *mv[2000]; int n = 0; char msg[100];
    FILE *fp = fopen("moves.txt","r"); size_t len = fread(buf,1,sizeof buf-1,fp); buf[len]=0; fclose(fp);
    for (char *t = strtok(buf," \n\r"); t; t = strtok(NULL," \n\r")) mv[n++] = t;
    FILE *out = fopen("resultados_c.txt","w");
    const char *ak = "a2";
    verificar(ak, mv, n, msg);
    fprintf(out, "Total de movimientos en la lista: %d  (Ak inicial=%s)\nResultado lista completa: %s\n", n, ak, msg);
    int tam[2] = {100, n};
    for (int k = 0; k < 2; k++) {
        double tot = 0;
        for (int r = 0; r < 10; r++) { double t = ahora(); verificar(ak, mv, tam[k], msg); tot += ahora()-t; }
        fprintf(out, "[C] n=%d: 10 repeticiones, total=%.2f us, promedio=%.2f us\n", tam[k], tot, tot/10);
    }
    fclose(out);
    return 0;
}
