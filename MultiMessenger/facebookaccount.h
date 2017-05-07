#ifndef FACEBOOKACCOUNT_H
#define FACEBOOKACCOUNT_H

#include "account.h"

class FacebookAccount : public Account
{
   // Q_OBJECT

public:
    FacebookAccount();
    FacebookAccount(QDataStream &);
    QString getAccountTypeInfo() const;
    QString getAccountType() const;
    QIcon getLogo() const;
    QIcon getIcon() const;

    QString setUsername(QString);

    static const QString type;
};

#endif // FACEBOOKACCOUNT_H
