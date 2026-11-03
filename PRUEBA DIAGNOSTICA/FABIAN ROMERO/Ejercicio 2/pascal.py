"""(x+1)^n con el triangulo de Pascal (programacion dinamica).
Uso: python pascal.py [n] [x]     (n y x enteros no negativos; por defecto n=7, x=2)"""
import sys, time

def pascal(n, triangulo=None):
    """Fila n del triangulo de Pascal. Memoria dinamica: una sola lista que crece
    con append y se actualiza de derecha a izquierda (O(n) memoria, O(n^2) tiempo)."""
    fila = [1]
    if triangulo is not None:
        triangulo.append("  n=0: 1")
    for i in range(1, n + 1):
        fila.append(1)
        for j in range(i - 1, 0, -1):
            fila[j] += fila[j - 1]
        if triangulo is not None:
            triangulo.append(f"  n={i}: " + " ".join(map(str, fila)))
    return fila

def polinomio(fila):
    n = len(fila) - 1
    terminos = []
    for k, c in enumerate(fila):
        p = n - k
        cs = "" if (c == 1 and p > 0) else str(c)
        xs = "" if p == 0 else ("x" if p == 1 else f"x^{p}")
        terminos.append(cs + xs)
    return " + ".join(terminos)

def evaluar(fila, x, pasos=None):
    n = len(fila) - 1
    total = 0
    for k, c in enumerate(fila):
        p = n - k
        term = c * x ** p
        total += term
        if pasos is not None:
            pasos.append(f"  termino {k+1}: {c} * {x}^{p} = {term}   (suma parcial = {total})")
    return total

if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 7
    x = int(sys.argv[2]) if len(sys.argv) > 2 else 2
    if n < 0 or x < 0:
        sys.exit("n y x deben ser enteros no negativos")
    tri = [] if n <= 12 else None
    fila = pascal(n, tri)
    pasos = []
    r = evaluar(fila, x, pasos)
    out = [f"n = {n}, x = {x}"]
    if tri:
        out += ["Triangulo de Pascal:"] + tri
    out += [f"Coeficientes: {' '.join(map(str, fila))}",
            f"Polinomio: f(x) = {polinomio(fila)}",
            "Calculo paso a paso:"] + pasos + [
            f"f({x}) = {r}",
            f"Verificacion ({x}+1)^{n} = {(x + 1) ** n}",
            f"Coinciden: {'SI' if r == (x + 1) ** n else 'NO'}"]
    # Tiempo para n=100 (promedio de REPS repeticiones)
    REPS = 1000
    t = time.perf_counter()
    for _ in range(REPS):
        pascal(100)
    tg = (time.perf_counter() - t) / REPS
    t = time.perf_counter()
    for _ in range(REPS):
        evaluar(pascal(100), x)
    tge = (time.perf_counter() - t) / REPS
    out += ["", f"[Python] n=100: generar coeficientes = {tg*1e6:.2f} us (promedio de {REPS} repeticiones)",
            f"[Python] n=100, x={x}: generar + evaluar = {tge*1e6:.2f} us (promedio de {REPS} repeticiones)"]
    texto = "\n".join(out)
    print(texto)
    open("resultados_pascal_python.txt", "w").write(texto + "\n")
