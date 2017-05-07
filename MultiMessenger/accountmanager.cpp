#include <QMessageBox>

#include "addaccount.h"
#include "facebookaccount.h"
#include "googleaccount.h"
#include "relation.h"
#include "startdialog.h"

#include "accountmanager.h"
#include "ui_accountmanager.h"

AccountManager::AccountManager(QWidget* parent) : QWidget(parent),
                                                  ui(new Ui::AccountManager) {
    ui->setupUi(this);
    font.setFamily(QStringLiteral("Droid Sans"));
    font.setPointSize(12);
}

AccountManager::~AccountManager() {
    delete ui;
}

void AccountManager::resizeEvent(QResizeEvent*) {
    ui->accountsTree->setColumnWidth(0, ui->accountsTree->width() * 3 / 4);
}

bool AccountManager::loadAccounts(QFile* f) {
    Account::loadFromFile(f);
    // Додавання акаунтів, якщо потрібно
    if (Account::items.empty()) {
        StartDialog d;
        if (d.exec() != QDialog::Accepted)
            return false;
    }
    // Додаємо всі нові записи в Purple!!
    QHash<QUuid, Account *>::iterator it = Account::items.begin(),
                                      end = Account::items.end();
    for (; it != end; it++) {
        addNewAcc(it.value());
    }
    return true;
}

void AccountManager::on_addAccount_clicked() {
    if (ui->accountsTree->selectedItems().empty() || ui->accountsTree->selectedItems().first()->parent()) {
        QMessageBox::warning(this, "Не обрано тип", "Спершу оберіть тип облікового запису!");
    } else {
        Account* acc = NULL;
        if (ui->accountsTree->selectedItems().first()->text(0) == FacebookAccount::type) {
            acc = new FacebookAccount;
        } else if (ui->accountsTree->selectedItems().first()->text(0) == GoogleAccount::type) {
            acc = new GoogleAccount;
        }
        AddAccount acc_d(acc, this);
        if (acc_d.exec() != QDialog::Accepted) {
            delete acc;
        } else {
            addNewAcc(acc);
        }
    }
}

void AccountManager::on_removeAccount_clicked()
{
    if (ui->accountsTree->selectedItems().empty() || !ui->accountsTree->selectedItems().first()->parent()) {
        QMessageBox::warning(this, "Не обрано запис", "Спершу оберіть обліковий запис!");
    } else {
        QTreeWidgetItem* it = ui->accountsTree->selectedItems().front(), *par = it->parent();
        if (par->text(1) == "1") {
            par->setText(1, "Немає");
        } else {
            par->setText(1, QString::number(par->text(1).toInt() - 1));
        }
        Account *acc = Account::findAccType(it->text(0), par->text(0));
        acc->deleteRelations();
        delete acc;
        par->removeChild(it);
        delete it;
    }
}


QTreeWidgetItem* AccountManager::findAccount(Account* acc) {
    QList<QTreeWidgetItem*> list = ui->accountsTree->findItems(acc->getAccountType(), Qt::MatchFixedString | Qt::MatchCaseSensitive, 0);
    if (list.empty())
        QMessageBox::critical(this, "Error", "Not found " + acc->getAccountType());
    QTreeWidgetItem* it = list.front();
    if (!it->childCount())
        QMessageBox::critical(this, "Error", "Знайшли не те, що треба: " + it->text(0));
    for (int i = 0; i < it->childCount(); i++) {
        QTreeWidgetItem* el = it->child(i);
        if (el->text(0) == acc->getLogin())
            return el;
    }
    return 0;
}

bool AccountManager::updateItem(Account* acc, QTreeWidgetItem* it) {
    if (!it)
        return false;
    it->setFont(0, font);
    it->setFont(1, font);
    it->setText(0, acc->getLogin());
    QString status;
    QColor color;
    if (acc->isEnabled()) {
        switch (acc->getStatus()) {
        case Account::Connected:
            status = "Зʼєднано";
            color = Qt::green;
            break;
        case Account::Waiting:
            status = "Зʼєднання…";
            color = Qt::yellow;
            break;
        case Account::Error:
            status = "Помилка";
            color = Qt::red;
            it->setToolTip(1, acc->getConnectionError());
            break;
        case Account::Disconnected:
            status = "Розʼєднано";
            color = Qt::gray;
            break;
        }
    } else {
        status = "Вимкнено";
        color = Qt::black;
    }
    it->setText(1, status);
    it->setTextColor(1, color);
    return true;
}

void AccountManager::addNewAcc(Account* acc) {
    QTreeWidgetItem* it = ui->accountsTree->findItems(acc->getAccountType(), Qt::MatchExactly, 0).front();
    it->setExpanded(true);
    if (it->text(1) == "Немає") {
        it->setText(1, "1");
    } else {
        it->setText(1, QString::number(it->text(1).toInt() + 1));
    }
    QTreeWidgetItem* newit = new QTreeWidgetItem(it);
    if (updateItem(acc, newit)) {
        emit accountAdded(*acc);
        if (acc->autoOnline()) {
            emit accountOnline(*acc);
        } else {
            emit accountInvisible(*acc);
        }
    }
}

void AccountManager::accDisable(PurpleInteraction::AccountStruct acc_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->disable();
    QTreeWidgetItem* it = findAccount(acc);
    updateItem(acc, it);
}

void AccountManager::accEnable(PurpleInteraction::AccountStruct acc_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->enable(acc_str.password);
    QTreeWidgetItem* it = findAccount(acc);
    updateItem(acc, it);
}

void AccountManager::accStatusConnecting(PurpleInteraction::AccountStruct acc_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->setStatus(Account::Waiting);
    QTreeWidgetItem* it = findAccount(acc);
    updateItem(acc, it);
}

void AccountManager::accStatusConnected(PurpleInteraction::AccountStruct acc_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->setStatus(Account::Connected);
    QTreeWidgetItem* it = findAccount(acc);
    updateItem(acc, it);
}

void AccountManager::accStatusDisconnect(PurpleInteraction::AccountStruct acc_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->setStatus(Account::Disconnected);
    QTreeWidgetItem* it = findAccount(acc);
    updateItem(acc, it);
}

void AccountManager::accStatusError(PurpleInteraction::AccountStruct acc_str, QString err) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->setConnectionError(err);
    QTreeWidgetItem* it = findAccount(acc);
    updateItem(acc, it);
}

void AccountManager::accSaveContacts(PurpleInteraction::AccountStruct acc_str, QList<PurpleInteraction::ContactStruct> cont) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    acc->setContacts(cont);
}

void AccountManager::accContactChange(PurpleInteraction::AccountStruct acc_str, QString old_name, PurpleInteraction::ContactStruct cont_str) {
    Account* acc = Account::findAccProtocol(acc_str.username, acc_str.protocol);
    if (acc) {
        acc->updateContactByName(old_name, cont_str);
        Relation* rel = Relation::findAccName(acc->id, old_name);
        if (rel) {
            rel->avatar = cont_str.avatar;
            rel->alias = cont_str.alias;
            rel->name = cont_str.name;
        }
    }
}

