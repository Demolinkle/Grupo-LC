# Script de instalacion de dependencias - Prueba Diagnostica (Kelvys Concepcion)
# Instala el compilador de C (GCC / MinGW-w64) y Python 3 en Windows usando winget.
# Ejecutar en PowerShell:   powershell -ExecutionPolicy Bypass -File instalar.ps1

function Instalar-SiFalta($comando, $idWinget, $nombre) {
    if (Get-Command $comando -ErrorAction SilentlyContinue) {
        Write-Host "$nombre ya esta instalado: $((Get-Command $comando).Source)"
    } else {
        Write-Host "Instalando $nombre..."
        winget install --id $idWinget -e --accept-source-agreements --accept-package-agreements
    }
}

Instalar-SiFalta "gcc"    "BrechtSanders.WinLibs.POSIX.UCRT" "GCC (MinGW-w64)"
Instalar-SiFalta "python" "Python.Python.3.13"               "Python 3"

Write-Host ""
Write-Host "Listo. Cierre y abra de nuevo la terminal para que se actualice el PATH."
Write-Host "Luego verifique con:  gcc --version   y   python --version"
