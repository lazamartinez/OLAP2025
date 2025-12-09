#ifndef CUBODATOS_H
#define CUBODATOS_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVector>


namespace SistemaOLAP {
namespace Nucleo {

struct Dimension {
  QString nombre;
  QVector<QString> jerarquias;
};

struct Medida {
  QString nombre;
  QString tipoDatos; // "float", "int"
};

/**
 * @brief Abstraccion del Almacenamiento Multidimensional MOLAP.
 * Gestiona la persistencia en TileDB/RocksDB y provee acceso rapido a celdas.
 */
class CuboDatos : public QObject {
  Q_OBJECT
public:
  explicit CuboDatos(QString nombre, QObject *parent = nullptr);

  void inicializarEsquema(); // Carga definicion desde RepositorioMetadatos

  // Query basica para visualizacion (retorna voxels)
  struct VoxelData {
    float x, y, z;
    float valor;
    QString categoria;
  };
  QVector<VoxelData> obtenerSlice(const QString &dimensionEjeX,
                                  const QString &dimensionEjeY,
                                  const QString &dimensionEjeZ);

  // Operaciones OLAP
  void slice(const QString &dimension, const QString &valor);
  void dice(const QMap<QString, QString> &filtros);
  void drillDown(const QString &dimension);
  void rollUp(const QString &dimension);

private:
  QString m_nombre;
  QVector<Dimension> m_dimensiones;
  QVector<Medida> m_medidas;

  // Simulado: Datos en memoria
  QVector<VoxelData> m_datosSimulados;
  void generarDatosSimulados();
};

} // namespace Nucleo
} // namespace SistemaOLAP

#endif // CUBODATOS_H
