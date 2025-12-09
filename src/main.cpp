#include "ui/VentanaPrincipal.h"
#include <QApplication>
#include <QStyleFactory>
#include <iostream>

void aplicarEstiloVisual(QApplication &app) {
  // Estilo Moderno Oscuro (Dark Theme Premium)
  app.setStyle(QStyleFactory::create("Fusion"));

  QPalette darkPalette;
  QColor darkGray(53, 53, 53);
  QColor gray(90, 90, 90);
  QColor black(25, 25, 25);
  QColor blue(42, 130, 218);

  darkPalette.setColor(QPalette::Window, darkGray);
  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Base, black);
  darkPalette.setColor(QPalette::AlternateBase, darkGray);
  darkPalette.setColor(QPalette::ToolTipBase, blue);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Button, darkGray);
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::Link, blue);
  darkPalette.setColor(QPalette::Highlight, blue);
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);

  app.setPalette(darkPalette);

  // Hoja de Estilos Premium (CSS)
  app.setStyleSheet(
      R"(
        QWidget {
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            font-size: 10pt;
        }

        QMainWindow {
            background-color: #1e1e1e;
        }

        /* --- Tabs --- */
        QTabWidget::pane {
            border: 1px solid #333;
            background: #252526;
            border-radius: 5px;
            margin-top: -1px;
        }
        QTabBar::tab {
            background: #2d2d30;
            color: #ccc;
            padding: 8px 20px;
            border: 1px solid #333;
            border-bottom: none;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            min-width: 120px;
            font-weight: bold;
        }
        QTabBar::tab:selected {
            background: #2a82da;
            color: white;
            border-bottom: 2px solid #2a82da;
        }
        QTabBar::tab:hover {
            background: #3e3e42;
        }

        /* --- Buttons --- */
        QPushButton {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3e3e42, stop:1 #2d2d30);
            color: white;
            border: 1px solid #555;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #505050;
            border-color: #2a82da;
        }
        QPushButton:pressed {
            background-color: #2a82da;
            color: white;
        }

        /* --- GroupBox --- */
        QGroupBox {
            border: 1px solid #444;
            border-radius: 6px;
            margin-top: 20px;
            padding: 15px;
            background-color: #252526;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 5px;
            color: #2a82da;
            font-weight: bold;
            font-size: 11pt;
        }

        /* --- Tables --- */
        QTableView {
            background-color: #1e1e1e;
            color: #ddd;
            gridline-color: #333;
            selection-background-color: #2a82da;
            selection-color: white;
            border: 1px solid #333;
            border-radius: 4px;
        }
        QHeaderView::section {
            background-color: #2d2d30;
            color: #ccc;
            padding: 6px;
            border: 1px solid #333;
            font-weight: bold;
        }

        /* --- Inputs --- */
        QLineEdit, QTextEdit, QSpinBox, QComboBox {
            background-color: #333337;
            border: 1px solid #444;
            color: white;
            padding: 6px;
            border-radius: 4px;
        }
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid #2a82da;
        }
        
        /* --- ProgressBar --- */
        QProgressBar {
            border: 1px solid #444;
            border-radius: 4px;
            text-align: center;
            background-color: #2d2d30;
        }
        QProgressBar::chunk {
            background-color: #2a82da;
            width: 10px;
            margin: 0.5px;
        }
    )");
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
