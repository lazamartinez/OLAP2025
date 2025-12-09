# Plan de Evolución: Sistema OLAP & DW (Proyecto Final Cátedra)

Este plan tiene como objetivo elevar el "Sistema OLAP 2025" actual al nivel de un **Trabajo Final Universitario** completo, cubriendo todos los requisitos pedagógicos (ETL, OLAP, Seguridad, Minería, Documentación) y agregando capacidades visuales avanzadas (GIS 3D).

## 1. Documentación Pedagógica y Académica
Generar los entregables teóricos solicitados para la defensa.
- [ ] **Guía Pedagógica (`docs/Pedagogical_Guide.md`)**: Explicar cómo el código implementa cada unidad del programa (Transacciones, ACID, ETL, Vistas Materializadas).
- [ ] **Diagramas de Arquitectura (`docs/Architecture.md`)**: Diagramas actualizados (Mermaid).
- [ ] **Presentación de Defensa (`docs/Defense_Slides.md`)**: Esquema de diapositivas.

## 2. Módulo GIS y Visualización Real-Time (NUEVO)
Implementación de Mapa 3D y Simulación en Vivo.
- [x] **Integración Visualizador Nativo**: Implementado con `QGraphicsView` (High Performance).
- [x] **Widget Mapa 3D (`WidgetMapa`)**: Mejorar a **Globo Terráqueo 3D Rotatorio** (Proyección Ortográfica Simulada) con textura satelital (Mapbox).
- [x] **Motor en Tiempo Real**: Implementado con `QTimer` y señales en `VentanaPrincipal`.
- [x] **CRUD Sucursales**: Implementado dialogo `DialogoSucursal` y botón en mapa.
- [x] **Controles de Rotación**: Botones Pausa/Play y dirección (Overlay UI).
- [x] **Efectos de Profundidad**: Z-Scaling para puntos y Atmósfera mejorada (Starfield + Atmosphere Glow).
- [x] **HUD Estadístico**: Overlay con datos globales de ventas en vivo.
- [x] **Feed de Transacciones en Vivo**: Panel lateral "Glass" con log de ventas detallado (Producto, Ciudad, Monto).
- [x] **Dual View (Globo/Plano)**: Switch para alternar entre "Holograma 3D" y "Mapa Plano 2D" (visualizando la misma data).
- [x] **Estética Premium + Iconos**: refinar UI con estilo transparente y usar iconos (Unicode/FontAwesome-like).

## 3. Global UI Overhaul (Aesthetic Upgrade)
- [ ] **Modern White Theme**: Implementar estilo limpio (Fondo Blanco/Gris, Acentos Azules) en VentanaPrincipal.
- [ ] **Limpieza de Emojis**: Remover emojis de botones y etiquetas en todos los widgets.
- [ ] **Styling Profesional**: Aplicar temas claros a Gráficos y Tablas.

## 3. Fortalecimiento de Base de Datos (Seguridad y Triggers)
Implementar características avanzadas de BD.
- [ ] **Script de Roles y Permisos (`sql/roles_security.sql`)**.
- [ ] **Triggers de Auditoría (`sql/triggers_audit.sql`)**.
- [ ] **Validación de Integridad**.

## 4. Minería de Datos y Analítica Avanzada
Expandir el módulo de analítica.
- [ ] **Mejora del Visualizador K-Means**.
- [ ] **(Opcional) Regresión Lineal**.

## 5. Calidad de Software y Testing
- [ ] **Tests Unitarios (`tests/`)**.
- [ ] **Checklist de Instalación**.

---

### Estado Actual del Sistema (Base Sólida)
*   ✅ **ETL**: Funcional (CSV -> COPY -> DW).
*   ✅ **OLAP**: Vistas Materializadas.
*   ✅ **UI**: Qt6 Moderno.
*   ✅ **Store**: PostgreSQL 14+.
