#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *EN[] = {
    "auto","break","case","char","const","continue","default","do",
    "double","else","enum","extern","float","for","goto","if","int","long",
    "register","return","short","signed","sizeof","static","struct","switch",
    "typedef","union","unsigned","void","volatile","while"
};
const char *ES[] = {
    "automatico","romper","caso","caracter","constante","continuar",
    "predeterminado","hacer","doble","sino","enumeracion","externo","flotante",
    "para","ir a","si","entero","largo","registro","retornar","corto",
    "con signo","tamano de","estatico","estructura","conmutador",
    "definicion de tipo","union","sin signo","vacio","volatil","mientras"
};
#define N (sizeof(EN)/sizeof(EN[0]))

const char *CODIGO_PRUEBA =
"#include <stdio.h>\n"
"static int suma(int a, int b) { return a + b; }\n"
"int main(void) {\n"
"    const char *msg = \"while if return\";  // no contar\n"
"    int x;\n"
"    for (x = 0; x < 5; x++) {\n"
"        if (x % 2 == 0) printf(\"%s %d\\n\", msg, suma(x, 1));\n"
"        else continue;\n"
"    }\n"
"    return 0;\n"
"}\n";

char *cargar_archivo(const char *ruta) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    rewind(f);
    char *buf = malloc((size_t)n + 1);
    if (buf) {
        size_t leidos = fread(buf, 1, (size_t)n, f);
        buf[leidos] = '\0';
    }
    fclose(f);
    return buf;
}

char *cargar_texto(const char *t) {
    size_t n = strlen(t);
    char *buf = malloc(n + 1);
    if (buf) memcpy(buf, t, n + 1);
    return buf;
}

int buscar(const char *w) {
    for (size_t i = 0; i < N; i++)
        if (strcmp(w, EN[i]) == 0) return (int)i;
    return -1;
}

int analizar(const char *buf) {
    char w[64];                 // palabra actual (máx. 63 + '\0')
    int linea = 1, col = 1;     // posición actual en el fuente
    int hallazgos = 0;          // nº de palabras reservadas encontradas
    const char *p = buf;        // puntero de recorrido

    while (*p) {                // mientras no lleguemos al '\0'

        // RAMA 1: comentario de línea  // ...
        if (p[0]=='/' && p[1]=='/') { while (*p && *p!='\n') { p++; col++; } continue; }
        // avanza hasta el '\n' sin consumirlo (lo verá la RAMA 5)

        // RAMA 2: comentario de bloque  /* ... */
        if (p[0]=='/' && p[1]=='*') {
            p += 2; col += 2;                          // saltac "/*"
            while (*p && !(p[0]=='*' && p[1]=='/')) {  // hasta "*/"
                if (*p=='\n') { linea++; col=1; }      // nueva línea
                else col++;                            // misma línea
                p++;
            }
            if (*p) { p += 2; col += 2; }              // salta "*/"
            continue;
        }

        // RAMA 3: string  "..."
        if (*p=='"') {
            p++; col++;                                // salta la " de apertura
            while (*p && *p!='"') {
                if (*p=='\\' && p[1]) { p+=2; col+=2; continue; }  // secuencia escapada
                if (*p=='\n') { linea++; col=1; } else col++;
                p++;
            }
            if (*p) { p++; col++; }                    // salta la " de cierre
            continue;
        }

        // RAMA 4: carácter  '...'
        if (*p=='\'') {
            p++; col++;                                // salta la ' de apertura
            while (*p && *p!='\'') {
                if (*p=='\\' && p[1]) { p+=2; col+=2; continue; }  // secuencia escapada
                p++; col++;
            }
            if (*p) { p++; col++; }                    // salta la ' de cierre
            continue;
        }

        // RAMA 5: salto de línea normal
        if (*p=='\n') { linea++; col=1; p++; continue; }

        // RAMA 6: identificador (letra o '_')  ← única que reporta
        if (isalpha((unsigned char)*p) || *p=='_') {
            int ic = col, k = 0;                       // ic = columna de inicio
            while (isalnum((unsigned char)*p) || *p=='_') {
                if (k < (int)sizeof(w)-1) w[k++] = *p; // copia si cabe
                p++; col++;
            }
            w[k] = '\0';                               // cierra la cadena
            int idx = buscar(w);                       // ¿es reservada?
            if (idx >= 0) {
                printf("  [linea %2d, col %2d]  %-10s -> %s\n", linea, ic, w, ES[idx]);
                hallazgos++;                           // cuenta el hallazgo
            }
            continue;
        }

        // RAMA 7: cualquier otro símbolo
        p++; col++;
    }
    return hallazgos;           // total de reservadas encontradas
}

int main(int argc, char *argv[]) {
    char *buf = (argc >= 2) ? cargar_archivo(argv[1]) : NULL;
    if (buf) printf("=== Archivo cargado dinamicamente: %s ===\n\n", argv[1]);
    else {
        if (argc >= 2) fprintf(stderr, "No se pudo abrir '%s'. Usando codigo embebido.\n\n", argv[1]);
        buf = cargar_texto(CODIGO_PRUEBA);
        if (!buf) return 1;
    }

    printf("----- Contenido analizado -----\n%s\n", buf);
    printf("----- Palabras reservadas encontradas -----\n");
    int total = analizar(buf);
    printf("\nTotal: %d\n", total);
    free(buf);
    return 0;
}