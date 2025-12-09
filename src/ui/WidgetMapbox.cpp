#include "WidgetMapbox.h"
#include <QDebug>

WidgetMapbox::WidgetMapbox(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_webView = new QWebEngineView(this);
  m_channel = new QWebChannel(this);

  // Cargar HTML con Mapbox
  QString html = generarHTML();
  m_webView->setHtml(html);

  layout->addWidget(m_webView);
}

QString WidgetMapbox::generarHTML() {
  // Token público de demo (reemplazar con token real del usuario)
  QString mapboxToken =
      "pk.eyJ1IjoiZGVtb3VzZXIiLCJhIjoiY2wxMjM0NTY3In0.demo_token_here";

  return R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset='utf-8' />
    <title>OLAP Mapa Interactivo</title>
    <meta name='viewport' content='initial-scale=1,maximum-scale=1,user-scalable=no' />
    <script src='https://api.mapbox.com/mapbox-gl-js/v2.15.0/mapbox-gl.js'></script>
    <link href='https://api.mapbox.com/mapbox-gl-js/v2.15.0/mapbox-gl.css' rel='stylesheet' />
    <style>
        body { margin: 0; padding: 0; }
        #map { position: absolute; top: 0; bottom: 0; width: 100%; }
    </style>
</head>
<body>
    <div id='map'></div>
    <script>
        mapboxgl.accessToken = ')" +
         mapboxToken + R"(';
        
        const map = new mapboxgl.Map({
            container: 'map',
            style: 'mapbox://styles/mapbox/light-v11',
            center: [-58.3816, -34.6037], // Buenos Aires
            zoom: 3,
            projection: 'globe'
        });
        
        map.on('load', () => {
            map.setFog({
                'color': 'rgb(220, 230, 240)',
                'high-color': 'rgb(36, 92, 223)',
                'horizon-blend': 0.02,
                'space-color': 'rgb(11, 11, 25)',
                'star-intensity': 0.6
            });
        });
        
        // Marcadores globales
        const markers = [];
        
        function agregarMarcador(lat, lon, titulo, descripcion) {
            const marker = new mapboxgl.Marker({ color: '#3498DB' })
                .setLngLat([lon, lat])
                .setPopup(new mapboxgl.Popup().setHTML('<h3>' + titulo + '</h3><p>' + descripcion + '</p>'))
                .addTo(map);
            markers.push(marker);
        }
        
        function centrarEn(lat, lon, zoom) {
            map.flyTo({ center: [lon, lat], zoom: zoom, duration: 2000 });
        }
        
        function limpiarMarcadores() {
            markers.forEach(m => m.remove());
            markers.length = 0;
        }
        
        // Datos de prueba
        agregarMarcador(-34.6037, -58.3816, 'Buenos Aires', 'Ventas: $125,000');
        agregarMarcador(40.7128, -74.0060, 'New York', 'Ventas: $340,000');
        agregarMarcador(51.5074, -0.1278, 'London', 'Ventas: $280,000');
    </script>
</body>
</html>
    )";
}

void WidgetMapbox::agregarMarcador(double lat, double lon,
                                   const QString &titulo,
                                   const QString &descripcion) {
  QString script = QString("agregarMarcador(%1, %2, '%3', '%4');")
                       .arg(lat)
                       .arg(lon)
                       .arg(titulo)
                       .arg(descripcion);
  ejecutarJS(script);
}

void WidgetMapbox::centrarEn(double lat, double lon, int zoom) {
  QString script =
      QString("centrarEn(%1, %2, %3);").arg(lat).arg(lon).arg(zoom);
  ejecutarJS(script);
}

void WidgetMapbox::limpiarMarcadores() { ejecutarJS("limpiarMarcadores();"); }

void WidgetMapbox::ejecutarJS(const QString &script) {
  m_webView->page()->runJavaScript(script, [](const QVariant &result) {
    qDebug() << "JS ejecutado:" << result;
  });
}
