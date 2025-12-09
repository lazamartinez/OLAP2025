#pragma once

#include "WidgetVisor3D.h"
#include "WidgetVisor3DSimple.h"
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>


class WidgetVisor3DDual : public QWidget {
  Q_OBJECT
public:
  explicit WidgetVisor3DDual(QWidget *parent = nullptr);

private:
  QStackedWidget *m_stack;
  WidgetVisor3D *m_visorQML;
  WidgetVisor3DSimple *m_visorSimple;
  QPushButton *m_btnToggle;
  bool m_usandoQML = true;

  void toggleViewer();
};
