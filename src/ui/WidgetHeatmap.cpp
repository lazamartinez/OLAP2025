#include "WidgetHeatmap.h"
#include <QRandomGenerator>
#include <QTextCharFormat>

WidgetHeatmap::WidgetHeatmap(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QLabel *titulo = new QLabel("📅 Heatmap Temporal de Ventas", this);
  titulo->setStyleSheet("font-size: 18px; font-weight: bold; color: #2C3E50; "
                        "margin-bottom: 10px;");
  titulo->setToolTip(
      "<b>Heatmap Temporal</b><br>Visualización de intensidad de ventas por "
      "día.<br>Verde = bajo, Amarillo = medio, Rojo = alto");
  layout->addWidget(titulo);

  m_calendar = new QCalendarWidget(this);
  m_calendar->setGridVisible(true);
  m_calendar->setMinimumDate(QDate(2024, 1, 1));
  m_calendar->setMaximumDate(QDate::currentDate());

  // Estilo del calendario
  m_calendar->setStyleSheet(R"(
        QCalendarWidget QWidget {
            background-color: white;
        }
        QCalendarWidget QTableView {
            selection-background-color: #3498DB;
            font-size: 12px;
        }
        QCalendarWidget QToolButton {
            color: #2C3E50;
            background-color: #ECF0F1;
            border-radius: 4px;
            padding: 5px;
        }
        QCalendarWidget QToolButton:hover {
            background-color: #3498DB;
            color: white;
        }
    )");

  layout->addWidget(m_calendar);

  // Generar datos de prueba
  QMap<QDate, double> datosDemo;
  QDate inicio(2024, 1, 1);
  for (int i = 0; i < 365; ++i) {
    QDate fecha = inicio.addDays(i);
    double valor = 5000.0 + QRandomGenerator::global()->bounded(45000);
    datosDemo[fecha] = valor;
  }

  setDatos(datosDemo);
}

void WidgetHeatmap::setDatos(const QMap<QDate, double> &datos) {
  m_datos = datos;

  // Calcular min/max
  if (!datos.isEmpty()) {
    auto valores = datos.values();
    m_min = *std::min_element(valores.begin(), valores.end());
    m_max = *std::max_element(valores.begin(), valores.end());
  }

  // Aplicar colores al calendario
  for (auto it = m_datos.begin(); it != m_datos.end(); ++it) {
    QTextCharFormat format;
    format.setBackground(obtenerColor(it.value()));
    format.setForeground(Qt::white);
    format.setFontWeight(QFont::Bold);
    m_calendar->setDateTextFormat(it.key(), format);
  }
}

void WidgetHeatmap::setRango(double min, double max) {
  m_min = min;
  m_max = max;
}

QColor WidgetHeatmap::obtenerColor(double valor) {
  // Normalizar valor entre 0 y 1
  double normalizado = (valor - m_min) / (m_max - m_min);

  // Gradiente: Verde -> Amarillo -> Rojo
  if (normalizado < 0.33) {
    // Verde a Amarillo
    double t = normalizado / 0.33;
    return QColor::fromRgbF(t, 1.0, 0.0);
  } else if (normalizado < 0.66) {
    // Amarillo a Naranja
    double t = (normalizado - 0.33) / 0.33;
    return QColor::fromRgbF(1.0, 1.0 - t * 0.5, 0.0);
  } else {
    // Naranja a Rojo
    double t = (normalizado - 0.66) / 0.34;
    return QColor::fromRgbF(1.0, 0.5 - t * 0.5, 0.0);
  }
}

void WidgetHeatmap::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
}
