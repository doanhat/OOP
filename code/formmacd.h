#ifndef FORMMACD_H
#define FORMMACD_H
/*!
* \file formMACD.h
* \brief les classes pour afficher la fenetre MACD
*/
#include <QWidget>
#include <QMainWindow>
#include <QtCharts>
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

#include "options.h"
#include "trading.h"
#include "graphique.h"
#include "textedit.h"
#include "action.h"
/*
#include "ui_mainwindow.h"
*/
/**
*\namespace Ui
*/
namespace Ui {
class FormMACD;
}
/**
*\class FormMACD
*/
class FormMACD : public QWidget
{
    Q_OBJECT

public:
    //! methode FormMACD
    /**
    *\param parent : QWidget*
    */
    explicit FormMACD(QWidget *parent = nullptr);
    //! desstructeur
    ~FormMACD();

private slots:
    //void showChart(EvolutionCours* e,QDate* d);

    //void on_pushButton_clicked();

private:
    Ui::FormMACD *ui;/**< ui*/
    //QVBoxLayout *chartsLayout;
};

#endif // FORMMACD_H
