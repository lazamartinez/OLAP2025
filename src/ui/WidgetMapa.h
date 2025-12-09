#pragma once

#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QListWidget>
#include <QMap>
#include <QTimer>
#include <QVector>

/**
 * @brief Widget al estilo "War Room" / "Cyberpunk Map"
 * Implementacion nativa con QGraphicsView sin dependencias de WebEngine.
 */
class WidgetMapa : public QGraphicsView {
  Q_OBJECT

public:
  explicit WidgetMapa(QWidget *parent = nullptr);

  struct Point3D {
    double x, y, z;
  };

public slots:
  void procesarVenta(const QString &origen, const QString &destino,
                     double monto);
  void agregarSucursal();

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  void dibujarMapaBase();
  QPointF coordenadasAProyeccion(double lat, double lon);

  QGraphicsScene *m_scene;
  QTimer *m_timerAnimacion;

  struct Coordenada {
    double lat;
    double lon;
  };
  QMap<QString, Coordenada> m_ubicaciones;

  double m_rotationAngle;
  double m_rotationSpeed = 0.05; // Reducido para rotación más lenta

  struct EffectData {
    QString locationName;
    int life;
  };
  QList<EffectData> m_colaEfectos;

  QImage m_textureImage;
  QPixmap m_earthTexture;        // Textura realista de la Tierra
  QPixmap m_earthTextureMinimal; // Textura minimalista B&N
  bool m_estiloMinimalista = false;
  QVector<Point3D> m_spherePoints;
  QVector<Point3D> m_starPoints;
  double m_totalVentas = 0;
  bool m_is2DMode = false;
  QListWidget *m_listTransactions;

  void setupUI();
  void logTransaction(const QString &msg, double amount);
  void toggleEstiloMapa();
  Point3D rotateYMAP(const Point3D &p, double angle);
};
