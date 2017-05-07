#include <QIcon>
#include "googleaccount.h"

const QString GoogleAccount::type = QString("Google Hangouts");

GoogleAccount::GoogleAccount() : Account()
{
protocol = "prpl-jabber";
}

GoogleAccount::GoogleAccount(QDataStream& stream)
// Відновлюємо обʼєкт
    : Account(stream)
{
    protocol = "prpl-jabber";
}

QString GoogleAccount::getAccountTypeInfo() const{
    return "<img src=\"qrc://logo/google\" height=\"16\"> Google (Gmail/Hangouts)";
}

QString GoogleAccount::getAccountType() const{
    return "Google Hangouts";
}

QIcon GoogleAccount::getLogo() const
{
    return QIcon(":/logo/google");
}

QIcon GoogleAccount::getIcon() const
{
    return QIcon(":/logo/hangouts");
}

QString GoogleAccount::setUsername(QString newName) {
    int pos = newName.indexOf('@');
    if (pos < 0)
        newName.append("@gmail.com");
    username = newName;
    return "";
}

QString GoogleAccount::getLogin() const
{
    QString login = username;
    login.truncate(login.indexOf("@"));
    return login;
}
