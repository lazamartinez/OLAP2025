#include "VentanaPrincipal.h"
#include "SistemaOLAP/Nucleo/CuboDatos.h"
#include "SistemaOLAP/Nucleo/MotorETL.h"
#include "WidgetAnalitica.h"
#include "WidgetEtl.h"
#include "WidgetFiltrosGlobales.h"
#include "WidgetHeatmap.h"
#include "WidgetHistorial.h"
#include "WidgetMapa.h"
#include "WidgetTablero.h"
#include "WidgetVisor3D.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QListWidget>
#include <QRandomGenerator>
#include <QSettings>
#include <QStackedWidget>
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

  QVBoxLayout *layoutPrincipal = new QVBoxLayout(widgetCentral);
  layoutPrincipal->setContentsMargins(0, 0, 0, 0);
  layoutPrincipal->setSpacing(0);

  // --- FILTROS GLOBALES ---
  m_filtrosGlobales = new WidgetFiltrosGlobales(this);
  m_filtrosGlobales->setStyleSheet(
      "background-color: #FFFFFF; border-bottom: 1px solid #DCE1E7;");
  layoutPrincipal->addWidget(m_filtrosGlobales);

  connect(m_filtrosGlobales, &WidgetFiltrosGlobales::filtrosActualizados, this,
          &VentanaPrincipal::aplicarFiltrosGlobales);

  // --- CONTENIDO PRINCIPAL ---
  QHBoxLayout *mainLayout = new QHBoxLayout();
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

  // Sidebar Items with Icons
  sidebar->addItem("  📊  Inicio / Resumen");
  sidebar->addItem("  🔄  Consola ETL (Carga)");
  sidebar->addItem("  🌍  Mapa Geo-Espacial");
  sidebar->addItem("  🧊  Exploración Voxel 3D");
  sidebar->addItem("  📈  Analitica & Mining");
  sidebar->addItem("  📅  Heatmap Temporal");
  sidebar->addItem("  📜  Historial Queries");

  mainLayout->addWidget(sidebar);

  layoutPrincipal->addLayout(mainLayout);

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

  // Page 5: Heatmap
  WidgetHeatmap *heatmap = new WidgetHeatmap(this);
  stackedWidget->addWidget(heatmap);

  // Page 6: Historial
  WidgetHistorial *historial = new WidgetHistorial(this);
  stackedWidget->addWidget(historial);

  // Conectar Navegacion
  connect(sidebar, &QListWidget::currentRowChanged, stackedWidget,
          &QStackedWidget::setCurrentIndex);

  // Default selection
  sidebar->setCurrentRow(0);

  // --- Motor de Simulación en Tiempo Real (Demo) ---
  QTimer *timerSimulacion = new QTimer(this);
  // Use 'visor3d' in capture if needed, though not used in body, so removed to
  // avoid unused variable warning
  connect(timerSimulacion, &QTimer::timeout, this, [this, mapa]() {
    // 1. Simular nueva transacción de venta
    double monto = QRandomGenerator::global()->bounded(
        50, 5000); // Int arg for ambiguity resolution
    QStringList ciudades = {"Buenos Aires", "New York", "London",
                            "Tokyo",        "Moscow",   "Cordoba"};

    int idxOrigen = QRandomGenerator::global()->bounded((int)ciudades.size());
    int idxDestino = QRandomGenerator::global()->bounded((int)ciudades.size());

    QString origen = ciudades[idxOrigen];
    QString destino = ciudades[idxDestino];

    // Enviar a Mapa
    mapa->procesarVenta(origen, destino, monto);
  });
  timerSimulacion->start(800); // 1 venta cada 0.8 segundos

  // Status Bar with Theme Toggle
  QStatusBar *statusBar = new QStatusBar(this);
  setStatusBar(statusBar);

  // Cargar preferencia de tema
  QSettings settings("OLAP2025", "Sistema");
  m_temaOscuro = settings.value("tema/oscuro", false).toBool();

  m_actionTema =
      new QAction(m_temaOscuro ? "☀️ Modo Claro" : "🌙 Modo Oscuro", this);
  m_actionTema->setToolTip("Cambiar entre tema claro y oscuro");
  connect(m_actionTema, &QAction::triggered, this,
          &VentanaPrincipal::toggleTema);
  statusBar->addPermanentWidget(new QPushButton(m_actionTema->text()));

  QPushButton *btnTema =
      qobject_cast<QPushButton *>(statusBar->children().last());
  if (btnTema) {
    btnTema->setFlat(true);
    btnTema->setCursor(Qt::PointingHandCursor);
    connect(btnTema, &QPushButton::clicked, this,
            &VentanaPrincipal::toggleTema);
  }

  statusBar->showMessage("Sistema OLAP 2025 Inicializado. Motor ETL: "
                         "Listo. Cubo MOLAP: Cargado (Voxels).");
}

void VentanaPrincipal::toggleTema() {
  m_temaOscuro = !m_temaOscuro;
  aplicarTema(m_temaOscuro);

  // Guardar preferencia
  QSettings settings("OLAP2025", "Sistema");
  settings.setValue("tema/oscuro", m_temaOscuro);

  m_actionTema->setText(m_temaOscuro ? "☀️ Modo Claro" : "🌙 Modo Oscuro");
  statusBar()->showMessage(
      m_temaOscuro ? "Tema oscuro activado" : "Tema claro activado", 2000);
}

void VentanaPrincipal::aplicarTema(bool oscuro) {
  if (oscuro) {
    // Tema Oscuro
    setStyleSheet(R"(
      QMainWindow { background-color: #1E1E1E; }
      QWidget { color: #E0E0E0; }
      QListWidget { background-color: #2D2D30; border-right: 1px solid #3E3E42; }
      QListWidget::item { color: #CCCCCC; }
      QListWidget::item:selected { background-color: #3E3E42; color: #2980B9; }
    )");
  } else {
    // Tema Claro (actual)
    setStyleSheet(R"(
      QMainWindow { background-color: #F4F6F9; }
      QWidget { color: #2C3E50; }
    )");
  }
}

void VentanaPrincipal::aplicarFiltrosGlobales(const QString &region,
                                              const QDate &inicio,
                                              const QDate &fin) {
  qDebug() << "Filtros aplicados:" << region << inicio << fin;
  statusBar()->showMessage(QString("Filtros: %1 | %2 - %3")
                               .arg(region)
                               .arg(inicio.toString("dd/MM/yyyy"))
                               .arg(fin.toString("dd/MM/yyyy")),
                           3000);

  // TODO: Propagar filtros a widgets (Tablero, Analytics, Mapa)
  // Cada widget debería tener un método aplicarFiltros(region, inicio, fin)
}
