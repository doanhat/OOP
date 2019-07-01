#include "options.h"
#include "ui_options.h"

#include <iostream>

Options::Options(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Options),
  oBase(0), oCp(10000000),
  oBroker(0.10)
{
  ui->setupUi(this);
}

Options::~Options()
{
  delete ui;
}

void Options::updateUi(){
  ui->dateVal->setDate(oToday);
}

void Options::on_btnManu_toggled(bool checked)
{
  Options::setMode(!checked);
}

void Options::on_dateVal_userDateChanged(const QDate &date)
{
  Options::setDate(date);
}

void Options::on_baseVal_valueChanged(int arg1)
{
  Options::setSoldeBase(arg1);
}

void Options::on_cpVal_valueChanged(int arg1)
{
   Options::setSoldeCp(arg1);
}

void Options::on_brokerVal_valueChanged(const double &arg1)
{
  Options::setBroker(arg1);
}
