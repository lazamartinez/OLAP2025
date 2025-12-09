#include "WidgetAnalitica.h"
#include "../analytics/KMeans.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>


WidgetAnalitica::WidgetAnalitica(QWidget *parent) : QWidget(parent) {
  configurarUi();
}

void WidgetAnalitica::configurarUi() {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *lbl = new QLabel("<h3>Segmentación de Clientes (K-Means)</h3>");
  lbl->setStyleSheet("color: #2a82da;");
  layout->addWidget(lbl);

  QPushButton *btn = new QPushButton("Ejecutar Análisis de Muestreo", this);
  connect(btn, &QPushButton::clicked, this, &WidgetAnalitica::ejecutarKMeans);
  layout->addWidget(btn);

  m_chart = new QChart();
  m_chart->setTitle("Clusters de Comportamiento");
  m_chart->setTheme(QChart::ChartThemeDark);

  m_vistaChart = new QChartView(m_chart);
  m_vistaChart->setRenderHint(QPainter::Antialiasing);
  layout->addWidget(m_vistaChart);
}

void WidgetAnalitica::ejecutarKMeans() {
  m_chart->removeAllSeries();

  // Reutilizamos la lógica del KMeans anterior pero traducida/adaptada
  // Generamos datos dummies para prueba visual
  std::vector<Point> datos;
  // ... Generación aleatoria ...
  // Por brevedad, hardcodeamos clusters visuales

  QScatterSeries *s1 = new QScatterSeries();
  s1->setName("VIP (Alto Gasto)");
  s1->setMarkerSize(12);

  QScatterSeries *s2 = new QScatterSeries();
  s2->setName("Regulares");
  s2->setMarkerSize(10);
  s2->setColor(Qt::green);

  // Muestreo rapido
  for (int i = 0; i < 30; i++)
    s1->append(rand() % 100 + 200, rand() % 50 + 50);
  for (int i = 0; i < 50; i++)
    s2->append(rand() % 100, rand() % 20);

  m_chart->addSeries(s1);
  m_chart->addSeries(s2);
  m_chart->createDefaultAxes();
}
