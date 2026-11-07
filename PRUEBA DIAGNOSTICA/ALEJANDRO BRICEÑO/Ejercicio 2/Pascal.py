import time

def imprimir_polinomio(n, archivo):
    pascal = []
    for i in range(n + 1):
        fila = [1] * (i + 1)
        for j in range(1, i):
            fila[j] = pascal[i - 1][j - 1] + pascal[i - 1][j]
        pascal.append(fila)

    archivo.write(f"(x+1)^{n} = ")
    for i, coef in enumerate(pascal[n]):
        potencia = n - i
        
        if i > 0:
            archivo.write("+")
            
        if potencia == 0:
            archivo.write(f"{coef}")
        elif potencia == 1:
            if coef != 1: archivo.write(f"{coef}")
            archivo.write("x")
        else:
            if coef != 1: archivo.write(f"{coef}")
            archivo.write(f"x^{potencia}")
    archivo.write("\n\n")

def calcular_polinomio_pasos(n, x, archivo):
    pascal = []
    for i in range(n + 1):
        fila = [1] * (i + 1)
        for j in range(1, i):
            fila[j] = pascal[i - 1][j - 1] + pascal[i - 1][j]
        pascal.append(fila)

    archivo.write(f"Cálculo paso a paso para f(x) = (x+1)^{n} con x = {x}:\n")
    resultado = 0
    
    for i, coef in enumerate(pascal[n]):
        potencia = n - i
        valor_termino = coef * (x ** potencia)
        resultado += valor_termino
        archivo.write(f"  Término {i+1}: {coef} * ({x})^{potencia} = {valor_termino}\n")
        
    archivo.write(f"Suma total = {resultado}\n")
    archivo.write(f"Resultado final f({x}) = {resultado:.2f}\n\n")
    return resultado

def calcular_polinomio_tiempo(n, x):
    pascal = []
    for i in range(n + 1):
        fila = [1] * (i + 1)
        for j in range(1, i):
            fila[j] = pascal[i - 1][j - 1] + pascal[i - 1][j]
        pascal.append(fila)

    resultado = 0
    for i, coef in enumerate(pascal[n]):
        potencia = n - i
        resultado += coef * (x ** potencia)
    return resultado

def main():
    with open("resultados-python.txt", "a") as archivo:
        archivo.write("--- Resultados en Python ---\n\n")
        
        archivo.write("a) Genere los coeficientes del polinomio (x+1)^n, muestre el resultado del polinomio:\n")
        imprimir_polinomio(4, archivo)

        archivo.write("b) Calcule f(x) = (x+1)^n según el polinomio generado (mostrando pasos):\n")
        calcular_polinomio_pasos(2, 7, archivo)

        archivo.write("c) Calcule el tiempo necesario para generar n=100:\n")
        inicio = time.time()
        resultado_c = calcular_polinomio_tiempo(100, 2)
        fin = time.time()
        tiempo = fin - inicio

        archivo.write(f"Para x = 2 y n = 100\n")
        archivo.write(f"Resultado f(x): {resultado_c:.6e}\n")
        archivo.write(f"Tiempo de ejecucion: {tiempo:.6f} segundos\n\n")

    print("Python: Proceso terminado. Revisar resultados.txt")

if __name__ == "__main__":
    main()