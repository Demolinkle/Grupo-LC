# Compila y ejecuta los tres ejercicios
# Uso: powershell -ExecutionPolicy Bypass -File compilar.ps1
$raiz = $PSScriptRoot

Write-Host "`n##### Ejercicio 1: caballo (C) #####"
Set-Location "$raiz\Ejercicio 1"
gcc -Wall -O2 caballo.c -o caballo.exe
.\caballo.exe

Write-Host "`n##### Ejercicio 2: Pascal en C #####"
Set-Location "$raiz\Ejercicio 2"
gcc -Wall -O2 pascal.c -o pascal.exe -lm
.\pascal.exe

Write-Host "`n##### Ejercicio 2: Pascal en Python #####"
python pascal.py

Write-Host "`n##### Ejercicio 3: traductor (C) #####"
Set-Location "$raiz\Ejercicio 3"
gcc -Wall -O2 traductor.c -o traductor.exe
.\traductor.exe

Set-Location $raiz
