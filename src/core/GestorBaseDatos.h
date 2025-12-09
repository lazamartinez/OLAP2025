#pragma once

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <memory>
#include <vector>

/**
 * @brief Singleton para manejar la conexión a PostgreSQL con QSqlDatabase.
 */
class GestorBaseDatos {
public:
  static GestorBaseDatos &instancia();

  // Conecta a la base de datos PostgreSQL
  bool conectar(const QString &host, int puerto, const QString &nombreDb,
                const QString &usuario, const QString &password);
  void desconectar();

  // Ejecuta una consulta SQL sin retorno (INSERT, UPDATE, CREATE)
  bool ejecutarConsulta(const QString &sql);

  // Ejecuta una consulta y devuelve un objeto QSqlQuery listo para iterar
  QSqlQuery consultar(const QString &sql);

  // Obtiene el último error
  QString ultimoError() const;

private:
  GestorBaseDatos() = default;
  ~GestorBaseDatos();

  GestorBaseDatos(const GestorBaseDatos &) = delete;
  GestorBaseDatos &operator=(const GestorBaseDatos &) = delete;

  QSqlDatabase m_db;
};
