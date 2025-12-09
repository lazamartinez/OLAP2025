#include "GestorFavoritos.h"
#include <QDateTime>

void GestorFavoritos::agregarFavorito(const QString &nombre,
                                      const QString &tipo,
                                      const QVariantMap &config) {
  QSettings settings("OLAP2025", "Sistema");

  // Cargar favoritos existentes
  QByteArray data = settings.value(favoritosKey()).toByteArray();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  QJsonArray favoritos = doc.array();

  // Crear nuevo favorito
  QJsonObject nuevoFav;
  nuevoFav["nombre"] = nombre;
  nuevoFav["tipo"] = tipo;
  nuevoFav["fechaCreacion"] =
      QDateTime::currentDateTime().toString(Qt::ISODate);

  // Convertir config a JSON
  QJsonObject configJson = QJsonObject::fromVariantMap(config);
  nuevoFav["configuracion"] = configJson;

  // Agregar
  favoritos.append(nuevoFav);

  // Guardar
  doc.setArray(favoritos);
  settings.setValue(favoritosKey(), doc.toJson(QJsonDocument::Compact));
}

QList<GestorFavoritos::Favorito> GestorFavoritos::obtenerFavoritos() {
  QList<Favorito> resultado;
  QSettings settings("OLAP2025", "Sistema");

  QByteArray data = settings.value(favoritosKey()).toByteArray();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  QJsonArray favoritos = doc.array();

  for (const QJsonValue &val : favoritos) {
    QJsonObject obj = val.toObject();
    Favorito fav;
    fav.nombre = obj["nombre"].toString();
    fav.tipo = obj["tipo"].toString();
    fav.fechaCreacion = obj["fechaCreacion"].toString();
    fav.configuracion = obj["configuracion"].toObject().toVariantMap();
    resultado.append(fav);
  }

  return resultado;
}

void GestorFavoritos::eliminarFavorito(const QString &nombre) {
  QSettings settings("OLAP2025", "Sistema");

  QByteArray data = settings.value(favoritosKey()).toByteArray();
  QJsonDocument doc = QJsonDocument::fromJson(data);
  QJsonArray favoritos = doc.array();

  // Filtrar
  QJsonArray nuevoArray;
  for (const QJsonValue &val : favoritos) {
    if (val.toObject()["nombre"].toString() != nombre) {
      nuevoArray.append(val);
    }
  }

  doc.setArray(nuevoArray);
  settings.setValue(favoritosKey(), doc.toJson(QJsonDocument::Compact));
}

bool GestorFavoritos::existeFavorito(const QString &nombre) {
  auto favoritos = obtenerFavoritos();
  for (const auto &fav : favoritos) {
    if (fav.nombre == nombre)
      return true;
  }
  return false;
}

GestorFavoritos::Favorito
GestorFavoritos::obtenerFavorito(const QString &nombre) {
  auto favoritos = obtenerFavoritos();
  for (const auto &fav : favoritos) {
    if (fav.nombre == nombre)
      return fav;
  }
  return Favorito();
}
