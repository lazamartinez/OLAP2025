#include "WidgetVisor3D.h"
#include <QCoreApplication>
#include <QDebug>
#include <QQmlContext>
#include <QQuickItem>
#include <QTimer>
#include <QVBoxLayout>


WidgetVisor3D::WidgetVisor3D(QWidget *parent)
    : QWidget(parent), m_cuboRef(nullptr) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_quickWidget = new QQuickWidget(this);
  m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

  // Usar ruta absoluta basada en el ejecutable
  QString qmlPath =
      QCoreApplication::applicationDirPath() + "/../src/ui/Visor3D.qml";

  qDebug() << "Cargando QML desde:" << qmlPath;
  m_quickWidget->setSource(QUrl::fromLocalFile(qmlPath));

  if (m_quickWidget->status() == QQuickWidget::Error) {
    qCritical() << "Error cargando QML:" << m_quickWidget->errors();
    qCritical() << "Ruta intentada:" << qmlPath;
  } else {
    qDebug() << "QML cargado exitosamente";

    // Cargar datos de prueba después de un pequeño delay
    QTimer::singleShot(500, this, [this]() { cargarDatosPrueba(); });
  }

  layout->addWidget(m_quickWidget);
}

void WidgetVisor3D::cargarDatosPrueba() {
  qDebug() << "Cargando datos de prueba para Visor 3D...";

  // Generar grid 5x5x5 de voxels
  QVariantList listaVoxels;
  for (int x = -2; x <= 2; ++x) {
    for (int y = 0; y <= 4; ++y) {
      for (int z = -2; z <= 2; ++z) {
        QVariantMap voxel;
        voxel["x"] = x * 20.0;
        voxel["y"] = y * 20.0;
        voxel["z"] = z * 20.0;
        voxel["val"] = (x + y + z) * 1000.0; // Valor para colorear
        listaVoxels.append(voxel);
      }
    }
  }

  qDebug() << "Generados" << listaVoxels.size() << "voxels de prueba";

  QObject *rootObject = m_quickWidget->rootObject();
  if (rootObject) {
    QMetaObject::invokeMethod(
        rootObject, "setData",
        Q_ARG(QVariant, QVariant::fromValue(listaVoxels)));
    qDebug() << "Datos enviados a QML";
  } else {
    qWarning() << "Root Object de QML no disponible";
  }
}

void WidgetVisor3D::cargarDatos(SistemaOLAP::Nucleo::CuboDatos *cubo) {
  m_cuboRef = cubo;
  if (!m_cuboRef) {
    qWarning() << "CuboDatos es nullptr, usando datos de prueba";
    cargarDatosPrueba();
    return;
  }

  // Obtener datos del cubo
  auto voxels = m_cuboRef->obtenerSlice("X", "Y", "Z");

  if (voxels.empty()) {
    qWarning() << "Cubo vacío, usando datos de prueba";
    cargarDatosPrueba();
    return;
  }

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
    qDebug() << "Cargados" << listaVoxels.size() << "voxels del cubo";
  } else {
    qWarning() << "Root Object de QML no listo aun.";
  }
}

void WidgetVisor3D::onCuboActualizado() { cargarDatos(m_cuboRef); }
