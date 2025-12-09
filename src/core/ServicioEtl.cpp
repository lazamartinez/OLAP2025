#include "ServicioEtl.h"
#include "GestorBaseDatos.h"
#include <QDebug>
#include <QDir>

ServicioEtl::ServicioEtl() {}

bool ServicioEtl::inicializarEsquema() {
  auto &db = GestorBaseDatos::instancia();

  // 1. Dimension Productos
  QString sqlProd = R"(
        CREATE TABLE IF NOT EXISTS dim_producto (
            id_producto INTEGER PRIMARY KEY,
            nombre VARCHAR(100),
            categoria VARCHAR(50),
            marca VARCHAR(50),
            precio_base DECIMAL(10,2)
        );
    )";

  // 2. Dimension Sucursales
  QString sqlSuc = R"(
        CREATE TABLE IF NOT EXISTS dim_sucursal (
            id_sucursal INTEGER PRIMARY KEY,
            ciudad VARCHAR(50),
            region VARCHAR(50),
            pais VARCHAR(50)
        );
    )";

  // 3. Fact Ventas
  QString sqlVentas = R"(
        CREATE TABLE IF NOT EXISTS fact_ventas (
            id_venta INTEGER PRIMARY KEY,
            timestamp TIMESTAMP,
            id_producto INTEGER,
            id_cliente INTEGER,
            id_sucursal INTEGER,
            cantidad INTEGER,
            precio_unitario DECIMAL(10,2),
            total DECIMAL(10,2),
            FOREIGN KEY (id_producto) REFERENCES dim_producto(id_producto),
            FOREIGN KEY (id_sucursal) REFERENCES dim_sucursal(id_sucursal)
        );
    )";

  qDebug() << "Creando tablas...";
  if (!db.ejecutarConsulta(sqlProd))
    return false;
  if (!db.ejecutarConsulta(sqlSuc))
    return false;
  if (!db.ejecutarConsulta(sqlVentas))
    return false;

  return true;
}

bool ServicioEtl::cargarDesdeCSV(const std::string &directorioDatos) {
  auto &db = GestorBaseDatos::instancia();

  // En PostgreSQL, COPY requiere superusuario o ser dueño, y archivo en
  // SERVIDOR. Para carga cliente, usamos \copy (solo psql) o COPY ... FROM
  // STDIN. Como esto es C++ cliente, la forma mas robusta sin depender de la
  // ubicacion del server es leer el archivo y hacer batch inserts, o usar 'COPY
  // table FROM STDIN' con libpq. QSqlDriver no soporta COPY directamente de
  // forma facil. TRUCO: Si es local, podemos usar COPY ... FROM 'absolute/path'
  // si el usuario de postgres tiene permisos. Intentaremos COPY con path
  // absoluto. Si falla, fallback a nada (por simplicidad del demo).

  QDir dir(QString::fromStdString(directorioDatos));
  QString absPath = dir.absolutePath();

  qDebug() << "Intentando carga masiva (COPY) desde:" << absPath;

  // Limpieza previa
  db.ejecutarConsulta(
      "TRUNCATE TABLE fact_ventas, dim_producto, dim_sucursal CASCADE;");

  // NOTA: Postgres requiere path con forward slashes o escapeados.
  QString pathProd = absPath + "/productos.csv";
  QString pathSuc = absPath + "/sucursales.csv";
  QString pathVentas = absPath + "/ventas.csv";

  // COPY table FROM 'path' DELIMITER ',' CSV HEADER;
  auto copyCmd = [](QString table, QString path) {
    return QString("COPY %1 FROM '%2' DELIMITER ',' CSV HEADER;")
        .arg(table, path);
  };

  if (!db.ejecutarConsulta(copyCmd("dim_producto", pathProd))) {
    qWarning() << "Fallo COPY dim_producto. Asegúrate que el usuario postgres "
                  "tenga permisos de lectura en:"
               << pathProd;
    // Fallback? Seria complejo implementar batch insert aqui mismo sin bloat.
    // Asumimos exito o error de permisos visible.
    return false;
  }

  if (!db.ejecutarConsulta(copyCmd("dim_sucursal", pathSuc)))
    return false;
  if (!db.ejecutarConsulta(copyCmd("fact_ventas", pathVentas)))
    return false;

  return true;
}

bool ServicioEtl::transformarDatos() {
  auto &db = GestorBaseDatos::instancia();

  // Vista Materializada para OLAP
  QString sqlVista = R"(
        CREATE MATERIALIZED VIEW IF NOT EXISTS ventas_por_categoria AS
        SELECT 
            p.categoria, 
            s.region, 
            SUM(v.total) as total_ventas,
            COUNT(*) as cantidad_operaciones
        FROM fact_ventas v
        JOIN dim_producto p ON v.id_producto = p.id_producto
        JOIN dim_sucursal s ON v.id_sucursal = s.id_sucursal
        GROUP BY p.categoria, s.region
        WITH DATA;
    )";

  // Refresh si ya existe
  db.ejecutarConsulta("REFRESH MATERIALIZED VIEW ventas_por_categoria;");

  return db.ejecutarConsulta(sqlVista);
}
