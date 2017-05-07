#include "qpurple.h"

QPurple::QPurple(bool useAnotherThread) : QObject(0) {
    obj = new PurpleInteraction;
    PurpleInteraction::setInstance(obj);
    if (useAnotherThread) {
        obj->moveToThread(&interactThread);
        connect(&interactThread, &QThread::finished, obj, &QObject::deleteLater);
        connect(&interactThread, &QThread::destroyed, obj, &QObject::deleteLater);
        connect(obj,&PurpleInteraction::finished, obj, &QObject::deleteLater);
        connect(obj,&PurpleInteraction::finished,this, &QPurple::interactionFinished);
        interactThread.start();
    }
}

QPurple::~QPurple() {
    if (otherThread) {
        interactThread.quit();
        interactThread.wait();
        interactThread.deleteLater();
    }
}

void QPurple::interactionFinished(){
    obj = 0;
}

PurpleInteraction* QPurple::getInstance(){
    return obj;
}
