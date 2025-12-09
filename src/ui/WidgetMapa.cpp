#include "WidgetMapa.h"
#include "DialogoSucursal.h"
#include <QCoreApplication>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QRandomGenerator>
#include <QSettings>
#include <QTimer>
#include <QVBoxLayout>
#include <cmath>

// Helper Methods
WidgetMapa::Point3D latLonTo3DMAP(double lat, double lon) {
  double phi = (90.0 - lat) * (M_PI / 180.0);
  double theta = (lon + 180.0) * (M_PI / 180.0);
  double x = -(std::sin(phi) * std::cos(theta));
  double z = (std::sin(phi) * std::sin(theta));
  double y = (std::cos(phi));
  return {x, y, z};
}

WidgetMapa::Point3D rotateYMAP(WidgetMapa::Point3D p, double angleDeg) {
  double rad = angleDeg * (M_PI / 180.0);
  double cosA = std::cos(rad);
  double sinA = std::sin(rad);
  return {p.x * cosA - p.z * sinA, p.y, p.x * sinA + p.z * cosA};
}

// Mercator projection helpers
double rad2deg(double rad) { return rad * 180.0 / M_PI; }
double deg2rad(double deg) { return deg * M_PI / 180.0; }

WidgetMapa::WidgetMapa(QWidget *parent) : QGraphicsView(parent) {
  m_scene = new QGraphicsScene(this);
  setScene(m_scene);

  // --- LIGHT THEME MAP CONFIGURATION ---
  setBackgroundBrush(QColor(245, 247, 250)); // Off-White Background
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::NoDrag);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  // Cargar textura de la Tierra
  QString texturePath = QCoreApplication::applicationDirPath() +
                        "/../src/ui/map_resources/earth_texture.png";
  m_earthTexture = QPixmap(texturePath);

  QString textureMinPath = QCoreApplication::applicationDirPath() +
                           "/../src/ui/map_resources/earth_texture_minimal.png";
  m_earthTextureMinimal = QPixmap(textureMinPath);

  if (m_earthTexture.isNull()) {
    qWarning() << "No se pudo cargar textura de la Tierra desde:"
               << texturePath;
  } else {
    qDebug() << "Textura de la Tierra cargada exitosamente";
  }

  if (m_earthTextureMinimal.isNull()) {
    qWarning() << "No se pudo cargar textura minimalista desde:"
               << textureMinPath;
  } else {
    qDebug() << "Textura minimalista cargada exitosamente";
  }

  // Cargar preferencia de estilo
  QSettings settings("OLAP2025", "Sistema");
  m_estiloMinimalista = settings.value("mapa/minimalista", false).toBool();

  // 2. Pre-calcular puntos de la esfera
  int samples = 3500;
  double phi = M_PI * (3.0 - std::sqrt(5.0));

  for (int i = 0; i < samples; i++) {
    double y = 1 - (i / (double)(samples - 1)) * 2;
    double radius = std::sqrt(1 - y * y);
    double theta = phi * i;

    double x = std::cos(theta) * radius;
    double z = std::sin(theta) * radius;

    m_spherePoints.append({x, y, z});
  }

  // Inicializar Ubicaciones
  m_ubicaciones["Buenos Aires"] = {-34.6037, -58.3816};
  m_ubicaciones["Cordoba"] = {-31.4201, -64.1888};
  m_ubicaciones["New York"] = {40.7128, -74.0060};
  m_ubicaciones["London"] = {51.5074, -0.1278};
  m_ubicaciones["Tokyo"] = {35.6762, 139.6503};
  m_ubicaciones["Moscow"] = {55.7558, 37.6173};

  // 3. Generar Estrellas (REMOVED FOR CLEAN LOOK)

  m_rotationAngle = 0.0;
  m_timerAnimacion = new QTimer(this);
  connect(m_timerAnimacion, &QTimer::timeout, this, [this]() {
    if (!m_is2DMode && std::abs(m_rotationSpeed) > 0.01) {
      m_rotationAngle += m_rotationSpeed;
      if (m_rotationAngle >= 360.0)
        m_rotationAngle -= 360.0;
      dibujarMapaBase();
    }
  });
  m_timerAnimacion->start(33);

  setupUI();
}

void WidgetMapa::setupUI() {
  QPushButton *btnView = new QPushButton("Vista 2D / 3D", this);
  btnView->setStyleSheet(R"(
    QPushButton {
        background-color: #FFFFFF;
        border: 1px solid #BDC3C7;
        color: #2C3E50;
        border-radius: 4px;
        font-weight: 600;
    }
    QPushButton:hover {
        border-color: #3498DB;
        color: #3498DB;
    }
  )");
  btnView->setGeometry(20, 20, 100, 35);
  connect(btnView, &QPushButton::clicked, this, [this]() {
    m_is2DMode = !m_is2DMode;
    dibujarMapaBase();
  });

  m_listTransactions = new QListWidget(this);
  // Clean White List Style
  m_listTransactions->setStyleSheet(R"(
      QListWidget {
          background-color: rgba(255, 255, 255, 0.9);
          border: 1px solid #DCE1E7;
          color: #2C3E50;
          font-family: 'Segoe UI', sans-serif;
          font-size: 11px;
          border-radius: 6px;
      }
      QListWidget::item {
          padding: 4px;
          border-bottom: 1px solid #F0F0F0;
      }
  )");
  m_listTransactions->setGeometry(width() - 220, 20, 200, 200);
}

void WidgetMapa::logTransaction(const QString &msg, double amount) {
  m_listTransactions->addItem(
      QString("[%1] %2: $%3")
          .arg(QTime::currentTime().toString("hh:mm:ss"))
          .arg(msg)
          .arg(amount, 0, 'f', 2));
  m_listTransactions->scrollToBottom();
}

WidgetMapa::Point3D WidgetMapa::rotateYMAP(const Point3D &p, double angle) {
  double cosA = std::cos(angle);
  double sinA = std::sin(angle);
  return {p.x * cosA - p.z * sinA, p.y, p.x * sinA + p.z * cosA};
}

void WidgetMapa::toggleEstiloMapa() {
  m_estiloMinimalista = !m_estiloMinimalista;

  QSettings settings("OLAP2025", "Sistema");
  settings.setValue("mapa/minimalista", m_estiloMinimalista);

  dibujarMapaBase();
}

void WidgetMapa::agregarSucursal() {
  DialogoSucursal dlg(this);
  if (dlg.exec() == QDialog::Accepted) {
    m_ubicaciones[dlg.getNombre()] = {dlg.getLat(), dlg.getLon()};
  }
}

void WidgetMapa::resizeEvent(QResizeEvent *event) {
  QGraphicsView::resizeEvent(event);
  m_scene->setSceneRect(-width() / 2, -height() / 2, width(), height());
  if (m_listTransactions)
    m_listTransactions->setGeometry(width() - 220, 20, 200, 200);
}

// Map 3D point to Texture UV
QPointF sphereToTextureUVMAP(double x, double y, double z) {
  double lat = std::asin(y);
  double lon = std::atan2(z, -x);
  double u = (lon + M_PI) / (2.0 * M_PI);
  double v = 0.5 - (lat / M_PI);
  return QPointF(u, v);
}

QPointF WidgetMapa::coordenadasAProyeccion(double lat, double lon) {
  if (!m_is2DMode) {
    auto p = latLonTo3DMAP(lat, lon);
    p = rotateYMAP(p, m_rotationAngle);
    if (p.z < -0.2)
      return QPointF(-9999, -9999);
    return QPointF(p.x * 250, -p.y * 250);
  } else {
    return QPointF(lon * 3, -lat * 3);
  }
}

void WidgetMapa::dibujarMapaBase() {
  m_scene->clear();

  // No Stars (Clean Look)

  // Globe Dots
  if (!m_is2DMode) {
    for (const auto &origP : m_spherePoints) {
      auto p = rotateYMAP(origP, m_rotationAngle);
      if (p.z < 0.1)
        continue;

      double sx = p.x * 250;
      double sy = -p.y * 250;

      // Professional Blue Palette
      QColor col(41, 128, 185, 100); // Base Blue
      /* Texture logic skipped for clean aesthetic or use light variation */

      m_scene->addEllipse(sx, sy, 2, 2, Qt::NoPen, QBrush(col));
    }
  }

  // Cities
  QFont f("Segoe UI", 8, QFont::Bold);
  for (auto it = m_ubicaciones.begin(); it != m_ubicaciones.end(); ++it) {
    QPointF p = coordenadasAProyeccion(it.value().lat, it.value().lon);
    if (p.x() == -9999)
      continue;

    m_scene->addEllipse(p.x() - 4, p.y() - 4, 8, 8, Qt::NoPen,
                        QBrush(QColor("#E74C3C"))); // Red accent for cities
    auto t = m_scene->addText(it.key(), f);
    t->setDefaultTextColor(QColor("#2C3E50")); // Dark text
    t->setPos(p.x() + 6, p.y() - 12);
  }
}

void WidgetMapa::procesarVenta(const QString &origen, const QString &destino,
                               double monto) {
  logTransaction(origen + " -> " + destino, monto);
}
