#pragma once

#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>


class WidgetTablero;

class ExportadorCSV {
public:
  static bool exportarTabla(QTableView *tabla, const QString &nombreArchivo);
  static bool exportarModelo(QStandardItemModel *modelo,
                             const QString &nombreArchivo);
  static void agregarBotonExport(QWidget *widget, QTableView *tabla,
                                 const QString &nombreBase = "datos");
};
