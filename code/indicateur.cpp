#include "indicateur.h"
#include <QDebug>
Indicator::Indicator(EvolutionCours* evolutionCours, QString nom): nom(nom) {
    if (!evolutionCours) throw TradingException("Idicateur: evolutionCours n'existe pas");
        this->evolutionCours = evolutionCours;
    nbIndicator = evolutionCours->getNbCours();
}

void EMA::generateValue() {

    double k=double(2)/double((period+1));
    //qDebug()<<values[0].getValue()<<" "<<values[2].getValue();
    EvolutionCours::iterator coursIterator;
    double sum=0;
    unsigned int i =0;
    for (coursIterator = evolutionCours->begin();coursIterator != evolutionCours->end();coursIterator++){

        if (i < period) {
            values[i].setValue(coursIterator->getClose());
            sum+=coursIterator->getClose();
        //qDebug()<<values[i].getValue()<<" "<<coursIterator->getClose();
        }
        else if (i==period){
            values[i].setValue(sum/double(period));
        }else{
            values[i].setValue((coursIterator->getClose()*k)+(values[i-1].getValue())*(1-k));
            //qDebug()<<values[i].getValue()<<" "<<k<<" "<<values[i].getDate()<<" "<<values[i-1].getValue();
        }
        values[i].setDate(coursIterator->getDate());
        i++;
    }
    //qDebug()<<values[0].getValue()<<" "<<values[2].getValue();
}

void MACD::generateValue() {

    EMA* shortEMA = new EMA(evolutionCours,shortPeriod);
    //qDebug()<<shortEMA->getPeriod();
    shortEMA->generateValue();
    //qDebug()<< shortEMA->begin()->getValue();

    //qDebug()<< shortEMA->begin()->getDate();
    //qDebug()<< (shortEMA->begin()+1)->getValue();
    //qDebug()<< (shortEMA->begin()+1)->getDate();
    //qDebug()<<macd[0].getValue()<<" "<<macd[2].getValue();
    EMA* longEMA = new EMA(evolutionCours,longPeriod);
    longEMA->generateValue();
    EMA::iterator shortValueIterator;
    int i=0;
    for (shortValueIterator = shortEMA->begin();shortValueIterator !=shortEMA->begin()+shortEMA->getNbIndicator();shortValueIterator++){
        macd[i].setValue(shortValueIterator->getValue());//-longEMA->getValuei(i));
        //qDebug()<<shortValueIterator->getDate();
        //qDebug()<<"value"<<macd[i].getValue()<<"date"<<macd[i].getDate();
        macd[i].setDate(shortValueIterator->getDate());
        i++;
    }
    //qDebug()<<macd[0].getValue()<<" "<<macd[2].getValue();
    EMA::iterator longValueIterator;
    i=0;
    for (longValueIterator = longEMA->begin();longValueIterator!=longEMA->begin()+longEMA->getNbIndicator();longValueIterator++){
        macd[i].setValue(macd[i].getValue()-longValueIterator->getValue());
        //qDebug()<<"value"<<macd[i].getValue()<< " "<<longValueIterator->getValue()<<"date"<<macd[i].getDate();
        i++;
    }
    EMA* signalMACD = new EMA(evolutionCours,signalPeriod);
    signalMACD->generateValue();
    EMA::iterator signalIterator;
    //MACD::iterator hisIterator;
    i=0;
    for (signalIterator= signalMACD->begin();signalIterator!=signalMACD->begin()+signalMACD->getNbIndicator();signalIterator++){
        macd_signal[i].setValue(signalIterator->getValue());
        macd_signal[i].setDate(signalIterator->getDate());
        //qDebug()<<"value"<<macd[i].getValue()<<" "<<macd_signal[i].getValue()<<" " <<i;
//        macd_histogram[i].setValue(macd[i].getValue()-macd_signal[i].getValue());
//        macd_histogram[i].setDate(signalIterator->getDate());
        //qDebug()<<"value"<<macd_signal[i].getValue()<<"date"<<macd_histogram[i].getDate();
        i++;
    }
    i=0;

    while (macd_signal[i].getValue() == 0.0 || macd[i].getValue() == 0.0){
        i++;
    }
    double k=macd_signal[nbIndicator-1].getValue()/macd[nbIndicator-1].getValue();
    //qDebug()<<k;
    i=0;
    for (signalIterator= signalMACD->begin();signalIterator!=signalMACD->begin()+signalMACD->getNbIndicator();signalIterator++){
        macd_histogram[i].setValue(macd[i].getValue()-(signalIterator->getValue())/k);
        macd_histogram[i].setDate(signalIterator->getDate());
        //qDebug()<<"value"<<macd_histogram[i].getValue()<<" "<<macd[i].getValue()<<"date"<<macd_histogram[i].getDate();
        i++;
    }

    delete shortEMA;
    delete longEMA;
    delete signalMACD;
}

void RSI::generateValue(){
    double up=0;
    double upAverage=0;
    double sumUp=0;
    double down=0;
    double downAverage=0;
    double sumDown=0;
    double *close=new double[nbIndicator];
    double rs=0;
    EvolutionCours::iterator coursIterator;
    unsigned int i=0;
    for (coursIterator=evolutionCours->begin();coursIterator!=evolutionCours->begin()+nbIndicator;coursIterator++){
        close[i]=coursIterator->getClose();
        if (i==0){
            up=0;
            down=0;
            values[i].setValue(0);
        }else{
            if (close[i-1]<close[i])
                up=close[i]-close[i-1];
            else up=0;

            if (close[i-1]<=close[i])
                down=0;
            else down=close[i-1]-close[i];
            sumUp+=up;
            sumDown+=down;
            if (i==standardPeriod){
                upAverage=sumUp/standardPeriod;
                downAverage=sumDown/standardPeriod;
                rs=upAverage/downAverage;
                }else{
                upAverage=(upAverage*(standardPeriod-1)+up)/standardPeriod;
                downAverage=(downAverage*(standardPeriod-1)+down)/standardPeriod;
                rs=upAverage/downAverage;
            }
            values[i].setValue(100-100/(1+rs));
            //qDebug()<<values[i].getValue();
        }
        values[i].setDate(coursIterator->getDate());
        i++;
    }
}
