# Guía Pedagógica: Vínculo con Conceptos de Bases de Datos

Este documento explica cómo el proyecto **Sistema OLAP + DW** cubre las unidades del programa de la materia Bases de Datos.

## 1. Data Warehousing y Modelo Dimensional
- **Concepto**: Implementación de un esquema estrella (Star Schema) vs Normalización (3NF).
- **En el Código**:
  - `src/core/EtlService.cpp` -> `initializeSchema()` define `dim_producto`, `dim_sucursal` y `fact_ventas`.
- **Ejercicio**: Comparar el rendimiento de consultas agregadas sobre el esquema estrella vs un esquema normalizado.

## 2. ETL (Extracción, Transformación y Carga)
- **Concepto**: Proceso de limpieza y carga masiva de datos.
- **En el Código**:
  - **E**: `DataSimulator` genera los CSVs (fuente).
  - **T**: El simulador limpia datos (ej. fechas, precios coherentes).
  - **L**: `EtlService::loadFromCSV` usa `COPY ... FROM` de DuckDB para ingesta masiva.
- **Ejercicio**: Modificar `EtlService` para filtrar ventas menores a cierto monto durante la carga (Transformación).

## 3. OLAP (Online Analytical Processing)
- **Concepto**: Consultas multidimensionales para toma de decisiones.
- **En el Código**:
  - `src/ui/DashboardWidget.cpp` ejecuta queries con `GROUP BY`, `SUM`, `ROLLUP`.
  - Interactividad: Gráficos de evolución temporal y tablas pivote.
- **Ejercicio**: Implementar una consulta `CUBE` o `ROLLUP` adicional y mostrarla en el dashboard.

## 4. Minería de Datos y Descubrimiento de Conocimiento
- **Concepto**: Identificación de patrones ocultos.
- **En el Código**:
  - `src/analytics/KMeans.cpp`: Implementación desde cero del algoritmo de clustering.
  - `src/ui/AnalyticsWidget.cpp`: Segmentación de clientes basada en RFM (Recencia, Frecuencia, Monto - simplificado).
- **Ejercicio**: Variar el número de clusters `K` y observar cómo cambia la agrupación gráfica.

## 5. Integridad y Transacciones
- **Concepto**: ACID.
- **En el Código**:
  - DuckDB maneja transacciones implícitas en cada query.
  - `DatabaseManager` puede extenderse para usar `BEGIN TRANSACTION` / `COMMIT` al cargar lotes.
- **Ejercicio**: Envolver la carga ETL en una sola transacción para asegurar atomicidad (todo o nada).

## 6. Arquitectura Cliente-Servidor vs Embebida
- **Concepto**: Modelos de despliegue.
- **En el Código**: Uso de DuckDB (Embebido, In-Process) para alto rendimiento local, eliminando latencia de red.
