#include "ExportadorCSV.h"
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>
#include <QVBoxLayout>


bool ExportadorCSV::exportarTabla(QTableView *tabla,
                                  const QString &nombreArchivo) {
  if (!tabla || !tabla->model())
    return false;
  return exportarModelo(qobject_cast<QStandardItemModel *>(tabla->model()),
                        nombreArchivo);
}

bool ExportadorCSV::exportarModelo(QStandardItemModel *modelo,
                                   const QString &nombreArchivo) {
  if (!modelo)
    return false;

  QFile file(nombreArchivo);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(nullptr, "Error",
                         "No se pudo crear el archivo: " + nombreArchivo);
    return false;
  }

  QTextStream out(&file);
  out.setEncoding(QStringConverter::Utf8);

  // Headers
  QStringList headers;
  for (int col = 0; col < modelo->columnCount(); ++col) {
    headers << modelo->headerData(col, Qt::Horizontal).toString();
  }
  out << headers.join(",") << "\n";

  // Data
  for (int row = 0; row < modelo->rowCount(); ++row) {
    QStringList rowData;
    for (int col = 0; col < modelo->columnCount(); ++col) {
      QStandardItem *item = modelo->item(row, col);
      QString value = item ? item->text() : "";
      // Escapar comas y comillas
      if (value.contains(",") || value.contains("\"")) {
        value = "\"" + value.replace("\"", "\"\"") + "\"";
      }
      rowData << value;
    }
    out << rowData.join(",") << "\n";
  }

  file.close();
  return true;
}

void ExportadorCSV::agregarBotonExport(QWidget *widget, QTableView *tabla,
                                       const QString &nombreBase) {
  QPushButton *btnExport = new QPushButton("📥 Exportar CSV", widget);
  btnExport->setStyleSheet(R"(
        QPushButton {
            background-color: #27AE60;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: 600;
        }
        QPushButton:hover {
            background-color: #229954;
        }
    )");
  btnExport->setToolTip("Exportar esta tabla a formato CSV");
  btnExport->setCursor(Qt::PointingHandCursor);

  QObject::connect(btnExport, &QPushButton::clicked, [tabla, nombreBase]() {
    QString fileName = QFileDialog::getSaveFileName(
        nullptr, "Exportar a CSV", nombreBase + ".csv",
        "CSV Files (*.csv);;All Files (*)");

    if (!fileName.isEmpty()) {
      if (exportarTabla(tabla, fileName)) {
        QMessageBox::information(
            nullptr, "Éxito", "Datos exportados correctamente a:\n" + fileName);
      }
    }
  });

  // Intentar agregar al layout del widget
  if (widget->layout()) {
    widget->layout()->addWidget(btnExport);
  }
}
