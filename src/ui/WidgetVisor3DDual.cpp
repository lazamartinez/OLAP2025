#include "WidgetVisor3DDual.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QQuickWidget>
#include <QTimer>
#include <QVBoxLayout>


WidgetVisor3DDual::WidgetVisor3DDual(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  // Header con toggle
  QWidget *header = new QWidget(this);
  header->setStyleSheet(
      "background-color: #FFFFFF; border-bottom: 2px solid #3498DB;");
  header->setFixedHeight(60);

  QHBoxLayout *headerLayout = new QHBoxLayout(header);
  headerLayout->setContentsMargins(20, 10, 20, 10);

  QLabel *titulo = new QLabel("🧊 Visor 3D OLAP", header);
  titulo->setStyleSheet("font-size: 18px; font-weight: bold; color: #2C3E50;");
  headerLayout->addWidget(titulo);

  headerLayout->addStretch();

  m_btnToggle = new QPushButton("🔄 Cambiar a Versión Simple", header);
  m_btnToggle->setStyleSheet(R"(
        QPushButton {
            background-color: #3498DB;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #2980B9;
        }
    )");
  m_btnToggle->setToolTip("<b>Toggle Visor</b><br>QML: Avanzado, suave, 3D "
                          "real<br>Simple: Rápido, ligero, proyección 2D");
  connect(m_btnToggle, &QPushButton::clicked, this,
          &WidgetVisor3DDual::toggleViewer);
  headerLayout->addWidget(m_btnToggle);

  layout->addWidget(header);

  // Stack con ambos visores
  m_stack = new QStackedWidget(this);

  // Página 0: QML (avanzado)
  m_visorQML = new WidgetVisor3D(this);
  m_stack->addWidget(m_visorQML);

  // Página 1: Simple (QPainter)
  m_visorSimple = new WidgetVisor3DSimple(this);
  m_stack->addWidget(m_visorSimple);

  // Auto-detectar si QML funciona y hacer fallback
  QTimer::singleShot(200, this, [this, titulo]() {
    // Verificar si QML cargó correctamente
    if (m_visorQML) {
      auto *quickWidget = m_visorQML->findChild<QQuickWidget *>();
      if (quickWidget && quickWidget->status() == QQuickWidget::Error) {
        qWarning() << "⚠️ QtQuick3D no disponible, usando versión Simple "
                      "automáticamente";
        m_stack->setCurrentIndex(1);
        m_usandoQML = false;
        m_btnToggle->setText("⚠️ QML No Disponible");
        m_btnToggle->setEnabled(false);
        m_btnToggle->setStyleSheet(R"(
                    QPushButton {
                        background-color: #95A5A6;
                        color: white;
                        border: none;
                        padding: 8px 16px;
                        border-radius: 4px;
                    }
                )");
        m_btnToggle->setToolTip(
            "<b>QtQuick3D No Instalado</b><br>Usando versión Simple "
            "(QPainter)<br>Para activar QML: instalar Qt Quick 3D");
        titulo->setText("🧊 Visor 3D OLAP (Modo Simple)");
        titulo->setStyleSheet(
            "font-size: 18px; font-weight: bold; color: #E67E22;");
      }
    }
  });

  m_stack->setCurrentIndex(0); // Intentar con QML primero
  layout->addWidget(m_stack);
}

void WidgetVisor3DDual::toggleViewer() {
  m_usandoQML = !m_usandoQML;

  if (m_usandoQML) {
    m_stack->setCurrentIndex(0);
    m_btnToggle->setText("🔄 Cambiar a Versión Simple");
  } else {
    m_stack->setCurrentIndex(1);
    m_btnToggle->setText("🔄 Cambiar a Versión QML");
  }
}
