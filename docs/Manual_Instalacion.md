# Manual de Instalación y Ejecución

## Requisitos Previos

1. **Compilador C++**: Microsoft Visual Studio 2019/2022 (con carga de trabajo C++) o MinGW (GCC 9+).
2. **CMake**: Versión 3.16 o superior.
3. **Qt 6**: Instalar via Qt Online Installer o usar `vcpkg`.
4. **Git**: Para clonar dependencias.

## Gestión de Dependencias (Opción: vcpkg)

Si usas [vcpkg](https://github.com/microsoft/vcpkg), instala las librerías:

```powershell
./vcpkg install qt6-base duckdb
```

## Compilación con CMake

Desde la raíz del proyecto (`c:\Proyectos Facu\BD2025`):

```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[ruta/a/vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build . --config Release
```

_Nota: Si no usas vcpkg, asegúrate de que CMake pueda encontrar Qt6 y DuckDB (añadiendo sus rutas al PATH o variables de CMake)._

## Ejecución

El ejecutable se generará en `build/Release/OlapSystem.exe`.
Asegúrate de ejecutarlo desde un entorno que tenga acceso a las DLLs de Qt.
