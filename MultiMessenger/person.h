#ifndef PERSON_H
#define PERSON_H

#include <QImage>
#include <QObject>

#include "baseelement.h"

class Relation;
class Message;

class Person : public QObject, public BaseElement<Person>
{
    Q_OBJECT

public:
    QString name;
    QImage avatar;
    QUuid mainRelation;
    Person();
    Person(QDataStream &);
    ~Person();
    QList<Relation *> getConnections();
    QList<Message *> getMessages();

    void deleteRelations();
    void insertToStream(QDataStream& stream) const;

signals:
    void newMessage(QUuid rel, QUuid msg);
    void avatarChanged(QImage avatar);
    void nameChanged(QString newName);

public slots:
    void changeMainRel(QUuid);
};

QDataStream& operator <<( QDataStream& dataStream, Person* account );
QDataStream& operator >>( QDataStream& dataStream, Person*& account );

#endif // PERSON_H
