#include "WidgetHistorial.h"
#include <QDateTime>
#include <QLabel>
#include <QMessageBox>


WidgetHistorial::WidgetHistorial(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(10, 10, 10, 10);
  layout->setSpacing(10);

  // Título
  QLabel *lblTitulo = new QLabel("📜 Historial de Consultas", this);
  lblTitulo->setStyleSheet("font-size: 14px; font-weight: bold; color: "
                           "#2C3E50; margin-bottom: 5px;");
  lblTitulo->setToolTip(
      "Últimas 10 queries ejecutadas. Doble click para re-ejecutar.");
  layout->addWidget(lblTitulo);

  // Lista
  m_listaHistorial = new QListWidget(this);
  m_listaHistorial->setStyleSheet(R"(
        QListWidget {
            background-color: #FFFFFF;
            border: 1px solid #DCE1E7;
            border-radius: 4px;
            font-family: 'Consolas', monospace;
            font-size: 11px;
        }
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #F0F0F0;
        }
        QListWidget::item:hover {
            background-color: #F4F6F9;
        }
        QListWidget::item:selected {
            background-color: #D6EAF8;
            color: #2C3E50;
        }
    )");
  layout->addWidget(m_listaHistorial);

  // Botón limpiar
  m_btnLimpiar = new QPushButton("🗑️ Limpiar Historial", this);
  m_btnLimpiar->setStyleSheet(R"(
        QPushButton {
            background-color: #E74C3C;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 12px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #C0392B;
        }
    )");
  layout->addWidget(m_btnLimpiar);

  connect(m_listaHistorial, &QListWidget::itemDoubleClicked, this,
          &WidgetHistorial::onItemDoubleClick);
  connect(m_btnLimpiar, &QPushButton::clicked, this,
          &WidgetHistorial::limpiarHistorial);

  cargarHistorial();
}

void WidgetHistorial::agregarQuery(const QString &query, const QString &tipo) {
  QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
  QString itemText = QString("[%1] %2: %3").arg(timestamp).arg(tipo).arg(query);

  m_listaHistorial->insertItem(0, itemText);

  // Mantener solo MAX_HISTORIAL items
  while (m_listaHistorial->count() > MAX_HISTORIAL) {
    delete m_listaHistorial->takeItem(MAX_HISTORIAL);
  }

  guardarHistorial();
}

void WidgetHistorial::limpiarHistorial() {
  auto reply = QMessageBox::question(this, "Confirmar",
                                     "¿Desea limpiar todo el historial?",
                                     QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    m_listaHistorial->clear();
    guardarHistorial();
  }
}

void WidgetHistorial::onItemDoubleClick(QListWidgetItem *item) {
  if (!item)
    return;

  QString fullText = item->text();
  // Extraer query (después del segundo ":")
  int idx = fullText.indexOf(":", fullText.indexOf(":") + 1);
  if (idx != -1) {
    QString query = fullText.mid(idx + 2).trimmed();
    emit querySeleccionada(query);
  }
}

void WidgetHistorial::cargarHistorial() {
  QSettings settings("OLAP2025", "Sistema");
  QStringList historial = settings.value("historial/queries").toStringList();

  for (const QString &item : historial) {
    m_listaHistorial->addItem(item);
  }
}

void WidgetHistorial::guardarHistorial() {
  QSettings settings("OLAP2025", "Sistema");
  QStringList historial;

  for (int i = 0; i < m_listaHistorial->count(); ++i) {
    historial.append(m_listaHistorial->item(i)->text());
  }

  settings.setValue("historial/queries", historial);
}
