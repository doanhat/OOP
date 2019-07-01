#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDateTime>
#include <QDate>
#include <vector>
#include <QFile>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <string>
#include <QMainWindow>
#include "trading.h"


//qmake:	QT += core
class LoadSave
{
    //Q_OBJECT
public:
    LoadSave(const QString &fName);
    ~LoadSave(){}
    void load();
    std::vector<Transaction*>& getTransList(){return transList;}
    //std::vector<Note*> getNoteList(){return noteList;}
    QString& getNoteList(){return note;}
    //void save(const std::vector<Transaction*>& tList,const std::vector<Note*>& nList);
    void save(const std::vector<Transaction*>& tList,const QString & n);
    /***
     * Chaque fichier appartient à un seul utilisateur.
     * Hypothèse: Il n'y a qu'une seule transaction par jour, donc la date peut devenir la clé primaire de chaque transaction ou note. Mais je ne la met pas dans les attributes d'element pour garder extensibilité
    ***/
    void writeXmlDemo();///pour tester
private:
    QString fileName;///garder le nom de fichier
    QXmlStreamReader reader;
    QXmlStreamWriter writer;
    std::vector<Transaction*> transList;///pour garder les données des transactions dans mémoire centrale
    QString note;
    //std::vector<Note*> noteList;///pour garder les données des notes dans mémoire centrale
    /**ouvrire la fichier XML fileName.
     * Si la fichier n'existe pas, il crée automatiquement une nouvelle fichier avec le nom fileName.
     * examiner la fichier est bien formée.
     * entrer dans la racine simulation, appeler readSimulationElement() pour lire des élements
     * fermer la fichier.
    **/
    bool readFile();
    /***
     * On est déjà entré dans la racine <simulation></simulation>
     * entrer dans des élements transaction et note,
     * appeler des fonctions readSimulationElement() ou readNoteElement() selon le nom de élement.
     * Si l'élement est inconnu, appeler skipUnknownElement().
    **/
    void readSimulationElement();
    void readTransactionsElement();
    void readNotesElement();
    /***
     * On est déjà entré dans l'élément <transaction></transaction>
     * lire des données de transaction,
     * et les envoyer à vector<Transaction*> transList pour charger dans mémoire centrale.
     * **/
    Transaction* readTransactionElement();
    /***
     * On est déjà entré dans l'élément <note></note>
     * lire des données de transaction,
     * et les envoyer à vector<Note*> noteList pour charger dans mémoire centrale.
     * **/
    //Note& readNoteElement()const;
    QString& readNoteElement();
    void skipUnknownElement();
    void writeTransaction( Transaction &trans);
    void writeNote(const QString &note);
    //void writeXmlTitle();///écrire le title si la fichier XML est vide.
    /***
     * Chaque fois quand on veut ecrire dans la fichier XML, il faut d'abord supprimer le dernier racine </Simulation>
     * Cette fonction permet de override la fichier sans la dernier ligne.
     * J'ai pas trouve autre solution.
     * **/
    //void deleteLastLine();
    //void writeLastLine();
    
};

