#ifndef PURPLEIONOTIFIER_H
#define PURPLEIONOTIFIER_H

#include <QObject>
#include <QSocketNotifier>
#include <QPointer>

#include "qpurple_global.h"

#include "purple.h"

class PurpleIONotifier : public QObject
{
    Q_OBJECT

    QSocketNotifier *readNotifier, *writeNotifier;
    PurpleInputFunction func;
    gpointer func_data;
    bool inSignal;

public:
    explicit PurpleIONotifier(gint fd, PurpleInputCondition condition, PurpleInputFunction function,
                              gpointer data
                              //,guint handl
                              );
    ~PurpleIONotifier();
   // guint handle;

signals:

public slots:
    void readSmth(int);
    void writeSmth(int);
};

#endif // PURPLEIONOTIFIER_H
