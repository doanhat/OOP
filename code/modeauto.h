#ifndef MODEAUTO_H
#define MODEAUTO_H

#include "strategie.h"
#include "graphique.h"

#include <QDialog>
#include <QCalendarWidget>

namespace Ui {
    class ModeAuto;
}

class ModeAuto : public QDialog {
    Q_OBJECT
public:
    explicit ModeAuto(QWidget* parent = nullptr);
    ~ModeAuto();

  void setStrategie(const QString& string);
  void setDateDepart(const QDate& date);
  void setDateFin(const QDate& date);

  QString getStrategie() const { return type_str; }
  QDate getDateDepart() const { return debut; }
  QDate getDateFin() const { return fin; }

private slots:

    void on_combo_strategie_currentIndexChanged(const QString& name);
    void on_depart_clicked(const QDate &date);
    void on_fin_clicked(const QDate &date);
    void on_buttonBox_accepted();

private:
    Ui::ModeAuto *ui; //instance de mode auto
    QString type_str; //savoir quelle stratégie va être appliquée
    QDate debut;
    QDate fin;
};

#endif // MODEAUTO_H
