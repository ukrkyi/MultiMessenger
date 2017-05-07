#ifndef BASEELEMENT_H
#define BASEELEMENT_H

#include <QDataStream>
#include <QFile>
#include <QHash>
#include <QUuid>

template <class T>
class BaseElement {

public:
    QUuid id;
    static QHash<QUuid, T*> items;
    BaseElement(){}
    BaseElement(T* element) {
        id = QUuid::createUuid();
        while (items.contains(id))
            id = QUuid::createUuid();
        items.insert(id, element);
    }
    static void loadFromFile(QFile* f) {
        bool wasOpen = f->isOpen();
        if (!wasOpen) {
            if (f->exists())
                f->open(QIODevice::ReadOnly);
            else
                return;
        }
        QDataStream in(f);
        in >> items;
        if (!wasOpen)
            f->close();
    }
    static void saveToFile(QFile* f) {
        bool wasOpen = f->isOpen();
        if (!wasOpen)
            f->open(QIODevice::WriteOnly);
        QDataStream out(f);
        out << items;
        if (!wasOpen)
            f->close();
    }
    ~BaseElement() {
        items.remove(id);
    }
};

template <class T>
QHash<QUuid, T*> BaseElement<T>::items;

#endif // BASEELEMENT_H
