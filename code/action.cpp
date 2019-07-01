#include "action.h"
#include "ui_action.h"

Action::Action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Action),
    value(0)
{
    ui->setupUi(this);
}

Action::~Action()
{
    delete ui;
}


void Action::on_buttonBox_accepted()
{
}

void Action::on_selectedAmount_textChanged(const QString &arg1)
{
    double val = arg1.toDouble();
    Action::setValue(val);
}
