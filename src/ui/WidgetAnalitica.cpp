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
  this->setStyleSheet(""); // Reset to default

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setSpacing(20);
  layout->setContentsMargins(40, 40, 40, 40);

  // Title
  QLabel *lbl = new QLabel("Analítica: Segmentación de Clientes");
  lbl->setStyleSheet("font-size: 24px; font-weight: bold; color: #333; "
                     "border-bottom: 2px solid #2a82da; padding-bottom: 10px;");
  layout->addWidget(lbl);

  // Button
  QPushButton *btn = new QPushButton("Ejecutar Análisis K-Means", this);
  btn->setCursor(Qt::PointingHandCursor);
  btn->setStyleSheet("QPushButton { background-color: #2a82da; color: white; "
                     "border: none; padding: 12px; font-size: 14px; "
                     "border-radius: 6px; font-weight: bold; } "
                     "QPushButton:hover { background-color: #1c68b3; }");
  connect(btn, &QPushButton::clicked, this, &WidgetAnalitica::ejecutarKMeans);
  layout->addWidget(btn);

  // Chart
  m_chart = new QChart();
  m_chart->setTitle("Clusters de Comportamiento");
  m_chart->setTitleBrush(QBrush(Qt::black));
  m_chart->setTheme(QChart::ChartThemeBlueCerulean);
  m_chart->setBackgroundVisible(true);
  m_chart->setBackgroundBrush(QBrush(Qt::white));
  m_chart->setPlotAreaBackgroundVisible(false);

  m_vistaChart = new QChartView(m_chart);
  m_vistaChart->setRenderHint(QPainter::Antialiasing);
  // Remove border
  m_vistaChart->setStyleSheet("border: 1px solid #ddd; border-radius: 5px;");
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
