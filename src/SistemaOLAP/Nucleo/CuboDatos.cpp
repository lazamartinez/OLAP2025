#include "CuboDatos.h"
#include <QRandomGenerator>
#include <cmath>

namespace SistemaOLAP {
namespace Nucleo {

CuboDatos::CuboDatos(QString nombre, QObject *parent)
    : QObject(parent), m_nombre(nombre) {
  inicializarEsquema();
  generarDatosSimulados();
}

void CuboDatos::inicializarEsquema() {
  // Esquema Demo
  m_dimensiones.push_back({"Tiempo", {"Año", "Trimestre", "Mes"}});
  m_dimensiones.push_back({"Geografia", {"Pais", "Region", "Ciudad"}});
  m_dimensiones.push_back({"Producto", {"Categoria", "Subcategoria", "SKU"}});
  m_medidas.push_back({"Ventas", "double"});
}

void CuboDatos::generarDatosSimulados() {
  // Generar una nube de puntos 3D interesante para visualizar
  // Simulando una distribucion de ventas cruzando 3 dimensiones
  for (int x = 0; x < 20; ++x) {
    for (int y = 0; y < 20; ++y) {
      for (int z = 0; z < 20; ++z) {
        // Simular algun patron (e.g. cluster)
        float cx = x - 10.0f;
        float cy = (y - 10.0f) * 0.5f;
        float cz = z - 10.0f;
        float dist = std::sqrt(cx * cx + cy * cy + cz * cz);

        if (dist < 8.0f) {
          float val = (10.0f - dist) * 1000.0f +
                      QRandomGenerator::global()->bounded(500);
          if (val > 0) {
            VoxelData voxel;
            voxel.x = x * 10.0f; // Espaciado visual
            voxel.y = y * 10.0f;
            voxel.z = z * 10.0f;
            voxel.valor = val;
            voxel.categoria = (val > 5000) ? "High" : "Low";
            m_datosSimulados.append(voxel);
          }
        }
      }
    }
  }
}

QVector<CuboDatos::VoxelData> CuboDatos::obtenerSlice(const QString &dimX,
                                                      const QString &dimY,
                                                      const QString &dimZ) {
  // En una implementacion real, consultaria TileDB/RocksDB
  // Aqui retornamos el set simulado completo o filtrado
  return m_datosSimulados;
}

void CuboDatos::slice(const QString &dimension, const QString &valor) {
  // Logica para filtrar m_datosSimulados
}

void CuboDatos::dice(const QMap<QString, QString> &filtros) {}
void CuboDatos::drillDown(const QString &dimension) {}
void CuboDatos::rollUp(const QString &dimension) {}

} // namespace Nucleo
} // namespace SistemaOLAP
