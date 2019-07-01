#include "trading.h"
#include "mainwindow.h"


Devise::Devise(const QString& c, const QString& m, double value, const QString& z) :
    monnaie(m), zone(z), montant(value) {
    if (c.size() != 3) throw TradingException("code devise incorrect");
    // for (unsigned int i = 0; i < 3; i++) {
    //     if (c[i] <= 'A' || c[i] >= 'Z') throw TradingException("code devise incorrect");
    // }
    code = c;
}

PaireDevises::PaireDevises( Devise* b,  Devise* c, const QString& s) :
    base(b), contrepartie(c), surnom(s) {}

QString PaireDevises::toString() const{
    QString str;
    str = getBase().getCode() + "/" + getContrepartie().getCode();
    if (getSurnom()!="") str += "(" + getSurnom() + ")";
    return str;
}

void CoursOHLCV::setCours(double o, double h, double l, double c, double v) {
    if (o < 0 || h < 0 || l < 0 || c<0 || l>h) throw TradingException("cours OHLC incorrect");
        open = o; high = h; low = l; close = c; volume=v;
}

CoursOHLCV::CoursOHLCV(double o, double h, double l, double c, double v, const QDate& d):date(d) {
    if (o < 0 || h < 0 || l < 0 || c<0 || l>h) throw TradingException("cours OHLC incorrect");
        open = o; high = h; low = l; close = c; volume=v;
}

void EvolutionCours::addCours(double o, double h, double l, double c, double v, const QDate& d) {
    if (nbMaxCours == nbCours) { // agrandissement du tableau
        CoursOHLCV* newtab = new CoursOHLCV[nbMaxCours + 100];
        for (unsigned int i = 0; i < nbCours; i++) newtab[i] = cours[i];
        // mise à jour des attributs
        CoursOHLCV* old = cours;
        cours = newtab;
        nbMaxCours += 100;
        delete[] old; // destruction de l'ancien tableau
    }
    // ajout du cours
    cours[nbCours].setCours(o, h, l, c, v);
    cours[nbCours].setDate(d);
    nbCours++;
}

EvolutionCours::~EvolutionCours() {
    delete[] cours;
}

EvolutionCours::EvolutionCours(const EvolutionCours& e) :
    paire(e.paire),
    cours(new CoursOHLCV[e.nbCours]),
    nbCours(e.nbCours),
    nbMaxCours(e.nbCours)
{
    for (unsigned int i = 0; i < nbCours; i++) cours[i] = e.cours[i];
}

EvolutionCours& EvolutionCours::operator=(const EvolutionCours& e) {
    if (this != &e) { // se protéger de l'auto-affectation
        paire = e.paire;
        nbCours = 0;
        for (unsigned int i = 0; i < e.nbCours; i++)
            addCours(e.cours[i].getOpen(), e.cours[i].getHigh(),e.cours[i].getLow(), e.cours[i].getClose(),e.cours[i].getVolume(),e.cours[i].getDate());
    }
    return *this;
}

const CoursOHLCV& EvolutionCours::getCours(const QDate& d) const {
    if(d.isValid()) {
        for(const_iterator it=begin(); it!=end(); ++it) {
            if(d.daysTo(it->getDate())==0)
                return *it;
        }
      } else {
        throw TradingException("Cours Introuvable!");
      }
}

void CSVReader::readFile() {
  // Get rid of first line
  getline(file, line);

  while(getline(file, line)){
    std::stringstream ss(line);
    getline(ss, sdate, ',');
    getline(ss, sopen, ',');
    getline(ss, shigh, ',');
    getline(ss, slow, ',');
    getline(ss, sclose, ',');
    getline(ss, sadjclose, ',');
    getline(ss, svolume);

    // std::cout << sdate << ":" << sopen << std::endl;

    e->addCours(std::stod(sopen, nullptr), std::stod(shigh, nullptr),
                std::stod(slow, nullptr), std::stod(sclose, nullptr),
                std::stod(svolume, nullptr),
                QDate::fromString(QString::fromStdString(sdate), "yyyy-MM-dd"));
  }
}

bool::Transaction::updateBalance(Devise* b, Devise* c, EvolutionCours *e, double broker){
    if(type==0)
    {
        if ( (c->getMontant() - getSum() * e->getCours((date)).getOpen()) < 0){
            QMessageBox errorBox;
            errorBox.setText("Opération invalide.");
            errorBox.exec();
            return false;
          }
        b->setMontant( (1-broker) * getSum() + b->getMontant());
        c->setMontant( c->getMontant() - getSum() * e->getCours((date)).getOpen() );

        return true;
    }
        else if(type==1){
        if (b->getMontant()-getSum()<0)
        {
            QMessageBox errorBox;
            errorBox.setText("Opération invalide.");
            errorBox.exec();
            return false;
        }

        b->setMontant(b->getMontant()-getSum());
        c->setMontant(c->getMontant() + getSum() / e->getCours(date).getClose() * (1-broker));

        return true;
      }

    }

Devise& Transaction::getBase(EvolutionCours * e) const {
  return e->getPaireDevises().getBase();
}

Devise& Transaction::getContrePartie(EvolutionCours * e) const {
  return e->getPaireDevises().getContrepartie();
}

double Transaction::getClosePrice(EvolutionCours *e) const
{
  return e->getCours(MainWindow::getDate()).getClose();
}


void Historique::update_transaction(double montant, QDate date)
{
    if (montant < 0) {
        Transaction* t = new Transaction(-montant,true,date);
        addTransaction(t);
    }
    else if (montant >0) {
        Transaction* t = new Transaction(montant,false,date);
        addTransaction(t);
    }
}
