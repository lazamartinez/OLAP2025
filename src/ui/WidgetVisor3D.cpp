#include "WidgetVisor3D.h"
#include <QDebug>
#include <QQmlContext>
#include <QQuickItem>
#include <QVBoxLayout>


WidgetVisor3D::WidgetVisor3D(QWidget *parent)
    : QWidget(parent), m_cuboRef(nullptr) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_quickWidget = new QQuickWidget(this);
  m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
  m_quickWidget->setSource(QUrl::fromLocalFile("src/ui/Visor3D.qml"));

  if (m_quickWidget->status() == QQuickWidget::Error) {
    qCritical() << "Error cargando QML:" << m_quickWidget->errors();
  }

  layout->addWidget(m_quickWidget);
}

void WidgetVisor3D::cargarDatos(SistemaOLAP::Nucleo::CuboDatos *cubo) {
  m_cuboRef = cubo;
  if (!m_cuboRef)
    return;

  // Obtener datos del cubo
  auto voxels = m_cuboRef->obtenerSlice("X", "Y", "Z");

  // Convertir a QVariantList para QML
  QVariantList listaVoxels;
  for (const auto &v : voxels) {
    QVariantMap map;
    map["x"] = v.x;
    map["y"] = v.y;
    map["z"] = v.z;
    map["val"] = v.valor;
    listaVoxels.append(map);
  }

  QObject *rootObject = m_quickWidget->rootObject();
  if (rootObject) {
    QMetaObject::invokeMethod(
        rootObject, "setData",
        Q_ARG(QVariant, QVariant::fromValue(listaVoxels)));
  } else {
    qWarning() << "Root Object de QML no listo aun.";
  }
}

void WidgetVisor3D::onCuboActualizado() { cargarDatos(m_cuboRef); }
