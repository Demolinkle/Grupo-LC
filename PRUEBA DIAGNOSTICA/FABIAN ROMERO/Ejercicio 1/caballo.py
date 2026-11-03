import sys, time

def verificar(ak, movs):
    """Devuelve (ok, mensaje). Se detiene en el primer movimiento incorrecto."""
    c, f = ord(ak[0]) - 97, int(ak[1]) - 1
    for i, m in enumerate(movs, 1):
        if len(m) != 3 or m[0] != 'K':
            return False, f"INCORRECTO falla letra {m[0]} no pertenece a un caballo." if m[0] != 'K' else f"INCORRECTO falla en el movimiento {i}"
        nc, nf = ord(m[1]) - 97, ord(m[2]) - 49
        if not (0 <= nc < 8 and 0 <= nf < 8) or (abs(nc - c), abs(nf - f)) not in ((1, 2), (2, 1)):
            return False, f"INCORRECTO falla en el movimiento {i}"
        c, f = nc, nf
    return True, "movimiento CORRECTO"

if __name__ == "__main__":
    ejemplos = [
        ("a1", "Kc2 Kd4 Kb3 Kc5"), ("a1", "Kc2 Kd4 Kb4 Kc5"),
        ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"), ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7"),
        ("a8", "Kb6 Kd7"), ("a8", "Kb6 Kb7"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4")]
    out = ["== Ejemplos (Python) =="]
    for ak, s in ejemplos:
        out.append(f"Ak={ak} C={s} -> {verificar(ak, s.split())[1]}")
    todos = open("moves.txt").read().split()
    ak = "a2"
    out.append(f"\nTotal de movimientos en la lista: {len(todos)}  (Ak inicial={ak})")
    out.append(f"Resultado lista completa: {verificar(ak, todos)[1]}")
    for etiqueta, lista in (("n=100", todos[:100]), (f"n={len(todos)}", todos)):
        tiempos = []
        for _ in range(10):
            t = time.perf_counter(); verificar(ak, lista); tiempos.append(time.perf_counter() - t)
        out.append(f"[Python] {etiqueta}: 10 repeticiones, total={sum(tiempos)*1e6:.2f} us, promedio={sum(tiempos)/10*1e6:.2f} us")
    texto = "\n".join(out)
    print(texto)
    open("resultados_python.txt", "w").write(texto + "\n")
