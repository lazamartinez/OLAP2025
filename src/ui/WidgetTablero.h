#pragma once

#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


class WidgetTablero : public QWidget {
  Q_OBJECT

public:
  explicit WidgetTablero(QWidget *parent = nullptr);

public slots:
  void actualizarDatos();

private:
  void configurarUi();

  QChartView *m_vistaGrafico;
  QChart *m_grafico;
  QLineSeries *m_serieVentas;

  QTableView *m_tablaPivot;
  QStandardItemModel *m_modeloPivot;
};
