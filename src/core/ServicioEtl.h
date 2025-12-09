#pragma once

#include <QString>
#include <string>


/**
 * @brief Servicio de Extracción, Transformación y Carga (ETL).
 * Se encarga de crear el esquema estrella y poblarlo en PostgreSQL.
 */
class ServicioEtl {
public:
  ServicioEtl();

  // Inicializa el esquema (tablas y relaciones)
  bool inicializarEsquema();

  // Carga datos desde los CSV generados
  bool cargarDesdeCSV(const std::string &directorioDatos);

  // Crea vistas materializadas para agilizar consultas
  bool transformarDatos();
};
