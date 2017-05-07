#ifndef PERSONMANAGER_H
#define PERSONMANAGER_H

#include <QButtonGroup>
#include <QFile>
#include <QMap>
#include <QObject>
#include <QUuid>
#include <QPushButton>

#include "qpurple.h"

class Relation;
class AddPerson;
class personWidget;
class Person;

class PersonManager : public QObject
{
    Q_OBJECT

    QButtonGroup personButtonsGroup;
    QMap <int, personWidget *> widgets;
    AddPerson * editing;
    int lastId;

public:
    explicit PersonManager(QObject *parent = 0);
    void add();

    bool load(QFile *pers, QFile*rel, QFile*msg);
    bool check();

signals:
    void currentWidgetChanged(QWidget *);
    void newWidgetAdded(QWidget*);
    void newButtonAdded(QPushButton*);
    void actionsEnabled(bool);
    void createConversation(PurpleInteraction::AccountStruct, QString);
    void sendMessage(PurpleInteraction::AccountStruct, QString, QString);
    void chSetBtn(bool);

public slots:
    void activate(int);
    void messageLoad(PurpleInteraction::AccountStruct, QString, PurpleInteraction::MessageStruct);
    void startEdit(QUuid);
    void addPerson(Person*);
    void newMessage(QUuid,QString);
    void uncheck();
    void newRelation(QUuid rel_id);

private slots:
    void editCompleted(bool = true);
    void persDeleted();
};

#endif // PERSONMANAGER_H
