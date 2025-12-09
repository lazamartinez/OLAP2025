#pragma once

#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineView>
#include <QWidget>


class WidgetMapbox : public QWidget {
  Q_OBJECT
public:
  explicit WidgetMapbox(QWidget *parent = nullptr);

  void agregarMarcador(double lat, double lon, const QString &titulo,
                       const QString &descripcion = "");
  void centrarEn(double lat, double lon, int zoom = 10);
  void limpiarMarcadores();

signals:
  void marcadorClickeado(double lat, double lon);

private:
  QWebEngineView *m_webView;
  QWebChannel *m_channel;

  QString generarHTML();
  void ejecutarJS(const QString &script);
};
