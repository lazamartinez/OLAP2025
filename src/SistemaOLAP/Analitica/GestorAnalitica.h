#ifndef GESTORANALITICA_H
#define GESTORANALITICA_H

#include <QObject>
#include <QVariantMap>
#include <QVector>


namespace SistemaOLAP {
namespace Analitica {

/**
 * @brief Modulo de Analitica Avanzada y Mineria de Datos.
 * Implementa algoritmos de Clustering y Forecasting integrados con el Cubo.
 */
class GestorAnalitica : public QObject {
  Q_OBJECT
public:
  explicit GestorAnalitica(QObject *parent = nullptr);

  // Algoritmos de Clustering
  struct ClusterResult {
    int clusterId;
    QVector<double> centroide;
    int numPuntos;
  };
  QVector<ClusterResult> ejecutarClusteringKMeans(int k);
  void ejecutarDBSCAN(double epsilon, int minPts);

  // Algoritmos de Series Temporales (Forecasting)
  struct ForecastResult {
    QString fecha;
    double valorPredicho;
    double intervaloConfianza;
  };
  QVector<ForecastResult> predecirVentas(int diasFuturos);

signals:
  void analisisCompletado(QString reporte);
};

} // namespace Analitica
} // namespace SistemaOLAP

#endif // GESTORANALITICA_H
