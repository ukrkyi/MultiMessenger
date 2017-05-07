#ifndef ADDRELATION_H
#define ADDRELATION_H

#include <QDialog>

#include "qpurple.h"

namespace Ui {
class AddRelation;
}

class Person;
class Relation;

class AddRelation : public QDialog
{
    Q_OBJECT

    void accept();

public:
    explicit AddRelation(Person*pers, QWidget *parent = 0);
    ~AddRelation();
    Relation* rel;


private slots:
    void on_accountsBox_currentIndexChanged();

private:
    Ui::AddRelation *ui;
    Person* who;
    const QString sep = " - ";
};

#endif // ADDRELATION_H
