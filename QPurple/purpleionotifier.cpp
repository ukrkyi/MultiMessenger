#include "purpleionotifier.h"

//#include "purple.h"

PurpleIONotifier::PurpleIONotifier(gint fd,
    PurpleInputCondition condition,
    PurpleInputFunction function,
    gpointer data)
    : QObject(0),
      func(function),
      func_data(data),
      inSignal(false) {
    if (condition & PURPLE_INPUT_WRITE) {
        writeNotifier = new QSocketNotifier(fd, QSocketNotifier::Write);
        connect(writeNotifier, &QSocketNotifier::activated, this, &PurpleIONotifier::writeSmth);
    } else {
        writeNotifier = NULL;
    }

    if (condition & PURPLE_INPUT_READ) {
        readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
        connect(readNotifier, &QSocketNotifier::activated, this, &PurpleIONotifier::readSmth);
    } else {
        readNotifier = NULL;
    }
}

PurpleIONotifier::~PurpleIONotifier() {
    if (readNotifier != NULL) {
        readNotifier->setEnabled(false);
        if (!inSignal)
            readNotifier->deleteLater();
    }
    if (writeNotifier != NULL) {
        writeNotifier->setEnabled(false);
        if (!inSignal)
            writeNotifier->deleteLater();
    }
}

void PurpleIONotifier::readSmth(int fd) {
    inSignal = true;
    QSocketNotifier* tmp = readNotifier;
    QPointer<QObject> self = this;
    func(func_data, fd, PURPLE_INPUT_READ);
    if (!self)
        tmp->deleteLater();
    else
        inSignal = false;
}

void PurpleIONotifier::writeSmth(int fd) {
    inSignal = true;
    QSocketNotifier* tmp = writeNotifier;
    QPointer<QObject> self = this;
    func(func_data, fd, PURPLE_INPUT_WRITE);
    if (!self)
        tmp->deleteLater();
    else
        inSignal = false;
}
