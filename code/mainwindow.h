#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QDialog>
#include <QTimer>

#include "ui_mainwindow.h"

#include "action.h"
#include "formmacd.h"
#include "formrsi.h"
#include "graphique.h"
#include "indicateur.h"
#include "LoadSave.h"
#include "modeauto.h"
#include "options.h"
#include "strategie.h"
#include "textedit.h"
#include "trading.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void setSoldeBase(const double& b) { soldeBase = b; }
  void setSoldeCp(const double& c) { soldeCp = c; }
  void setDate(const QDate& date) { today = date; }
  void setBroker(const double& i){ broker=i; }

  static QDate getDate();

  QString getDBase() const { return base; }
  QString getDC() const { return cp; }
  double getBaseInitial() const { return soldeBase; }
  double getCpInitial() const { return soldeCp; }
  double getBroker() const { return broker; }

private slots:

  void newData();
  void autoMode();
  void setOptions();
  void drawGraph();
  void updateGraph();
  void addTable();

  void on_actionNouvelle_partie_triggered();
  void on_actionQuitter_triggered();
  void on_pushButton_clicked();
  void on_acheterBtn_clicked();
  void on_vendreBtn_clicked();

  void transaction_update();


  void on_pushButton_2_clicked();
  void on_pushButton_3_pressed();
  void on_pushButton_3_released();
  void on_pushButton_4_clicked();

  void on_actionSauvegarder_triggered();

  void on_actionCharger_partie_triggered();

private:
  // Static attributes
  static bool firstLaunch;
  static QDate today;

  // Windows attributes
  Ui::MainWindow *ui;
  TextEdit *textEdit;
  Action *sell;
  Options *options;
  ModeAuto *modeauto;
  Historique *historique;

  // Layout attributes
  QVBoxLayout *chartsLayout;

  // Data attribues
  EvolutionCours* e; // L'ensemble du fichier est chargé ici
  Devise* DBase; // Information sur la devise de base
  Devise* DContre; // Information sur la devise de contrepartie
  double soldeBase; // Information du solde de la devise de base
  double soldeCp; // Information du solde de la devise de contrepartie
  double broker; // Part du broker
  double value; // ???
  bool mode; // Manuel = 0; Automatique = 1
  bool etatEMA; // Cacher = 0; Afficher = 1
  QStringList filepath; // Chemin jusqu'au fichier des données
  QString filename; // Nom du fichier des données
  QString base; // Nom de la devise de base
  QString cp; // Nom de la devise de contrepartie
  QDateTime now; // ???
  QDateTime t; // ???

  // Graphs/Forms attributes
  Graphique_Chandelier *ev; // Graphique représentant les bougies
  FormMACD *macdChart; // Fenêtre représentant l'indicateur MACD
  Formrsi *rsiChart; // Fenêtre représentant l'indicateur RSI

  // Automatic play
  Robot *robot;
};

#endif // MAINWINDOW_H
