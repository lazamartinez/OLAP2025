#pragma once

#include <random>
#include <string>
#include <vector>


/**
 * @brief Genera datos sintéticos (CSVs) para simular transaccionalidad OLTP.
 */
class SimuladorDatos {
public:
  struct ConfiguracionSimulacion {
    int numRegistros = 100000;
    std::string directorioSalida = "data/source";
    std::string fechaInicio = "2023-01-01";
    std::string fechaFin = "2023-12-31";
  };

  SimuladorDatos();
  bool generarDatos(const ConfiguracionSimulacion &config);

private:
  std::mt19937 m_rng;
};
