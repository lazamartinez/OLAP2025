cmake_minimum_required(VERSION 3.16)

project(SistemaOlap VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configurar Qt6 con módulo SQL
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core Gui Sql)

# Includes
include_directories(src)

# Archivos fuente (Nuevos nombres en Español)
set(SOURCES
    src/main.cpp
    src/core/GestorBaseDatos.cpp
    src/core/SimuladorDatos.cpp
    src/core/ServicioEtl.cpp
    src/ui/VentanaPrincipal.cpp
    src/ui/WidgetTablero.cpp
    src/ui/WidgetEtl.cpp
    src/ui/WidgetAnalitica.cpp
    src/analytics/KMeans.cpp
)

# Ejecutable
add_executable(SistemaOlap ${SOURCES})

# Enlace de librerías
target_link_libraries(SistemaOlap PRIVATE
    Qt6::Widgets
    Qt6::Charts
    Qt6::Core
    Qt6::Gui
    Qt6::Sql
)

# Copiar carpeta de datos
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
