#pragma once

#include <QAction>
#include <QMainWindow>
#include <QTabWidget>

class WidgetFiltrosGlobales;

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

private slots:
  void toggleTema();
  void aplicarFiltrosGlobales(const QString &region, const QDate &inicio,
                              const QDate &fin);

private:
  void configurarUi();
  void aplicarTema(bool oscuro);

  QTabWidget *m_tabWidget;
  WidgetFiltrosGlobales *m_filtrosGlobales;
  QAction *m_actionTema;
  bool m_temaOscuro;

  // Componentes Nucleo
  SistemaOLAP::Nucleo::MotorETL *m_motorEtl;
  SistemaOLAP::Nucleo::CuboDatos *m_cuboDatos;
};
