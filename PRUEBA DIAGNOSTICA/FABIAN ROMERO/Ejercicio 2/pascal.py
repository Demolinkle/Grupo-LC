import sys, time

def pascal(n):
    """Fila n del triangulo de Pascal con programacion dinamica:
    una sola fila (O(n) memoria) que se actualiza de derecha a izquierda."""
    fila = [1]
    for i in range(1, n + 1):
        fila.append(1)
        for j in range(i - 1, 0, -1):
            fila[j] += fila[j - 1]
    return fila

def polinomio(fila):
    n = len(fila) - 1
    t = []
    for k, c in enumerate(fila):
        p = n - k
        cs = "" if (c == 1 and p > 0) else str(c)
        xs = "" if p == 0 else ("x" if p == 1 else f"x^{p}")
        t.append(cs + xs)
    return " + ".join(t)

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
    fila = pascal(n)
    pasos = []
    r = evaluar(fila, x, pasos)
    out = [f"n = {n}, x = {x}", f"Coeficientes: {fila}", f"f(x) = {polinomio(fila)}",
           "Calculo paso a paso:"] + pasos + [f"f({x}) = {r}", f"Verificacion ({x}+1)^{n} = {(x+1)**n}"]
    # tiempo para n=100 (generar coeficientes + evaluar), 10 repeticiones
    ts = []
    for _ in range(10):
        t = time.perf_counter(); f = pascal(100); evaluar(f, x); ts.append(time.perf_counter() - t)
    out.append(f"\n[Python] n=100, x={x}: 10 repeticiones, total={sum(ts)*1e6:.2f} us, promedio={sum(ts)/10*1e6:.2f} us")
    texto = "\n".join(out)
    print(texto)
    open("resultados_pascal_python.txt", "w").write(texto + "\n")
