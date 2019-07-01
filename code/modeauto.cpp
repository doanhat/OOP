#include "modeauto.h"
#include "ui_modeauto.h"

ModeAuto::ModeAuto(QWidget *parent) : QDialog(parent), ui(new Ui::ModeAuto) {
    ui->setupUi(this);

    ui->strategie->addItem("EMA_Convergence", "EMA_Convergence");
    ui->strategie->addItem("RSI_Strategie", "RSI_Strategie");
    ui->strategie->addItem("MACD_Strategie", "MACD_Strategie");
    ui->strategie->setCurrentText("EMA_Convergence");
}

ModeAuto::~ModeAuto() {
    delete ui;
}

void ModeAuto::on_depart_clicked(const QDate &date)
{
    setDateDepart(date);

}

void ModeAuto::on_fin_clicked(const QDate &date)
{
  setDateFin(date);
}

void ModeAuto::on_combo_strategie_currentIndexChanged(const QString& name)
{
    setStrategie(name);
}

void ModeAuto::setDateDepart(const QDate& date){
    debut = date;
}

void ModeAuto::setDateFin(const QDate& date){
   fin = date;
}

void ModeAuto::setStrategie(const QString& string) {
    type_str = string;
}

void ModeAuto::on_buttonBox_accepted()
{

}
