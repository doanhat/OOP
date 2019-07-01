#include "graphique.h"
#include <math.h>
#include <QDebug>
double Graphique_Chandelier::max=0;
double Graphique_Chandelier::min=10000000;

double Graphique_Chandelier::getMax() { return max; }
double Graphique_Chandelier::getMin()  { return min; }

Bougie::Bougie(CoursOHLCV* c, qreal timestamp, QObject *parent):
  QCandlestickSet(c->getOpen(), c->getHigh(), c->getLow(), c->getClose(), timestamp, parent),
  cours(c), couleur((cours->getOpen()>cours->getClose()) ? rouge : vert)
{
  connect(this,SIGNAL(clicked()),this, SLOT(viewCoursOHLCV()));

  if(abs(cours->getOpen()-cours->getClose())<0.20*abs(Graphique_Chandelier::getMax()-Graphique_Chandelier::getMin()))
    {
      forme="Toupie";
      if(cours->getOpen()==cours->getClose()) forme="Doji";
      if(couleur==rouge && cours->getClose()-cours->getLow()>=2*abs(cours->getOpen()-cours->getClose())) forme="Marteau";
      if(couleur==vert && cours->getHigh()-cours->getClose()>=2*abs(cours->getOpen()-cours->getClose())) forme="Etoile filante";
      if(couleur==vert && cours->getOpen()-cours->getLow()>=2*abs(cours->getOpen()-cours->getClose())) forme="Pendu";
    }
  else forme="Bougie";
}

Graphique_Chandelier::Graphique_Chandelier(EvolutionCours& e, QDate& date, QWidget *parent):Graphique (e, parent)
{
  series = new QCandlestickSeries(this);
  series->setIncreasingColor(QColor(Qt::green));
  series->setDecreasingColor(QColor(Qt::red));

  QStringList categories;

  EvolutionCours& evolution = Graphique::getEvolution();
  for(EvolutionCours::iterator it=evolution.begin(); it!=evolution.end(); ++it){
      if ( (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
           (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ) {
          CoursOHLCV & cours = *it;
          Bougie * bougie = new Bougie(&cours);
          series->append(bougie);
          categories << cours.getDate().toString("dd-MM");

          if(cours.getHigh()>max) max = cours.getHigh();
          if(cours.getLow()<min) min = cours.getLow();

          connect(bougie, SIGNAL(clickBougie(Bougie*)), this, SLOT(viewCoursOHLCV(Bougie*)));
          connect(bougie, SIGNAL(clickBougie(Bougie*)), this, SLOT(showFormes(Bougie*)));
        }
    }



  emaSeries=new QLineSeries;
  ema= new EMA(&evolution);
  ema->generateValue();
  QString name = QString("EMA %1").arg(ema->getPeriod());
  emaSeries->setName(name);
  EMA::iterator valueIterator;
  int i=0;
  for (valueIterator=ema->begin();valueIterator!=ema->begin()+ema->getNbIndicator();valueIterator++){
      if ( (QDate::fromString(valueIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
           (QDate::fromString(valueIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){
          emaSeries->append(i,valueIterator->getValue());
          i++;
      }
  }
  //QtChart
  chart = new QChart();
  axisX=new QBarCategoryAxis;
  axisX->append(categories);
  chart->addSeries(series);
  chart->addSeries(emaSeries);
  //chart->setTitle("EMA indicator");
  chart->addAxis(axisX,Qt::AlignBottom);
  //chart->setAxisX(axisX,emaSeries);
  series->attachAxis(axisX);
  emaSeries->attachAxis(axisX);
  axisY=new QValueAxis;
  chart->addAxis(axisY,Qt::AlignLeft);
  series->attachAxis(axisY);
  emaSeries->attachAxis(axisY);
  chartView=new QChartView(chart,this);

  //QtChartsView
  chart->setTitle("Cours OHLC");
  emaSeries->setVisible(false);
  //chart->setAxisX(axisX, series);
  openl = new QLabel("Open", this);
  highl = new QLabel("High", this);
  lowl = new QLabel("Low", this);
  closel = new QLabel("Close", this);

  open = new QLineEdit(this);
  high = new QLineEdit(this);
  low = new QLineEdit(this);
  close = new QLineEdit(this);

  coucheCours = new QVBoxLayout;
  copen = new QHBoxLayout;
  chigh = new QHBoxLayout;
  clow = new QHBoxLayout;
  cclose = new QHBoxLayout;

  copen->addWidget(openl);
  copen->addWidget(open);
  chigh->addWidget(highl);
  chigh->addWidget(high);
  clow->addWidget(lowl);
  clow->addWidget(low);
  cclose->addWidget(closel);
  cclose->addWidget(close);

  coucheCours->addLayout(copen);
  coucheCours->addLayout(chigh);
  coucheCours->addLayout(clow);
  coucheCours->addLayout(cclose);

  /*!!*/
  cForme = new QHBoxLayout();
  formCBox = new QCheckBox(this);
  formCBox->setText("Forme des bougies");
  formeBougiel = new QLabel("Forme", this);
  formeBougie = new QLineEdit(this);
  cForme->addWidget(formeBougiel);
  cForme->addWidget(formeBougie);
  coucheCours->addLayout(cForme);
  coucheCours->addWidget(formCBox);
  /*/!!*/

  fenetre = new QHBoxLayout(this);
  fenetre->addWidget(chartView);
  fenetre->addLayout(coucheCours);
  fenetre->setStretchFactor(chartView, 8);
  fenetre->setStretchFactor(coucheCours, 1);
  this->setLayout(fenetre);

  formeBougiel->setVisible(false);
  formeBougie->setVisible(false);
  // QObject::connect(formCBox, SIGNAL(changeEvent()), this, SLOT(showFormes()));
}

void Graphique_Chandelier::viewCoursOHLCV(Bougie* b){
  QString str;
  str.setNum(b->getCoursOHLCV().getOpen());
  open->setText(str);
  str.setNum(b->getCoursOHLCV().getHigh());
  high->setText(str);
  str.setNum(b->getCoursOHLCV().getLow());
  low->setText(str);
  str.setNum(b->getCoursOHLCV().getClose());
  close->setText(str);
  lastBougieClicked = b;
}

void Graphique_Chandelier::showFormes(Bougie* b){
  if(formCBox->isChecked())
    {
      formeBougie->setText(QString::fromStdString((b->getForme())));
      formeBougiel->setVisible(true);
      formeBougie->setVisible(true);
    }
  else {
      formeBougiel->setVisible(false);
      formeBougie->setVisible(false);
    }
}


Graphique_Barre::Graphique_Barre(EvolutionCours& e, QDate& date, QWidget *parent): Graphique(e, parent)
{
  QStringList categories;

  series=new QBarSeries();
  chart = new QChart();
  set = new QBarSet("Volume");

  EvolutionCours& evolution = Graphique::getEvolution();
  for(EvolutionCours::iterator it=evolution.begin(); it!=evolution.end(); ++it){
      if ( (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
           (QDate::fromString(it->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){
          categories << it->getDate().toString("dd-MM");
          *set << it->getVolume();
        }
    }

  series->append(set);
  chart->addSeries(series);

  QBarCategoryAxis * axisX = new QBarCategoryAxis;

  axisX->append(categories);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  // QValueAxis *axisY = new QValueAxis();
  // axisY->setRange(10000000,300000000);
  // chart->addAxis(axisY, Qt::AlignLeft);
  // series->attachAxis(axisY);


  chart->setTitle("Volume");
  chartView=new QChartView(chart, this);

  layout=new QHBoxLayout(this);
  layout->addWidget(chartView);
  layout->setStretchFactor(chartView, 1);
  this->setLayout(layout);
}

Graphique_EMA::Graphique_EMA(EvolutionCours& e, QDate& date, QWidget *parent):Graphique (e,parent){
    emaSeries=new QLineSeries;
    chart=new QChart;
    QStringList category;
    EvolutionCours& evolution= Graphique::getEvolution();
    ema= new EMA(&evolution);
    ema->generateValue();
    QString name = QString("EMA %1").arg(ema->getPeriod());
    emaSeries->setName(name);
    EMA::iterator valueIterator;
    int i=0;
    for (valueIterator=ema->begin();valueIterator!=ema->begin()+ema->getNbIndicator();valueIterator++){
        if ( (QDate::fromString(valueIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
             (QDate::fromString(valueIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){

            category<<valueIterator->getDate().toString("dd-MM");
            emaSeries->append(i,valueIterator->getValue());

            i++;
        }
    }
    axisX=new QBarCategoryAxis;
    //axisY=new QValueAxis;
    axisX->append(category);
    chart->addSeries(emaSeries);
    //chart->setTitle("EMA indicator");
    chart->setAxisX(axisX,emaSeries);

    chartView=new QChartView(chart,this);
    layout = new QHBoxLayout;
    layout->addWidget(chartView);
    layout->setStretchFactor(chartView, 1);
    this->setLayout(layout);
    //chart->setVisible(false);
}

Graphique_MACD::Graphique_MACD(EvolutionCours& e, QDate& date, QWidget *parent):Graphique (e,parent){
    macdSeries = new QLineSeries;
    macdSeries->setColor(Qt::blue);
    macdSignalSeries = new QLineSeries;
    macdSignalSeries->setColor(Qt::red);
    macdHistogramSeries = new QStackedBarSeries();
    QStringList category;
    setPos = new QBarSet("+");
    setPos->setColor(Qt::black);
    setNeg = new QBarSet("-");
    setNeg->setColor(Qt::green);
    EvolutionCours& evolution= Graphique::getEvolution();
    macds = new MACD(&evolution);
    macds->generateValue();
    QString nameMacd = QString("MACD");
    QString nameSignal = QString("Signal line (period :%1)").arg(macds->getSignalPeriod());
    macdSeries->setName(nameMacd);
    macdSignalSeries->setName(nameSignal);
    MACD::iterator valueIterator;
    int i=0;
    for(valueIterator=macds->begin();valueIterator!=macds->begin()+macds->getNbIndicator();valueIterator++){
        if ( (QDate::fromString(valueIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
             (QDate::fromString(valueIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){

            category<<valueIterator->getDate().toString("dd-MM");
            if ( valueIterator->getValue()>=0){
                *setPos<<valueIterator->getValue();
                *setNeg<<0;
            }else{
                *setNeg<<valueIterator->getValue();
                *setPos<<0;
            }
            i++;
        }

    }
    EMA* shortEMA = new EMA(&evolution,macds->getShortPeriod());
    shortEMA->generateValue();
    EMA* longEMA = new EMA(&evolution,macds->getLongPeriod());
    longEMA->generateValue();
    EMA* signalEMA = new EMA(&evolution,macds->getSignalPeriod());
    signalEMA->generateValue();
    double *valuesMACD = new double[macds->getNbIndicator()];

    i=0;
    EMA::iterator shortIterator;
    for(shortIterator=shortEMA->begin();shortIterator!=shortEMA->begin()+shortEMA->getNbIndicator();shortIterator++){
        if ( (QDate::fromString(shortIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
             (QDate::fromString(shortIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){
            valuesMACD[i]=shortIterator->getValue();
            i++;
        }

    }
    i=0;
    EMA::iterator longIterator;
    for(longIterator=longEMA->begin();longIterator!=longEMA->begin()+longEMA->getNbIndicator();longIterator++){
        if ( (QDate::fromString(longIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
             (QDate::fromString(longIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){
            macdSeries->append(i,valuesMACD[i]-longIterator->getValue());
            i++;
        }

    }
    i=0;
    EMA::iterator signalIterator;
    for(signalIterator=signalEMA->begin();signalIterator!=signalEMA->begin()+signalEMA->getNbIndicator();signalIterator++){
        if ( (QDate::fromString(signalIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
             (QDate::fromString(signalIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){
            macdSignalSeries->append(i,signalIterator->getValue());
            i++;
        }

    }
    macdHistogramSeries->append(setPos);
    macdHistogramSeries->append(setNeg);
    chart =new QChart;

    axisX = new QBarCategoryAxis;
    axisX->append(category);
    chart->addSeries(macdHistogramSeries);
    chart->addSeries(macdSeries);
    chart->addSeries(macdSignalSeries);


    chart->addAxis(axisX,Qt::AlignBottom);
    macdSeries->attachAxis(axisX);
    macdSignalSeries->attachAxis(axisX);
    macdHistogramSeries->attachAxis(axisX);
    //axisY = new QValueAxis;
    //chart->addAxis(axisY,Qt::AlignLeft);
    //macdSeries->attachAxis(axisY);
    //macdSignalSeries->attachAxis(axisY);
    //macdHistogramSeries->attachAxis(axisY);
   chartView = new QChartView(chart,this);
   //chartViewHis = new QChartView(chartHis,this);
   layout = new QVBoxLayout;
   //layout->addWidget(chartViewHis);
   layout->addWidget(chartView);
   layout->setStretchFactor(chartView,1);
   //layout->setStretchFactor(chartViewHis,1);
   this->setLayout(layout);

}
Graphique_RSI::Graphique_RSI(EvolutionCours& e, QDate& date, QWidget *parent):Graphique (e,parent){
    rsi = new RSI(&e);
    QStringList categories;
    rsiSeries = new QLineSeries;
    //rsiSeries = new QBarSeries;
    chart = new QChart;
    //set = new QBarSet("RSI");
    rsiSeries = new QLineSeries;
    rsiSeries->setName("RSI");
    topSeries = new QLineSeries;
    QString name1 = QString("Top %1").arg(rsi->getTop());
    topSeries->setName(name1);
    bottomSeries = new QLineSeries;
    QString name2 = QString("Top %1").arg(rsi->getBottom());
    bottomSeries->setName(name2);
    //EvolutionCours& evolution= Graphique::getEvolution();

    rsi->generateValue();
    qDebug()<<"---------"<< rsi->getTop()<<" "<<rsi->getBottom();
    RSI::iterator rsiIterator;

    int i = 0;
    for (rsiIterator=rsi->begin();rsiIterator!=rsi->begin()+rsi->getNbIndicator();rsiIterator++){

        if ( (QDate::fromString(rsiIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") >= QDate::fromString(date.addDays(-10).toString("yyyy-MM-dd"), "yyyy-MM-dd")) &&
             (QDate::fromString(rsiIterator->getDate().toString("yyyy-MM-dd"), "yyyy-MM-dd") <= QDate::fromString(date.addDays(0).toString("yyyy-MM-dd"), "yyyy-MM-dd")) ){
            categories<<rsiIterator->getDate().toString("dd-MM");
            //*set<<rsiIterator->getValue();
            qDebug()<<rsiIterator->getValue()<<"----"<<rsiIterator->getDate();
            rsiSeries->append(i,rsiIterator->getValue());
            topSeries->append(i,rsi->getTop());
            bottomSeries->append(i,rsi->getBottom());

            i++;
        }
        //qDebug()<<rsiIterator->getValue();
    }
    //rsiSeries->append(set);
    chart->addSeries(rsiSeries);
    axisX = new QBarCategoryAxis;
    axisX->append(categories);
    axisY = new QValueAxis;
    axisY->setRange(0,100);
    chart->addSeries(topSeries);
    chart->addSeries(bottomSeries);

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    rsiSeries->attachAxis(axisX);
    rsiSeries->attachAxis(axisY);

    topSeries->attachAxis(axisX);
    topSeries->attachAxis(axisY);

    bottomSeries->attachAxis(axisX);
    bottomSeries->attachAxis(axisY);

    chartView = new QChartView(chart, this);
    layout = new QHBoxLayout;
    layout->addWidget(chartView);
    layout->setStretchFactor(chartView,1);
    this->setLayout(layout);

}
