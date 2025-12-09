@echo off
setlocal

:: Configurar rutas de Qt y PostgreSQL
:: Ajusta estas rutas si tu instalacion es diferente
set QT_BIN=C:\Qt\6.10.1\mingw_64\bin
set PG_BIN=C:\Program Files\PostgreSQL\14\bin

:: Agregar al PATH temporalmente
set PATH=%QT_BIN%;%PG_BIN%;%PATH%

:: Establecer ruta de plugins de Qt (opcional pero recomendado)
set QT_PLUGIN_PATH=%~dp0build

echo Iniciando Sistema OLAP 2025...
echo ------------------------------

if exist "build\SistemaOlap.exe" (
    start "" "build\SistemaOlap.exe"
) else (
    echo ERROR: No se encuentra el ejecutable en 'build\SistemaOlap.exe'.
    echo Por favor, compila el proyecto primero.
    pause
)

endlocal
