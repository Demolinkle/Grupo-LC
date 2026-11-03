"""Verificador de movimientos de un caballo de ajedrez.
Uso:
  python caballo.py                           modo interactivo (pide Ak, n y C)
  python caballo.py a1 4 Kc2 Kd4 Kb3 Kc5      verifica: Ak, n y los movimientos
  python caballo.py --ejemplos                corre los 9 ejemplos del enunciado
  python caballo.py --tiempo [moves.txt] [Ak] mide el tiempo y escribe un txt
"""
import sys, time

EJEMPLOS = [
    ("a1", "Kc2 Kd4 Kb3 Kc5"), ("a1", "Kc2 Kd4 Kb4 Kc5"),
    ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"), ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7"),
    ("a8", "Kb6 Kd7"), ("a8", "Kb6 Kb7"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4")]

def casilla(s):
    """'a1' -> (columna, fila) con valores 0..7, o None si no es una casilla valida."""
    if len(s) == 2 and 'a' <= s[0] <= 'h' and '1' <= s[1] <= '8':
        return ord(s[0]) - 97, int(s[1]) - 1
    return None

def verificar(ak, movs):
    """Devuelve (ok, mensaje). Se detiene en el primer movimiento incorrecto."""
    pos = casilla(ak)
    if pos is None:
        return False, f"Posicion inicial invalida: {ak}"
    c, f = pos
    for i, m in enumerate(movs, 1):
        if m[0] != 'K':
            return False, f"INCORRECTO falla letra {m[0]} no pertenece a un caballo."
        d = casilla(m[1:])
        if d is None or (abs(d[0] - c), abs(d[1] - f)) not in ((1, 2), (2, 1)):
            return False, f"INCORRECTO falla en el movimiento {i}"
        c, f = d
    return True, "movimiento CORRECTO"

def resolver(ak, n, movs):
    if n != len(movs):
        print(f"Aviso: n={n} pero la cadena tiene {len(movs)} movimientos; se verifican los {len(movs)} recibidos.")
    print(verificar(ak, movs)[1])

def modo_ejemplos():
    for ak, s in EJEMPLOS:
        movs = s.split()
        print(f"Ak = {ak} n= {len(movs)} C= {s}  ->  {verificar(ak, movs)[1]}")

def medir(ak, movs, reps):
    t = time.perf_counter()
    for _ in range(reps):
        verificar(ak, movs)
    return time.perf_counter() - t

def modo_tiempo(ruta, ak):
    todos = open(ruta).read().split()
    lineas = [f"Archivo: {ruta} | movimientos: {len(todos)} | Ak inicial: {ak}",
              f"Resultado de la lista completa: {verificar(ak, todos)[1]}"]
    for etiqueta, lista in (("n=100", todos[:100]), (f"n={len(todos)}", todos)):
        t10 = medir(ak, lista, 10)
        t1000 = medir(ak, lista, 1000) / 1000
        lineas.append(f"[Python] {etiqueta}: 10 verificaciones = {t10*1e6:.2f} us en total | "
                      f"promedio de 1000 repeticiones = {t1000*1e6:.2f} us por verificacion")
    texto = "\n".join(lineas)
    print(texto)
    open("resultados_caballo_python.txt", "w").write(texto + "\n")

if __name__ == "__main__":
    a = sys.argv[1:]
    if not a:
        ak = input("Posicion inicial del caballo Ak (ej. a1): ").strip()
        n = int(input("Cantidad de movimientos n: "))
        resolver(ak, n, input("Cadena C (ej. Kc2 Kd4 Kb3 Kc5): ").split())
    elif a[0] == "--ejemplos":
        modo_ejemplos()
    elif a[0] == "--tiempo":
        modo_tiempo(a[1] if len(a) > 1 else "moves.txt", a[2] if len(a) > 2 else "a2")
    elif len(a) >= 3:
        resolver(a[0], int(a[1]), " ".join(a[2:]).split())
    else:
        print(__doc__)
