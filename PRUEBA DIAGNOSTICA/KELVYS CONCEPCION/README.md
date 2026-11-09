# Prueba Diagnóstica — Kelvys Concepción

Lenguajes y Compiladores · UNEG 2026-II · Prof. Félix Márquez

Enlace del video de defensa: ver [kelvys.txt](kelvys.txt).

## Instalación y ejecución

```powershell
powershell -ExecutionPolicy Bypass -File instalar.ps1   # instala GCC y Python
powershell -ExecutionPolicy Bypass -File compilar.ps1   # compila y ejecuta todo
```

## Ejercicio 1 — Movimientos del caballo (C)

- Cada movimiento debe ser `K` + columna (a-h) + fila (1-8).
- Es un salto de caballo si `|Δcolumna| × |Δfila| = 2` (movimiento en "L").
- Se detiene en el primer error y dice en qué movimiento falló, o qué letra no es caballo.
- Ejecuta los ejemplos del enunciado y mide el tiempo de verificar 10 veces los 500 movimientos
  (guardados en la constante `MOVIMIENTOS_500` dentro del código).
- Para probar otra secuencia: `caballo.exe a1 "Kc2 Kd4 Kb3"`.

## Ejercicio 2 — (x+1)^n con el triángulo de Pascal (Python y C)

- Cada fila se calcula con la anterior: `fila[i] = fila[i-1] + fila[i]`.
- Muestra los coeficientes, el polinomio y f(x) paso a paso.
- Memoria dinámica: listas en Python; `malloc`/`calloc`/`free` en C.
- En C, C(100,50) tiene 30 dígitos y no cabe en un `long long`, así que cada número se guarda
  como un arreglo de bloques de 9 dígitos y se suma con acarreo, como a mano.
- Al final mide el tiempo promedio de generar n = 100 en cada lenguaje.

## Ejercicio 3 — Traductor de palabras reservadas (C)

- Carga el archivo `.c` en memoria dinámica.
- Lo recorre carácter por carácter, salta comentarios y cadenas, y cada palabra que encuentra la
  busca en la tabla de las 32 palabras reservadas de ANSI C.
- Imprime el código traducido (`if` → `si`, `while` → `mientras`, ...) y cuántas veces aparece cada una.
- Uso: `traductor.exe` (analiza `ejemplo.c`) o `traductor.exe otro_archivo.c`.
