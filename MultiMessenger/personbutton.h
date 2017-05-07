#ifndef PERSONBUTTON_H
#define PERSONBUTTON_H

#include <QPushButton>

class Person;

class personButton : public QPushButton
{
    Q_OBJECT

public:
    explicit personButton(Person* person, QWidget *parent = 0);
    ~personButton();

public slots:
    void increaseUnread();
    void updateImage(QImage);
};

#endif // PERSONBUTTON_H
