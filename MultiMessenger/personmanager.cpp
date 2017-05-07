#include "addperson.h"
#include "personbutton.h"
#include "personwidget.h"

#include "account.h"
#include "message.h"
#include "person.h"
#include "relation.h"

#include "personmanager.h"

PersonManager::PersonManager(QObject* parent) : QObject(parent), lastId(0) {
    editing = 0;
    connect(&personButtonsGroup, SIGNAL(buttonClicked(int)), this, SLOT(activate(int)));
}

void PersonManager::add() {
    if (editing)
        return;
    Person* nowPerson = new Person();
    addPerson(nowPerson);
    emit actionsEnabled(false);
    editing = new AddPerson(nowPerson);
    connect(editing, SIGNAL(accepted()), this, SLOT(editCompleted()));
    connect(editing, &AddPerson::rejected, this, &PersonManager::persDeleted);
    connect(editing, &AddPerson::relationAdded, this, &PersonManager::newRelation);
    connect(editing, &AddPerson::deletePerson, this, &PersonManager::persDeleted);
    emit newWidgetAdded(editing);
    emit currentWidgetChanged(editing);
}

bool PersonManager::load(QFile* pers, QFile* rel, QFile* msg) {
    Person::loadFromFile(pers);
    Relation::loadFromFile(rel);
    Message::loadFromFile(msg);
    for (QHash<QUuid, Person *>::iterator it = Person::items.begin(),
                                          end = Person::items.end();
         it != end; it++) {
        addPerson(*it);
    }
    uncheck();
    for (QHash<QUuid, Relation *>::iterator it = Relation::items.begin(),
                                            end = Relation::items.end();
         it != end; it++) {
        connect((*it)->getAccount(), &Account::accConnected, this, &PersonManager::newRelation);
    }
    return true;
}


void PersonManager::activate(int id) {
    if (!editing) {
        emit currentWidgetChanged(widgets[id]);
        emit chSetBtn(false);
    }
}

void PersonManager::messageLoad(PurpleInteraction::AccountStruct acc_str,
    QString cont_name,
    PurpleInteraction::MessageStruct msg_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    Relation* rel = Relation::findAccName(acc->id, cont_name);
    if (rel) {
        Message* msg = new Message(rel, msg_str);
        emit Person::items[rel->person]->newMessage(rel->id, msg->id);
    }
}

void PersonManager::startEdit(QUuid person) {
    emit actionsEnabled(false);
    editing = new AddPerson(Person::items[person]);
    connect(editing, &AddPerson::finished, this, &PersonManager::editCompleted);
    connect(editing, &AddPerson::relationAdded, this, &PersonManager::newRelation);
    connect(editing, &AddPerson::deletePerson, this, &PersonManager::persDeleted);
    emit newWidgetAdded(editing);
    emit currentWidgetChanged(editing);
}

void PersonManager::addPerson(Person* person) {
    personButton* btn = new personButton(person);
    personButtonsGroup.addButton(btn);
    personWidget* pers = new personWidget(person);
    widgets.insert(personButtonsGroup.id(btn), pers);
    connect(pers, &personWidget::editingReq, this, &PersonManager::startEdit);
    connect(pers, &personWidget::msgSend, this, &PersonManager::newMessage);
    emit newButtonAdded(btn);
    emit newWidgetAdded(pers);
}

void PersonManager::newMessage(QUuid rel_id, QString msg)
{
    Relation*rel = Relation::items[rel_id];
    emit sendMessage(*rel->getAccount(),rel->name,msg);
}

void PersonManager::uncheck()
{
    personButtonsGroup.setExclusive(false);
    if (personButtonsGroup.checkedButton()){
        lastId = personButtonsGroup.checkedId();
        personButtonsGroup.checkedButton()->setChecked(false);
    } else {
        lastId = 0;
    }
    personButtonsGroup.setExclusive(true);
}


bool PersonManager::check()
{
    if (lastId)
        personButtonsGroup.button(lastId)->click();
    return (bool) lastId;
}

void PersonManager::editCompleted(bool res) {
    emit currentWidgetChanged(widgets[personButtonsGroup.checkedId()]);
    emit actionsEnabled(true);
    if (res) {
        editing->who->avatarChanged(editing->who->avatar);
        editing->who->nameChanged(editing->who->name);
        widgets[personButtonsGroup.checkedId()]->updateRelations();
    }
    editing->deleteLater();
    editing = 0;
}

void PersonManager::persDeleted() {
    editing->who->deleteRelations();
    editing->who->deleteLater();
    editing->deleteLater();
    editing = 0;
    QWidget* edt = widgets.take(personButtonsGroup.checkedId());
    if (edt)
        delete edt;
    delete personButtonsGroup.checkedButton();
    emit actionsEnabled(true);
}

void PersonManager::newRelation(QUuid rel_id) {
    Relation* rel = Relation::items[rel_id];
    emit createConversation(*rel->getAccount(), rel->name);
}
