#include "WidgetFiltrosGlobales.h"
#include <QSettings>

WidgetFiltrosGlobales::WidgetFiltrosGlobales(QWidget *parent)
    : QWidget(parent) {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(15, 8, 15, 8);
  layout->setSpacing(15);

  // Región
  QLabel *lblRegion = new QLabel("Región:", this);
  lblRegion->setStyleSheet("font-weight: 600; color: #2C3E50;");
  layout->addWidget(lblRegion);

  m_comboRegion = new QComboBox(this);
  m_comboRegion->addItems(
      {"Todas", "América", "Europa", "Asia", "Oceanía", "África"});
  m_comboRegion->setMinimumWidth(120);
  m_comboRegion->setStyleSheet(R"(
        QComboBox {
            background: white;
            border: 1px solid #DCE1E7;
            border-radius: 4px;
            padding: 6px 10px;
            color: #2C3E50;
        }
        QComboBox:hover {
            border-color: #3498DB;
        }
    )");
  layout->addWidget(m_comboRegion);

  layout->addSpacing(10);

  // Fecha Inicio
  QLabel *lblInicio = new QLabel("Desde:", this);
  lblInicio->setStyleSheet("font-weight: 600; color: #2C3E50;");
  layout->addWidget(lblInicio);

  m_dateInicio = new QDateEdit(this);
  m_dateInicio->setDate(QDate::currentDate().addMonths(-6));
  m_dateInicio->setCalendarPopup(true);
  m_dateInicio->setDisplayFormat("dd/MM/yyyy");
  m_dateInicio->setStyleSheet(R"(
        QDateEdit {
            background: white;
            border: 1px solid #DCE1E7;
            border-radius: 4px;
            padding: 6px 10px;
            color: #2C3E50;
        }
    )");
  layout->addWidget(m_dateInicio);

  // Fecha Fin
  QLabel *lblFin = new QLabel("Hasta:", this);
  lblFin->setStyleSheet("font-weight: 600; color: #2C3E50;");
  layout->addWidget(lblFin);

  m_dateFin = new QDateEdit(this);
  m_dateFin->setDate(QDate::currentDate());
  m_dateFin->setCalendarPopup(true);
  m_dateFin->setDisplayFormat("dd/MM/yyyy");
  m_dateFin->setStyleSheet(m_dateInicio->styleSheet());
  layout->addWidget(m_dateFin);

  layout->addSpacing(10);

  // Botones
  m_btnAplicar = new QPushButton("Aplicar", this);
  m_btnAplicar->setStyleSheet(R"(
        QPushButton {
            background-color: #3498DB;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 20px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #2980B9;
        }
    )");
  layout->addWidget(m_btnAplicar);

  m_btnLimpiar = new QPushButton("Limpiar", this);
  m_btnLimpiar->setStyleSheet(R"(
        QPushButton {
            background-color: #ECF0F1;
            color: #2C3E50;
            border: 1px solid #BDC3C7;
            border-radius: 4px;
            padding: 8px 20px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #D5DBDB;
        }
    )");
  layout->addWidget(m_btnLimpiar);

  layout->addStretch();

  // Conexiones
  connect(m_btnAplicar, &QPushButton::clicked, this,
          &WidgetFiltrosGlobales::aplicarFiltros);
  connect(m_btnLimpiar, &QPushButton::clicked, this,
          &WidgetFiltrosGlobales::limpiarFiltros);

  // Cargar filtros guardados
  QSettings settings("OLAP2025", "Sistema");
  m_comboRegion->setCurrentText(
      settings.value("filtros/region", "Todas").toString());
  m_dateInicio->setDate(
      settings.value("filtros/fechaInicio", m_dateInicio->date()).toDate());
  m_dateFin->setDate(
      settings.value("filtros/fechaFin", m_dateFin->date()).toDate());
}

QString WidgetFiltrosGlobales::getRegionSeleccionada() const {
  return m_comboRegion->currentText();
}

QDate WidgetFiltrosGlobales::getFechaInicio() const {
  return m_dateInicio->date();
}

QDate WidgetFiltrosGlobales::getFechaFin() const { return m_dateFin->date(); }

void WidgetFiltrosGlobales::aplicarFiltros() {
  // Guardar en settings
  QSettings settings("OLAP2025", "Sistema");
  settings.setValue("filtros/region", m_comboRegion->currentText());
  settings.setValue("filtros/fechaInicio", m_dateInicio->date());
  settings.setValue("filtros/fechaFin", m_dateFin->date());

  emit filtrosActualizados(getRegionSeleccionada(), getFechaInicio(),
                           getFechaFin());
}

void WidgetFiltrosGlobales::limpiarFiltros() {
  m_comboRegion->setCurrentIndex(0);
  m_dateInicio->setDate(QDate::currentDate().addMonths(-6));
  m_dateFin->setDate(QDate::currentDate());
  aplicarFiltros();
}
