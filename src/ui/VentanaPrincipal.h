#pragma once

#include <QMainWindow>
#include <QTabWidget>

class VentanaPrincipal : public QMainWindow {
  Q_OBJECT

public:
  VentanaPrincipal(QWidget *parent = nullptr);
  ~VentanaPrincipal();

private:
  void configurarUi();

  QTabWidget *m_tabWidget;
};
