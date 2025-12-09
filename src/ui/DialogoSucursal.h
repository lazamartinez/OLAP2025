#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class DialogoSucursal : public QDialog {
  Q_OBJECT
public:
  explicit DialogoSucursal(QWidget *parent = nullptr);

  QString getNombre() const;
  double getLat() const;
  double getLon() const;

private:
  QLineEdit *m_nombre;
  QDoubleSpinBox *m_lat;
  QDoubleSpinBox *m_lon;
};
