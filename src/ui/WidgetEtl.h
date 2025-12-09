#pragma once

#include <QProgressBar>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

// Forward Decl
namespace SistemaOLAP {
namespace Nucleo {
class MotorETL;
}
} // namespace SistemaOLAP

class WidgetEtl : public QWidget {
  Q_OBJECT

public:
  explicit WidgetEtl(QWidget *parent = nullptr);
  void setMotor(SistemaOLAP::Nucleo::MotorETL *motor) { m_motorRef = motor; }

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

  // Nuevo Motor
  SistemaOLAP::Nucleo::MotorETL *m_motorRef = nullptr;
};
