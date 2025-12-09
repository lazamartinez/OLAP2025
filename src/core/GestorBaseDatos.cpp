#include "GestorBaseDatos.h"
#include <QDebug>

GestorBaseDatos &GestorBaseDatos::instancia() {
  static GestorBaseDatos instancia;
  return instancia;
}

GestorBaseDatos::~GestorBaseDatos() { desconectar(); }

bool GestorBaseDatos::conectar(const QString &host, int puerto,
                               const QString &nombreDb, const QString &usuario,
                               const QString &password) {
  if (QSqlDatabase::contains("qt_sql_default_connection")) {
    m_db = QSqlDatabase::database("qt_sql_default_connection");
  } else {
    m_db = QSqlDatabase::addDatabase("QPSQL");
  }

  m_db.setHostName(host);
  m_db.setPort(puerto);
  m_db.setDatabaseName(nombreDb);
  m_db.setUserName(usuario);
  m_db.setPassword(password);

  if (!m_db.open()) {
    qCritical() << "Error conexion BD:" << m_db.lastError().text();
    return false;
  }

  qDebug() << "Conectado exitosamente a PostgreSQL:" << nombreDb;
  return true;
}

void GestorBaseDatos::desconectar() {
  if (m_db.isOpen()) {
    m_db.close();
    qDebug() << "Desconectado de BD.";
  }
}

bool GestorBaseDatos::ejecutarConsulta(const QString &sql) {
  if (!m_db.isOpen())
    return false;

  QSqlQuery query(m_db);
  if (!query.exec(sql)) {
    qCritical() << "Error Query:" << query.lastError().text() << "SQL:" << sql;
    return false;
  }
  return true;
}

QSqlQuery GestorBaseDatos::consultar(const QString &sql) {
  if (!m_db.isOpen())
    return QSqlQuery();

  QSqlQuery query(m_db);
  if (!query.exec(sql)) {
    qCritical() << "Error Query (Select):" << query.lastError().text();
  }
  return query;
}

QString GestorBaseDatos::ultimoError() const { return m_db.lastError().text(); }
