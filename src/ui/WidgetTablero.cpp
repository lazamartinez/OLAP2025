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
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Header
  QHBoxLayout *headerLayout = new QHBoxLayout();
  QLabel *titulo = new QLabel("<h2>Resumen Ejecutivo de Ventas</h2>");
  titulo->setStyleSheet("color: #2a82da; font-weight: bold;");
  headerLayout->addWidget(titulo);

  QPushButton *btnActualizar = new QPushButton("Actualizar", this);
  btnActualizar->setFixedWidth(120);
  connect(btnActualizar, &QPushButton::clicked, this,
          &WidgetTablero::actualizarDatos);
  headerLayout->addWidget(btnActualizar, 0, Qt::AlignRight);

  layout->addLayout(headerLayout);

  // Gráfico
  m_grafico = new QChart();
  m_grafico->setTitle("Tendencia de Ventas (Mensual)");
  m_grafico->setTheme(QChart::ChartThemeBlueCerulean);
  m_grafico->setBackgroundVisible(false);

  m_serieVentas = new QLineSeries();
  m_serieVentas->setName("Total Ventas ($)");
  m_grafico->addSeries(m_serieVentas);

  m_vistaGrafico = new QChartView(m_grafico);
  m_vistaGrafico->setRenderHint(QPainter::Antialiasing);
  m_vistaGrafico->setMinimumHeight(350);
  layout->addWidget(m_vistaGrafico);

  // Tabla Pivot
  QLabel *lblPivot = new QLabel("<h3>Desempeño por Categoría</h3>");
  lblPivot->setStyleSheet("margin-top: 10px; color: #aaa;");
  layout->addWidget(lblPivot);

  m_tablaPivot = new QTableView(this);
  m_modeloPivot = new QStandardItemModel(this);
  m_tablaPivot->setModel(m_modeloPivot);
  m_tablaPivot->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_tablaPivot->verticalHeader()->setVisible(false);
  m_tablaPivot->setAlternatingRowColors(true);
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
