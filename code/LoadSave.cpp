#include "LoadSave.h"

/*
pour appeler un objet LoadSave, c'est mieux de garder des listes dans global. Parceque on peut les utliser apres.
std::vector<Transaction> tList=new std::vector;
std::vector<Note> nList=new std::vector;
LoadSave w(file1,tList,nList);
w.readFile(file1);
*/
LoadSave::LoadSave(const QString &fName){
    fileName=fName;

}
void LoadSave::load(){
    readFile();
}

/*----------lire charger fichier-------------*/
bool LoadSave::readFile()
{
    QFile file(fileName);
    /***
     * Ici pour QIODevice::openMode, on utilise flag ReadWrite pas ReadOnly.
     * Si la fichier n'existe pas, il crée automatiquement une nouvelle fichier avec le nom fileName.
     * **/
    if (!file.open(QIODevice:: ReadWrite | QIODevice::Text)) {
        //QMessageBox::critical(this, tr("Error"),tr("Cannot read file %1").arg(fileName));
        return false;
    }
    reader.setDevice(&file);
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == "simulation") {
                readSimulationElement();
            } else {
                //reader.raiseError(tr("Not a valid simulation file"));
                reader.raiseError("Not a valid simulation file");
            }
        } else {
            reader.readNext();
        }
    }
    file.close();
    if (reader.hasError()) {
        //QMessageBox::critical(this, tr("Error"),tr("Failed to parse file %1").arg(fileName));
        return false;
    } else if (file.error() != QFile::NoError) {
        //QMessageBox::critical(this, tr("Error"),tr("Cannot read file %1").arg(fileName));
        return false;
    }
    return true;
}

void LoadSave::readSimulationElement()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "simulation");
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "transactions") {
                //Transaction t(readTransactionElement());
                //transList.push_back(&t);
                readTransactionsElement();
            }
            else if(reader.name() == "notes"){
                //Note n(readNoteElement());
                //noteList.push_back(&n);
                readNotesElement();
            }
            else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}
void LoadSave::readTransactionsElement()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "transactions");
    QString type=reader.attributes().value("paire").toString();
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if (reader.name() == "transaction") {
                Transaction* t;
                t=readTransactionElement();
                transList.push_back(t);

            }
            else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}
void LoadSave::readNotesElement()
{
    Q_ASSERT(reader.isStartElement() && reader.name() == "notes");
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            if(reader.name() == "note"){
                //Note n(readNoteElement());
                //noteList.push_back(&n);
                note=readNoteElement();
            }
            else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

Transaction* LoadSave::readTransactionElement()
{
    QString type;
    bool type2;
    double nombre;
    QDate date;
   if(reader.isStartElement() && reader.name() == "transaction")
    {
        type=reader.attributes().value("type").toString();
        reader.readNext();
    }
    while (!reader.atEnd()) {
        if(reader.name() == "date"){
        date= QDate::fromString(reader.readElementText(),"yyyy/MM/dd");
        reader.readNext();
        }
        else if(reader.name() == "nombre"){
        nombre= reader.readElementText().toDouble();
        reader.readNext();
        }
        else
        {
            reader.readNext();
        }
    }
    if (reader.isEndElement()) {
        reader.readNext();
    }
    if(type=="achat") type2=0;
    else if(type=="vente") {
        type2=1;
    }
    else{
        throw "error";
    }

    Transaction* t=new Transaction(nombre,type2,date);
    return t;
}

QString& LoadSave::readNoteElement(){
    if(reader.isStartElement() && reader.name() == "note")
    reader.readNext();
    QString text;
    while (!reader.atEnd()) {
        if(reader.name() == "text"){
        text = reader.readElementText();
        break;
        }
        else
        {
            reader.readNext();
        }
    }
    if (reader.isEndElement()) {
        reader.readNext();
    }

    return text;
}

void LoadSave::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}
/*
Note& readXmlNote()const{
    return *(notelist.getbegin());//on a une seul note.
}
Transaction& readXmlTransaction(const QDate &date)const{
    for(std::vector<transaction*>::const_iterator it=transList.begin();it!=transList.end();it++){
        if (it.getDate()==date)
        break;
    }
    return *it;
}*/

/*----------ecrire sauvegarder fichier-------------*/

void LoadSave::writeXmlDemo(){
    QFile file("demo.xml");
    if (!file.open(QIODevice:: WriteOnly | QIODevice::Truncate)) {
        //QMessageBox::critical(this, tr("Error"),tr("Cannot write file %1").arg("demo.xml"));
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Simulation");
    xmlWriter.writeStartElement("transaction");
    xmlWriter.writeAttribute("transID", "000001");
    xmlWriter.writeTextElement("date", "2008-01-10");
    xmlWriter.writeTextElement("type", "achat");
    xmlWriter.writeTextElement("paire", "USDERO");
    xmlWriter.writeTextElement("nombre", "500");
    xmlWriter.writeEndElement();//fermer transID="000001"

    xmlWriter.writeStartElement("note");
    xmlWriter.writeAttribute("noteID", "000001");
    xmlWriter.writeTextElement("date", "2008-01-10");
    xmlWriter.writeTextElement("text", "Hello!");
    xmlWriter.writeEndElement();//fermer noteID="000001"

    xmlWriter.writeEndElement();//fermer simulation
    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        //QMessageBox::critical(this, tr("Error"),tr("Cannot write file %1").arg("demo.xml"));
        return;
    }
}

void LoadSave::writeTransaction(Transaction &trans){
    writer.setAutoFormatting(true);
    writer.writeStartElement("transaction");
    QString type;
    if(trans.getType()) type="vente";
    else type="achat";
    writer.writeAttribute("type", type);
        // writer.writeAttribute("transID", "000001");comment generer une Id unique?
        //writer.writeTextElement("date", trans.getDate());
        //writer.writeTextElement("type", trans.getType());
         //writer.writeTextElement("paire", trans.getPaire());

    writer.writeTextElement("nombre", QString::number(trans.getSum()));
    writer.writeEndElement();
}

 void LoadSave::writeNote(const QString &note){
    writer.setAutoFormatting(true);
    writer.writeStartElement("note");
        //xmlWriter.writeAttribute("noteID", "000001");comment generer une Id unique?
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
        writer.writeTextElement("date", str);
        writer.writeTextElement("text", note);
    writer.writeEndElement();
 }

void LoadSave::save(const std::vector<Transaction*>& tList,const QString & n){
  QFile file(fileName);
    if (!file.open(QIODevice:: WriteOnly | QIODevice::Truncate)) {
        //QMessageBox::critical(this, tr("Error"),tr("Cannot write file %1").arg("demo.xml"));
        return;
    }
    writer.setDevice(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("simulation");
    writer.writeStartElement("transactions");

    //QString paire=**(tList.begin()).getPaire().toString();
    //getPaireDevises()
    //writer.writeAttribute("paire", "USDERO");
    for(auto it=tList.begin();it!=tList.end();it++) {
        writeTransaction(**it);
    }
    writer.writeEndElement();

    writer.writeStartElement("notes");
    writeNote(n);
    writer.writeEndElement();

    writer.writeEndElement();//fermer simulation
    writer.writeEndDocument();
    file.close();
  return ;
}
/*
void writeXmlTitle(){
    QFile file(fileName);
    if (!file.open(QIODevice:: WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(this, tr("Error"),tr("Cannot write file %1").arg("demo.xml"));
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Simulation");

    xmlWriter.writeStartElement("note");
    xmlWriter.writeEmptyElement("date");
    xmlWriter.writeEmptyElement("text");
    xmlWriter.writeEndElement();//fermer noteID="000001"

    xmlWriter.writeEndElement();//fermer simulation
    xmlWriter.writeEndDocument();
    file.close();
    if (file.error()) {
        QMessageBox::critical(this, tr("Error"),tr("Cannot write file %1").arg("demo.xml"));
        return;
    }
} */
/*
void deleteLastLine(){
    std::vector<string> tmp_files;

         std::ifstream infile(fileName.toStdString());
        if (!infile) {
            //std::cout << "fail!" << std:: endl;
            return;
        }

        string lineContent;
        while ( std::getline( infile,lineContent, '\n' ) ){
            tmp_files.push_back(lineContent + '\n');
        }
        infile.close();

         std::ofstream outfile(fileName.toStdString(),ios::out );
        std::vector<string>::iterator siter = tmp_files.begin();

        copy( tmp_files.begin(), tmp_files.end()-1, std::ostream_iterator<string>(outfile) );
        //std::cout << "ok!" << std::endl;
        outfile.close();

} */

/*
QDateTime time = QDateTime::currentDateTimeUtc();
QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
QDate(int y, int m, int d)
QDateTime(const QDate &date)
*/
/* problem
writer.writeAttribute("paire", "USDERO");
writer.writeAttribute("type", trans.getType());
*/
