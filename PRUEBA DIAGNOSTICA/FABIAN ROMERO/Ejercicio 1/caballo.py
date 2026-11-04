# VERIFICADOR DE MOVIMIENTOS DE UN CABALLO DE AJEDREZ
#
#   python caballo.py                                     
#   python caballo.py a1 4 Kc2 Kd4 Kb3 Kc5   
#   python caballo.py --ejemplos            
#   python caballo.py --tiempo              

import sys
import time

COLUMNAS = "abcdefgh"
FILAS = "12345678"


def columna_y_fila(casilla):
    if len(casilla) != 2:
        return None
    letra = casilla[0]
    numero = casilla[1]
    if letra not in COLUMNAS or numero not in FILAS:
        return None
    columna = COLUMNAS.index(letra)
    fila = FILAS.index(numero)
    return columna, fila


def es_salto_de_caballo(col_a, fila_a, col_b, fila_b):
    diferencia_columnas = abs(col_b - col_a)
    diferencia_filas = abs(fila_b - fila_a)
    if diferencia_columnas == 1 and diferencia_filas == 2:
        return True
    if diferencia_columnas == 2 and diferencia_filas == 1:
        return True
    return False


def verificar(inicio, movimientos):
    posicion = columna_y_fila(inicio)
    if posicion is None:
        return "Posicion inicial invalida: " + inicio

    columna_actual = posicion[0]
    fila_actual = posicion[1]
    numero_movimiento = 0

    for movimiento in movimientos:
        numero_movimiento = numero_movimiento + 1

        # 1) La primera letra debe ser K (caballo)
        if movimiento[0] != "K":
            return "INCORRECTO falla letra " + movimiento[0] + " no pertenece a un caballo."

        # 2) Lo que queda despues de la K debe ser una casilla que exista
        destino = columna_y_fila(movimiento[1:])
        if destino is None:
            return "INCORRECTO falla en el movimiento " + str(numero_movimiento)
        columna_nueva = destino[0]
        fila_nueva = destino[1]

        # 3) El salto debe ser en L
        if not es_salto_de_caballo(columna_actual, fila_actual, columna_nueva, fila_nueva):
            return "INCORRECTO falla en el movimiento " + str(numero_movimiento)

        # el caballo ahora esta en la casilla nueva
        columna_actual = columna_nueva
        fila_actual = fila_nueva

    return "movimiento CORRECTO"


def mostrar_resultado(inicio, n, movimientos):
    if n != len(movimientos):
        print("Aviso: n=" + str(n) + " pero hay " + str(len(movimientos)) + " movimientos. Se verifican los " + str(len(movimientos)) + ".")
    print(verificar(inicio, movimientos))


def probar_ejemplos():
    ejemplos = [
        ("a1", "Kc2 Kd4 Kb3 Kc5"),
        ("a1", "Kc2 Kd4 Kb4 Kc5"),
        ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"),
        ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7"),
        ("a8", "Kb6 Kd7"),
        ("a8", "Kb6 Kb7"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
        ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"),
    ]
    for inicio, texto in ejemplos:
        movimientos = texto.split()   # separa el texto en palabras
        print("Ak = " + inicio + " n= " + str(len(movimientos)) + " C= " + texto + "  ->  " + verificar(inicio, movimientos))


def segundos_en_repetir(inicio, movimientos, veces):
    empieza = time.perf_counter()
    for i in range(veces):
        verificar(inicio, movimientos)
    termina = time.perf_counter()
    return termina - empieza


def medir_tiempo(archivo, inicio):
    todos = open(archivo).read().split()
    lineas = []
    lineas.append("Archivo: " + archivo + " | movimientos: " + str(len(todos)) + " | Ak inicial: " + inicio)
    lineas.append("Resultado de la lista completa: " + verificar(inicio, todos))

    grupos = [("n=100", todos[:100]), ("n=" + str(len(todos)), todos)]
    for nombre, lista in grupos:
        total_10 = segundos_en_repetir(inicio, lista, 10)
        promedio_1000 = segundos_en_repetir(inicio, lista, 1000) / 1000
        lineas.append("[Python] " + nombre + ": 10 verificaciones = " + format(total_10 * 1000000, ".2f")
                      + " us en total | promedio de 1000 repeticiones = " + format(promedio_1000 * 1000000, ".2f")
                      + " us por verificacion")

    texto = "\n".join(lineas)
    print(texto)
    salida = open("resultados_caballo_python.txt", "w")
    salida.write(texto + "\n")
    salida.close()


datos = sys.argv[1:]

if len(datos) == 0:
    inicio = input("Posicion inicial del caballo Ak (ej. a1): ").strip()
    n = int(input("Cantidad de movimientos n: "))
    movimientos = input("Cadena C (ej. Kc2 Kd4 Kb3 Kc5): ").split()
    mostrar_resultado(inicio, n, movimientos)
elif datos[0] == "--ejemplos":
    probar_ejemplos()
elif datos[0] == "--tiempo":
    archivo = "moves.txt"
    inicio = "a2"
    if len(datos) > 1:
        archivo = datos[1]
    if len(datos) > 2:
        inicio = datos[2]
    medir_tiempo(archivo, inicio)
elif len(datos) >= 3:
    mostrar_resultado(datos[0], int(datos[1]), " ".join(datos[2:]).split())
else:
    print("Uso: python caballo.py Ak n movimientos...   |   --ejemplos   |   --tiempo")