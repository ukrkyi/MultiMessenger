#ifndef ADDPERSON_H
#define ADDPERSON_H

#include <QDialog>
#include <QUuid>

namespace Ui {
class AddPerson;
}

class Relation;
class Person;

class AddPerson : public QDialog
{
    Q_OBJECT

    void accept();

public:
    explicit AddPerson(Person *pers, QWidget *parent = 0);
    ~AddPerson();
    Person * who;

    void addNewRel(Relation* rel);

private slots:
    void on_deleteButton_clicked();

    void on_addButton_clicked();

signals:
    void deletePerson();
    void relationAdded(QUuid);

private:
    Ui::AddPerson *ui;
};

#endif // ADDPERSON_H
