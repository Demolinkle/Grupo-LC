
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Estructura para relacionar una palabra reservada con su traduccion */
typedef struct {
    const char *ingles;
    const char *espanol;
} Palabra;

/* Diccionario de palabras reservadas de C11 */
Palabra reservadas[] = {
    {"auto", "automatico"},
    {"break", "romper"},
    {"case", "caso"},
    {"char", "caracter"},
    {"const", "constante"},
    {"continue", "continuar"},
    {"default", "predeterminado"},
    {"do", "hacer"},
    {"double", "doble"},
    {"else", "sino"},
    {"enum", "enumeracion"},
    {"extern", "externo"},
    {"float", "flotante"},
    {"for", "para"},
    {"goto", "ir_a"},
    {"if", "si"},
    {"int", "entero"},
    {"long", "largo"},
    {"register", "registro"},
    {"return", "retornar"},
    {"short", "corto"},
    {"signed", "con_signo"},
    {"sizeof", "tamano_de"},
    {"static", "estatico"},
    {"struct", "estructura"},
    {"switch", "seleccionar"},
    {"typedef", "definir_tipo"},
    {"union", "union"},
    {"unsigned", "sin_signo"},
    {"void", "vacio"},
    {"volatile", "volatil"},
    {"while", "mientras"},
    {"inline", "en_linea"},
    {"restrict", "restringido"},
    {"_Alignas", "alineacion"},
    {"_Alignof", "alineacion_de"},
    {"_Atomic", "atomico"},
    {"_Bool", "booleano"},
    {"_Complex", "complejo"},
    {"_Generic", "generico"},
    {"_Imaginary", "imaginario"},
    {"_Noreturn", "no_retornar"},
    {"_Static_assert", "asercion_estatica"},
    {"_Thread_local", "local_de_hilo"}
};

#define TOTAL_RESERVADAS (sizeof(reservadas) / sizeof(reservadas[0]))

/* Comprueba si un identificador es una palabra reservada */
const char *traducir_palabra(const char *palabra) {

    for (int i = 0; i < TOTAL_RESERVADAS; i++) {

        if (strcmp(palabra, reservadas[i].ingles) == 0) {
            return reservadas[i].espanol;
        }
    }

    return NULL;
}

/* Carga el archivo utilizando memoria dinamica */
char *cargar_archivo(const char *nombre, size_t *longitud) {

    FILE *archivo = fopen(nombre, "rb");

    if (archivo == NULL) {
        printf("Error: no se pudo abrir el archivo %s\n", nombre);
        return NULL;
    }
    size_t capacidad = 1024;
    size_t usados = 0;

    char *contenido = malloc(capacidad);

    if (contenido == NULL) {
        printf("Error al reservar memoria.\n");
        fclose(archivo);
        return NULL;
    }
    int caracter;

    while ((caracter = fgetc(archivo)) != EOF) {

        if (usados + 1 >= capacidad) {

            capacidad *= 2;

            char *temporal = realloc(contenido, capacidad);

            if (temporal == NULL) {
                printf("Error al ampliar la memoria.\n");
                free(contenido);
                fclose(archivo);
                return NULL;
            }

            contenido = temporal;
        }

        contenido[usados++] = (char)caracter;
    }
    contenido[usados] = '\0';
    *longitud = usados;

    fclose(archivo);

    return contenido;
}

/* Agrega texto al resultado y amplia su memoria cuando es necesario */
int agregar_texto(char **resultado, size_t *usados,
                  size_t *capacidad, const char *texto) {

    size_t longitud = strlen(texto);

    if (*usados + longitud + 1 > *capacidad) {

        size_t nueva_capacidad = *capacidad;

        while (*usados + longitud + 1 > nueva_capacidad) {
            nueva_capacidad *= 2;
        }

        char *temporal = realloc(*resultado, nueva_capacidad);

        if (temporal == NULL) {
            return 0;
        }

        *resultado = temporal;
        *capacidad = nueva_capacidad;
    }

    memcpy(*resultado + *usados, texto, longitud);

    *usados += longitud;
    (*resultado)[*usados] = '\0';

    return 1;
}

/* Analiza el codigo y sustituye las palabras reservadas */
char *analizar_codigo(const char *codigo, size_t longitud) {

    size_t capacidad = longitud + 1024;
    size_t usados = 0;
    size_t i = 0;

    char *resultado = malloc(capacidad);

    if (resultado == NULL) {
        return NULL;
    }

    resultado[0] = '\0';

    while (i < longitud) {

        /* Reconocer comentarios de una sola linea */
        if (codigo[i] == '/' && i + 1 < longitud &&
            codigo[i + 1] == '/') {

            while (i < longitud && codigo[i] != '\n') {

                char temporal[2] = {codigo[i], '\0'};

                if (!agregar_texto(&resultado, &usados,
                                   &capacidad, temporal)) {
                    free(resultado);
                    return NULL;
                }

                i++;
            }
        }

        /* Reconocer comentarios de varias lineas */
        else if (codigo[i] == '/' && i + 1 < longitud &&
                 codigo[i + 1] == '*') {

            while (i < longitud) {

                char temporal[2] = {codigo[i], '\0'};

                if (!agregar_texto(&resultado, &usados,
                                   &capacidad, temporal)) {
                    free(resultado);
                    return NULL;
                }

                if (codigo[i] == '*' && i + 1 < longitud &&
                    codigo[i + 1] == '/') {

                    i++;

                    char cierre[2] = {codigo[i], '\0'};

                    if (!agregar_texto(&resultado, &usados,
                                       &capacidad, cierre)) {
                        free(resultado);
                        return NULL;
                    }

                    i++;
                    break;
                }

                i++;
            }
        }

        /* Reconocer cadenas de texto y caracteres */
        else if (codigo[i] == '"' || codigo[i] == '\'') {

            char comilla = codigo[i];

            /* Copiar la comilla inicial */
            char temporal[2] = {codigo[i], '\0'};

            if (!agregar_texto(&resultado, &usados,
                               &capacidad, temporal)) {
                free(resultado);
                return NULL;
            }

            i++;

            /* Recorrer el contenido hasta encontrar la comilla final */
            while (i < longitud) {

                char actual = codigo[i];

                temporal[0] = actual;
                temporal[1] = '\0';

                if (!agregar_texto(&resultado, &usados,
                                   &capacidad, temporal)) {
                    free(resultado);
                    return NULL;
                }

                i++;

                /* Copiar tambien el siguiente caracter si hay escape */
                if (actual == '\\' && i < longitud) {

                    temporal[0] = codigo[i];
                    temporal[1] = '\0';

                    if (!agregar_texto(&resultado, &usados,
                                       &capacidad, temporal)) {
                        free(resultado);
                        return NULL;
                    }

                    i++;
                }

                /* Terminar al encontrar la comilla de cierre */
                else if (actual == comilla) {
                    break;
                }
            }
        }

        /* Reconocer identificadores y palabras reservadas */
        else if (isalpha((unsigned char)codigo[i]) ||
                 codigo[i] == '_') {

            /* Guardar la posicion inicial del identificador */
            size_t inicio = i;

            /* Recorrer el identificador completo */
            while (i < longitud &&
                   (isalnum((unsigned char)codigo[i]) ||
                    codigo[i] == '_')) {

                i++;
            }

            /* Calcular la longitud real del identificador */
            size_t longitud_palabra = i - inicio;

            /* Reservar memoria segun su longitud */
            char *palabra = malloc(longitud_palabra + 1);

            if (palabra == NULL) {
                free(resultado);
                return NULL;
            }

            /* Copiar el identificador completo */
            memcpy(palabra, codigo + inicio, longitud_palabra);

            palabra[longitud_palabra] = '\0';

            /* Comprobar si es una palabra reservada */
            const char *traduccion = traducir_palabra(palabra);

            if (traduccion != NULL) {

                if (!agregar_texto(&resultado, &usados,
                                   &capacidad, traduccion)) {

                    free(palabra);
                    free(resultado);
                    return NULL;
                }

                printf("Encontrada: %-16s -> %s\n",
                       palabra, traduccion);
            }

            else {

                /* Copiar el identificador sin modificarlo */
                if (!agregar_texto(&resultado, &usados,
                                   &capacidad, palabra)) {

                    free(palabra);
                    free(resultado);
                    return NULL;
                }
            }

            /* Liberar la memoria del identificador */
            free(palabra);
        }

        /* Copiar cualquier otro caracter sin modificarlo */
        else {

            char temporal[2] = {codigo[i], '\0'};

            if (!agregar_texto(&resultado, &usados,
                               &capacidad, temporal)) {
                free(resultado);
                return NULL;
            }

            i++;
        }
    }

    return resultado;
}

/* Guarda el resultado en un archivo nuevo */
int guardar_resultado(const char *nombre, const char *contenido) {

    FILE *archivo = fopen(nombre, "w");

    if (archivo == NULL) {
        printf("Error al crear el archivo de salida.\n");
        return 0;
    }

    fputs(contenido, archivo);

    fclose(archivo);

    return 1;
}

/* Funcion principal */
int main() {

    char nombre_entrada[256];
    size_t longitud = 0;

    printf("====================================\n");
    printf("   TRADUCTOR DE PALABRAS DE C\n");
    printf("====================================\n");

    printf("Ingrese el nombre del archivo: ");

    if (scanf("%255s", nombre_entrada) != 1) {
        printf("Error al leer el nombre del archivo.\n");
        return 1;
    }

    /* Cargar el archivo en memoria dinamica */
    char *codigo = cargar_archivo(nombre_entrada, &longitud);

    if (codigo == NULL) {
        return 1;
    }

    printf("\nArchivo cargado correctamente.\n");
    printf("Tamano del archivo: %zu bytes\n", longitud);

    printf("\nPALABRAS RESERVADAS ENCONTRADAS:\n\n");

    /* Analizar y traducir el codigo */
    char *traducido = analizar_codigo(codigo, longitud);

    if (traducido == NULL) {
        printf("Error al analizar el codigo.\n");
        free(codigo);
        return 1;
    }

    printf("\n====================================\n");
    printf("       CODIGO TRADUCIDO\n");
    printf("====================================\n\n");

    printf("%s\n", traducido);

    /* Guardar la traduccion */
    if (guardar_resultado("prueba_traducida.txt", traducido)) {
        printf("\nResultado guardado en prueba_traducida.txt\n");
    }

    /* Liberar toda la memoria dinamica */
    free(codigo);
    free(traducido);

    printf("\nMemoria liberada correctamente.\n");

    return 0;
}
