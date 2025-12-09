#include "VentanaPrincipal.h"
#include "SistemaOLAP/Nucleo/CuboDatos.h"
#include "SistemaOLAP/Nucleo/MotorETL.h"
#include "WidgetAnalitica.h"
#include "WidgetEtl.h"
#include "WidgetMapa.h"
#include "WidgetTablero.h"
#include "WidgetVisor3D.h"
#include <QRandomGenerator>
#include <QStatusBar>
#include <QTabWidget>
#include <QTimer>
#include <QVBoxLayout>

VentanaPrincipal::VentanaPrincipal(QWidget *parent) : QMainWindow(parent) {
  configurarUi();
}

VentanaPrincipal::~VentanaPrincipal() {}

void VentanaPrincipal::configurarUi() {
  this->setWindowTitle("Sistema OLAP 3D Educativo - Arquitectura Integral C++");
  this->resize(1280, 800);

  // --- GLOBAL PROFESSIONAL LIGHT THEME ---
  // Palette: Background #F4F6F9, Surface #FFFFFF, Primary #2980B9, Text #2C3E50
  QString styleSheet = R"(
      QMainWindow {
          background-color: #F4F6F9;
      }
      QWidget {
          font-family: 'Segoe UI', 'Roboto', sans-serif;
          color: #2C3E50;
      }
      QTabWidget::pane {
          border: 1px solid #DCE1E7;
          background: #FFFFFF;
          border-radius: 6px;
          top: -1px;
      }
      QTabBar::tab {
          background: #E8ECEF;
          color: #576574;
          padding: 12px 24px;
          margin-right: 6px;
          border-top-left-radius: 6px;
          border-top-right-radius: 6px;
          font-weight: 600;
          border: 1px solid #DCE1E7;
          border-bottom: none;
      }
      QTabBar::tab:selected {
          background: #FFFFFF;
          color: #2980B9;
          border-top: 3px solid #2980B9;
          border-bottom: 2px solid #FFFFFF; /* Merge with pane */
      }
      QTabBar::tab:hover {
          background: #FDFDFD;
          color: #2980B9;
      }
      QStatusBar {
          background: #FFFFFF;
          color: #7F8C8D;
          border-top: 1px solid #E0E0E0;
      }
      /* Standard Button Style for child widgets if they inherit */
      QPushButton {
          background-color: #FFFFFF;
          border: 1px solid #BDC3C7;
          color: #2C3E50;
          border-radius: 4px;
          padding: 6px 12px;
          font-weight: 600;
      }
      QPushButton:hover {
          background-color: #ECF0F1;
          border-color: #2980B9;
          color: #2980B9;
      }
      QPushButton:pressed {
          background-color: #D6EAF8;
      }
  )";
  this->setStyleSheet(styleSheet);

  // --- INICIALIZACION NUCLEO ---
  m_motorEtl = new SistemaOLAP::Nucleo::MotorETL(this);
  m_cuboDatos = new SistemaOLAP::Nucleo::CuboDatos("Ventas2025", this);

  QWidget *widgetCentral = new QWidget(this);
  this->setCentralWidget(widgetCentral);

  QHBoxLayout *mainLayout = new QHBoxLayout(widgetCentral);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  // --- 1. SIDEBAR NAVIGATION ---
  QListWidget *sidebar = new QListWidget(this);
  sidebar->setFixedWidth(250);
  sidebar->setStyleSheet(R"(
      QListWidget {
          background-color: #FFFFFF;
          border-right: 1px solid #DCE1E7;
          outline: none;
          padding-top: 20px;
      }
      QListWidget::item {
          padding: 15px 20px;
          color: #7F8C8D;
          font-family: 'Segoe UI';
          font-size: 15px;
          font-weight: 500;
          border-left: 4px solid transparent;
      }
      QListWidget::item:selected {
          color: #2980B9;
          background-color: #F4F6F9;
          border-left: 4px solid #2980B9;
          font-weight: 700;
      }
      QListWidget::item:hover {
          background-color: #FAFAFA;
          color: #2980B9;
      }
  )");

  // Sidebar Items with Icons (unicode fallback for now)
  sidebar->addItem("  📊  Inicio / Resumen");
  sidebar->addItem("  🔄  Consola ETL (Carga)");
  sidebar->addItem("  🌍  Mapa Geo-Espacial");
  sidebar->addItem("  🧊  Exploración Voxel 3D");
  sidebar->addItem("  📈  Analitica & Mining");

  mainLayout->addWidget(sidebar);

  // --- 2. CONTENT AREA (STACKED) ---
  QStackedWidget *stackedWidget = new QStackedWidget(this);
  mainLayout->addWidget(stackedWidget);

  // Módulos
  // Page 0: Dashboard (Inicio)
  WidgetTablero *tablero = new WidgetTablero(this);
  stackedWidget->addWidget(tablero);

  // Page 1: ETL
  WidgetEtl *etl = new WidgetEtl(this);
  // Inyeccion de dependencias
  if (m_motorEtl)
    etl->setMotor(m_motorEtl);
  stackedWidget->addWidget(etl);

  // Page 2: Mapa
  WidgetMapa *mapa = new WidgetMapa(this);
  stackedWidget->addWidget(mapa);

  // Page 3: Visor 3D
  WidgetVisor3D *visor3d = new WidgetVisor3D(this);
  if (m_cuboDatos)
    visor3d->cargarDatos(m_cuboDatos);
  stackedWidget->addWidget(visor3d);

  // Page 4: Analitica
  WidgetAnalitica *analitica = new WidgetAnalitica(this);
  stackedWidget->addWidget(analitica);

  // Conectar Navegacion
  connect(sidebar, &QListWidget::currentRowChanged, stackedWidget,
          &QStackedWidget::setCurrentIndex);

  // Default selection
  sidebar->setCurrentRow(0);

  // --- Motor de Simulación en Tiempo Real (Demo) ---
  QTimer *timerSimulacion = new QTimer(this);
  connect(timerSimulacion, &QTimer::timeout, this, [this, mapa, visor3d]() {
    // 1. Simular nueva transacción de venta
    double monto = QRandomGenerator::global()->bounded(50.0, 5000.0);
    QStringList ciudades = {"Buenos Aires", "New York", "London",
                            "Tokyo",        "Moscow",   "Cordoba"};
    QString origen =
        ciudades[QRandomGenerator::global()->bounded((int)ciudades.size())];
    QString destino =
        ciudades[QRandomGenerator::global()->bounded((int)ciudades.size())];

    // Enviar a Mapa
    mapa->procesarVenta(origen, destino, monto);

    // Enviar a Data Cube (Simulado)
    // m_cuboDatos->registrarHecho(...);
  });
  timerSimulacion->start(800); // 1 venta cada 0.8 segundos

  layoutPrincipal->addWidget(m_tabWidget);
  this->setCentralWidget(widgetCentral);

  this->statusBar()->showMessage("Sistema OLAP 3D Inicializado. Motor ETL: "
                                 "Listo. Cubo MOLAP: Cargado (Voxels).");
}
