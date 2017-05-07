#ifndef QPURPLE_H
#define QPURPLE_H

#include <QObject>
#include <QThread>
#include "qpurple_global.h"
#include "purpleinteraction.h"

class QPURPLESHARED_EXPORT QPurple : public QObject
{
    Q_OBJECT

    QThread interactThread;
    PurpleInteraction *obj;
    bool otherThread;
    void interactionFinished();

public:
    QPurple(bool useAnotherThread = true);
    ~QPurple();
    PurpleInteraction* getInstance();
};

#endif // QPURPLE_H
