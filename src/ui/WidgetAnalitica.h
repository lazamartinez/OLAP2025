#pragma once

#include <QPushButton>
#include <QWidget>
#include <QtCharts/QChartView>

class WidgetAnalitica : public QWidget {
  Q_OBJECT

public:
  explicit WidgetAnalitica(QWidget *parent = nullptr);

private slots:
  void ejecutarClustering();
  void ejecutarPrediccion();
  void detectarAnomalias();

private:
  void configurarUi();

  QChartView *m_vistaChart;
  QChart *m_chart;
};
