#include "WidgetTablero.h"
#include "../core/GestorBaseDatos.h"
#include <QDateTime>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

WidgetTablero::WidgetTablero(QWidget *parent) : QWidget(parent) {
  configurarUi();
}

void WidgetTablero::configurarUi() {
  this->setStyleSheet(""); // Reset to default/global
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setSpacing(20);
  layout->setContentsMargins(40, 40, 40, 40);

  // Header
  QHBoxLayout *headerLayout = new QHBoxLayout();
  QLabel *titulo = new QLabel("Resumen Ejecutivo de Ventas");
  titulo->setStyleSheet(
      "font-size: 24px; font-weight: bold; color: #333; border-bottom: 2px "
      "solid #2a82da; padding-bottom: 10px;");
  headerLayout->addWidget(titulo);

  QPushButton *btnActualizar = new QPushButton("Actualizar Datos", this);
  btnActualizar->setCursor(Qt::PointingHandCursor);
  btnActualizar->setFixedWidth(150);
  btnActualizar->setStyleSheet(
      "QPushButton { background-color: #2a82da; color: white; border: none; "
      "padding: 8px; border-radius: 4px; font-weight: 600; } "
      "QPushButton:hover { background-color: #1c68b3; }");
  connect(btnActualizar, &QPushButton::clicked, this,
          &WidgetTablero::actualizarDatos);
  headerLayout->addWidget(btnActualizar, 0, Qt::AlignRight);

  layout->addLayout(headerLayout);

  // Gráfico
  m_grafico = new QChart();
  m_grafico->setTitle("Tendencia Mensual");
  m_grafico->setTitleBrush(QBrush(Qt::black));
  m_grafico->setTheme(QChart::ChartThemeBlueCerulean); // Light Theme
  m_grafico->setBackgroundVisible(true);
  m_grafico->setBackgroundBrush(QBrush(Qt::white));
  m_grafico->setPlotAreaBackgroundVisible(false);

  m_serieVentas = new QLineSeries();
  m_serieVentas->setName("Ingresos Totales ($)");
  m_grafico->addSeries(m_serieVentas);

  m_vistaGrafico = new QChartView(m_grafico);
  m_vistaGrafico->setRenderHint(QPainter::Antialiasing);
  m_vistaGrafico->setMinimumHeight(350);
  m_vistaGrafico->setStyleSheet(
      "border: 1px solid #ddd; border-radius: 5px; background: white;");
  layout->addWidget(m_vistaGrafico);

  // Tabla Pivot
  QLabel *lblPivot = new QLabel("Desempeño por Categoría");
  lblPivot->setStyleSheet(
      "margin-top: 10px; color: #333; font-weight: bold; font-size: 16px;");
  layout->addWidget(lblPivot);

  m_tablaPivot = new QTableView(this);
  m_modeloPivot = new QStandardItemModel(this);
  m_tablaPivot->setModel(m_modeloPivot);
  m_tablaPivot->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_tablaPivot->verticalHeader()->setVisible(false);
  m_tablaPivot->setAlternatingRowColors(true);

  // Table Styling (Light)
  m_tablaPivot->setStyleSheet(
      "QTableView { background-color: #ffffff; gridline-color: #e0e0e0; color: "
      "#333; border: 1px solid #ddd; }"
      "QHeaderView::section { background-color: #f5f5f5; color: #333; border: "
      "nne; padding: 5px; font-weight: bold; border-bottom: 1px solid #ddd; }"
      "QTableView::item:selected { background-color: #e6f7ff; color: #333; }"
      "QTableView::item { padding: 5px; }");

  layout->addWidget(m_tablaPivot);

  actualizarDatos(); // Carga inicial
}

void WidgetTablero::actualizarDatos() {
  m_serieVentas->clear();
  auto &db = GestorBaseDatos::instancia(); // Se asume conectado en main o login

  // 1. Gráfico: Ventas por Mes
  QString sqlGrafico = R"(
        SELECT DATE_TRUNC('month', timestamp) as mes, SUM(total) 
        FROM fact_ventas 
        GROUP BY mes 
        ORDER BY mes ASC LIMIT 12
    )";

  QSqlQuery qGrafico = db.consultar(sqlGrafico);
  int count = 0;
  while (qGrafico.next()) {
    // Simplemente usaremos index incremental xq QDateTimeAxis es complejo de
    // configurar rapido
    double total = qGrafico.value(1).toDouble();
    m_serieVentas->append(count++, total);
  }

  if (count == 0) {
    // Datos Mock si DB vacia
    *m_serieVentas << QPointF(0, 1000) << QPointF(1, 2500) << QPointF(2, 2100)
                   << QPointF(3, 3200);
  }

  m_grafico->createDefaultAxes();

  // 2. Tabla: Ventas por Categoria
  m_modeloPivot->clear();
  m_modeloPivot->setHorizontalHeaderLabels(
      {"Categoría", "Región", "Total ($)", "Transacciones"});

  // Usamos la vista materializada si existe, sino query directa
  QString sqlTabla =
      "SELECT * FROM ventas_por_categoria ORDER BY total_ventas DESC LIMIT 100";
  QSqlQuery qTabla = db.consultar(sqlTabla);

  int row = 0;
  while (qTabla.next()) {
    QString cat = qTabla.value(0).toString();
    QString reg = qTabla.value(1).toString();
    double tot = qTabla.value(2).toDouble();
    int trans = qTabla.value(3).toInt();

    m_modeloPivot->setItem(row, 0, new QStandardItem(cat));
    m_modeloPivot->setItem(row, 1, new QStandardItem(reg));
    m_modeloPivot->setItem(row, 2,
                           new QStandardItem(QString::number(tot, 'f', 2)));
    m_modeloPivot->setItem(row, 3, new QStandardItem(QString::number(trans)));
    row++;
  }

  if (row == 0) {
    m_modeloPivot->setItem(0, 0, new QStandardItem("Sin Datos"));
  }
}
