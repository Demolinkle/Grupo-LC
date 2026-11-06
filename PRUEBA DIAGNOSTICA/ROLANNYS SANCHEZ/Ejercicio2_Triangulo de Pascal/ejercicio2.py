
import time


def generar_pascal(n):
    """Genera la fila n del triangulo de Pascal."""
    coeficientes = [0] * (n + 1)
    coeficientes[0] = 1

    for fila in range(1, n + 1):
        coeficientes[fila] = 1

        for columna in range(fila - 1, 0, -1):
            coeficientes[columna] += coeficientes[columna - 1]

    return coeficientes


def mostrar_polinomio(coeficientes, n):
    """Muestra el polinomio generado en bloques de cinco terminos."""
    print("\nPolinomio generado:")

    contador = 0

    for exp in range(n, -1, -1):
        coef = coeficientes[exp]

        if exp == 0:
            termino = str(coef)
        elif exp == 1:
            termino = "x" if coef == 1 else f"{coef}x"
        else:
            termino = f"x^{exp}" if coef == 1 else f"{coef}x^{exp}"

        if contador > 0:
            print(" + ", end="")

        print(termino, end="")

        contador += 1

        if contador == 5 and exp != 0:
            print()
            contador = 0

    print()


def mostrar_calculo(x, coeficientes, n):
    """Evalua el polinomio mostrando el calculo de cada termino."""
    resultado = 0.0

    print(f"\nCalculo por pasos para x = {x}:")

    for exp in range(n, -1, -1):
        termino = coeficientes[exp] * (x ** exp)
        resultado += termino

        print(
            f"{coeficientes[exp]} * ({x})^{exp} = {termino:.10g}"
        )

    print(f"Resultado aproximado del polinomio: {resultado:.10g}")
    print(f"Comprobacion directa (x+1)^n: {(x + 1) ** n:.10g}")


def comparar_tiempos(n):
    """Mide diez generaciones de la fila n del triangulo de Pascal."""
    repeticiones = 10
    tiempos = []

    for _ in range(repeticiones):
        inicio = time.perf_counter()

        temporal = generar_pascal(n)

        fin = time.perf_counter()

        tiempos.append((fin - inicio) * 1000)

    total = sum(tiempos)
    promedio = total / repeticiones

    with open("resultados_python.txt", "w", encoding="utf-8") as archivo:
        archivo.write("Ejercicio 2 - Triangulo de Pascal (Python)\n")
        archivo.write(f"n = {n}\n")
        archivo.write(f"Repeticiones = {repeticiones}\n\n")

        archivo.write("Tiempos de ejecucion:\n")

        for i, tiempo in enumerate(tiempos, start=1):
            archivo.write(f"Ejecucion {i}: {tiempo:.8f} ms\n")

        archivo.write(f"\nTiempo total: {total:.8f} ms\n")
        archivo.write(f"Tiempo promedio: {promedio:.8f} ms\n")

    print("\nTiempos de las 10 generaciones:")

    for i, tiempo in enumerate(tiempos, start=1):
        print(f"Ejecucion {i}: {tiempo:.8f} ms")

    print(f"Tiempo total: {total:.8f} ms")
    print(f"Tiempo promedio: {promedio:.8f} ms")
    print("Resultados guardados en resultados_python.txt")


def main():
    print("=== EJERCICIO 2: TRIANGULO DE PASCAL ===")

    try:
        n = int(input("Ingrese n (entero no negativo): "))

        if n < 0 or n > 500:
            print("Error: n debe estar entre 0 y 500.")
            return

        x = float(input("Ingrese el valor de x: "))

    except ValueError:
        print("Error: debe introducir valores numericos validos.")
        return

    coeficientes = generar_pascal(n)

    mostrar_polinomio(coeficientes, n)

    mostrar_calculo(x, coeficientes, n)

    comparar_tiempos(n)


if __name__ == "__main__":
    main()
