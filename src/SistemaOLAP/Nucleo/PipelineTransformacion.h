#ifndef PIPELINETRANSFORMACION_H
#define PIPELINETRANSFORMACION_H

#include <QList>
#include <QObject>
#include <QVariantMap>


namespace SistemaOLAP {
namespace Nucleo {

class FiltroBase {
public:
  virtual ~FiltroBase() = default;
  virtual void procesar(QVariantMap &registro) = 0;
  virtual QString nombre() const = 0;
};

class FiltroNormalizacion : public FiltroBase {
public:
  void procesar(QVariantMap &registro) override; // Z-score scaling mock
  QString nombre() const override { return "Normalizacion (Eigen Wrapper)"; }
};

class FiltroAnonimizacion : public FiltroBase {
public:
  void procesar(QVariantMap &registro) override; // k-anonymity mock
  QString nombre() const override { return "Anonimizacion (ARX Wrapper)"; }
};

class PipelineTransformacion : public QObject {
  Q_OBJECT
public:
  explicit PipelineTransformacion(QObject *parent = nullptr);
  void agregarFiltro(FiltroBase *filtro);
  void ejecutar(QVector<QVariantMap> &loteDatos);

private:
  QList<FiltroBase *> m_filtros;
};

} // namespace Nucleo
} // namespace SistemaOLAP

#endif // PIPELINETRANSFORMACION_H
