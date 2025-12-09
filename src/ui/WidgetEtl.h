#pragma once

#include <QProgressBar>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>


class WidgetEtl : public QWidget {
  Q_OBJECT

public:
  explicit WidgetEtl(QWidget *parent = nullptr);

public slots:
  void conectarBD();
  void correrSimulacion();
  void correrEtl();

private:
  void configurarUi();
  void log(const QString &msj);

  QPushButton *m_btnConectar;
  QPushButton *m_btnSimular;
  QPushButton *m_btnCargar;
  QTextEdit *m_log;
  QProgressBar *m_progreso;
};
