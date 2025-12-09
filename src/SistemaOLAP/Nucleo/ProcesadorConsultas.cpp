#include "ProcesadorConsultas.h"
#include <QDebug>
#include <QRegularExpression>


namespace SistemaOLAP {
namespace Nucleo {

ProcesadorConsultasOLAP::ProcesadorConsultasOLAP(CuboDatos *cubo,
                                                 QObject *parent)
    : QObject(parent), m_cubo(cubo) {}

void ProcesadorConsultasOLAP::ejecutarConsulta(const QString &mdxQuery) {
  qDebug() << "[ProcesadorOLAP] Recibida consulta MDX:" << mdxQuery;

  try {
    ConsultaMDX ast = parsearMDX(mdxQuery);
    // Simular optimizacion con Apache Calcite (Pushdown predicates)
    qDebug() << "[Optimizador] Aplicando reglas de reescritura Calcite...";

    // Simular ejecucion
    QString resultado = "Resultado para Cubo [" + ast.cubo + "]:\n";
    resultado += "  Eje X (Cols): " + ast.ejeColumnas + "\n";
    resultado += "  Eje Y (Filas): " + ast.ejeFilas + "\n";
    resultado += "  Datos: [ ... Renderizado por Motor ... ]";

    emit resultadoListo(resultado);

  } catch (const std::exception &e) {
    emit errorOcurrido(QString("Error de Parsing: ") + e.what());
  }
}

ConsultaMDX ProcesadorConsultasOLAP::parsearMDX(const QString &query) {
  ConsultaMDX consulta;
  // Regex simplificado para: SELECT ... ON COLUMNS, ... ON ROWS FROM [Cubo]
  QRegularExpression reSelect("SELECT\\s+(.+)\\s+ON\\s+COLUMNS",
                              QRegularExpression::CaseInsensitiveOption);
  QRegularExpression reFrom("FROM\\s+\\[?(\\w+)\\]?",
                            QRegularExpression::CaseInsensitiveOption);

  auto matchSelect = reSelect.match(query);
  if (matchSelect.hasMatch()) {
    consulta.ejeColumnas = matchSelect.captured(1);
  } else {
    // Fallback dummy
    consulta.ejeColumnas = "[Measures].[Ventas]";
  }

  auto matchFrom = reFrom.match(query);
  if (matchFrom.hasMatch()) {
    consulta.cubo = matchFrom.captured(1);
  } else {
    consulta.cubo = "CuboDesconocido";
  }

  return consulta;
}

} // namespace Nucleo
} // namespace SistemaOLAP
