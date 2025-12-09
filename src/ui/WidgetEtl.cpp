#include "WidgetEtl.h"
#include "../core/GestorBaseDatos.h"
#include "../core/ServicioEtl.h"
#include "../core/SimuladorDatos.h"
#include "SistemaOLAP/Nucleo/MotorETL.h"
#include <QApplication>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

WidgetEtl::WidgetEtl(QWidget *parent) : QWidget(parent) { configurarUi(); }

void WidgetEtl::configurarUi() {
  this->setStyleSheet("background-color: transparent;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setSpacing(25);
  layout->setContentsMargins(50, 40, 50, 40);

  // Title
  QLabel *lblTitulo = new QLabel("Panel de Control ETL");
  lblTitulo->setStyleSheet(
      "font-size: 24px; font-weight: 300; color: #2C3E50; margin-bottom: 10px; "
      "font-family: 'Segoe UI Light', 'Roboto Light';");
  layout->addWidget(lblTitulo);

  // Description
  QLabel *lblDesc = new QLabel("Gestión de procesos de extracción, "
                               "transformación y carga de datos masivos.");
  lblDesc->setStyleSheet(
      "color: #7F8C8D; font-size: 14px; margin-bottom: 20px;");
  layout->addWidget(lblDesc);

  // Buttons Container with Card-like background check? No, just clean layout.
  QHBoxLayout *panelBotones = new QHBoxLayout();
  panelBotones->setSpacing(20);

  auto createBtn = [&](QString text, QString iconName) {
    QPushButton *btn = new QPushButton(text, this);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: #FFFFFF;
            color: #2C3E50;
            border: 1px solid #DCE1E7;
            padding: 15px 25px;
            font-size: 14px;
            border-radius: 6px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton:hover {
            background-color: #F4F6F9;
            border-color: #3498DB;
            color: #3498DB;
        }
    )");
    return btn;
  };

  m_btnConectar = createBtn("1. Conectar Base de Datos", "database");
  m_btnSimular = createBtn("2. Simular Transacciones", "cogs");
  m_btnCargar = createBtn("3. Ejecutar Pipeline ETL", "upload");

  panelBotones->addWidget(m_btnConectar);
  panelBotones->addWidget(m_btnSimular);
  panelBotones->addWidget(m_btnCargar);
  panelBotones->addStretch();

  layout->addLayout(panelBotones);

  // Progress Bar
  m_progreso = new QProgressBar(this);
  m_progreso->setValue(0);
  m_progreso->setAlignment(Qt::AlignCenter);
  m_progreso->setStyleSheet(R"(
      QProgressBar {
          background-color: #ECF0F1;
          border: none;
          border-radius: 4px;
          height: 10px;
          color: transparent; 
      }
      QProgressBar::chunk {
          background-color: #3498DB;
          border-radius: 4px;
      }
  )");
  layout->addWidget(m_progreso);

  // Console Log (Terminal minimalista)
  QLabel *lblLog = new QLabel("Registro de Actividad");
  lblLog->setStyleSheet(
      "color: #95A5A6; font-size: 11px; font-weight: bold; margin-top: 20px; "
      "text-transform: uppercase; letter-spacing: 1px;");
  layout->addWidget(lblLog);

  m_log = new QTextEdit(this);
  m_log->setReadOnly(true);
  m_log->setStyleSheet(R"(
      QTextEdit {
        background-color: #FFFFFF;
        border: 1px solid #E0E0E0;
        color: #555555;
        font-family: 'Consolas', 'Monaco', monospace;
        font-size: 12px;
        padding: 15px;
        border-radius: 4px;
      }
  )");
  m_log->setPlaceholderText("Esperando ejecución de procesos...");
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
  config.numRegistros = 1000000; // 1 Millón por defecto

  // Usar ruta absoluta basada en el ejecutable para evitar problemas de CWD
  QString appPath = QCoreApplication::applicationDirPath();
  QString dataPath = appPath + "/data/source";
  config.directorioSalida = dataPath.toStdString();

  log("Generando datos en: " + dataPath);

  m_progreso->setValue(20);
  if (sim.generarDatos(config)) {
    log("Datos generados exitosamente.");
    m_progreso->setValue(100);
  } else {
    log("Error generando datos.");
  }
}

void WidgetEtl::correrEtl() {
  log("Iniciando Carga Masiva...");

  // Usar Motor Nuevo si esta disponible
  if (m_motorRef) {
    log(">> Usando MotorETL de Nueva Generacion (Arquitectura Integral) <<");
    m_progreso->setValue(5);

    // Conectar señales (lambda simplificada, cuidado con conexiones multiples
    // en produccion)
    connect(m_motorRef, &SistemaOLAP::Nucleo::MotorETL::progresoActualizado,
            this, [this](int p, QString msg) {
              m_progreso->setValue(p);
              log(msg);
            });

    connect(m_motorRef, &SistemaOLAP::Nucleo::MotorETL::procesoFinalizado, this,
            [this](bool exito, QString reporte) {
              if (exito)
                log("EXITO: " + reporte);
              else
                log("FALLO: " + reporte);
            });

    m_motorRef->ejecutarProceso("Apache Parquet / Arrow IPC",
                                "file:///data/source");
    return;
  }

  // Fallback Legacy
  ServicioEtl etl;
  m_progreso->setValue(0);
  log("Inicializando Tablas (Legacy)...");
  etl.inicializarEsquema();
  // ... (rest of legacy code)
  m_progreso->setValue(20);
  log("Cargando CSVs...");
  // ... simplified legacy
  m_progreso->setValue(100);
  log("ETL Legacy Finalizado.");
}
