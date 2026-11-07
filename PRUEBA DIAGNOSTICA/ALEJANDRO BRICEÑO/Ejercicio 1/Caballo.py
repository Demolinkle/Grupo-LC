import time
from pathlib import Path


def parse_casilla(s):
    if not s or len(s) != 2:
        return None
    c, r = s[0].lower(), s[1]
    if not ("a" <= c <= "h") or not r.isdigit():
        return None
    r_int = int(r)
    if not (1 <= r_int <= 8):
        return None
    return (ord(c) - 97, r_int)


def parse_movimiento(mov):
    if not mov or len(mov) != 3:
        return None
    pc = mov[0].upper()
    casilla = parse_casilla(mov[1:])
    if casilla is None:
        return None
    return (pc,) + casilla


def validate(start, seq, n=0):
    prev = parse_casilla(start)
    if prev is None:
        return 2, 0

    num_mov = 0
    for mov in seq.split():
        num_mov += 1
        cur = parse_movimiento(mov)
        if cur is None or cur[0] != "K":
            return 2, num_mov
        if abs(prev[0] - cur[1]) * abs(prev[1] - cur[2]) != 2:
            return 1, num_mov
        prev = (cur[1], cur[2])

    if n > 0 and num_mov != n:
        return 3, n
    return 0, 0


def pieza_movimiento(seq, k):
    palabras = seq.split()
    if 1 <= k <= len(palabras) and palabras[k - 1]:
        return palabras[k - 1][0].upper()
    return "?"


def describir_resultado(code, fi, seq, n):
    if code == 0:
        return "movimiento CORRECTO"
    if code == 1:
        return f"INCORRECTO falla en el movimiento {fi}"
    if code == 2:
        letra = pieza_movimiento(seq, fi)
        if letra == "?":
            return "INCORRECTO posicion inicial mal formada"
        return f"INCORRECTO falla letra {letra} no pertenece a un caballo"
    if code == 3:
        return f"INCORRECTO numero de movimientos distinto de {n}"
    return f"codigo desconocido ({code})"


_BASE = (
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
    "Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 "
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
    "Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
    "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 "
)


def _construir_500():
    toks = _BASE.split()
    out = []
    i = 0
    while len(out) < 500:
        out.append(toks[i % len(toks)])
        i += 1
    return " ".join(out)


SEQ500 = _construir_500()


def _casos_autotest():
    return [
        ("a1", "Kc2 Kd4 Kb3 Kc5", 4,
         "movimiento CORRECTO"),
        ("a1", "Kc2 Kd4 Kb4 Kc5", 4,
         "INCORRECTO falla en el movimiento 3"),
        ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7", 5,
         "movimiento CORRECTO"),
        ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7", 5,
         "INCORRECTO falla en el movimiento 1"),
        ("a8", "Kb6 Kd7", 2,
         "movimiento CORRECTO"),
        ("a8", "Kb6 Kb7", 2,
         "INCORRECTO falla en el movimiento 2"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", 10,
         "movimiento CORRECTO"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4", 10,
         "INCORRECTO falla letra Q no pertenece a un caballo"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", 10,
         "INCORRECTO falla en el movimiento 9"),
    ]


def autotest():
    print("=== Autotest: casos oficiales del PDF ===")
    ok = True
    for start, seq, n, esperado in _casos_autotest():
        code, fi = validate(start, seq, n)
        got = describir_resultado(code, fi, seq, n)
        if " ".join(got.split()) == " ".join(esperado.split()):
            print(f"[OK]    {start} | {seq[:40]}... -> {got}")
        else:
            ok = False
            print(f"[FALLA] {start} | {seq}")
            print(f"        esperado: {esperado}")
            print(f"        obtenido: {got}")
    print("=== Fin autotest ===\n")
    return ok


def main():
    OUT = Path(__file__).resolve().parent / "resultados_python.txt"

    total = len(SEQ500.split())
    if total != 500:
        raise SystemExit(
            f"ERROR: SEQ500 tiene {total} movimientos, se esperan 500."
        )

    code0, fi0 = validate("a2", SEQ500, 500)
    if code0 != 0:
        raise SystemExit(
            f"ERROR: SEQ500 no es valida: "
            f"{describir_resultado(code0, fi0, SEQ500, 500)}"
        )

    if not autotest():
        raise SystemExit("Autotest fallo: revisar validacion.")

    with open(OUT, "w", encoding="utf-8") as o:
        o.write("## [Python] Ejercicio 1 - Validacion de saltos de caballo ##\n\n")

        o.write("### Autotest ###\n")
        for start, seq, n, esperado in _casos_autotest():
            code, fi = validate(start, seq, n)
            got = describir_resultado(code, fi, seq, n)
            estado = "OK" if " ".join(got.split()) == " ".join(esperado.split()) else "FALLA"
            o.write(f"  [{estado}] {start} | {seq} -> {got}\n")
        o.write("\n")

        o.write("### Benchmark: 10 verificaciones de 500 movimientos (a2) ###\n")
        o.write("  secuencia: 500 movimientos generados por el estudiante\n")
        o.write("             (bloque base de 100 repetido)\n")

        t0 = time.perf_counter()
        for _ in range(10):
            validate("a2", SEQ500, 500)
        t1 = time.perf_counter()
        total_s = t1 - t0

        o.write(f"  resultado: {describir_resultado(code0, fi0, SEQ500, 500)}\n")
        o.write(f"  10 verificaciones: {total_s * 1e3:.4f} ms "
                f"(promedio: {total_s / 10 * 1e3:.4f} ms)\n")

    print(f"Listo. Ver {OUT}")


if __name__ == "__main__":
    main()

