#include "formmacd.h"
#include "ui_formmacd.h"

FormMACD::FormMACD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMACD)
{
    ui->setupUi(this);
}

FormMACD::~FormMACD()
{
    delete ui;
}



