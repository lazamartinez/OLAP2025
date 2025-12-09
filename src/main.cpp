#include "ui/VentanaPrincipal.h"
#include <QApplication>
#include <QStyleFactory>
#include <iostream>

void aplicarEstiloVisual(QApplication &app) {
  // Estilo Moderno Oscuro (Dark Theme)
  app.setStyle(QStyleFactory::create("Fusion"));

  QPalette darkPalette;
  darkPalette.setColor(QPalette::Window, QColor(45, 45, 45));
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
  darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);

  app.setPalette(darkPalette);

  // Hoja de Estilos adicional para bordes y detalles
  app.setStyleSheet(
      "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid "
      "white; }"
      "QMainWindow { background-color: #2d2d2d; }"
      "QTabWidget::pane { border: 1px solid #444; top: -1px; }"
      "QTabBar::tab { background: #333; color: #AAA; padding: 8px; border: 1px "
      "solid #444; border-bottom: none; border-top-left-radius: 4px; "
      "border-top-right-radius: 4px; min-width: 100px; }"
      "QTabBar::tab:selected { background: #555; color: white; margin-bottom: "
      "-1px; }"
      "QPushButton { background-color: #3b3b3b; color: white; border: 1px "
      "solid #555; padding: 6px; border-radius: 4px; }"
      "QPushButton:hover { background-color: #505050; border: 1px solid "
      "#2a82da; }"
      "QPushButton:pressed { background-color: #2a82da; color: black; }"
      "QLineEdit { background-color: #1e1e1e; border: 1px solid #555; color: "
      "white; padding: 4px; border-radius: 3px; }"
      "QTextEdit { background-color: #1e1e1e; border: 1px solid #555; color: "
      "#eee; border-radius: 3px; }"
      "QTableView { background-color: #1e1e1e; color: white; gridline-color: "
      "#444; selection-background-color: #2a82da; }"
      "QHeaderView::section { background-color: #333; color: white; padding: "
      "4px; border: 1px solid #444; }");
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  aplicarEstiloVisual(app);

  VentanaPrincipal ventana;
  ventana.show();

  std::cout << "Sistema Iniciado en Español. Estilo Visual Aplicado."
            << std::endl;

  return app.exec();
}
