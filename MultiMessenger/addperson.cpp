#include "addrelation.h"

#include "account.h"
#include "person.h"
#include "relation.h"

#include "addperson.h"
#include "ui_addperson.h"

#include <QMessageBox>

AddPerson::AddPerson(Person* pers, QWidget* parent) : QDialog(parent),
                                                      who(pers),
                                                      ui(new Ui::AddPerson) {
    ui->setupUi(this);
    ui->imageBox->setItemData(0, QImage(":/ico/user"));
    QList<Relation*> list = pers->getConnections();
    for (QList<Relation *>::iterator it = list.begin();it != list.end();it++)
        addNewRel(*it);
    ui->nameBox->setCurrentText(pers->name);
    ui->imageBox->setCurrentIndex(ui->imageBox->findData(pers->avatar));
}

AddPerson::~AddPerson() {
    delete ui;
}

void AddPerson::addNewRel(Relation* rel) {
    QListWidgetItem* it = new QListWidgetItem(ui->relationsList);
    it->setIcon(QIcon(rel->getAccount()->getLogo()));
    it->setText(rel->alias + " (" + rel->name + ")");
    ui->nameBox->addItem(rel->getAccount()->getLogo(), rel->alias);
    if (!rel->avatar.isNull()){
        ui->imageBox->addItem(QIcon(QPixmap::fromImage(rel->avatar)), "", rel->avatar);
        if (!ui->imageBox->currentIndex())
            ui->imageBox->setCurrentIndex(1);
    }
}

void AddPerson::on_deleteButton_clicked() {
    emit deletePerson();
}

void AddPerson::on_addButton_clicked() {
    AddRelation rel_d(who, this);
    if (rel_d.exec() == QDialog::Accepted) {
        Relation* rel = rel_d.rel;
        addNewRel(rel);
        emit relationAdded(rel->id);
    }
}

void AddPerson::accept() {
    if (ui->relationsList->count() && ui->nameBox->currentText().length()) {
        who->name = ui->nameBox->currentText();
        who->avatar = ui->imageBox->currentData().value<QImage>();
        QDialog::accept();
    } else {
        QMessageBox::critical(this, "Помилка", "Не обрано імʼя контакту або порожній список звʼязків!");
    }
}
