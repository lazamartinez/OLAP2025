# Defensa del Trabajo Final: Sistema OLAP + DW

## Diapositiva 1: Título
- **Proyecto**: Sistema de Gestión y Análisis de Datos (OLAP)
- **Cátedra**: Bases de Datos 2025
- **Alumno**: [Tu Nombre]
- **Tecnologías**: C++17, Qt6, DuckDB

## Diapositiva 2: Objetivos
- Simular un entorno real de ventas retail.
- Construir un Data Warehouse funcional (Modelo Estrella).
- Demonstrar capacidades de análisis (OLAP) y minería (K-Means).
- Rendimiento nativo mediante arquitectura C++ embebida.

## Diapositiva 3: Arquitectura
- **Frontend**: Qt Widgets (Dashboard, Gráficos).
- **Backend**: C++ Services (ETL, Simulador).
- **Persistencia**: DuckDB (Motor Columnar para analítica).
- **Ventaja**: El motor embebido elimina latencia de red y permite procesar millones de filas en milisegundos.

## Diapositiva 4: Modelo de Datos (Star Schema)
- **Tabla de Hechos**: `fact_ventas` (Métrica: total, cantidad).
- **Dimensiones**: `dim_producto`, `dim_cliente`, `dim_sucursal`, `dim_tiempo`.
- *Justificación*: Optimiza consultas de agregación típicas de BI.

## Diapositiva 5: Proceso ETL
1. **Generación**: Simulador crea CSVs con distribución estadística realista.
2. **Carga**: Ingesta masiva via `COPY` command (Zero-copy loading si usáramos Arrow).
3. **Consolidación**: Vistas materializadas para el dashboard.

## Diapositiva 6: Minería de Datos
- Algoritmo **K-Means** implementado nativamente.
- Objetivo: Segmentación de clientes (Clustering).
- Variables: Gasto Anual vs Frecuencia de Compra.
- Resultado: Identificación de clientes "VIP", "Promedio" y "Ocasionales".

## Diapositiva 7: Conclusiones
- Se logró un sistema de alto rendimiento.
- Integración exitosa de C++ con motor SQL moderno.
- Cumplimiento de requisitos pedagógicos (Transacciones, DW, OLAP).
