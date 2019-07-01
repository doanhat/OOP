#ifndef FORMRSI_H
#define FORMRSI_H
/*!
* \file formrsi.h
* \brief les classes pour afficher la fenetre RSI
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

class Formrsi;
}
/**
\class Formrsi
*/
class Formrsi : public QWidget
{
    Q_OBJECT

public:
    //! constructeur
    /**
    *\param parent : QWidget*
    */
    explicit Formrsi(QWidget *parent = nullptr);
    //! destructeur
    ~Formrsi();

private:
    Ui::Formrsi *ui;/**< ui*/
};

#endif // FORMRSI_H
