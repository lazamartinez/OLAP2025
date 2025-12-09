#ifndef WIDGETVISOR3D_H
#define WIDGETVISOR3D_H

#include "SistemaOLAP/Nucleo/CuboDatos.h"
#include <QWidget>
#include <QtQuickWidgets/QQuickWidget>

class WidgetVisor3D : public QWidget {
  Q_OBJECT
public:
  explicit WidgetVisor3D(QWidget *parent = nullptr);
  void cargarDatos(SistemaOLAP::Nucleo::CuboDatos *cubo);

private slots:
  void onCuboActualizado();

private:
  QQuickWidget *m_quickWidget;
  SistemaOLAP::Nucleo::CuboDatos *m_cuboRef;
};

#endif // WIDGETVISOR3D_H
