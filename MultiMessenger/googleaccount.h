#ifndef GOOGLEACCOUNT_H
#define GOOGLEACCOUNT_H

#include "account.h"

class GoogleAccount : public Account
{
    //Q_OBJECT

public:
    GoogleAccount();
    GoogleAccount(QDataStream &);
    QString getAccountTypeInfo() const;
    QString getAccountType() const;
    QIcon getLogo() const;
    QIcon getIcon() const;

    QString setUsername(QString);
    QString getLogin() const;

    static const QString type;
};

#endif // GOOGLEACCOUNT_H
