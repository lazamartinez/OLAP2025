#pragma once

#include <QPointF>
#include <QVector>
#include <cmath>


class AnaliticaML {
public:
  struct ResultadoRegresion {
    double pendiente;
    double intercepto;
    double r2; // Coeficiente de determinación
    QVector<double> predicciones;
  };

  struct ResultadoAnomalias {
    QVector<int> indices;
    QVector<double> zScores;
    double media;
    double desviacionEstandar;
  };

  // Regresión lineal simple (y = mx + b)
  static ResultadoRegresion regresionLineal(const QVector<QPointF> &datos,
                                            int diasFuturos = 30) {
    ResultadoRegresion resultado;

    if (datos.isEmpty())
      return resultado;

    int n = datos.size();
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;

    for (const auto &p : datos) {
      sumX += p.x();
      sumY += p.y();
      sumXY += p.x() * p.y();
      sumX2 += p.x() * p.x();
    }

    // Calcular pendiente e intercepto
    resultado.pendiente = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    resultado.intercepto = (sumY - resultado.pendiente * sumX) / n;

    // Calcular R²
    double sumYPred = 0, sumYDiff = 0;
    double meanY = sumY / n;

    for (const auto &p : datos) {
      double yPred = resultado.pendiente * p.x() + resultado.intercepto;
      sumYPred += (yPred - meanY) * (yPred - meanY);
      sumYDiff += (p.y() - meanY) * (p.y() - meanY);
    }

    resultado.r2 = sumYPred / sumYDiff;

    // Generar predicciones futuras
    double ultimoX = datos.last().x();
    for (int i = 1; i <= diasFuturos; ++i) {
      double x = ultimoX + i;
      double y = resultado.pendiente * x + resultado.intercepto;
      resultado.predicciones.append(y);
    }

    return resultado;
  }

  // Detección de anomalías usando Z-Score
  static ResultadoAnomalias detectarAnomalias(const QVector<double> &valores,
                                              double umbral = 3.0) {
    ResultadoAnomalias resultado;

    if (valores.isEmpty())
      return resultado;

    // Calcular media
    double suma = 0;
    for (double v : valores)
      suma += v;
    resultado.media = suma / valores.size();

    // Calcular desviación estándar
    double sumaCuadrados = 0;
    for (double v : valores) {
      double diff = v - resultado.media;
      sumaCuadrados += diff * diff;
    }
    resultado.desviacionEstandar = std::sqrt(sumaCuadrados / valores.size());

    // Detectar anomalías
    for (int i = 0; i < valores.size(); ++i) {
      double zScore = std::abs((valores[i] - resultado.media) /
                               resultado.desviacionEstandar);
      if (zScore > umbral) {
        resultado.indices.append(i);
        resultado.zScores.append(zScore);
      }
    }

    return resultado;
  }

  // Suavizado exponencial simple para forecasting
  static QVector<double> suavizadoExponencial(const QVector<double> &serie,
                                              double alpha = 0.3,
                                              int pasos = 10) {
    QVector<double> suavizada;
    if (serie.isEmpty())
      return suavizada;

    suavizada.append(serie.first());

    for (int i = 1; i < serie.size(); ++i) {
      double valor = alpha * serie[i] + (1 - alpha) * suavizada.last();
      suavizada.append(valor);
    }

    // Predicciones futuras
    for (int i = 0; i < pasos; ++i) {
      suavizada.append(suavizada.last());
    }

    return suavizada;
  }
};
