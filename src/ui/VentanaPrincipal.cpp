#include "VentanaPrincipal.h"
#include "WidgetAnalitica.h"
#include "WidgetEtl.h"
#include "WidgetTablero.h"
#include <QStatusBar>
#include <QVBoxLayout>


VentanaPrincipal::VentanaPrincipal(QWidget *parent) : QMainWindow(parent) {
  configurarUi();
}

VentanaPrincipal::~VentanaPrincipal() {}

void VentanaPrincipal::configurarUi() {
  this->setWindowTitle("Sistema OLAP + DW - BD2025 (PostgreSQL Edition)");
  this->resize(1280, 800);

  QWidget *widgetCentral = new QWidget(this);
  QVBoxLayout *layoutPrincipal = new QVBoxLayout(widgetCentral);
  layoutPrincipal->setContentsMargins(5, 5, 5, 5);

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setDocumentMode(true);

  // 1. Tablero (Dashboard)
  WidgetTablero *tablero = new WidgetTablero(this);
  m_tabWidget->addTab(tablero, "Tablero de Control");

  // 2. ETL
  WidgetEtl *etl = new WidgetEtl(this);
  m_tabWidget->addTab(etl, "Gestión ETL");

  // 3. Analítica
  WidgetAnalitica *analitica = new WidgetAnalitica(this);
  m_tabWidget->addTab(analitica, "Minería de Datos");

  layoutPrincipal->addWidget(m_tabWidget);
  this->setCentralWidget(widgetCentral);

  this->statusBar()->showMessage(
      "Sistema listo. Conectado a PostgreSQL en localhost:5432");
}
