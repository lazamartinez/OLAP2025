#ifndef MOTORETL_H
#define MOTORETL_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>
#include <memory>


// Namespace structure as requested
namespace SistemaOLAP {
namespace Nucleo {

/**
 * @brief Clase base para adaptadores de fuentes de datos.
 * Simula la interfaz para adaptadores especializados (Parquet/Arrow, CSV Fast,
 * JSON SIMD).
 */
class AdaptadorFuenteDatos {
public:
  virtual ~AdaptadorFuenteDatos() = default;
  virtual bool conectar(const QString &connectionString) = 0;
  virtual QVector<QVariantMap>
  extraerDatos(int limite = -1) = 0; // Return generic structure for now
};

/**
 * @brief Motor de Extraccion, Transformacion y Carga.
 * Utiliza internamente conceptos de Apache Arrow para procesamiento columnar en
 * memoria.
 */
class MotorETL : public QObject {
  Q_OBJECT
public:
  explicit MotorETL(QObject *parent = nullptr);
  ~MotorETL();

  // Configura el pipeline de transformacion (YAML config mock)
  void configurarPipeline(const QString &yamlConfigPath);

  // Ejecuta el proceso ETL completo
  void ejecutarProceso(const QString &fuenteTipo,
                       const QString &connectionString);

  // Simulacion de carga incremental con Watermarks (etcd)
  void cargarIncremental();

signals:
  void progresoActualizado(int porcentaje, QString mensaje);
  void procesoFinalizado(bool exito, QString reporte);

private:
  // Puntero a la implementacion interna (Pimpl pattern) para esconder
  // dependencias complejas
  struct Impl;
  std::unique_ptr<Impl> m_impl;

  void procesarLote(const QVector<QVariantMap> &lote);
};

} // namespace Nucleo
} // namespace SistemaOLAP

#endif // MOTORETL_H
