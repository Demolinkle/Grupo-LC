/* Programa de ejemplo para probar el traductor.*/
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nombre[32];
    unsigned int edad;
    double promedio;
} Estudiante;

enum Estado { APROBADO, REPROBADO };

static const int NOTA_MINIMA = 10;

enum Estado evaluar(const Estudiante *e)
{
    if (e->promedio >= NOTA_MINIMA)
        return APROBADO;
    else
        return REPROBADO;
}

int main(void)
{
    Estudiante grupo[3] = {
        { "Ana", 20, 15.5 },
        { "Luis", 22, 8.0 },
        { "Maria", 21, 12.25 }
    };

    long total = 0;
    int i = 0;

    while (i < 3) 
    {
        total += grupo[i].edad;
        i++;
    }

    do 
    {
        i--;
    } while (i > 0);

    for (i = 0; i < 3; i++) 
    {
        switch (evaluar(&grupo[i])) 
        {
        case APROBADO:
            printf("%s: aprobado (if/else no se traduce aqui)\n", grupo[i].nombre);
            break;

        default:
            printf("%s: reprobado\n", grupo[i].nombre);
            continue;
        }
    }

    printf("Edad promedio: %.2f, tamano: %zu\n", (float)total / 3, sizeof(Estudiante));
        return 0;
}
