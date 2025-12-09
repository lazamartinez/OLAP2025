#pragma once

#include <QMainWindow>
#include <QTabWidget>

// Forward declarations
namespace SistemaOLAP {
namespace Nucleo {
class MotorETL;
class CuboDatos;
} // namespace Nucleo
} // namespace SistemaOLAP

class VentanaPrincipal : public QMainWindow {
  Q_OBJECT

public:
  VentanaPrincipal(QWidget *parent = nullptr);
  ~VentanaPrincipal();

private:
  void configurarUi();

  QTabWidget *m_tabWidget;

  // Componentes Nucleo
  SistemaOLAP::Nucleo::MotorETL *m_motorEtl;
  SistemaOLAP::Nucleo::CuboDatos *m_cuboDatos;
};
