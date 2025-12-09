#pragma once

#include <QCache>
#include <QDebug>
#include <QMutex>
#include <QSqlQuery>
#include <QString>


class CacheConsultas {
public:
  static CacheConsultas &instancia() {
    static CacheConsultas instance;
    return instance;
  }

  QSqlQuery *obtener(const QString &sql) {
    QMutexLocker locker(&m_mutex);

    if (m_cache.contains(sql)) {
      qDebug() << "Cache HIT:" << sql.left(50);
      return m_cache.object(sql);
    }

    qDebug() << "Cache MISS:" << sql.left(50);
    return nullptr;
  }

  void insertar(const QString &sql, QSqlQuery *query) {
    QMutexLocker locker(&m_mutex);
    m_cache.insert(sql, query, 1);
    qDebug() << "Cache INSERT. Total:" << m_cache.count();
  }

  void limpiar() {
    QMutexLocker locker(&m_mutex);
    m_cache.clear();
    qDebug() << "Cache limpiado";
  }

  int tamanio() const { return m_cache.count(); }

private:
  CacheConsultas() {
    m_cache.setMaxCost(100); // 100 queries en cache
  }

  QCache<QString, QSqlQuery> m_cache;
  QMutex m_mutex;
};
