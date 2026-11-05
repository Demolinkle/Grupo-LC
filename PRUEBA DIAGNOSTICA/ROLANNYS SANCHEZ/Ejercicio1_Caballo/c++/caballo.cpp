
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <iomanip>

using namespace std;

// Representa una posicion del tablero de ajedrez.
struct Posicion {
    int x;
    int y;
};

// Convierte una casilla como "b4" en coordenadas numericas.
bool convertirPosicion(const string& casilla, Posicion& posicion) {

    if (casilla.length() != 2) {
        return false;
    }

    char columna = casilla[0];
    char fila = casilla[1];

    if (columna < 'a' || columna > 'h') {
        return false;
    }

    if (fila < '1' || fila > '8') {
        return false;
    }

    posicion.x = columna - 'a' + 1;
    posicion.y = fila - '0';

    return true;
}

// Comprueba si el caballo puede desplazarse
// desde una posicion hasta otra.
bool movimientoValido(const Posicion& origen,
                      const Posicion& destino) {

    int dx = abs(destino.x - origen.x);
    int dy = abs(destino.y - origen.y);

    return (dx == 2 && dy == 1) ||
           (dx == 1 && dy == 2);
}

// Resultado de la validacion.
struct Resultado {
    bool correcto;
    int movimientoFalla;
    string motivo;
};

// Valida todos los movimientos y se detiene
// al encontrar el primero incorrecto.
Resultado validarSecuencia(
    const string& posicionInicial,
    const vector<string>& movimientos
) {

    Posicion actual;

    if (!convertirPosicion(posicionInicial, actual)) {
        return {false, 0, "Posicion inicial fuera del tablero."};
    }

    for (int i = 0; i < (int)movimientos.size(); i++) {

        string movimiento = movimientos[i];

        // Verificar que sea un movimiento de caballo.
        if (movimiento.length() != 3 ||
            movimiento[0] != 'K') {

            return {
                false,
                i + 1,
                "Formato incorrecto o letra diferente de K."
            };
        }

        // Extraer la casilla de destino.
        string casillaDestino = movimiento.substr(1);

        Posicion destino;

        // Verificar que la casilla exista en el tablero.
        if (!convertirPosicion(casillaDestino, destino)) {

            return {
                false,
                i + 1,
                "Casilla de destino fuera del tablero."
            };
        }

        // Comprobar el movimiento del caballo.
        if (!movimientoValido(actual, destino)) {

            return {
                false,
                i + 1,
                "El caballo no puede realizar ese desplazamiento."
            };
        }

        // Actualizar la posicion del caballo.
        actual = destino;
    }

    return {true, 0, "Todos los movimientos son correctos."};
}

int main() {

    cout << "======================================" << endl;
    cout << "       VALIDACION DE CABALLO" << endl;
    cout << "======================================" << endl;

    // Abrir el archivo que contiene la secuencia.
    ifstream archivo("secuencia.txt");

    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir secuencia.txt" << endl;
        return 1;
    }

    // Leer el primer elemento.
    string primeraPosicion;

    if (!(archivo >> primeraPosicion)) {
        cout << "Error: el archivo esta vacio." << endl;
        return 1;
    }

    // La primera casilla representa la posicion inicial.
    if (primeraPosicion.length() != 3 ||
        primeraPosicion[0] != 'K') {

        cout << "Error: posicion inicial incorrecta." << endl;
        return 1;
    }

    string posicionInicial = primeraPosicion.substr(1);

    // Almacenar los movimientos restantes.
    vector<string> movimientos;
    string movimiento;

    while (archivo >> movimiento) {
        movimientos.push_back(movimiento);
    }

    archivo.close();

    int n = movimientos.size();

    cout << "Posicion inicial: " << posicionInicial << endl;
    cout << "Cantidad de movimientos: " << n << endl;

    // Medir el tiempo de 10 validaciones.
    const int REPETICIONES = 10;

    Resultado resultado;

    // Variable para asegurar que cada validacion
    // sea ejecutada y su resultado sea utilizado.
    volatile int contadorValidaciones = 0;

    // Iniciar la medicion del tiempo.
    auto inicio = chrono::steady_clock::now();

    for (int i = 0; i < REPETICIONES; i++) {

        resultado = validarSecuencia(
            posicionInicial,
            movimientos
        );

        // Registrar el resultado de cada ejecucion.
        contadorValidaciones =
            contadorValidaciones + (resultado.correcto ? 1 : 0);
    }

    // Finalizar la medicion del tiempo.
    auto fin = chrono::steady_clock::now();

    // Calcular el tiempo transcurrido en nanosegundos.
    chrono::duration<double, nano> tiempo = fin - inicio;

    double tiempoTotalNano = tiempo.count();

    double tiempoPromedioNano =
        tiempoTotalNano / REPETICIONES;

    // Convertir nanosegundos a milisegundos.
    double tiempoTotalMs =
        tiempoTotalNano / 1000000.0;

    double tiempoPromedioMs =
        tiempoPromedioNano / 1000000.0;

    // Mostrar el resultado de la validacion.
    cout << "\n========== RESULTADO ==========" << endl;

    if (resultado.correcto) {
        cout << "MOVIMIENTO CORRECTO." << endl;
    }
    else {
        cout << "MOVIMIENTO INCORRECTO." << endl;

        cout << "Falla en el movimiento: "
             << resultado.movimientoFalla << endl;

        cout << "Motivo: "
             << resultado.motivo << endl;
    }

    // Mostrar la cantidad de validaciones correctas.
    cout << "Validaciones correctas: "
         << contadorValidaciones << " de "
         << REPETICIONES << endl;

    // Mostrar los tiempos de ejecucion.
    cout << fixed << setprecision(6);

    cout << "\n========== MEDICION ==========" << endl;

    cout << "Cantidad de ejecuciones: "
         << REPETICIONES << endl;

    cout << "Tiempo total: "
         << tiempoTotalNano << " nanosegundos" << endl;

    cout << "Tiempo promedio: "
         << tiempoPromedioNano << " nanosegundos" << endl;

    cout << "Tiempo total: "
         << tiempoTotalMs << " milisegundos" << endl;

    cout << "Tiempo promedio: "
         << tiempoPromedioMs << " milisegundos" << endl;

    return 0;
}
