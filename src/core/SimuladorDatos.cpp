#include "SimuladorDatos.h"
#include "../core/GestorBaseDatos.h"
#include <QSqlQuery>
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

  // Listas de datos realistas para la "Semilla"
  const std::vector<std::string> nombresProductos = {
      "Laptop Gamer Xtreme",          "Monitor 4K Ultra",
      "Teclado Mecanico RGB",         "Mouse Inalambrico Pro",
      "Auriculares Noise Cancelling", "Tablet Grafica 10p",
      "Smartphone Flagship Z",        "Smartwatch Fitness",
      "Disco SSD NVMe 1TB",           "Memoria RAM DDR4 16GB",
      "Placa de Video RTX 4090",      "Procesador Core i9",
      "Fuente Modular 850W",          "Gabinete Vidrio Templado",
      "Silla Gamer Ergonomica",       "Webcam 1080p Streaming",
      "Microfono Condensador USB",    "Router WiFi 6 Mesh",
      "Impresora Laser Color",        "Proyector Portatil Mini"};

  const std::vector<std::string> marcas = {"TechBrand", "CyberSystems",
                                           "LogicWare", "FutureTech",
                                           "GigaPower", "NanoSoft"};

  const std::vector<std::string> categorias = {"Computacion", "Perifericos",
                                               "Gaming", "Redes", "Oficina"};

  // 1. Generar Productos (Catalogo mas amplio de ejemplo, 200 items)
  std::ofstream archivoProd(config.directorioSalida + "/productos.csv");
  archivoProd << "id_producto,nombre,categoria,marca,precio_base\n";

  std::vector<double> preciosBase;
  for (int i = 1; i <= 200; ++i) {
    // Seleccion aleatoria de propiedades
    std::string nombre =
        nombresProductos[i % nombresProductos.size()] + " " + std::to_string(i);
    std::string marca = marcas[i % marcas.size()];
    std::string cat = categorias[i % categorias.size()];

    double precio = std::uniform_real_distribution<>(50.0, 3000.0)(m_rng);
    preciosBase.push_back(precio);

    archivoProd << i << "," << nombre << "," << cat << "," << marca << ","
                << std::fixed << std::setprecision(2) << precio << "\n";
  }
  archivoProd.close();

  // 2. Generar Sucursales (Mas regiones)
  std::ofstream archivoSuc(config.directorioSalida + "/sucursales.csv");
  archivoSuc << "id_sucursal,ciudad,region,pais\n";
  archivoSuc << "1,Buenos Aires,AMBA,Argentina\n";
  archivoSuc << "2,Cordoba,Centro,Argentina\n";
  archivoSuc << "3,Rosario,Litoral,Argentina\n";
  archivoSuc << "4,Mendoza,Cuyo,Argentina\n";
  archivoSuc << "5,Tucuman,NOA,Argentina\n";
  archivoSuc << "6,Neuquen,Patagonia,Argentina\n";
  archivoSuc << "7,Mar del Plata,Costa,Argentina\n";
  archivoSuc.close();

  // 3. Generar Ventas (Fact) - VOLUMEN MASIVO
  std::ofstream archivoVentas(config.directorioSalida + "/ventas.csv");
  archivoVentas << "id_venta,timestamp,id_producto,id_cliente,id_sucursal,"
                   "cantidad,precio_unitario,total\n";

  // Usamos 1 Millon de registros si no se especifica otro
  int maxRegistros =
      (config.numRegistros < 100000) ? 1000000 : config.numRegistros;

  std::cout << "Generando SEMILLA MASIVA de " << maxRegistros
            << " transacciones... (Esto puede tardar unos segundos)"
            << std::endl;

  std::uniform_int_distribution<> distProd(1, 200);
  std::uniform_int_distribution<> distSuc(1, 7);
  std::uniform_int_distribution<> distCliente(1, 50000); // 50k clientes unicos
  std::uniform_int_distribution<> distCant(1,
                                           20); // Compras mayoristas posibles
  std::uniform_int_distribution<> distTiempo(0, 31536000 *
                                                    2); // 2 años de historia

  // Buffer para optimizar escritura
  char bufferLine[256];

  for (int i = 1; i <= maxRegistros; ++i) {
    int idProd = distProd(m_rng);
    int idSuc = distSuc(m_rng);
    int idCli = distCliente(m_rng);
    int cantidad = distCant(m_rng);

    // Variacion de precio por oferta/inflacion
    double precio = preciosBase[idProd - 1] *
                    std::uniform_real_distribution<>(0.85, 1.15)(m_rng);
    double total = cantidad * precio;

    // Fecha aleatoria desde 2023
    time_t rawTime = 1672531200 + distTiempo(m_rng);
    std::tm *tm = std::localtime(&rawTime);

    // Formateo rapido manual o snprintf es mas rapido que strftime para loop
    // masivo? strftime es seguro.
    char bufferFecha[20];
    std::strftime(bufferFecha, sizeof(bufferFecha), "%Y-%m-%d %H:%M:%S", tm);

    // Escribir linea
    // id_venta, timestamp, id_prod, id_cli, id_suc, cant, precio, total
    archivoVentas << i << "," << bufferFecha << "," << idProd << "," << idCli
                  << "," << idSuc << "," << cantidad << "," << std::fixed
                  << std::setprecision(2) << precio << "," << total << "\n";

    if (i % 10000 == 0) {
      std::cout << "\rProgreso: " << (i * 100 / maxRegistros) << "%"
                << std::flush;
    }
  }
  std::cout << "\nGeneracion completada." << std::endl;
  archivoVentas.close();

  return true;
}

bool SimuladorDatos::generarDatosDemo(int numRegistros) {
  std::cout << "Generación de datos demo: " << numRegistros << " registros."
            << std::endl;
  std::cout
      << "Nota: Para datos demo completos, ejecutar ETL desde la interfaz."
      << std::endl;
  // TODO: Implementar inserción directa cuando el esquema esté listo
  return true;
}
