#include "ServicioEtl.h"
#include "GestorBaseDatos.h"
#include <QDebug>
#include <QDir>
#include <QProcess>

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

  QDir dir(QString::fromStdString(directorioDatos));
  QString absPath = dir.absolutePath();

  qDebug() << "Intentando carga masiva (Cliente-Side \\copy) desde:" << absPath;

  // Limpieza previa (Truncate)
  if (!db.ejecutarConsulta(
          "TRUNCATE TABLE fact_ventas, dim_producto, dim_sucursal CASCADE;")) {
    qWarning() << "No se pudo truncar tablas. Tal vez no existen aun.";
  }

  // Definir paths de archivos (usar forward slashes para evitar problemas)
  QString pathProd = absPath + "/productos.csv";
  QString pathSuc = absPath + "/sucursales.csv";
  QString pathVentas = absPath + "/ventas.csv";

  // Función helper para ejecutar psql \copy
  auto ejecutarCopy = [](QString tabla, QString archivo) {
    QProcess process;
    // Configurar entorno para contraseña (evita prompt)
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("PGPASSWORD", "laza"); // Contraseña hardcodeada para el demo
    process.setProcessEnvironment(env);

    QString cmd = "psql";
    QStringList args;
    args << "-h" << "localhost"
         << "-p" << "5432"
         << "-U" << "postgres"
         << "-d" << "bd2025"
         << "-c"
         << QString("\\copy %1 FROM '%2' DELIMITER ',' CSV HEADER")
                .arg(tabla, archivo);

    qDebug() << "Ejecutando:" << cmd << args.join(" "); // Debug info

    process.start(cmd, args);
    // Timeout aumentado a 5 minutos (300000 ms) para carga masiva
    if (!process.waitForFinished(300000)) {
      qCritical() << "Timeout o error ejecutando psql para" << tabla;
      return false;
    }

    if (process.exitCode() != 0) {
      qCritical() << "Error psql:" << process.readAllStandardError();
      return false;
    }

    qDebug() << "Carga OK:" << tabla;
    return true;
  };

  bool ok = true;
  if (!ejecutarCopy("dim_producto", pathProd))
    ok = false;
  if (!ejecutarCopy("dim_sucursal", pathSuc))
    ok = false;
  if (!ejecutarCopy("fact_ventas", pathVentas))
    ok = false; // Este es el mas pesado

  return ok;
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

  // 1. Asegurar que exista
  if (!db.ejecutarConsulta(sqlVista))
    return false;

  // 2. Refrescar datos (si ya existia o para actualizar datos nuevos)
  return db.ejecutarConsulta("REFRESH MATERIALIZED VIEW ventas_por_categoria;");
}
