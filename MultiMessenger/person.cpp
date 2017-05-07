#include <QMap>

#include "message.h"
#include "person.h"
#include "relation.h"

Person::Person() : BaseElement<Person>(this) {
    avatar = QImage(":/ico/user");
}

Person::Person(QDataStream& stream) {
    stream >> id;
    stream >> name >> avatar;
    stream >> mainRelation;
}

Person::~Person() {
}

QList<Relation*> Person::getConnections() {
    QList<Relation*> list;
    QHash<QUuid, Relation *>::iterator it = BaseElement<Relation>::items.begin(), end = BaseElement<Relation>::items.end();
    for (; it != end; it++)
        if (it.value()->person == this->id)
            list.append(*it);
    return list;
}

QList<Message*> Person::getMessages() {
    QMap<QDateTime, Message*> map;
    for (QHash<QUuid, Message *>::iterator it = Message::items.begin(), end = Message::items.end(); it != end; it++)
        if (Relation::items[it.value()->relation]->person == this->id) {
            map.insertMulti(it.value()->time, it.value());
        }
    QList<Message*> list;
    for (QMap<QDateTime, Message *>::iterator it = map.begin(), end = map.end(); it != end; it++)
        list.append(it.value());
    return list;
}

void Person::deleteRelations() {
    QList<Relation*> list = getConnections();
    QList<Relation *>::iterator it = list.begin(), end = list.end();
    for (; it != end; it++) {
        (*it)->deleteMessages();
        delete *it;
    }
}

void Person::insertToStream(QDataStream& stream) const {
    stream << id;
    stream << name << avatar;
    stream << mainRelation;
}

void Person::changeMainRel(QUuid newRel) {
    if (Relation::items.contains(newRel))
        mainRelation = newRel;
}

QDataStream& operator<<(QDataStream& dataStream, Person* person) {
    person->insertToStream(dataStream);
    return dataStream;
}

QDataStream& operator>>(QDataStream& dataStream, Person*& person) {
    person = new Person(dataStream);
    return dataStream;
}
