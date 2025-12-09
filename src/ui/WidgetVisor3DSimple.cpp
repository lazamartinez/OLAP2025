#include "WidgetVisor3DSimple.h"
#include <QPainter>
#include <QWheelEvent>
#include <cmath>

WidgetVisor3DSimple::WidgetVisor3DSimple(QWidget *parent) : QWidget(parent) {
  setMinimumSize(400, 400);

  // Generar voxels de prueba
  QVector<Voxel> testVoxels;
  for (int x = -2; x <= 2; ++x) {
    for (int y = 0; y <= 4; ++y) {
      for (int z = -2; z <= 2; ++z) {
        testVoxels.append({x * 20.0, y * 20.0, z * 20.0, (x + y + z) * 1000.0});
      }
    }
  }
  setVoxels(testVoxels);

  // Auto-rotación
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [this]() {
    m_rotationY += 0.5;
    if (m_rotationY >= 360)
      m_rotationY -= 360;
    update();
  });
  timer->start(50);
}

void WidgetVisor3DSimple::setVoxels(const QVector<Voxel> &voxels) {
  m_voxels = voxels;
  update();
}

WidgetVisor3DSimple::Point2D
WidgetVisor3DSimple::project3DTo2D(double x, double y, double z) {
  // Proyección isométrica
  double radX = m_rotationX * M_PI / 180.0;
  double radY = m_rotationY * M_PI / 180.0;

  // Rotar alrededor de Y
  double tempX = x * cos(radY) - z * sin(radY);
  double tempZ = x * sin(radY) + z * cos(radY);
  x = tempX;
  z = tempZ;

  // Rotar alrededor de X
  double tempY = y * cos(radX) - z * sin(radX);
  tempZ = y * sin(radX) + z * cos(radX);
  y = tempY;
  z = tempZ;

  // Proyección ortográfica
  int screenX = width() / 2 + static_cast<int>(x * m_zoom);
  int screenY = height() / 2 - static_cast<int>(y * m_zoom);

  return {screenX, screenY};
}

void WidgetVisor3DSimple::drawCube(QPainter &painter, double x, double y,
                                   double z, const QColor &color) {
  double size = 8;

  // 8 vértices del cubo
  Point2D v[8];
  v[0] = project3DTo2D(x - size, y - size, z - size);
  v[1] = project3DTo2D(x + size, y - size, z - size);
  v[2] = project3DTo2D(x + size, y + size, z - size);
  v[3] = project3DTo2D(x - size, y + size, z - size);
  v[4] = project3DTo2D(x - size, y - size, z + size);
  v[5] = project3DTo2D(x + size, y - size, z + size);
  v[6] = project3DTo2D(x + size, y + size, z + size);
  v[7] = project3DTo2D(x - size, y + size, z + size);

  // Dibujar caras visibles
  painter.setBrush(color);
  painter.setPen(color.darker(120));

  // Cara frontal
  QPolygon front;
  front << QPoint(v[4].x, v[4].y) << QPoint(v[5].x, v[5].y)
        << QPoint(v[6].x, v[6].y) << QPoint(v[7].x, v[7].y);
  painter.drawPolygon(front);

  // Cara superior
  painter.setBrush(color.lighter(110));
  QPolygon top;
  top << QPoint(v[3].x, v[3].y) << QPoint(v[2].x, v[2].y)
      << QPoint(v[6].x, v[6].y) << QPoint(v[7].x, v[7].y);
  painter.drawPolygon(top);

  // Cara derecha
  painter.setBrush(color.darker(110));
  QPolygon right;
  right << QPoint(v[1].x, v[1].y) << QPoint(v[2].x, v[2].y)
        << QPoint(v[6].x, v[6].y) << QPoint(v[5].x, v[5].y);
  painter.drawPolygon(right);
}

void WidgetVisor3DSimple::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Fondo
  painter.fillRect(rect(), QColor("#F4F6F9"));

  // Título
  painter.setPen(QColor("#2C3E50"));
  painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
  painter.drawText(10, 25,
                   QString("Visor 3D - %1 Voxels").arg(m_voxels.size()));
  painter.setFont(QFont("Segoe UI", 9));
  painter.drawText(10, 45, "Arrastra: Rotar | Scroll: Zoom");

  // Dibujar voxels
  for (const auto &voxel : m_voxels) {
    QColor color = (voxel.valor > 5000) ? QColor("#E67E22") : QColor("#3498DB");
    drawCube(painter, voxel.x, voxel.y, voxel.z, color);
  }
}

void WidgetVisor3DSimple::mousePressEvent(QMouseEvent *event) {
  m_lastMousePos = event->pos();
}

void WidgetVisor3DSimple::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    int dx = event->pos().x() - m_lastMousePos.x();
    int dy = event->pos().y() - m_lastMousePos.y();

    m_rotationY += dx * 0.5;
    m_rotationX += dy * 0.5;

    m_lastMousePos = event->pos();
    update();
  }
}

void WidgetVisor3DSimple::wheelEvent(QWheelEvent *event) {
  double delta = event->angleDelta().y() / 120.0;
  m_zoom *= (1.0 + delta * 0.1);
  m_zoom = qBound(0.5, m_zoom, 3.0);
  update();
}
