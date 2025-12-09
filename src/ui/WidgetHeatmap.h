#pragma once

#include <QCalendarWidget>
#include <QDate>
#include <QLabel>
#include <QMap>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>


class WidgetHeatmap : public QWidget {
  Q_OBJECT
public:
  explicit WidgetHeatmap(QWidget *parent = nullptr);

  void setDatos(const QMap<QDate, double> &datos);
  void setRango(double min, double max);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  QCalendarWidget *m_calendar;
  QMap<QDate, double> m_datos;
  double m_min = 0;
  double m_max = 100;

  QColor obtenerColor(double valor);
};
