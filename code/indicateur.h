#ifndef INDICATEUR_H
#define INDICATEUR_H
/*!
* \file indicateur.h
* \brief les classes des indicateurs
*/
#include <QString>
#include "trading.h"
#include <QDate>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include<cmath>
/**
*\class ValueIndicator
* la classe contenant le date et la valeur indicateur calculé pour chaque courOHLC
*/
class ValueIndicator {
private:
        double data; /**<le valeur calcule pour chaque cours d'une evolutionCours*/
        QDate  date; /**<le date du cour*/
public:
    //! constructeur
        /**
        *
        */
        ValueIndicator(){}
        //! méthode getValue
        /**
        * \return  double : data
        */
        double getValue() const { return data;}
        //! méthode getDate
        /**
        * \return  QDate : la date
        */
        QDate  getDate() const { return date;}
        //! méthode setValue
        /**
        * \param data : const double&
        * \return void :
        */
        void setValue(const double& data) { this->data = data;}
        //! méthode setDate
        /**
        * \param date : QDate
        * \return void :
        */
        void setDate(QDate date) { this->date = date;}

        using iterator = ValueIndicator*;/**< iterator sur ValueIndicator*/

};
/**
* \class Indicator
* \brief classe abstraite pour les indicateurs
*
*/
class Indicator {

protected:
    //! constructeur
    /**
    * \param evolutionCours :EvolutionCours*
    * \param nom :QString = "Autre indicateur" par default
    */
    Indicator(EvolutionCours* evolutionCours, QString nom = "Autre Indicateur");
    //! destructor virtual
    virtual ~Indicator() = default;

    EvolutionCours* evolutionCours;/**<la valeur du pointeur sur EvolutionCours */
    QString nom; /**<le nom de l'indicateur */
    unsigned int nbIndicator;/**<nombre d'éléments dans le tableau values */

public:
    using iterator = ValueIndicator*;/**<iterator correspond à ValueIndicator* */
    using const_iterator = const ValueIndicator*; /**<const_iterator correspond à const ValueIndicator* */
    //! méthode virtual pure generateIndice
    /**
    * \return  void :
    */
    virtual void generateValue() = 0;
    //! méthode getNbIndicator
    /**
    * \return  unsigned int : nbIndicator
    */
    unsigned int getNbIndicator() const {return nbIndicator;}
    //! méthode getNom
    /**
    * \return  QString : nom
    */
    QString getNom() const {return nom;}
    //! méthode gêtvolution
    /**
    * \return  EvolutionCours* : evolutionCours
    */
    EvolutionCours* getEvolution() const { return evolutionCours; }
};

/**
* \class EMA
* \brief l'indicateur EMA : Exponential Moving Average
*  friend class MACD
*/
class EMA : public Indicator{
    friend class MACD;
private:
    unsigned int period; /**< Nombre de jours */
    ValueIndicator* values = nullptr;/**< tableau des ValueIndicator*/

public:
    //! constructeur
    /**
    * \param evolutionCours :EvolutionCours*
    * \param unsigned int : period = 10 par default
    */
    EMA(EvolutionCours* evolutionCours, unsigned int period = 10) : Indicator(evolutionCours, "EMA"), period(period) {

      values = new ValueIndicator[nbIndicator];
    }
    //! destructeur
    /** delete values
    */
    ~EMA() {
      delete values;
    }
    //! methode generateValue
    /**
    *\return void :
    */
    void generateValue();
    //! methode setEvolutionCours
    /**
    *\param e : EvolutionCours&
    *\return void:
    */
    void setEvolutionCours(EvolutionCours& e){this->evolutionCours=&e;}
    //! methode setPeriod
    /**
    *\param period : unsigned int
    *\return void:
    */
    void setPeriod(unsigned int period){this->period=period;}
    //! methode getPeriod
    /**
    *\return period : unsigned int
    */
    unsigned int getPeriod() const {return period;}
    iterator begin(){return iterator(values);}/**< iterator begin sur values*/
    iterator end(){return iterator(values + nbIndicator);}/**< iterator end sur values*/
    const_iterator cbegin() const {return values;}/**< const iterator begin sur values*/
    const_iterator cend() const {return values+ nbIndicator;}/**< const iterator sur values*/
    //! methode getValuesi
    /**
    *\param i : int
    * mthode const
    *\return values[i].getvalue()
    */
    double getValuei(int i) const {return values[i].getValue();}

};
/**
* \class RSI
* \brief l'indicateur RSI : Relative Strength Index
*/
class RSI : public Indicator {

private:
    unsigned int standardPeriod;/**< le periode standard ( le rsi est calcule a partir de le standPeriod eme jour)*/
    double tops, bottoms;/**< les limites pour realiser le achat ou vendre*/
    ValueIndicator* values = nullptr;/**< tableau des ValueIndicator*/

public:
    //! constructeur
    /**
    *\param evolutionCours : EvolutionCours*
    *\param standardPeriod : unsigned int = 14 par default
    *\param tops : double = 70 par default
    *\param bottoms : double = 30 par default
    */
    RSI(EvolutionCours* evolutionCours, unsigned int standardPeriod = 14, double tops= 70, double bottoms= 30):
        Indicator(evolutionCours, "RSI"), standardPeriod(standardPeriod), tops(tops), bottoms(bottoms) {
        values = new ValueIndicator[nbIndicator];
    }
    //! methode generateValue
    /**
    *\return void :
    */
    void generateValue();
    //! methode setTops
    /**
    *\param tops : double = 70
    *\return void :
    */
    void setTops(double tops=70) {this->tops = tops;}
    //! methode setBottoms
    /**
    *\param bottoms : double = 30
    *\return void :
    */
    void setBottoms(double bottoms=30) {this->bottoms = bottoms;}
    //! methode setStandardPeriod
    /**
    *\param standardPeriod : unsigned int = 14
    *\return void :
    */
    void setStandardPeriod(unsigned int standardPeriod=14){this->standardPeriod=standardPeriod;}
    iterator begin(){return iterator(values);}/**< iterator begin sur values*/
    iterator end(){return iterator(values + nbIndicator);}/**< iterator end sur values*/
    const_iterator cbegin() const {return values;}/**< const iterator begin sur values*/
    const_iterator cend() const {return values+ nbIndicator;}/**< const iterator end sur values*/
    //! methode getStandardPeriod
    /**
    *\return standardPeriod : unsigned int
    */
    unsigned int getStandardPeriod() const {return standardPeriod;}
    //! methode getTop
    /**
    *\return tops : double
    */
    double getTop () const {return tops;}
    //! methode getBottom
    /**
    *\return bottoms : double
    */
    double getBottom() const {return bottoms;}


};
/**
* \class MACD
* \brief l'indicateur MACD : Moving Average Convergence Divergence
*
*/
class MACD : public Indicator {

private:
    unsigned int longPeriod;/**< long period*/
    unsigned int shortPeriod;/**< short period*/
    unsigned int signalPeriod;/**< signal Period*/
    ValueIndicator* macd = nullptr; /**< tableau des ValueIndicator */
    ValueIndicator* macd_signal = nullptr;/**< tableau des ValueIndicator */
    ValueIndicator* macd_histogram = nullptr;/**< tableau des ValueIndicator */

public:
    //! constructeur
    /**
    *\param evolutionCours : EvolutionCours
    *\param shortPeriod : unsigned int = 12
    *\param longPeriod : usigned int = 26
    *\param signalPeriod : unsigned int = 9
    */
    MACD(EvolutionCours* evolutionCours, unsigned int shortPeriod=12, unsigned int longPeriod=26, unsigned int signalPeriod=9) : Indicator(evolutionCours, "MACD") {
        if(longPeriod < shortPeriod || longPeriod < signalPeriod) throw TradingException("MACD: long period doit etre le plus grand");
        this->longPeriod = longPeriod;  this->shortPeriod = shortPeriod;
        this->signalPeriod = signalPeriod;
        macd = new ValueIndicator[nbIndicator];
        macd_signal = new ValueIndicator[nbIndicator];
        macd_histogram =new ValueIndicator[nbIndicator];
    }
    //! methode generateValue
    /**
    *\return void :
    */
    void generateValue();
    /**
    *\return longPeriod : unsigned int
    */
    unsigned int getLongPeriod() const {return longPeriod;}
    /**
    *\return shortPeriod : unsigned int
    */
    unsigned int getShortPeriod() const {return shortPeriod;}
    /**
    *\return signalPeriod : unsigned int
    */
    unsigned int getSignalPeriod() const {return signalPeriod;}
    /**
    *\return macd_signal : ValueIndicator*
    */
    ValueIndicator* getSignal() const {return macd_signal;}
    /**
    *\return madc_histogram : ValueIndicator*
    */
    ValueIndicator* getHistogram() const {return macd_histogram;}
    iterator begin(){return iterator(macd_histogram);}/**< iterator begin sur values*/
    iterator end(){return iterator(macd_histogram + nbIndicator);}/**< iterator end sur values*/
    const_iterator cbegin() const {return macd_histogram;}/**< const iterator begin sur values*/
    const_iterator cend() const {return macd_histogram+ nbIndicator;}/**<const iterator end sur values*/
    //! desstructeur
    ~MACD(){
        delete [] macd;
        delete [] macd_signal;
        delete [] macd_histogram;
    }
};

#endif
