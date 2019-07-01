#ifndef TRADING_H
#define TRADING_H

#include <QString>
#include <QDate>
#include <QMessageBox>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <QString>

using namespace std;

class TradingException {
public:
  TradingException(const QString& message) :info(message) {}
  QString getInfo() const { return info; }
private:
  QString info;
};

class Devise {
  QString code;
  QString monnaie, zone;
  double montant;

public:
  Devise(const QString& c, const QString& m,double value, const QString& z = "");
  ~Devise() = default;
  QString getCode() const { return code; }
  const QString& getMonnaie() const { return monnaie; }
  const QString& getZoneGeographique() const { return zone; }
  double getMontant() const { return montant; }
  void setMontant(const double& value) { montant=value; }
};

class PaireDevises {
  Devise* base;
  Devise* contrepartie;
  QString surnom;

public:
  PaireDevises( Devise* b,  Devise* c, const QString& s = "");
  ~PaireDevises() = default;
  Devise& getBase() const { return *base; }
  Devise& getContrepartie() const { return *contrepartie; }
  const QString& getSurnom() const { return surnom; }
  // void setSurnom(const QString& base, const QString& cp);
  QString toString() const;
};

class CoursOHLCV {
  double open = 0, high = 0, low = 0, close = 0, volume=0;
  QDate date;
public:
  CoursOHLCV() {}
  CoursOHLCV(double o, double h, double l, double c, double v, const QDate& d);
  double getOpen() const { return open; }
  double getHigh() const { return high; }
  double getLow() const { return low; }
  double getClose() const { return close; }
  double getVolume() const { return volume; }
  void setCours(double o, double h, double l, double c, double v);
  QDate getDate() const { return date; }
  void setDate(const QDate& d) { date=d; }
};

class EvolutionCours {
  const PaireDevises* paire;
  CoursOHLCV* cours = nullptr;
  unsigned int nbCours = 0;
  unsigned int nbMaxCours = 0;
public:
  EvolutionCours(const PaireDevises& p) : paire(&p) {}
  EvolutionCours(const EvolutionCours& e);
  ~EvolutionCours();

  EvolutionCours& operator=(const EvolutionCours& e);
  void addCours(double o, double h, double l, double c, double v, const QDate& d);
  unsigned int getNbCours() const { return nbCours; }

  const PaireDevises& getPaireDevises() const { return *paire; }
  const CoursOHLCV& getCours(const QDate& d) const;

  using iterator = CoursOHLCV*;
  iterator begin() { return iterator(cours); }
  iterator end() { return iterator(cours + nbCours); }

  using const_iterator = const CoursOHLCV*;
  const_iterator begin() const { return cours; }
  const_iterator end() const { return cours + nbCours; }
  const_iterator cbegin() const { return cours; }
  const_iterator cend() const { return cours + nbCours; }
};

class CSVReader {
  EvolutionCours* e;
  std::ifstream file;
  std::string line;
  std::string sdate;
  std::string sopen;
  std::string shigh;
  std::string slow;
  std::string sclose;
  std::string sadjclose;
  std::string svolume;
public:
  CSVReader(EvolutionCours* ec, const QStringList& f): e(ec) {
    file.open(f.toStdList().begin()->toStdString());
    if (!file.is_open()) {
        QMessageBox msgBox;
        msgBox.setText("Erreur de chargement");
        msgBox.exec();
      }
  }
  ~CSVReader() {
    file.close();
  }
  void readFile();
};
/**
 * \class      Classe Transaction
 * \details    Cette classe fait la mise en jour des montants des devises apres chaque transaction realisé.                  
 */
class Transaction {
  double sum;
  bool type;
  QDate date;

public:
  void setValue(double s) {sum=s;}
   //! methode updateBalance
    /**
   * \details    La méthode fait le control si l'opération c'est valide ou pas et puis fait les mises en jpour 
   * des montant de devises de base et de contrepartie. 
   * \param    devise de base
   * \param    devise de Contrepartie
   * \param   pointer d'EvolutionCours 
   * \param   l'interet de brokers
   */
  bool updateBalance(Devise*,Devise*,EvolutionCours *,double) ;
  Transaction(double value,bool t,QDate d):sum(value),type(t),date(d) {}
  ~Transaction(){}
  Devise& getBase(EvolutionCours * e) const;
  Devise& getContrePartie(EvolutionCours * e) const;
  double getClosePrice(EvolutionCours * e) const;
  double getSum() const { return sum; }
    bool getType() const { return type; }
   QDate getDate() const { return date; }
};

/**
 * \class      Classe Notes
 * \details    Cette classe a un attribut text qui continet les notes prises dans l'editeur de texte.                  
 */
class Notes{
  QString text;
public:
  Notes(QString t):text(t){}
  QString getText() const { return text; }
  void setText(QString t){text=t;}
};
/**
 * \class      Classe Historique
 * \details    Cette classe contient l'historique des transactions realisées.                  
 */
class Historique : public QObject {
    Q_OBJECT
    std::vector<Transaction*> tab;
public slots:
  void update_transaction(double montant, QDate date);
public:
    Historique() {}
    //! methode addTransaction
    /**
   * \details    La méthode fait l'ajout des transactions dans le vector tab 
   * \param    Objet de classe Transaction
   */
    void addTransaction(Transaction* t) { tab.push_back(t); }
    void removeLastTransaction() { tab.pop_back(); }
    unsigned int getSize() const { return tab.size(); }
    const std::vector<Transaction*>& getHistorique() const { return tab; }
    void setHistorique(std::vector<Transaction*> h) { tab = h; }
    using iterator = std::vector<Transaction*>::iterator;
    using const_iterator = std::vector<Transaction*>::const_iterator;
    iterator begin() { return tab.begin(); }
    iterator end() { return tab.end(); }
    const_iterator cbegin() const { return tab.begin(); }
    const_iterator cend() const { return tab.end(); }
};


#endif // TRADING_H
