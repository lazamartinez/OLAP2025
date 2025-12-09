@echo off
setlocal
echo ==========================================
echo  Construyendo Sistema OLAP (Qt/MinGW)
echo ==========================================

:: Configurar Rutas de Qt (Detectadas automaticamente por el Asistente)
set QT_DIR=C:\Qt\6.10.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64\bin
set CMAKE_DIR=C:\Qt\Tools\CMake_64\bin
set NINJA_DIR=C:\Qt\Tools\Ninja

:: Agregar al PATH
set PATH=%QT_DIR%\bin;%MINGW_DIR%;%CMAKE_DIR%;%NINJA_DIR%;%PATH%

:: Verificar herramientas
where cmake
where gcc
where ninja

if not exist build mkdir build
cd build

echo [1/2] Configurando CMake (Ninja)...
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="%QT_DIR%" ..
if %errorlevel% neq 0 (
    echo Error en CMake Configuration.
    exit /b %errorlevel%
)

echo [2/2] Compilando...
ninja
if %errorlevel% neq 0 (
    echo Error en Compilacion.
    exit /b %errorlevel%
)

echo ==========================================
echo  Build Exitoso!
echo  Ejecutando Sistema...
echo ==========================================

:: Ejecutar
start SistemaOlap.exe
