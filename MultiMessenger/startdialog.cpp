#include <QAbstractButton>
#include <QMessageBox>

#include "settings.h"

#include "startdialog.h"
#include "ui_startdialog.h"

#include "account.h"

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);
    // BUG: this don't work
    //connect(ui->nextButton,SIGNAL(clicked()),ui->settingsPage,SLOT(raise()));
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::on_nextButton_clicked()
{
    // HACK
    ui->stackedWidget->setCurrentIndex(1);
}

void StartDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ResetRole){
        ;
    }
}

void StartDialog::accept(){
    if (Account::items.empty()){
        QMessageBox::critical(this,"Помилка","Ви не додали жодного облікового запису!");
    } else {
        QDialog::accept();
    }
}
