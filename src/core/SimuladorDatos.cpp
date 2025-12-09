#include "SimuladorDatos.h"
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>


namespace fs = std::filesystem;

SimuladorDatos::SimuladorDatos() {
  std::random_device rd;
  m_rng.seed(rd());
}

bool SimuladorDatos::generarDatos(const ConfiguracionSimulacion &config) {
  if (!fs::exists(config.directorioSalida)) {
    fs::create_directories(config.directorioSalida);
  }

  // 1. Generar Productos
  std::ofstream archivoProd(config.directorioSalida + "/productos.csv");
  archivoProd << "id_producto,nombre,categoria,marca,precio_base\n";

  std::vector<double> preciosBase;
  for (int i = 1; i <= 100; ++i) {
    double precio = std::uniform_real_distribution<>(10.0, 1000.0)(m_rng);
    preciosBase.push_back(precio);
    archivoProd << i << ",Producto_" << i << ",Categoria_" << (i % 10)
                << ",Marca_" << (i % 5) << "," << std::fixed
                << std::setprecision(2) << precio << "\n";
  }
  archivoProd.close();

  // 2. Generar Sucursales
  std::ofstream archivoSuc(config.directorioSalida + "/sucursales.csv");
  archivoSuc << "id_sucursal,ciudad,region,pais\n";
  archivoSuc << "1,Buenos Aires,CABA,Argentina\n";
  archivoSuc << "2,Cordoba,Centro,Argentina\n";
  archivoSuc << "3,Posadas,NEA,Argentina\n"; // Ejemplo local
  archivoSuc << "4,Mendoza,Cuyo,Argentina\n";
  archivoSuc << "5,Rosario,Centro,Argentina\n";
  archivoSuc.close();

  // 3. Generar Ventas (Fact)
  std::ofstream archivoVentas(config.directorioSalida + "/ventas.csv");
  archivoVentas << "id_venta,timestamp,id_producto,id_cliente,id_sucursal,"
                   "cantidad,precio_unitario,total\n";

  std::cout << "Generando " << config.numRegistros
            << " transacciones simuladas..." << std::endl;

  std::uniform_int_distribution<> distProd(1, 100);
  std::uniform_int_distribution<> distSuc(1, 5);
  std::uniform_int_distribution<> distCliente(1, 5000);
  std::uniform_int_distribution<> distCant(1, 10);
  std::uniform_int_distribution<> distTiempo(0, 31536000); // Segundos en un año

  for (int i = 1; i <= config.numRegistros; ++i) {
    int idProd = distProd(m_rng);
    int idSuc = distSuc(m_rng);
    int idCli = distCliente(m_rng);
    int cantidad = distCant(m_rng);
    double precio = preciosBase[idProd - 1] *
                    std::uniform_real_distribution<>(0.9, 1.1)(m_rng);
    double total = cantidad * precio;

    // Fecha aleatoria en 2023
    time_t rawTime = 1672531200 + distTiempo(m_rng);
    std::tm *tm = std::localtime(&rawTime);
    char bufferFecha[30];
    std::strftime(bufferFecha, sizeof(bufferFecha), "%Y-%m-%d %H:%M:%S", tm);

    archivoVentas << i << "," << bufferFecha << "," << idProd << "," << idCli
                  << "," << idSuc << "," << cantidad << "," << std::fixed
                  << std::setprecision(2) << precio << "," << total << "\n";
  }
  archivoVentas.close();

  return true;
}
