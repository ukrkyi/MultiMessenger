#ifndef ADDACCOUNT_H
#define ADDACCOUNT_H

#include <QDialog>

namespace Ui {
class AddAccount;
}

class Account;

class AddAccount : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccount(Account *acc, QWidget *parent = 0);
    ~AddAccount();

private:
    Ui::AddAccount *ui;
    Account *acc;
    void accept();
};

#endif // ADDACCOUNT_H
