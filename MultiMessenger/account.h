#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>

#include "baseelement.h"
#include "relation.h"
#include "qpurple.h"

class Person;

class Account :
        public QObject,
        public BaseElement<Account>,
        public PurpleInteraction::AccountStruct
{
    Q_OBJECT

public:
    enum AccountStatus {
        Disconnected = 0x0, // Проблеми мережі
        Connected    = 0x1, // Все нормально
        Waiting      = 0x2, // Виконується зʼєднання
        Error        = 0x10,// Проблеми з акаунтом
    };

    Account();
    Account(QDataStream &);

    QList<Relation *> getConnections();
    void deleteRelations();

    virtual QString getAccountTypeInfo() const = 0;
    virtual QString getAccountType() const = 0;
    virtual QIcon getLogo() const = 0;
    virtual QIcon getIcon() const = 0;
    virtual QString getLogin() const;
    virtual ~Account(){}
    virtual void insertToStream(QDataStream& stream) const;

    void enable(QString pass);
    void disable();
    void setAutoOnline(bool);
    void setStatus(AccountStatus);
    void setContacts(const QList<PurpleInteraction::ContactStruct> &);

    bool isEnabled();
    bool isActive();
    bool hasError();
    bool autoOnline();
    AccountStatus getStatus();
    QString getUsername();

    // TODO: Замінити це на слот з перевіркою на місці
    virtual QString setUsername(QString);
    void setConnectionError(QString reason);
    QString getConnectionError();
    void updateContactByName(QString name, PurpleInteraction::ContactStruct new_info);

    static Account *findAccType    (QString username, QString type);
    static Account *findAccProtocol(QString username, QString protocol);
//    static QMap<QString, Account *> getMap(QString type);

    QList<PurpleInteraction::ContactStruct> contacts;
    PurpleInteraction::ContactStruct findContact(QString uname);

signals:
    void accConnected(QUuid);

protected:
    QString disconnectReason;
    bool a_online;
    AccountStatus status;
};

QDataStream& operator <<( QDataStream& dataStream, Account* account );
QDataStream& operator >>( QDataStream& dataStream, Account*& account );

#endif // ACCOUNT_H
