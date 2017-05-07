#include <QMessageBox>

#include "addrelation.h"
#include "ui_addrelation.h"

#include "account.h"
#include "person.h"
#include "relation.h"

AddRelation::AddRelation(Person* pers, QWidget* parent)
    : QDialog(parent),
      rel(NULL),
      ui(new Ui::AddRelation),
      who(pers)
      {
    ui->setupUi(this);
    QHash<QUuid, Account *>::iterator it = Account::items.begin(),
                                      end = Account::items.end();
    for (; it != end; it++) {
        ui->accountsBox->addItem((*it)->getLogo(), (*it)->getAccountType() + sep + (*it)->getUsername(),
                                 QVariant(QMetaType::VoidStar,&*it));
    }
}

AddRelation::~AddRelation() {
    delete ui;
}

void AddRelation::accept() {
    if (ui->contactsList->selectedItems().empty()) {
        QMessageBox::critical(this, "Помилка", "Не обрано контакт!");
    } else {
        Account* acc = (Account*)ui->accountsBox->currentData().value<void *>();
        PurpleInteraction::ContactStruct contact = ui->contactsList->currentItem()->data(Qt::UserRole).value<PurpleInteraction::ContactStruct>();
        if (Relation::findAccName(acc->id,contact.name)){
            QMessageBox::critical(this, "Помилка", "Такий звʼязок уже існує!");
        } else {
            rel = new Relation(who, acc, contact);
            QDialog::accept();
        }
    }
}

void AddRelation::on_accountsBox_currentIndexChanged()
{
    ui->contactsList->clear();
    Account* acc = (Account *) ui->accountsBox->currentData().value<void *>();
    if (acc) {
        for (QList<PurpleInteraction::ContactStruct>::iterator it = acc->contacts.begin(); it != acc->contacts.end(); it++) {
            QListWidgetItem* el = new QListWidgetItem(ui->contactsList);
            el->setIcon(QPixmap::fromImage((*it).avatar));
            el->setText((*it).alias + " (" + (*it).name + ")");
            QVariant v;
            v.setValue(*it);
            el->setData(Qt::UserRole,v);
        }
    }
}
