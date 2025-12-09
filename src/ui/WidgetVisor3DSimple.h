#pragma once

#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QVector>
#include <QWidget>


class WidgetVisor3DSimple : public QWidget {
  Q_OBJECT
public:
  explicit WidgetVisor3DSimple(QWidget *parent = nullptr);

  struct Voxel {
    double x, y, z;
    double valor;
  };

  void setVoxels(const QVector<Voxel> &voxels);

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  QVector<Voxel> m_voxels;
  double m_rotationX = 30;
  double m_rotationY = 45;
  double m_zoom = 1.0;
  QPoint m_lastMousePos;

  struct Point2D {
    int x, y;
  };

  Point2D project3DTo2D(double x, double y, double z);
  void drawCube(QPainter &painter, double x, double y, double z,
                const QColor &color);
};
