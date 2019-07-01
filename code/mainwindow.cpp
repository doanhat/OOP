#include "mainwindow.h"

bool MainWindow::firstLaunch = 1;
QDate MainWindow::today = QDate::currentDate();

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  soldeBase(0), soldeCp(0),
  broker(0), value(0),
  mode(false), etatEMA(0), robot(nullptr)
{
  ui->setupUi(this);
  historique = new Historique();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::newData()
{
  DBase=new Devise(base, "Base", 0, "Loc");
  DContre=new Devise(cp, "Contrepartie", 0, "Loc");
  PaireDevises *paire = new PaireDevises(DBase, DContre, " ");
  e = new EvolutionCours(*paire);
  CSVReader reader(e, filepath);
  reader.readFile();

  MainWindow::setOptions();
}

void MainWindow::setOptions()
{
  options = new Options;
  options->setModal(true);
  options->setDate(e->cbegin()->getDate());
  options->exec();

  setDate(options->getToday());
  setSoldeCp(options->getCp());
  setSoldeBase(options->getBase());
  setBroker(options->getBroker());

  mode = options->getMode();

  e->getPaireDevises().getBase().setMontant(options->getBase());
  e->getPaireDevises().getContrepartie().setMontant(options->getCp());
    /*
  if (mode == true) {
      MainWindow::autoMode();
  }
  */
  MainWindow::drawGraph();
}

void MainWindow::autoMode() {
// Init robot
  modeauto = new ModeAuto;
  modeauto->setModal(true);
  if (modeauto->exec() == QDialog::Accepted) {
    robot = new Robot(e);
      robot->setStrategie(modeauto->getStrategie());
      robot->setDateDepart(modeauto->getDateDepart());
      robot->setDateFin(modeauto->getDateFin());
      QObject::connect(robot,SIGNAL(transactionChanged(double,QDate)), historique, SLOT(update_transaction(double,QDate)));
  }
  else {
    mode = false;
  }
}


void MainWindow::drawGraph(){
  ev = new Graphique_Chandelier(*e, today);
  Graphique_Barre *vol = new Graphique_Barre(*e, today);
  //indicator
  //Graphique_EMA *gema = new Graphique_EMA(*e, today);
  //Graphique_MACD *gmacd = new Graphique_MACD(*e, today);
  //Graphique_RSI *grsi = new Graphique_RSI(*e,today);
  chartsLayout = new QVBoxLayout(this);

  //QStackedLayout * indicLayout = new QStackedLayout();
  //indicLayout->setStackingMode(QStackedLayout::StackAll);
  //indicLayout->addWidget(gema);
  //indicLayout->addWidget(gmacd);

  chartsLayout->addWidget(ev);
  //chartsLayout->addLayout(indicLayout);
  //chartsLayout->addWidget(gmacd);
  chartsLayout->addWidget(vol);
  //chartsLayout->addWidget(grsi);
  //indicator

  //
  chartsLayout->setStretchFactor(ev, 2);
  //chartsLayout->setStretchFactor(grsi,2);
  //chartsLayout->setStretchFactor(indicLayout, 1);
  chartsLayout->setStretchFactor(vol, 1);

  ui->tab->setLayout(chartsLayout);
  ui->dateTxt->setDate(today);
  ui->baseVal->setText(QString::number(soldeBase, 'f', 5));
  ui->cpVal->setText(QString::number(soldeCp, 'f', 5));

  if (robot != nullptr) {
      // start Robot
      robot->releaseStrategie();

  }
}

void MainWindow::updateGraph(){
  // qDebug() << chartsLayout->itemAt(0)->widget();
  // qDebug() << chartsLayout->itemAt(1)->widget();
  QWidget* oldEv = chartsLayout->itemAt(0)->widget();
  QWidget* oldVol = chartsLayout->itemAt(1)->widget();

  Graphique_Chandelier* newEv = new Graphique_Chandelier(*e, today);
  Graphique_Barre* newVol = new Graphique_Barre(*e, today);

  chartsLayout->replaceWidget(chartsLayout->itemAt(0)->widget(), newEv);
  chartsLayout->replaceWidget(chartsLayout->itemAt(1)->widget(), newVol);

  delete oldEv;
  delete oldVol;
}

void MainWindow::addTable() {
    qDeleteAll(ui->tab_2->children());
    QVBoxLayout* tabLayout = new QVBoxLayout(this);
    QTableWidget* table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setRowCount(1);
    unsigned int row=0;
    table->setHorizontalHeaderLabels(QStringList()<<"Date"<<"Action"<<"Base"<<"Contrepartie"<<"ROI");

    for(std::vector<Transaction*>::iterator it=historique->begin(); it!=historique->end(); ++it)
    {
        table->insertRow(row);
        QTableWidgetItem *date = new QTableWidgetItem();
        date->setText((*it)->getDate().toString("dd/MM/yyyy"));
        date->setFlags(date->flags() ^ Qt::ItemIsEditable);
        table->setItem(row, 0, date);

        QTableWidgetItem *action = new QTableWidgetItem();
        action->setText((*it)->getType() ? "Vente" : "Achat");
        action->setFlags(action->flags() ^ Qt::ItemIsEditable);
        table->setItem(row, 1, action);

        QTableWidgetItem *base = new QTableWidgetItem();
        base->setText(QString::number((*it)->getBase(e).getMontant()-(*it)->getSum()));
        base->setFlags(base->flags() ^ Qt::ItemIsEditable);
        table->setItem(row, 2, base);

        QTableWidgetItem *cp = new QTableWidgetItem();
        cp->setText(QString::number((*it)->getContrePartie(e).getMontant()-(*it)->getSum()));
        cp->setFlags(cp->flags() ^ Qt::ItemIsEditable);
        table->setItem(row, 3, cp);

        QTableWidgetItem *roi = new QTableWidgetItem();
        roi->setText(QString::number((((*it)->getBase(e).getMontant())-options->getBase())/options->getBase()));
        roi->setFlags(roi->flags() ^ Qt::ItemIsEditable);
        table->setItem(row, 4, roi);

        row++;
    }
    table->removeRow(row);
    tabLayout->addWidget(table);
    ui->tab_2->setLayout(tabLayout);
}

void MainWindow::on_actionNouvelle_partie_triggered()
{
  if (firstLaunch){
      firstLaunch = 0;
      QFileDialog dialog(this);
      dialog.setFileMode(QFileDialog::ExistingFile);
      dialog.setViewMode(QFileDialog::Detail);
      dialog.setNameFilter("*.csv");

      if (dialog.exec())
        filepath = dialog.selectedFiles();

      QFileInfo fileinfo(QString::fromStdString(filepath.toStdList().begin()->toStdString()));
      filename = fileinfo.fileName();

      base = filename.left(3);
      cp = filename.mid(4,3);

      // cout << "Base: " << base.toStdString() << " Cp: " << cp.toStdString() << "\n";

      MainWindow::newData();
    } else {
      // TODOS :
      // Exécuter la sauvegarde
      // Ouvrir une boite de dialogue pour confirmer le début de la nouvelle partie
      // Le bouton lancera l'action Nouvelle partie
      QMessageBox errorBox;
      errorBox.setText("Partie déjà chargée.");
      errorBox.exec();
    }
}

void MainWindow::on_actionQuitter_triggered()
{
   this->close();
}

void MainWindow::on_pushButton_clicked()
{
    textEdit=new TextEdit(this);
    textEdit->show();
}

void MainWindow::on_acheterBtn_clicked() {
  Action buy;
  buy.setModal(true);
  if ( buy.exec() == QDialog::Accepted ){
      Transaction* achat=new Transaction(buy.getValue(),0, getDate());
       if(achat->updateBalance(DBase,DContre,e,getBroker()))
       {
           transaction_update();
           historique->addTransaction(achat);
           addTable();
       }
  }
 /* {
      double value = getCp() - buy.getValue() * e->getCours(getDate()).getOpen();



      if (value < 0){
          QMessageBox * errBox = new QMessageBox();
          errBox->setText("Solde insuffisant");
          errBox->exec();
          delete errBox;
        } else {
          setSoldeBase(getBase() + buy.getValue() * (1-getBroker()));
          setSoldeCp(value);
          ui->baseVal->setText(QString::number(getBase(), 'f', 2));
          ui->cpVal->setText(QString::number(getCp(), 'f', 2));
          setDate(getDate().addDays(1));
          ui->dateTxt->setDate(getDate());

          updateGraph();
        }
    } */
}

void MainWindow::on_vendreBtn_clicked()
{
    Action sell;
    sell.setModal(true);

        if ( sell.exec() == QDialog::Accepted ){
            Transaction* vente=new Transaction(sell.getValue(),1,getDate());
             if(vente->updateBalance(DBase,DContre,e,getBroker()))
             {
                 transaction_update();
                 historique->addTransaction(vente);
                 addTable();
             }
        }

      /*
      double value = getBase() - sell.getValue();
      if (value < 0){
          QMessageBox * errBox = new QMessageBox();
          errBox->setText("Solde insuffisant");
          errBox->exec();
          delete errBox;
        } else {
          setSoldeBase(value);
          setSoldeCp(getCp() + sell.getValue() / e->getCours(getDate()).getClose() * (1-getBroker()));
          ui->baseVal->setText(QString::number(getBase(), 'f', 2));
          ui->cpVal->setText(QString::number(getCp(), 'f', 2));
          setDate(getDate().addDays(1));
          ui->dateTxt->setDate(getDate());

          updateGraph();
        }
 */
    }

void MainWindow::transaction_update(){

    ui->baseVal->setText(QString::number(e->getPaireDevises().getBase().getMontant(), 'f', 2));
    ui->cpVal->setText(QString::number(e->getPaireDevises().getContrepartie().getMontant(), 'f', 2));
}

void MainWindow::on_pushButton_2_clicked()
{
    Graphique_MACD *gmacd = new Graphique_MACD(*e, today,this);
    chartsLayout = new QVBoxLayout(this);
    chartsLayout->addWidget(gmacd);


    macdChart = new FormMACD;
    macdChart->setLayout(chartsLayout);
    macdChart->show();
}



void MainWindow::on_pushButton_3_pressed()
{
    // ev->activateEMA(true);
}

void MainWindow::on_pushButton_3_released()
{
  if (!etatEMA){
      ev->activateEMA(true);
      etatEMA = 1;
    }
  else {
      ev->activateEMA(false);
      etatEMA = 0;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    Graphique_RSI *grsi = new Graphique_RSI(*e,today,this);
    chartsLayout = new QVBoxLayout(this);
    chartsLayout->addWidget(grsi);

    rsiChart = new Formrsi;
    rsiChart->setLayout(chartsLayout);
    rsiChart->show();
}

QDate MainWindow::getDate() {
  return today;
}

void MainWindow::on_actionSauvegarder_triggered()
{
    QString filename;
    filename="save_"+QDate::currentDate().toString("yyyy_MM_dd")+".xml";
    LoadSave* ls=new LoadSave(filename);
    ls->save(historique->getHistorique(),textEdit->getNote()->getText());
    delete ls;
}

void MainWindow::on_actionCharger_partie_triggered()
{
    QStringList filepath;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter("*.xml");


    if (dialog.exec())
      filepath = dialog.selectedFiles();
    QFileInfo fileinfo(QString::fromStdString(filepath.toStdList().begin()->toStdString()));
    filename = fileinfo.fileName();

    LoadSave* ls=new LoadSave(filename);
    ls->load();
    historique=new Historique;
    historique->setHistorique(ls->getTransList());
    textEdit->setNote(ls->getNoteList());
    delete ls;

}
