// Prueba Diagnostica - Ejercicio 3: Traductor de palabras reservadas de C
// Estudiante: Kelvys Concepcion

#include <stdio.h>
#include <stdlib.h>

#define TOTAL_PALABRAS 32

typedef struct
{
    char *palabra;
    char *traduccion;
    int veces;
} Reservada;

// Las 32 palabras reservadas del estandar ANSI C.
Reservada tabla[TOTAL_PALABRAS] =
{
    {"auto", "automatico"},   {"break", "romper"},        {"case", "caso"},
    {"char", "caracter"},     {"const", "constante"},     {"continue", "continuar"},
    {"default", "defecto"},   {"do", "hacer"},            {"double", "doble"},
    {"else", "sino"},         {"enum", "enumeracion"},    {"extern", "externo"},
    {"float", "flotante"},    {"for", "para"},            {"goto", "ir_a"},
    {"if", "si"},             {"int", "entero"},          {"long", "largo"},
    {"register", "registro"}, {"return", "retornar"},     {"short", "corto"},
    {"signed", "con_signo"},  {"sizeof", "tamano_de"},    {"static", "estatico"},
    {"struct", "estructura"}, {"switch", "segun"},        {"typedef", "definir_tipo"},
    {"union", "union"},       {"unsigned", "sin_signo"},  {"void", "vacio"},
    {"volatile", "volatil"},  {"while", "mientras"}
};

// Función para verificar si un caracter es letra o guion bajo.
int es_letra(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// Función para buscar una palabra en la tabla, devuelve su posicion o -1.
int buscar(char *palabra)
{
    for (int i = 0; i < TOTAL_PALABRAS; i++)
    {
        int j = 0;

        while (tabla[i].palabra[j] != '\0' && tabla[i].palabra[j] == palabra[j])
        {
            j++;
        }

        if (tabla[i].palabra[j] == palabra[j])
            return i;
    }

    return -1;
}

// Función para cargar el archivo en memoria dinamica.
char *leer_archivo(char *nombre)
{
    FILE *archivo = fopen(nombre, "r");

    if (archivo == NULL)
        return NULL;

    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    char *texto = malloc(tamano + 1);
    size_t leidos = fread(texto, 1, tamano, archivo);
    texto[leidos] = '\0';

    fclose(archivo);
    return texto;
}

// Función para traducir el codigo, sin tocar comentarios ni cadenas.
void traducir(char *codigo)
{
    int i = 0;

    while (codigo[i] != '\0')
    {
        int inicio = i;

        if (codigo[i] == '/' && codigo[i + 1] == '/')
        {
            while (codigo[i] != '\0' && codigo[i] != '\n')
            {
                i++;
            }
        }

        else if (codigo[i] == '/' && codigo[i + 1] == '*')
        {
            i += 2;

            while (codigo[i] != '\0' && !(codigo[i] == '*' && codigo[i + 1] == '/'))
            {
                i++;
            }

            if (codigo[i] != '\0')
                i += 2;
        }

        else if (codigo[i] == '"' || codigo[i] == '\'')
        {
            char comilla = codigo[i];
            i++;

            while (codigo[i] != '\0' && codigo[i] != comilla)
            {
                if (codigo[i] == '\\')
                    i++;

                i++;
            }

            if (codigo[i] != '\0')
                i++;
        }

        else if (es_letra(codigo[i]))
        {
            char palabra[64];
            int largo = 0;

            while ((es_letra(codigo[i]) || (codigo[i] >= '0' && codigo[i] <= '9')) && largo < 63)
            {
                palabra[largo] = codigo[i];
                largo++;
                i++;
            }

            palabra[largo] = '\0';
            int posicion = buscar(palabra);

            if (posicion == -1)
                printf("%s", palabra);

            else
            {
                tabla[posicion].veces++;
                printf("%s", tabla[posicion].traduccion);
            }

            continue;
        }

        else
            i++;

        for (int k = inicio; k < i; k++)
        {
            putchar(codigo[k]);
        }
    }
}

// Función para mostrar cuantas veces aparecio cada palabra.
void mostrar_reporte(void)
{
    int total = 0;

    printf("\n\n%-12s %-15s %s\n", "Palabra", "Traduccion", "Veces");

    for (int i = 0; i < TOTAL_PALABRAS; i++)
    {
        if (tabla[i].veces > 0)
        {
            printf("%-12s %-15s %d\n", tabla[i].palabra, tabla[i].traduccion, tabla[i].veces);
            total += tabla[i].veces;
        }
    }

    printf("Total de palabras reservadas encontradas: %d\n", total);
}

int main(int cantidad_argumentos, char *args[])
{
    char *nombre = "ejemplo.c";

    if (cantidad_argumentos > 1)
        nombre = args[1];

    char *codigo = leer_archivo(nombre);

    if (codigo == NULL)
    {
        printf("No se pudo abrir %s\n", nombre);
        return 1;
    }

    printf("=== Codigo traducido (%s) ===\n\n", nombre);
    traducir(codigo);
    mostrar_reporte();

    free(codigo);
    return 0;
}
