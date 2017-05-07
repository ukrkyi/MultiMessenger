#ifndef MESSAGE_H
#define MESSAGE_H

#include "baseelement.h"

#include "qpurple.h"

class Relation;

class Message : public BaseElement<Message>, public PurpleInteraction::MessageStruct
{
   // Q_OBJECT

public:
    QUuid relation;
    Message(Relation*, PurpleInteraction::MessageStruct);
    Message(QDataStream &);
    void insertToStream(QDataStream& stream) const;
};

QDataStream& operator <<( QDataStream& dataStream, Message *);
QDataStream& operator >>( QDataStream& dataStream, Message*&);

#endif // MESSAGE_H
