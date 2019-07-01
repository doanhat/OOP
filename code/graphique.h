    #ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <QtCharts>
#include <QWidget>
#include <QDate>
#include <QBarCategoryAxis>

#include "trading.h"
#include "indicateur.h"

enum Couleur { rouge, vert };
/**
 * \class Classe Bougie
 * \details Cette classe affiche l'évolution du cours sur une journée
 */
class Bougie : public QCandlestickSet {
    Q_OBJECT
    CoursOHLCV* cours;
    Couleur couleur;
    string forme;

public:
//! constructeur
/**
 * \details Le constructeur fait l'analyse des formes de bougies
 * \param CoursOHLCV
 * \param timestamp
 * \param pointeur QWidget
 */
    Bougie(CoursOHLCV* c, qreal timestamp = 0.0, QObject *parent = nullptr);
    string getForme() const { return forme; }
    Couleur getCouleur() const { return couleur; }
    CoursOHLCV& getCoursOHLCV() { return *cours; }
    const CoursOHLCV& getCoursOHLCV() const { return *cours; }
signals:
    void clickBougie(Bougie* c);
private slots:
    void viewCoursOHLCV(){ emit clickBougie(this); }
};

/**
 * \class Classe Graphique
 * \details Cela est une classe abstraite qui conserve une reference sur une EvolutionCours
 */
class Graphique : public QWidget {
    Q_OBJECT
    EvolutionCours& evolution;
public:
    explicit Graphique(EvolutionCours& e, QWidget *parent = nullptr):QWidget(parent), evolution(e) {}
    EvolutionCours& getEvolution() const { return evolution; }
    virtual ~Graphique() {}
signals:

private slots:

public slots:

};

/**
 * \class Classe Graphique_Chandelier
 * \details Cette classe trace un graphique contenant 9/10 bougie sur la période [j-10; j-1]
 *  Elle donne la possibilité d'afficher l'indicateur EMA
 */
class Graphique_Chandelier : public Graphique {
    Q_OBJECT

    QCandlestickSeries* series; // un ensemble de bougies
    QChart *chart; // un graphique sur un ensemble de bougies
    QChartView *chartView; // un viewer de graphique
    QLineEdit* open; // barres d’édition
    QLineEdit* high;
    QLineEdit* low;
    QLineEdit* close;
    QLabel* openl; // labels
    QLabel* highl;
    QLabel* lowl;
    QLabel* closel;
    QHBoxLayout* copen; // couches de placement
    QHBoxLayout* chigh;
    QHBoxLayout* clow;
    QHBoxLayout* cclose;
    QVBoxLayout* coucheCours;
    QHBoxLayout* fenetre;
    Bougie* lastBougieClicked;

    QHBoxLayout* cForme;
    QCheckBox* formCBox;
    QLabel* formeBougiel;
    QLineEdit* formeBougie;

    QLineSeries *emaSeries;
    //QChart *chart;
    //QChartView *chartView;
    //QHBoxLayout *layout;
    EMA* ema;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    static double max;
    static double min;

public:
//! constructeur
/**
 * \details Le constructeur crée un graphique prennant les bougies
 * \param EvolutionCours
 * \param date de la simulation
 * \param pointeur QWidget
 */
    explicit Graphique_Chandelier(EvolutionCours& e, QDate& date, QWidget *parent = nullptr);
    //!methode getMax
    /**
     * \details Cette methode renvoie la valeur max des données affichées
     * \return valeurMax
     */
    static double getMax();
    //!methode getMin
    /**
     * \details Cette methode renvoie la valeur min des données affichées
     * \return valeurMin
     */
    static double getMin();

signals:

private slots:
    void viewCoursOHLCV(Bougie* b);
    void showFormes(Bougie* b);

public slots:
    void activateEMA(bool val){
        emaSeries->setVisible(val);
    }
};

/**
 * \class Classe Graphique_Barre
 * \details Cette classe trace un graphique en barre reprenant les volumes échangés sur la période [j-10; j-1]
 */
class Graphique_Barre : public Graphique {
    Q_OBJECT

    QBarSet *set;
    QBarSeries *series;
    QChart *chart;
    QChartView *chartView;
    QHBoxLayout *layout;

    public:
        explicit Graphique_Barre(EvolutionCours& e, QDate& date, QWidget *parent = nullptr);
    signals:

    private slots:

    public slots:
};

/**
 * \class Classe Graphique_EMA
 * \details Cette classe trace un graphique de l'indicateur EMA
 */
class Graphique_EMA : public Graphique{
    Q_OBJECT
    QLineSeries *emaSeries;
    QChart *chart;
    QChartView *chartView;
    QHBoxLayout *layout;
    EMA* ema;
    QBarCategoryAxis *axisX;
    //QValueAxis *axisY;
public:
    explicit Graphique_EMA(EvolutionCours& e,QDate& date, QWidget *parent = nullptr);
signals:
private slots:
    void activateEMA(bool val){
        emaSeries->setVisible(val);
    }
//    void activateMACD(bool val){
//        MACD_series->setVisible();
//        MACD_signal->setVisible();
//        MACD_histogram->setVisible();
//    }
public slots:


};

/**
 * \class Classe Graphique_MACD
 * \details Cette classe trace un graphique de l'indicateur MACD
 */
class Graphique_MACD : public Graphique {
    Q_OBJECT
    QLineSeries *macdSeries;
    QLineSeries *macdSignalSeries;
    QStackedBarSeries *macdHistogramSeries;
    //QBarSeries *macdHistogramSeries;
    QVBoxLayout *layout;
    MACD* macds;
    QBarSet *setPos;
    QBarSet *setNeg;
    QChart *chart;
    QChartView *chartView;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
public:
    explicit Graphique_MACD(EvolutionCours& e,QDate& date, QWidget *parent = nullptr);
signals:
private slots:

public slots:


};

/**
 * \class Classe Graphique_RSI
 * \details Cette classe trace un graphique de l'indicateur RSI
 */
class Graphique_RSI : public Graphique {
    Q_OBJECT

    //QBarSeries *rsiSeries;
    //QBarSet *set;
    QLineSeries *rsiSeries;
    QLineSeries *topSeries;
    QLineSeries *bottomSeries;
    QHBoxLayout *layout;
    QChart *chart;
    QChartView *chartView;
    RSI* rsi;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
public:
    explicit Graphique_RSI(EvolutionCours& e,QDate& date, QWidget *parent = nullptr);
signals:
private slots:

public slots:


};

//class Graphique_Line
#endif // GRAPHIQUE_H
