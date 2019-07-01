#include "strategie.h"

/*---------------------------------------------------------- Methodes de classe AI_Trading ------------------------------------------------*/
//void AI_Trading::setParameters(QComboBox box,QMap<QString, QVariant> parameters){
//    if (parameters.contains("debut")) {
//        QString d = parameters["debut"].toString();
//        setDebut(QDate::fromString(d,"dd/MM/yyyy"));
//    }
//    if (parameters.contains("fin")) {
//        QString f = parameters["fin"].toString();
//        setFin(QDate::fromString(f,"dd/MM/yyyy"));
//    if (parameters.contains(""))
//}

//QMap<QString, QVariant> AI_Trading::getParameters() const {
//    QMap<QString, QVariant> parameters;
//    parameters["debut"] = debut;
//    parameters["fin"] = fin;
//    return parameters;
//}

//void EMA_Convergence::setParameters(QMap<QString, QVariant> parameters){
//    if (!evolutionCours) throw TradingException("Strategie: Evolution Cours does not exist");
//    //ema = dynamic_cast<EMA*>(evolutionCours->getIndicator("EMA"));
//    ema->setParameters(parameters);
//    ema_it = ema->begin();
//}

//void AI_Trading::performStrategie(){
//    if (str->getNom() == "EMA Convergence") {
//        setStrategie(new EMA_Convergence);

//    }
//    if (str->getNom() == "RSI Stratégie") {
//        setStrategie(new RSI_Strategie);
//    }
//    if (str->getNom() == "MACD Stratégie") {
//        setStrategie(new MACD_Strategie);
//    }

//}

/*---------------------------------------------------------- Methodes de classe EMA_Convergence ----------------------------------------------*/
double EMA_Convergence::trading(EvolutionCours* evo) {
    // double montant_base = transaction->getBase(i->getEvolution()).getMontant();
    // double montant_contrepartie = transaction->getContrePartie(i->getEvolution()).getMontant();
    double montant_base = evo->getPaireDevises().getBase().getMontant();
    double montant_contrepartie = evo->getPaireDevises().getContrepartie().getMontant();
    double min = 0.01;
    int max_b = round(montant_contrepartie);
    int max_s = round(montant_base);

    if (evo->getCours(ema_it->getDate()).getOpen() > ema_it->getValue() && montant_contrepartie > 0) {
        return rand()%max_b + min; //buy
    }
    else if (evo->getCours(ema_it->getDate()).getOpen() < ema_it->getValue() && montant_base > 0) {
        return - rand()%max_s + min; //sell
    }
    else return 0;
}

/*---------------------------------------------------------- Methodes de classe RSI_Strategie ------------------------------------------------*/

//void RSI_Strategie::setParameters(QMap<QString, QVariant> parameters) {
//    if (!evolutionCours) throw TradingException("Strategie: Can not set parameters when evolutionCours is null");
//    rsi = dynamic_cast<RSI*>(evolutionCours->getIndicateur("RSI"));
//    rsi->setParameters(parameters);
//    rsi_it = rsi->begin();
//}

double RSI_Strategie::trading(EvolutionCours* evo) {
    double montant_base = evo->getPaireDevises().getBase().getMontant();
    double montant_contrepartie = evo->getPaireDevises().getContrepartie().getMontant();
    double min = 0.01;
    int max_b = round(montant_contrepartie);
    int max_s = round(montant_base);

    if (rsi_it->getValue() <= 30 && montant_contrepartie > 0) {
        //buy signal
        return rand()%max_b + min; //buy
    }
    else if (rsi_it->getValue() >= 70 && montant_base > 0) {
        //sell signal
        return - rand()%max_s + min; //sell
    }
    else {
        return 0;
    }
}

double MACD_Strategie::trading(EvolutionCours *evo) {
    double montant_base = evo->getPaireDevises().getBase().getMontant();
    double montant_contrepartie = evo->getPaireDevises().getContrepartie().getMontant();
    double min = 0.01;
    int max_b = round(montant_contrepartie);
    int max_s = round(montant_base);

    if (macd->getShortPeriod() >= macd->getLongPeriod() && montant_contrepartie > 0) {
        return rand()%max_b + min; //buy
    }
    else if (macd->getShortPeriod() <= macd->getLongPeriod() && montant_base > 0) {
        return - rand()%max_s + min; //sell
    }
    else {
        return 0;
    }
}

void Robot::setStrategie(const QString &string)
{
  type_strategie = string;
}

void Robot::setDateDepart(const QDate &date)
{
  debut = date;
}

void Robot::setDateFin(const QDate &date)
{
  fin = date;
}

void Robot::releaseStrategie() {
    double montant = 0.0;
    if (type_strategie == "EMA_Convergence") {

        EMA_Convergence* ema_str = dynamic_cast<EMA_Convergence*>(strategie);
        ema_str->setIndicateur(new EMA(evo));

        EMA::iterator it = ema_str->getEMA()->begin();

        while (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") < QDate::fromString(ema_str->getdepart().toString("yyyy-MM-dd"), "yyyy-MM-dd")) {
            it++;
        }

        ema_str->setIterator(it);
        while (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") < QDate::fromString(ema_str->getfin().toString("yyyy-MM-dd"), "yyyy-MM-dd")) {
            ema_str->setIterator(it);
            montant = ema_str->trading(evo);
            emit transactionChanged(montant, it->getDate());
            it++;
        }
    }
    else if (type_strategie == "RSI_Strategie") {

        RSI_Strategie* rsi_str = dynamic_cast<RSI_Strategie*>(strategie);
        rsi_str->setIndicateur(new RSI(evo));
        RSI::iterator it = rsi_str->getRSI()->begin();

        while (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") < QDate::fromString(rsi_str->getdepart().toString("yyyy-MM-dd"), "yyyy-MM-dd")) {
            it++;
        }
        rsi_str->setIterator(it);
        while (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") < QDate::fromString(rsi_str->getfin().toString("yyyy-MM-dd"), "yyyy-MM-dd")) {
            rsi_str->setIterator(it);
            montant = rsi_str->trading(evo);
            emit transactionChanged(montant, it->getDate());
            it++;
        }
    }
    else {
            MACD_Strategie* macd_str = dynamic_cast<MACD_Strategie*>(strategie);
            macd_str->setIndicateur(new MACD(evo));
            MACD::iterator it = macd_str->getMACD()->begin();
            while (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") < QDate::fromString(macd_str->getdepart().toString("yyyy-MM-dd"), "yyyy-MM-dd")) {
                it++;
            }
            macd_str->setIterator(it);
            while (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") < QDate::fromString(macd_str->getfin().toString("yyyy-MM-dd"), "yyyy-MM-dd")) {
                macd_str->setIterator(it);
                montant = macd_str->trading(evo);
                emit transactionChanged(montant, it->getDate());
                it++;
            }
    }
}
