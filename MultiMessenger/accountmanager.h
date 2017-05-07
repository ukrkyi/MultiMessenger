#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <QFile>
#include <QTreeWidgetItem>
#include <QWidget>

#include "qpurple.h"

class Account;

namespace Ui {
class AccountManager;
}

class AccountManager : public QWidget
{
    Q_OBJECT

    QTreeWidgetItem * findAccount(Account *);
    bool updateItem(Account*, QTreeWidgetItem*);

public:
    explicit AccountManager(QWidget *parent = 0);
    ~AccountManager();
    void resizeEvent(QResizeEvent *);
    bool loadAccounts(QFile*);

signals:
    void accountAdded(PurpleInteraction::AccountStruct);
    void accountOnline(PurpleInteraction::AccountStruct);
    void accountInvisible(PurpleInteraction::AccountStruct);

private slots:
    void on_addAccount_clicked();

    void on_removeAccount_clicked();

public slots:
    void addNewAcc(Account *);

    void accDisable(PurpleInteraction::AccountStruct);
    void accEnable (PurpleInteraction::AccountStruct);
    void accStatusConnecting(PurpleInteraction::AccountStruct);
    void accStatusConnected (PurpleInteraction::AccountStruct);
    void accStatusDisconnect(PurpleInteraction::AccountStruct);
    void accStatusError     (PurpleInteraction::AccountStruct, QString);
    void accSaveContacts (PurpleInteraction::AccountStruct, QList<PurpleInteraction::ContactStruct>);
    void accContactChange(PurpleInteraction::AccountStruct, QString, PurpleInteraction::ContactStruct);

private:
    Ui::AccountManager *ui;
    QFont font;
};

#endif // ACCOUNTMANAGER_H
