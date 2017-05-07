#include <QMessageBox>
#include <QDomDocument>
#include "accountmanager.h"

#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget* parent) : QWidget(parent),
                                                           ui(new Ui::Settings), sett("MultiMessenger","settings") {
    ui->setupUi(this);
    properties.reverseOrder = sett.value("appearance/reverse",false).toBool();
    current = properties;
    display(current);
}

Settings::~Settings() {
    delete ui;
}

AccountManager* Settings::getAccMan() {
    return ui->accMan;
}

void Settings::display(ProgramSettings sett) {
    ui->reverseBox->setChecked(sett.reverseOrder);
}

void Settings::defaults() {
    display(default_set);
}

void Settings::restore() {
    display(properties);
}

void Settings::reset()
{
    display(current);
    emit finished(false);
}

void Settings::save() {
    QMessageBox::information(this,"Увага!","Нові властивості будуть застосовані наступного запуску");
    current.reverseOrder = ui->reverseBox->isChecked();
    sett.setValue("appearance/reverse",current.reverseOrder);
}

void Settings::on_resultBox_clicked(QAbstractButton* button) {
    if (ui->resultBox->standardButton(button) == QDialogButtonBox::RestoreDefaults) {
        this->defaults();
    } else if (ui->resultBox->standardButton(button) == QDialogButtonBox::Reset) {
        this->restore();
    }
}
