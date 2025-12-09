#include "PipelineTransformacion.h"
#include <QDebug>
#include <QtMath>


namespace SistemaOLAP {
namespace Nucleo {

void FiltroNormalizacion::procesar(QVariantMap &registro) {
  // Simular Z-score scaling usando Eigen (Conceptual)
  // (x - mean) / std_dev
  // Aqui solo marcamos el registro como normalizado
  registro["meta_normalized"] = true;
}

void FiltroAnonimizacion::procesar(QVariantMap &registro) {
  // Simular k-anonymity
  // Ocultar ultimos digitos de IP o ID
  if (registro.contains("user_ip")) {
    QString ip = registro["user_ip"].toString();
    int lastDot = ip.lastIndexOf('.');
    if (lastDot != -1) {
      registro["user_ip"] = ip.left(lastDot) + ".XXX";
    }
  }
}

PipelineTransformacion::PipelineTransformacion(QObject *parent)
    : QObject(parent) {}

void PipelineTransformacion::agregarFiltro(FiltroBase *filtro) {
  m_filtros.append(filtro);
}

void PipelineTransformacion::ejecutar(QVector<QVariantMap> &loteDatos) {
  // Ejecucion secuencial optimizada
  // En una implementacion real, usaria TBB para paralelismo de datos
  for (auto &reg : loteDatos) {
    for (auto *filtro : m_filtros) {
      filtro->procesar(reg);
    }
  }
}

} // namespace Nucleo
} // namespace SistemaOLAP
