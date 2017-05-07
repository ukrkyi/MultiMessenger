#include <QMessageBox>
#include "account.h"

#include "addaccount.h"
#include "ui_addaccount.h"

AddAccount::AddAccount(Account *acc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAccount)
{
    ui->setupUi(this);
    this->acc = acc;
    ui->accountTypeLabel->setText(acc->getAccountTypeInfo());
    connect(ui->enabledBox,&QCheckBox::toggled,ui->passwordEdit,&QLineEdit::setEnabled);
    connect(ui->enabledBox,&QCheckBox::clicked,ui->passwordEdit,&QLineEdit::clear);
}

AddAccount::~AddAccount()
{
    delete ui;
}

void AddAccount::accept()
{
    if (!ui->usernameEdit->text().length()) {
        QMessageBox::critical(this,"Помилка","Порожнє імʼя користувача!");
    } else if (Account::findAccType(ui->usernameEdit->text(),acc->getAccountType())){
        QMessageBox::critical(this,"Помилка","Такий обліковий запис уже існує!");
    } else {
        QString err;
        if ((err = acc->setUsername(ui->usernameEdit->text())).length()){
            QMessageBox::critical(this,"Помилка",err);
        } else {
            if (ui->enabledBox->isChecked()){
                acc->enable(ui->passwordEdit->text());
                acc->setAutoOnline(ui->autoOnlineBox->isChecked());
            } else
                acc->disable();
        }
        QDialog::accept();
    }
}
