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


def mostrar_polinomio(c, n):
    print("\nPolinomio generado:")
    terminos = []
    for exp in range(n, -1, -1):
        coef = c[exp]
        if exp == 0:
            t = str(coef)
        elif exp == 1:
            t = "x" if coef == 1 else f"{coef}x"
        else:
            t = f"x^{exp}" if coef == 1 else f"{coef}x^{exp}"
        terminos.append(t)
    print(" + ".join(terminos))


def mostrar_calculo(x, c, n):
    resultado = 0.0
    print(f"\nCalculo por pasos para x = {x}:")
    for exp in range(n, -1, -1):
        termino = c[exp] * (x ** exp)
        resultado += termino
        print(f"{c[exp]} * ({x})^{exp} = {termino:.10g}")
    print(f"Resultado aproximado del polinomio: {resultado:.10g}")
    print(f"Comprobacion directa (x+1)^n: {(x + 1) ** n:.10g}")


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

    repeticiones = 10
    tiempos = []
    coeficientes = None

    for _ in range(repeticiones):
        inicio = time.perf_counter_ns()
        temporal = generar_pascal(n)
        fin = time.perf_counter_ns()
        tiempos.append(fin - inicio)
        coeficientes = temporal

    print(f"\nCoeficientes de la fila n={n}:")
    print(" ".join(str(v) for v in coeficientes))
    mostrar_polinomio(coeficientes, n)
    mostrar_calculo(x, coeficientes, n)

    total = sum(tiempos)
    promedio = total / repeticiones

    with open("resultados_python.txt", "w", encoding="utf-8") as f:
        f.write("Ejercicio 2 - Pascal (Python)\n")
        f.write(f"n = {n}\nRepeticiones = {repeticiones}\n")
        f.write(f"Tiempo total = {total} ns\n")
        f.write(f"Promedio = {promedio:.2f} ns\n")
        f.write(f"Promedio = {promedio / 1_000_000:.8f} ms\n")

    print("\nTiempos de las 10 generaciones:")
    for i, t in enumerate(tiempos, start=1):
        print(f"Ejecucion {i}: {t} ns")
    print(f"Tiempo promedio: {promedio:.2f} ns ({promedio / 1_000_000:.8f} ms)")
    print("Resultados guardados en resultados_python.txt")


if __name__ == "__main__":
    main()
