# Instrucciones de Ejecución: Sistema OLAP + DW

Sigue estos pasos para compilar y ejecutar el proyecto en tu máquina.

## 1. Requisitos Previos
Asegúrate de tener instalados:
- **CMake** (3.16+)
- **Compilador C++** (Visual Studio 2019/2022 con C++ workload o MinGW 64-bit).
- **Qt 6**: Librerías y compilador compatible.
- **PostgreSQL**: Servidor corriendo en `localhost:5432`.

## 2. Preparar Base de Datos
Abre pgAdmin o una terminal y ejecuta:
```sql
CREATE DATABASE bd2025;
-- El usuario 'postgres' con contraseña 'laza' debe existir.
-- Si la contraseña es diferente, edita src/ui/WidgetEtl.cpp línea 55.
```

## 3. Compilación (Opción Automática)
Ejecuta el script incluido (doble click o desde CMD):
`scripts\build_windows.bat`

## 4. Compilación (Manual paso a paso)
Si el script falla, abre una terminal (PowerShell o CMD) en la carpeta del proyecto y ejecuta:

```powershell
# 1. Crear carpeta de compilación
mkdir build
cd build

# 2. Configurar con CMake (Asegúrate de que encuentre Qt)
# Si Qt está en una ruta standard, esto basta:
cmake .. -DCMAKE_BUILD_TYPE=Release

# Si CMake no encuentra Qt, especifica la ruta (ejemplo para Qt 6.5.0 MinGW):
# cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.5.0\mingw_64"

# 3. Compilar
cmake --build . --config Release
```

## 5. Ejecutar
El ejecutable se creará en:
`build\Release\SistemaOlap.exe` (o `build\Debug\SistemaOlap.exe`).

**Nota**: Si al abrirlo da error de "missing .dll", debes ejecutarlo desde el **Qt Command Prompt** o copiar las DLLs de Qt (Qt6Core, Qt6Widgets, etc.) junto al ejecutable. Una herramienta útil es `windeployqt`:
```powershell
C:\Qt\6.x.x\...\bin\windeployqt.exe build\Release\SistemaOlap.exe
```

## 6. Uso del Sistema
1. Ve a la pestaña **Gestor ETL**.
2. Click en **Conectar BD**.
3. Click en **Cargar DW** para poblar la base de datos (Demo).
4. Ve al **Tablero** para ver los gráficos.
