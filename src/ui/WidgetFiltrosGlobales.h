#pragma once

#include <QComboBox>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class WidgetFiltrosGlobales : public QWidget {
  Q_OBJECT
public:
  explicit WidgetFiltrosGlobales(QWidget *parent = nullptr);

  QString getRegionSeleccionada() const;
  QDate getFechaInicio() const;
  QDate getFechaFin() const;

signals:
  void filtrosActualizados(const QString &region, const QDate &inicio,
                           const QDate &fin);

private slots:
  void aplicarFiltros();
  void limpiarFiltros();

private:
  QComboBox *m_comboRegion;
  QDateEdit *m_dateInicio;
  QDateEdit *m_dateFin;
  QPushButton *m_btnAplicar;
  QPushButton *m_btnLimpiar;
};
