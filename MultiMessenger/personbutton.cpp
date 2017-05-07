#include <QPixmap>

#include "person.h"

#include "personbutton.h"

personButton::personButton(Person* person, QWidget *parent) :
    QPushButton(QPixmap::fromImage(person->avatar),"",parent)
{
    this->setFlat(true);
    this->setIconSize(QSize(32,32));
    this->setCheckable(true);
    this->setChecked(true);
    connect(person,SIGNAL(avatarChanged(QImage)),this,SLOT(updateImage(QImage)));
}

personButton::~personButton(){}

void personButton::increaseUnread(){
    ;
}

void personButton::updateImage(QImage img)
{
    this->setIcon(QPixmap::fromImage(img));
}
