# Plan de Implementación: Sistema GIS 3D y Simulación Real-Time

## Objetivo
Implementar un "War Room" (Sala de Situación) interactiva donde se visualicen transacciones en tiempo real sobre un globo terráqueo 3D, conectando sucursales y mostrando métricas en vivo.

## Capa Visual: Mapa Interactivo Nativo (QGraphicsView)
Dado que WebEngine no está disponible en el entorno, implementaremos un visualizador de mapas nativo de alto rendimiento:
1.  **Tecnología**: `QGraphicsView` + `QGraphicsScene` altamente optimizado.
2.  **Estética**: Mapa "Cyberpunk" con proyección equirectangular y arcos de conexión curvos (Bézier) animados.
3.  **Animación**: Uso de `QTimer` para animar el "pulso" de las transacciones en tiempo real.

### Sin Dependencias Externas Pesadas
*   Utiliza solo `Qt6::Widgets` y `Qt6::Gui`.
*   Asegura compatibilidad total con cualquier instalación de Qt.

## Capa Lógica: Motor en Tiempo Real
Transformar `SimuladorDatos` para soportar modo "Live".
1.  **Clase `MotorSimulacion` (Thread)**:
    *   Ejecuta un loop infinito (controlado).
    *   Genera 1 venta aleatoria cada X milisegundos.
    *   Emite señales Qt para la UI.
    *   Inserta en BD (opcional, o solo en memoria para visualización).

## Funcionalidad de Sucursales (CRUD)
1.  **Tabla `dim_sucursal`**: Agregar campos `latitud` y `longitud`.
2.  **UI Gestión**:
    *   Tabla para ver sucursales.
    *   Formulario para Alta/Modificación con geolocalización.

## Arquitectura de Archivos
*   `src/ui/WidgetMapa.cpp`: Contenedor del WebEngine.
*   `src/ui/map_resources/`: Carpeta con HTML/JS y texturas del globo.
*   `src/core/MotorSimulacion.cpp`: Generador de eventos.

## Pasos de Ejecución
1.  Actualizar CMakeLists.txt (WebEngine).
2.  Crear estructura de archivos HTML/JS para el globo.
3.  Implementar `WidgetMapa`.
4.  Implementar `MotorSimulacion`.
5.  Conectar todo en `VentanaPrincipal`.
