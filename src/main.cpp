#include "ui/VentanaPrincipal.h"
#include <QApplication>
#include <QStyleFactory>
#include <iostream>

void aplicarEstiloVisual(QApplication &app) {
  // Estilo Base: Fusion (Limpio y cross-platform)
  app.setStyle(QStyleFactory::create("Fusion"));

  QPalette palette;
  QColor windowColor(245, 247, 250); // Off-white
  QColor textColor(44, 62, 80);      // Dark Blue-Grey

  palette.setColor(QPalette::Window, windowColor);
  palette.setColor(QPalette::WindowText, textColor);
  palette.setColor(QPalette::Base, Qt::white);
  palette.setColor(QPalette::AlternateBase, windowColor);
  palette.setColor(QPalette::ToolTipBase, Qt::white);
  palette.setColor(QPalette::ToolTipText, textColor);
  palette.setColor(QPalette::Text, textColor);
  palette.setColor(QPalette::Button, Qt::white);
  palette.setColor(QPalette::ButtonText, textColor);
  palette.setColor(QPalette::Link, QColor(41, 128, 185));
  palette.setColor(QPalette::Highlight, QColor(41, 128, 185));
  palette.setColor(QPalette::HighlightedText, Qt::white);

  app.setPalette(palette);

  // Global defaults for standard controls not covered by specific stylesheets
  app.setStyleSheet(R"(
      QToolTip {
          color: #2C3E50;
          background-color: #ffffff;
          border: 1px solid #DCE1E7;
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
