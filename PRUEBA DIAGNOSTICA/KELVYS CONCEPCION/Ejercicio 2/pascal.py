# Prueba Diagnostica - Ejercicio 2: Triangulo de Pascal (x+1)^n
# Estudiante: Kelvys Concepcion

import time

# Función para generar los coeficientes con el triangulo de Pascal.
def generar_coeficientes(n):
    fila = [1]

    for k in range(n):
        nueva = [1]

        for i in range(1, len(fila)):
            nueva.append(fila[i - 1] + fila[i])

        nueva.append(1)
        fila = nueva

    return fila

# Función para mostrar el polinomio.
def mostrar_polinomio(coef):
    n = len(coef) - 1
    texto = f"(x+1)^{n} = "

    for i in range(n + 1):
        grado = n - i

        if i > 0:
            texto += " + "

        if i > 0 or grado == 0:
            texto += str(coef[i])

        if grado == 1:
            texto += "x"

        elif grado > 1:
            texto += f"x^{grado}"

    print(texto)

# Función para evaluar f(x) paso a paso.
def evaluar_por_pasos(coef, x):
    n = len(coef) - 1
    total = 0

    for i in range(n + 1):
        grado = n - i
        termino = coef[i] * x ** grado
        total += termino

        print(f"Paso {i + 1}: {coef[i]} * ({x})^{grado} = {termino}   acumulado = {total}")

    print(f"f({x}) = {total}")

# Función para medir el tiempo del algoritmo.
def medir_tiempo(n, repeticiones):
    inicio = time.perf_counter()

    for r in range(repeticiones):
        generar_coeficientes(n)

    promedio = (time.perf_counter() - inicio) * 1000 / repeticiones

    print(f"\nTiempo promedio para generar n = {n}: {promedio:.4f} ms ({repeticiones} repeticiones)")

def main():
    n = int(input("Ingrese n: "))
    x = int(input("Ingrese x: "))

    if n < 0:
        print("n debe ser un entero no negativo")
        return

    coef = generar_coeficientes(n)

    print("\nCoeficientes:", *coef)
    mostrar_polinomio(coef)

    print()
    evaluar_por_pasos(coef, x)

    medir_tiempo(100, 1000)

main()
