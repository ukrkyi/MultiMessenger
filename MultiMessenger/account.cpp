#include <QList>
#include <QString>
#include <QMap>
#include <QDataStream>
#include <QIcon>

#include "account.h"
#include "facebookaccount.h"
#include "googleaccount.h"
#include "person.h"
#include "relation.h"

Account::Account() : BaseElement<Account>(this) {
    status = Disconnected;
}

Account::Account(QDataStream &stream)
{
    stream >> id;
    stream >> username >> password >> enabled;
}

QDataStream& operator<<(QDataStream& dataStream, Account *account) {
    dataStream << account->getAccountType();
    account->insertToStream(dataStream);
    return dataStream;
}

QDataStream& operator>>(QDataStream& dataStream, Account*& account) {
    // Увага: при відновленні не можна використовувати стандартний конструктор,
    // оскільки це створить новий збережений екземпляр класу.
    // Натомість, нам потрібно відновити існуючий - використовуємо особливий коструктор відновлення
    QString type;
    dataStream >> type;
    if (type == FacebookAccount::type) {
        account = new FacebookAccount(dataStream);
    } else if (type == GoogleAccount::type){
        account = new GoogleAccount(dataStream);
    }
    return dataStream;
}

void Account::insertToStream(QDataStream& stream) const {
    // Id обʼєкта
    stream << id;
    // Беремо до уваги, що тип обʼєкта вже збережено
    stream << username << password << enabled;
    // Зазвичай, цього має бути достатньо для відновлення
}

QString Account::getLogin() const {
    return username;
}

QList<Relation*> Account::getConnections() {
    QList<Relation*> list;
    QHash<QUuid, Relation *>::iterator it = Relation::items.begin(), end = Relation::items.end();
    for (; it != end; it++)
        if (it.value()->account == this->id)
            list.append(*it);
    return list;
}

void Account::deleteRelations() {
    QList<Relation*> list = getConnections();
    QList<Relation *>::iterator it = list.begin(), end = list.end();
    for (; it != end; it++) {
        (*it)->deleteMessages();
        delete *it;
    }
}


bool Account::isEnabled() {
    return enabled;
}

bool Account::isActive() {
    return status & Connected;
}

bool Account::hasError() {
    return status & Error;
}

bool Account::autoOnline() {
    return a_online;
}

Account::AccountStatus Account::getStatus() {
    return status;
}

QString Account::getUsername()
{
    return username;
}

QString Account::setUsername(QString newName) {
    username = newName;
    return "";
}

void Account::enable(QString pass) {
    password = pass;
    enabled = true;
}

void Account::disable() {
    enabled = false;
    status = Disconnected;
}

void Account::setAutoOnline(bool val) {
    a_online = val;
}

void Account::setStatus(Account::AccountStatus stat) {
    if (status == Waiting && stat == Connected){
        QList<Relation*> list = getConnections();
        QList<Relation *>::iterator it = list.begin(), end = list.end();
        for (; it != end; it++) {
            emit accConnected((*it)->id);
        }
    }
    if (stat || !enabled || !(status & Account::Error))
        status = stat;
}

void Account::setContacts(const QList<PurpleInteraction::ContactStruct>& cont)
{
    contacts = cont;
}

void Account::setConnectionError(QString reason) {
    status = Error;
    disconnectReason = reason;
}

QString Account::getConnectionError() {
    if (hasError())
        return disconnectReason;
    else
        return "";
}

void Account::updateContactByName(QString name, PurpleInteraction::ContactStruct new_info)
{
    for (int i=0;i<contacts.length();i++)
        if (contacts[i].name == name )
            contacts[i] = new_info;
}

Account* Account::findAccType(QString username, QString type) {
    QHash<QUuid, Account *>::iterator it = Account::items.begin(), end = Account::items.end();
    for (; it != end; it++)
        if (it.value()->getLogin() == username && (type == it.value()->getAccountType()))
            return *it;
    return 0;
}

Account* Account::findAccProtocol(QString username, QString protocol) {
    QHash<QUuid, Account *>::iterator it = Account::items.begin(), end = Account::items.end();
    for (; it != end; it++)
        if (it.value()->getUsername() == username && (protocol == it.value()->protocol))
            return *it;
    return 0;
}

PurpleInteraction::ContactStruct Account::findContact(QString uname)
{
    for (int i=0;i<contacts.length();i++)
        if (contacts[i].name == uname )
            return contacts[i];
    return PurpleInteraction::ContactStruct();
}

//QMap<QString, Account *> Account::getMap(QString type){
//    QMap<QString, Account *> list;
//    QHash<QUuid, Account *>::iterator it = Account::items.begin(), end = Account::items.end();
//    for (; it != end; it++)
//        if (type == it.value()->type)
//            list[(*it)->username] = *it;
//    return list;
//}
