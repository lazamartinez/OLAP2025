#include "WidgetAnalitica.h"
#include "../analytics/AnaliticaML.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <cstdlib>

WidgetAnalitica::WidgetAnalitica(QWidget *parent) : QWidget(parent) {
  configurarUi();
}

void WidgetAnalitica::configurarUi() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setSpacing(20);
  layout->setContentsMargins(40, 40, 40, 40);

  // Título
  QLabel *titulo = new QLabel("Analítica Avanzada & Machine Learning");
  titulo->setStyleSheet(
      "font-size: 24px; font-weight: bold; color: #333; border-bottom: 2px "
      "solid #2a82da; padding-bottom: 10px;");
  titulo->setToolTip("<b>Analytics ML</b><br>Predicción, detección de "
                     "anomalías y clustering avanzado.");
  layout->addWidget(titulo);

  // Botones ML
  QHBoxLayout *mlButtons = new QHBoxLayout();

  QPushButton *btnPrediccion =
      new QPushButton("📈 Predicción Ventas (30 días)", this);
  btnPrediccion->setStyleSheet(R"(
      QPushButton {
          background-color: #27AE60;
          color: white;
          border: none;
          padding: 12px 20px;
          border-radius: 6px;
          font-weight: 600;
      }
      QPushButton:hover { background-color: #229954; }
  )");
  btnPrediccion->setToolTip("<b>Regresión Lineal</b><br>Predice ventas futuras "
                            "basándose en tendencia histórica.");
  connect(btnPrediccion, &QPushButton::clicked, this,
          &WidgetAnalitica::ejecutarPrediccion);
  mlButtons->addWidget(btnPrediccion);

  QPushButton *btnAnomalias = new QPushButton("🔍 Detectar Anomalías", this);
  btnAnomalias->setStyleSheet(R"(
      QPushButton {
          background-color: #E74C3C;
          color: white;
          border: none;
          padding: 12px 20px;
          border-radius: 6px;
          font-weight: 600;
      }
      QPushButton:hover { background-color: #C0392B; }
  )");
  btnAnomalias->setToolTip("<b>Z-Score Detection</b><br>Identifica valores "
                           "atípicos usando desviación estándar.");
  connect(btnAnomalias, &QPushButton::clicked, this,
          &WidgetAnalitica::detectarAnomalias);
  mlButtons->addWidget(btnAnomalias);

  layout->addLayout(mlButtons);

  // Button
  QPushButton *btn = new QPushButton("Ejecutar Análisis K-Means", this);
  btn->setCursor(Qt::PointingHandCursor);
  btn->setStyleSheet("QPushButton { background-color: #2a82da; color: white; "
                     "border: none; padding: 12px; font-size: 14px; "
                     "border-radius: 6px; font-weight: bold; } "
                     "QPushButton:hover { background-color: #1c68b3; }");
  connect(btn, &QPushButton::clicked, this,
          &WidgetAnalitica::ejecutarClustering);
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

void WidgetAnalitica::ejecutarClustering() {
  m_chart->removeAllSeries();

  // Generamos datos dummies para prueba visual de clustering

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

void WidgetAnalitica::ejecutarPrediccion() {
  // Datos de prueba: ventas de últimos 30 días
  QVector<QPointF> datos;
  for (int i = 0; i < 30; ++i) {
    datos.append(QPointF(i, 10000 + i * 500 + (rand() % 2000)));
  }

  auto resultado = AnaliticaML::regresionLineal(datos, 30);

  QString mensaje = QString("📊 <b>Predicción Completada</b><br><br>"
                            "Pendiente: %1<br>"
                            "Intercepto: %2<br>"
                            "R² (ajuste): %3<br><br>"
                            "Predicción día 60: $%4")
                        .arg(resultado.pendiente, 0, 'f', 2)
                        .arg(resultado.intercepto, 0, 'f', 2)
                        .arg(resultado.r2, 0, 'f', 4)
                        .arg(resultado.predicciones.last(), 0, 'f', 0);

  QMessageBox::information(this, "Predicción ML", mensaje);
}

void WidgetAnalitica::detectarAnomalias() {
  // Datos de prueba con algunas anomalías
  QVector<double> ventas;
  for (int i = 0; i < 100; ++i) {
    double valor = 10000 + (rand() % 2000);
    if (i == 25 || i == 67)
      valor *= 5; // Anomalías artificiales
    ventas.append(valor);
  }

  auto resultado = AnaliticaML::detectarAnomalias(ventas, 3.0);

  QString mensaje = QString("🔍 <b>Detección de Anomalías</b><br><br>"
                            "Media: $%1<br>"
                            "Desv. Estándar: $%2<br>"
                            "Anomalías detectadas: %3<br><br>"
                            "Índices: %4")
                        .arg(resultado.media, 0, 'f', 0)
                        .arg(resultado.desviacionEstandar, 0, 'f', 0)
                        .arg(resultado.indices.size())
                        .arg(resultado.indices.size() > 0
                                 ? QString::number(resultado.indices.first())
                                 : "Ninguno");

  QMessageBox::warning(this, "Anomalías Detectadas", mensaje);
}
