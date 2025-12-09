#pragma once

#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>


class WidgetHistorial : public QWidget {
  Q_OBJECT
public:
  explicit WidgetHistorial(QWidget *parent = nullptr);

  void agregarQuery(const QString &query, const QString &tipo = "SQL");
  void limpiarHistorial();

signals:
  void querySeleccionada(QString query);

private slots:
  void onItemDoubleClick(QListWidgetItem *item);

private:
  QListWidget *m_listaHistorial;
  QPushButton *m_btnLimpiar;

  void cargarHistorial();
  void guardarHistorial();

  static const int MAX_HISTORIAL = 10;
};
