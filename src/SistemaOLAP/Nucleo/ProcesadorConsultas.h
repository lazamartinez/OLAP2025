#ifndef PROCESADORCONSULTAS_H
#define PROCESADORCONSULTAS_H

#include "CuboDatos.h"
#include <QObject>
#include <QString>


namespace SistemaOLAP {
namespace Nucleo {

struct ConsultaMDX {
  QString ejeColumnas;
  QString ejeFilas;
  QString cubo;
  QString where;
};

/**
 * @brief Procesador de Consultas OLAP.
 * Implementa un Parser MDX (Mocking ANTLR4) y un Optimizador de Consultas
 * (Mocking Apache Calcite).
 */
class ProcesadorConsultasOLAP : public QObject {
  Q_OBJECT
public:
  explicit ProcesadorConsultasOLAP(CuboDatos *cubo, QObject *parent = nullptr);

  void ejecutarConsulta(const QString &mdxQuery);

signals:
  void resultadoListo(const QString &resultadoFormateado);
  void errorOcurrido(const QString &mensaje);

private:
  CuboDatos *m_cubo;
  ConsultaMDX parsearMDX(const QString &query);
};

} // namespace Nucleo
} // namespace SistemaOLAP

#endif // PROCESADORCONSULTAS_H
