
import os
import time


# Convierte una casilla en coordenadas del tablero.
def convertir_posicion(casilla):

    if len(casilla) != 2:
        return None

    columna = casilla[0]
    fila = casilla[1]

    if columna < 'a' or columna > 'h' or fila < '1' or fila > '8':
        return None

    x = ord(columna) - ord('a')
    y = int(fila) - 1

    return (x, y)


# Comprueba si el caballo realiza un movimiento valido.
def movimiento_valido(origen, destino):

    dx = abs(destino[0] - origen[0])
    dy = abs(destino[1] - origen[1])

    return (dx == 2 and dy == 1) or (dx == 1 and dy == 2)


# Valida todos los movimientos de la secuencia.
def validar_secuencia(posicion_inicial, movimientos):

    actual = convertir_posicion(posicion_inicial)

    # Comprobar la posicion inicial.
    if actual is None:
        return False, 0, "Posicion inicial invalida"

    # Recorrer todos los movimientos.
    for i, movimiento in enumerate(movimientos):

        # Comprobar el formato del movimiento.
        if len(movimiento) != 3 or movimiento[0] != 'K':
            return False, i + 1, "Formato de movimiento invalido"

        # Obtener la casilla de destino.
        casilla = movimiento[1:3]

        siguiente = convertir_posicion(casilla)

        # Comprobar que la casilla este dentro del tablero.
        if siguiente is None:
            return False, i + 1, "Casilla fuera del tablero"

        # Comprobar que el movimiento sea legal.
        if not movimiento_valido(actual, siguiente):
            return False, i + 1, "Movimiento ilegal del caballo"

        # Actualizar la posicion actual.
        actual = siguiente

    return True, 0, "Todos los movimientos son validos"


# Funcion principal.
def main():

    # Obtener la carpeta donde se encuentra este programa.
    carpeta = os.path.dirname(os.path.abspath(__file__))

    # Construir la ruta del archivo secuencia.txt.
    ruta_archivo = os.path.join(carpeta, "secuencia.txt")

    # Abrir el archivo con la secuencia.
    try:
        with open(ruta_archivo, "r") as archivo:
            datos = archivo.read().split()

    except FileNotFoundError:
        print("Error: no se encontro secuencia.txt")
        print("Ruta buscada:", ruta_archivo)
        return

    # Comprobar que el archivo no este vacio.
    if not datos:
        print("Error: el archivo esta vacio")
        return

    # Leer la posicion inicial.
    posicion_inicial = datos[0]

    # Convertir Kb4 en b4.
    if len(posicion_inicial) == 3 and posicion_inicial[0] == 'K':
        posicion_inicial = posicion_inicial[1:]

    # Obtener los movimientos restantes.
    movimientos = datos[1:]

    # Mostrar informacion inicial.
    print("========================================")
    print("       VALIDACION DE MOVIMIENTOS")
    print("========================================")

    print("Posicion inicial:", posicion_inicial)
    print("Cantidad de movimientos:", len(movimientos))

    print("\nValidacion de 10 ejecuciones:\n")

    tiempo_total = 0

    # Ejecutar la validacion exactamente 10 veces.
    for i in range(10):

        # Registrar el tiempo inicial.
        inicio = time.perf_counter_ns()

        # Validar la secuencia completa.
        correcto, movimiento_falla, motivo = validar_secuencia(
            posicion_inicial,
            movimientos
        )

        # Registrar el tiempo final.
        fin = time.perf_counter_ns()

        # Calcular el tiempo transcurrido en nanosegundos.
        tiempo = fin - inicio

        tiempo_total += tiempo

        # Mostrar el resultado de la ejecucion.
        print(f"Ejecucion {i + 1}: ", end="")

        if correcto:
            print("CORRECTA", end="")
        else:
            print(
                f"INCORRECTA - Fallo en el movimiento {movimiento_falla}: {motivo}",
                end=""
            )

        print(f" | Tiempo: {tiempo:.2f} ns")

    # Calcular el promedio de las 10 ejecuciones.
    promedio = tiempo_total / 10

    # Mostrar los resultados finales.
    print("\n========================================")
    print("          RESULTADOS FINALES")
    print("========================================")

    print("Total de ejecuciones: 10")
    print(f"Tiempo acumulado: {tiempo_total:.2f} ns")
    print(f"Tiempo promedio: {promedio:.2f} ns")
    print(f"Tiempo promedio en milisegundos: {promedio / 1000000:.5f} ms")

    print("========================================")


# Punto de entrada del programa.
if __name__ == "__main__":
    main()
