#include "WidgetEtl.h"
#include "../core/GestorBaseDatos.h"
#include "../core/ServicioEtl.h"
#include "../core/SimuladorDatos.h"
#include <QApplication>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>


WidgetEtl::WidgetEtl(QWidget *parent) : QWidget(parent) { configurarUi(); }

void WidgetEtl::configurarUi() {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *lblTitulo = new QLabel(
      "<h3>Panel de Control ETL (Extracción, Transformación, Carga)</h3>");
  lblTitulo->setStyleSheet("color: #2a82da;");
  layout->addWidget(lblTitulo);

  QHBoxLayout *panelBotones = new QHBoxLayout();

  m_btnConectar = new QPushButton("1. Conectar BD (Local)", this);
  m_btnSimular = new QPushButton("2. Simular Datos (CSV)", this);
  m_btnCargar = new QPushButton("3. Cargar DW (Postgres)", this);

  panelBotones->addWidget(m_btnConectar);
  panelBotones->addWidget(m_btnSimular);
  panelBotones->addWidget(m_btnCargar);
  panelBotones->addStretch();

  layout->addLayout(panelBotones);

  m_progreso = new QProgressBar(this);
  m_progreso->setValue(0);
  layout->addWidget(m_progreso);

  m_log = new QTextEdit(this);
  m_log->setReadOnly(true);
  m_log->setPlaceholderText("Bitácora de procesos...");
  layout->addWidget(m_log);

  connect(m_btnConectar, &QPushButton::clicked, this, &WidgetEtl::conectarBD);
  connect(m_btnSimular, &QPushButton::clicked, this,
          &WidgetEtl::correrSimulacion);
  connect(m_btnCargar, &QPushButton::clicked, this, &WidgetEtl::correrEtl);
}

void WidgetEtl::log(const QString &msj) {
  m_log->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ") + msj);
  QApplication::processEvents();
}

void WidgetEtl::conectarBD() {
  log("Intentando conectar a PostgreSQL (localhost)...");
  auto &db = GestorBaseDatos::instancia();
  if (db.conectar("localhost", 5432, "bd2025", "postgres", "laza")) {
    log("Conexión Exitosa a bd2025.");
  } else {
    log("Error de Conexión: " + db.ultimoError());
    log("Asegúrate de crear la DB 'bd2025' en Postgres primero.");
  }
}

void WidgetEtl::correrSimulacion() {
  log("Iniciando Simulación...");
  SimuladorDatos sim;
  SimuladorDatos::ConfiguracionSimulacion config;
  config.numRegistros = 50000;

  m_progreso->setValue(20);
  if (sim.generarDatos(config)) {
    log("Datos generados en carpeta: data/source");
    m_progreso->setValue(100);
  } else {
    log("Error generando datos.");
  }
}

void WidgetEtl::correrEtl() {
  log("Iniciando Carga Masiva...");
  ServicioEtl etl;
  m_progreso->setValue(0);

  log("Inicializando Tablas...");
  etl.inicializarEsquema();
  m_progreso->setValue(20);

  log("Cargando CSVs a PostgreSQL (COPY)...");
  // Usamos QCoreApplication::applicationDirPath() para path absoluto fiable del
  // exe
  QString appPath = QCoreApplication::applicationDirPath();
  // data/source usualmente copiado a build dir via CMake
  QString dataPath = appPath + "/data/source";

  log("Buscando datos en: " + dataPath);

  if (etl.cargarDesdeCSV(dataPath.toStdString())) {
    log("Carga completada.");
  } else {
    log("Hubo errores en la carga (revísalos arriba).");
  }
  m_progreso->setValue(70);

  log("Actualizando Vistas Materializadas...");
  etl.transformarDatos();
  m_progreso->setValue(100);
  log("ETL Finalizado.");
}
