#include <QIcon>
#include "facebookaccount.h"

const QString FacebookAccount::type = QString("Facebook Messenger");

FacebookAccount::FacebookAccount() : Account() {protocol = "prpl-facebook";}

FacebookAccount::FacebookAccount(QDataStream& stream)
// Відновлюємо обʼєкт
    : Account(stream)
{
    protocol = "prpl-facebook";
}

QString FacebookAccount::getAccountTypeInfo() const {
    return "<img src=\"qrc://logo/facebook\" height=\"16\"> Facebook (Messenger)";
}

QString FacebookAccount::getAccountType() const {
    return "Facebook Messenger";
}

QIcon FacebookAccount::getLogo() const
{
    return QIcon(":/logo/facebook");
}

QIcon FacebookAccount::getIcon() const
{
    return QIcon(":/logo/messenger");
}
QString FacebookAccount::setUsername(QString newName) {
    int pos = newName.indexOf('@');
    QStringRef domain = newName.rightRef(newName.length() - pos);
    if (pos >= 0) {
        if (domain != "facebook.com" && domain != "fb.com")
            return "Неправильний домен!";
        newName.truncate(pos);
    }
    username = newName;
    return "";
}
