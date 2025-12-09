cmake_minimum_required(VERSION 3.16)

project(SistemaOlap VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configurar Qt6 con módulos necesarios
find_package(Qt6 REQUIRED COMPONENTS 
    Widgets 
    Charts 
    Core 
    Gui 
    Sql 
    Network 
    Qml 
    Quick 
    Quick3D 
    QuickWidgets
    # WebEngineWidgets  # Comentado - no instalado
)

# Includes
include_directories(src)
include_directories(src/core) # Legacy support
include_directories(src/SistemaOLAP/Nucleo)
include_directories(src/SistemaOLAP/Visualizacion)
include_directories(src/SistemaOLAP/Analitica)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/ui/VentanaPrincipal.cpp
    src/ui/WidgetTablero.cpp
    src/ui/WidgetEtl.cpp
    src/ui/WidgetAnalitica.cpp
    src/ui/WidgetMapa.cpp
    src/core/GestorBaseDatos.cpp
    src/core/SimuladorDatos.cpp
    src/core/ServicioEtl.cpp
    src/analytics/KMeans.cpp
    # Nuevos Modulos de la Arquitectura Integral
    src/SistemaOLAP/Nucleo/MotorETL.cpp
    src/SistemaOLAP/Nucleo/CuboDatos.cpp
    src/SistemaOLAP/Nucleo/PipelineTransformacion.cpp
    src/SistemaOLAP/Nucleo/ProcesadorConsultas.cpp
    src/SistemaOLAP/Analitica/GestorAnalitica.cpp
    src/ui/WidgetVisor3D.cpp
    src/ui/DialogoSucursal.cpp
    src/ui/WidgetFiltrosGlobales.cpp
    src/ui/ExportadorCSV.cpp
    src/ui/WidgetHistorial.cpp
    src/ui/GestorFavoritos.cpp
    src/ui/WidgetHeatmap.cpp
    # src/ui/WidgetMapbox.cpp  # Requiere WebEngine
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
    Qt6::Network
    Qt6::Qml
    Qt6::Quick
    Qt6::Quick3D
    Qt6::QuickWidgets
    # Qt6::WebEngineWidgets  # No instalado
)

# Copiar carpeta de datos
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
