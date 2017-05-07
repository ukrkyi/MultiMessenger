#ifndef RELATION_H
#define RELATION_H

//#include <QObject>

#include "baseelement.h"

#include "qpurple.h"

class Person;
class Account;
class Message;

class Relation :
    //public QObject,
    public BaseElement<Relation>,
    public PurpleInteraction::ContactStruct {
    // Q_OBJECT

public:
    QUuid account, person;
    Relation(Person*, Account*, PurpleInteraction::ContactStruct);
    Relation(QDataStream &);
    Account* getAccount();
    QList<Message*> getMessages();
    void deleteMessages();
    void clearMessages(int);

    void insertToStream(QDataStream& stream) const;

    static Relation* findAccName(QUuid acc, QString username);
};

QDataStream& operator <<( QDataStream& dataStream, Relation *rel );
QDataStream& operator >>( QDataStream& dataStream, Relation*&rel );

#endif // RELATION_H
