#pragma once

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <QString>
#include <QVariantMap>


class GestorFavoritos {
public:
  struct Favorito {
    QString nombre;
    QString tipo; // "dashboard", "query", "analisis"
    QVariantMap configuracion;
    QString fechaCreacion;
  };

  static void agregarFavorito(const QString &nombre, const QString &tipo,
                              const QVariantMap &config);
  static QList<Favorito> obtenerFavoritos();
  static void eliminarFavorito(const QString &nombre);
  static bool existeFavorito(const QString &nombre);
  static Favorito obtenerFavorito(const QString &nombre);

private:
  static QString favoritosKey() { return "favoritos/lista"; }
};
