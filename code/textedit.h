#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "trading.h"
#include "ui_textedit.h"

#include <QMainWindow>

namespace Ui {
class TextEdit;
}

class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    explicit TextEdit(QWidget *parent = nullptr);
    void setNote(const QString& n);
    Notes* getNote() { return notes; }
    ~TextEdit();

private slots:
    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();


    void on_actionSave_triggered();

    void on_actionDelete_triggered();

private:
    Ui::TextEdit *ui;
    Notes* notes;
};

#endif // TEXTEDIT_H
