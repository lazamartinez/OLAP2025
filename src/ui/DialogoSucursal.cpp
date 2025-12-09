#include "DialogoSucursal.h"

DialogoSucursal::DialogoSucursal(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Nueva Sucursal - GeoLocalización");

  QFormLayout *form = new QFormLayout;
  m_nombre = new QLineEdit;
  m_lat = new QDoubleSpinBox;
  m_lat->setRange(-90, 90);
  m_lat->setDecimals(4);
  m_lon = new QDoubleSpinBox;
  m_lon->setRange(-180, 180);
  m_lon->setDecimals(4);

  form->addRow("Ciudad / Nombre:", m_nombre);
  form->addRow("Latitud:", m_lat);
  form->addRow("Longitud:", m_lon);

  QDialogButtonBox *btns =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(btns, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(btns, &QDialogButtonBox::rejected, this, &QDialog::reject);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addLayout(form);
  layout->addWidget(btns);
}

QString DialogoSucursal::getNombre() const { return m_nombre->text(); }

double DialogoSucursal::getLat() const { return m_lat->value(); }

double DialogoSucursal::getLon() const { return m_lon->value(); }
