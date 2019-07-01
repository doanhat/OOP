#include "textedit.h"
#include "ui_textedit.h"

TextEdit::TextEdit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TextEdit)
{
    ui->setupUi(this);
    //LoadSave loadSave=new LoadSave();
    //note=loadSave.readXmlNote();
   //ui->plainTextEdit->setPlainText(note.getText());
}

TextEdit::~TextEdit()
{
    delete ui;
}

void TextEdit::setNote(const QString &n)
{
    notes->setText(n);
}

void TextEdit::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();

}

void TextEdit::on_actionCut_triggered()
{
ui->plainTextEdit->cut();
}

void TextEdit::on_actionPaste_triggered()
{
    ui->plainTextEdit->paste();
}

void TextEdit::on_actionUndo_triggered()
{
    ui->plainTextEdit->undo();
}

void TextEdit::on_actionRedo_triggered()
{ui->plainTextEdit->redo();

}


void TextEdit::on_actionSave_triggered()
{

    // QString text=ui->plainTextEdit->toPlainText();
    //writeXMLNote(note.setText(text));
}

void TextEdit::on_actionDelete_triggered()
{
     ui->plainTextEdit->clear();
}
