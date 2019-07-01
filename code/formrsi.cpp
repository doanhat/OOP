#include "formrsi.h"
#include "ui_formrsi.h"

Formrsi::Formrsi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Formrsi)
{
    ui->setupUi(this);
}

Formrsi::~Formrsi()
{
    delete ui;
}
