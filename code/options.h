#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QDate>
#include <QComboBox>

namespace Ui {
  class Options;
}

class Options : public QDialog
{
  Q_OBJECT

public:
  explicit Options(QWidget *parent = nullptr);
  ~Options();

  void setMode(const bool a) { mode = a; }
  void setSoldeBase(int b) { oBase = static_cast<unsigned int>(b); }
  void setSoldeCp(int c) { oCp = static_cast<unsigned int>(c); }
  void setDate(const QDate& date) { oToday = date; updateUi(); }
  void setBroker(const double& br) { oBroker = br; }

  bool getMode() const { return mode; }
  double getBase() const { return oBase; }
  double getCp() const { return oCp; }
  QDate getToday() const { return oToday; }
  double getBroker() const { return oBroker; }

private slots:

  void on_dateVal_userDateChanged(const QDate &date);
  void on_baseVal_valueChanged(int arg1);
  void on_cpVal_valueChanged(int arg1);
  void on_brokerVal_valueChanged(const double& arg1);

  void updateUi();

  void on_btnManu_toggled(bool checked);

private:
  Ui::Options *ui;

  bool mode;
  double oBase;
  double oCp;
  QDate oToday;
  double oBroker;
};

#endif // OPTIONS_H
