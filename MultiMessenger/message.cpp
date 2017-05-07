#include <QString>
#include <QDateTime>
#include <QUuid>

#include "message.h"
#include "relation.h"

Message::Message(Relation* rel, PurpleInteraction::MessageStruct msg)
    : BaseElement<Message>(this), PurpleInteraction::MessageStruct(msg), relation(rel->id) {
}

Message::Message(QDataStream &stream)
{
    stream >> id;
    qint8 tp;
    stream >> text >> time >> tp;
    type = (PurpleInteraction::MessageStruct::MessageType) tp;
    stream >> relation;
}

void Message::insertToStream(QDataStream &stream) const
{
    stream << id;
    stream << text << time << (qint8) type;
    stream << relation;
}

QDataStream &operator >>(QDataStream &dataStream, Message *&msg)
{
    msg = new Message(dataStream);
    return dataStream;
}

QDataStream &operator <<(QDataStream &dataStream, Message *msg)
{
    msg->insertToStream(dataStream);
    return dataStream;
}
