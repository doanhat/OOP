#ifndef STRATEGIE_H
#define STRATEGIE_H

#include "indicateur.h"
#include "trading.h"

#include <QDate>

class Strategie {
protected:
    QDate depart;
    QDate fin;
    QString nom = "UNKNOWN STRATEGY";
    Indicator* indi = nullptr;
    Strategie(QString nom): nom(nom){}
    Strategie(Strategie* str) = delete;
public:
    void setIndicateur(Indicator* i) {this->indi = i;}
    void setdepart(QDate d) {depart = d;}
    void setfin(QDate f) {fin = f;}
    const QString& getNom() const {return nom;}
    virtual const QDate& getdepart() const {return depart;}
    virtual const QDate& getfin() const {return fin;}
    virtual ~Strategie() = default;
    //virtual void setParameters(QMap<QString, QVariant> parameters) {}
    //virtual QMap<QString, QVariant> getParameters() const {return QMap<QString, QVariant>();}
    virtual double trading(EvolutionCours*){return 0;}
};

class EMA_Convergence : public Strategie {
    EMA* ema=nullptr;
    EMA::iterator ema_it=nullptr;

public:
    //void setParameters(QMap<QString, QVariant> parameters);
    //QMap<QString, QVariant> getParameters() const;
    EMA_Convergence(): Strategie("EMA Convergence Strategie") {}
    void setdepart(QDate d) {depart = d;}
    void setfin(QDate f) {fin = f;}
    EMA* getEMA() const {return ema;}
    void setIterator(EMA::iterator it) {ema_it = it;}
    const QDate& getdepart() const {return depart;}
    const QDate& getfin() const {return fin;}
    ~EMA_Convergence() {delete ema;}
    double trading(EvolutionCours* evo);
};

class RSI_Strategie : public Strategie {
    RSI* rsi=nullptr;
    RSI::iterator rsi_it=nullptr;

public:
    //void setParameters(QMap<QString, QVariant> parameters);
    //QMap<QString, QVariant> getParameters() const;
    RSI_Strategie(): Strategie("RSI Strategie") {}
    void setdepart(QDate d) {depart = d;}
    void setfin(QDate f) {fin = f;}
    void setIterator(RSI::iterator it) {rsi_it = it;}
    RSI* getRSI() const {return rsi;}
    const QDate& getdepart() const {return depart;}
    const QDate& getfin() const {return fin;}
    ~RSI_Strategie() {delete rsi;}

    double trading(EvolutionCours* evo);
};

class MACD_Strategie : public Strategie {
    MACD* macd=nullptr;
    MACD::iterator macd_it=nullptr;


public:
    //void setParameters(QMap<QString, QVariant> parameters);
    //QMap<QString, QVariant> getParameters() const;
    MACD_Strategie(): Strategie("MACD Strategie") {}
    void setdepart(QDate d) {depart = d;}
    void setfin(QDate f) {fin = f;}
    void setIterator(MACD::iterator it) {macd_it = it;}
    MACD* getMACD() const {return macd;}
    const QDate& getdepart() const {return depart;}
    const QDate& getfin() const {return fin;}
    ~MACD_Strategie() {delete macd;}
    double trading(EvolutionCours* evo);
};

class Robot : public QObject {
  Q_OBJECT
public:
  Robot(EvolutionCours* e): evo(e) {}
  ~Robot() {};

  void setStrategie(const QString& string);
  void setDateDepart(const QDate& date);
  void setDateFin(const QDate& date);
  void releaseStrategie();

signals:
  void transactionChanged(double montant, QDate date);

private:
  EvolutionCours* evo;
  Strategie* strategie; //faire strategie en mode automatique
  QString type_strategie;
  QDate debut;
  QDate fin;
};

#endif // STRATEGIE_H
