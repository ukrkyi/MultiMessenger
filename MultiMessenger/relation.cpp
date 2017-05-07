#include "relation.h"
#include "account.h"
#include "message.h"
#include "person.h"

Relation::Relation(Person* fr, Account* acc, PurpleInteraction::ContactStruct cont)
    : BaseElement<Relation>(this), PurpleInteraction::ContactStruct(cont), account(acc->id), person(fr->id) {
    if (fr->mainRelation.isNull())
        fr->mainRelation = this->id;
}

Relation::Relation(QDataStream& stream) {
    stream >> id;
    stream >> name >> alias;
    stream >> avatar;
    stream >> account >> person;
}

Account* Relation::getAccount() {
    return Account::items[account];
}

QList<Message*> Relation::getMessages() {
    QMap<QDateTime, Message*> map;
    for (QHash<QUuid, Message *>::iterator it = Message::items.begin(), end = Message::items.end(); it != end; it++)
        if (it.value()->relation == this->id) {
            map.insertMulti(it.value()->time, it.value());
        }
    QList<Message*> list;
    for (QMap<QDateTime, Message *>::iterator it = map.begin(), end = map.end(); it != end; it++)
        list.append(it.value());
    return list;
}

void Relation::deleteMessages() {
    QList<Message*> list = getMessages();
    QList<Message *>::iterator it = list.begin(), end = list.end();
    for (; it != end; it++)
        delete *it;
}

void Relation::clearMessages(int n) {
    QList<Message*> msg = getMessages();
    for (int i = msg.length() - n - 1; i >= 0; i--)
        delete msg[i];
}

void Relation::insertToStream(QDataStream& stream) const {
    stream << id;
    stream << name << alias;
    stream << avatar;
    stream << account << person;
}

Relation* Relation::findAccName(QUuid acc, QString username) {
    QHash<QUuid, Relation *>::iterator it = items.begin(), end = items.end();
    for (; it != end; it++)
        if (it.value()->account == acc && username.indexOf(it.value()->name) != -1)
            return *it;
    return NULL;
}

QDataStream& operator<<(QDataStream& dataStream, Relation* rel) {
    rel->insertToStream(dataStream);
    return dataStream;
}

QDataStream& operator>>(QDataStream& dataStream, Relation*& rel) {
    rel = new Relation(dataStream);
    return dataStream;
}
