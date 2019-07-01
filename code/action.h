#ifndef SELL_H
#define SELL_H

#include <QDialog>

#include "trading.h"

namespace Ui {
class Action;
}

class Action : public QDialog
{
  Q_OBJECT

public:
  explicit Action(QWidget *parent = nullptr);
  ~Action();

  void setValue(double b) { value = b; }

  double getValue() const { return value; }

private slots:

  void on_buttonBox_accepted();
  void on_selectedAmount_textChanged(const QString &arg1);

private:
  // Ui attributes
  Ui::Action *ui;

  // Data attributes
  double value; // Contient le montant pour la transaction
};

#endif // SELL_H
