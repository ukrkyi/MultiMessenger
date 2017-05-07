#include <QStandardPaths>
#include <QMessageBox>

#include "account.h"
#include "person.h"
#include "relation.h"
#include "message.h"

#include "programsettings.h"
#include "personbutton.h"
#include "personwidget.h"
#include "startdialog.h"
#include "addperson.h"

#include "accountmanager.h"
#include "personmanager.h"

#include "messenger.h"
#include "ui_messenger.h"

Messenger::Messenger(QWidget* parent) : QMainWindow(parent),
                                        ui(new Ui::Messenger) {
    ui->setupUi(this);
    configDir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    if (!configDir.exists())
        configDir.mkpath(".");

    lay = (QVBoxLayout*)(ui->personList->layout());
    persman = new PersonManager(this);
    connect(ui->addPerson, &QPushButton::clicked, persman, &PersonManager::add);
    connect(persman, &PersonManager::newButtonAdded, this, &Messenger::addPersonButton);
    connect(persman, &PersonManager::currentWidgetChanged, ui->displayStack, &QStackedWidget::setCurrentWidget);
    connect(persman, &PersonManager::newWidgetAdded, ui->displayStack, &QStackedWidget::addWidget);
    connect(persman, &PersonManager::actionsEnabled, ui->personList, &QWidget::setEnabled);
    connect(persman, &PersonManager::chSetBtn, ui->settingsBtn, &QPushButton::setChecked);

    accountsFile = new QFile(configDir.absoluteFilePath("accounts"));
    personFile = new QFile(configDir.absoluteFilePath("person"));
    relationsFile = new QFile(configDir.absoluteFilePath("relations"));
    messageCache = new QFile(configDir.absoluteFilePath("messages"));
    accman = ui->settingsPage->getAccMan();
    connect(ui->settingsPage,&Settings::finished,ui->settingsBtn,&QPushButton::click);
    // Ініціалізація Libpurple
    prpl = new QPurple();
#define purple (prpl->getInstance())
    connect(this, &Messenger::purpleConnect, purple, &PurpleInteraction::run);

    connect(accman, &AccountManager::accountAdded, purple, &PurpleInteraction::addAccount);
    connect(accman, &AccountManager::accountOnline, purple, &PurpleInteraction::putOnline);
    connect(accman, &AccountManager::accountInvisible, purple, &PurpleInteraction::putInvisible);

    connect(purple, &PurpleInteraction::accountConnecting, accman, &AccountManager::accStatusConnecting);
    connect(purple, &PurpleInteraction::accountConnected, accman, &AccountManager::accStatusConnected);
    connect(purple, &PurpleInteraction::accountDisconnected, accman, &AccountManager::accStatusDisconnect);
    connect(purple, &PurpleInteraction::accountConnectError, accman, &AccountManager::accStatusError);
    connect(purple, &PurpleInteraction::accountEnabled, accman, &AccountManager::accEnable);
    connect(purple, &PurpleInteraction::accountDisabled, accman, &AccountManager::accDisable);
    connect(purple, &PurpleInteraction::contactsLoaded, accman, &AccountManager::accSaveContacts);
    connect(purple, &PurpleInteraction::contactChanged, accman, &AccountManager::accContactChange);

    connect(persman, &PersonManager::createConversation, purple, &PurpleInteraction::createConversation);
    connect(purple, &PurpleInteraction::messageRecieved, persman, &PersonManager::messageLoad);
    connect(persman, &PersonManager::sendMessage, purple, &PurpleInteraction::sendMessage);

    //connect(purple,&PurpleInteraction::accountDisconnected,purple,&PurpleInteraction::connectAccount);

    connect(purple, &PurpleInteraction::init, this, &Messenger::purpleInit);
    emit purpleConnect();
}

void Messenger::purpleInit(bool success) {
    if (!success) {
        QMessageBox::critical(this, "Помилка запуску",
            "Не вдалося завантажити бібліотеку роботи з сервісами (Purple).\n"
            "Програму буде закрито.\n\n"
            "Повідомте розробника: yasia.o@ukr.net");
        QApplication::exit(1);
    }
    if (!accman->loadAccounts(accountsFile))
        QApplication::exit(0);
    persman->load(personFile,relationsFile,messageCache);
    show();
}

void Messenger::displayStart() {
    ui->displayStack->setCurrentWidget(ui->startPage);
}

Messenger::~Messenger() {
    delete ui;
    Account::saveToFile(accountsFile);
    Person::saveToFile(personFile);
    Relation::saveToFile(relationsFile);
    for (QHash<QUuid, Relation*>::iterator it = Relation::items.begin(),
         end = Relation::items.end();it!=end;it++){
        it.value()->clearMessages(10);
    }
    Message::saveToFile(messageCache);
    delete accountsFile;
    delete personFile;
    delete relationsFile;
    delete messageCache;
    accman->deleteLater();
    persman->deleteLater();
    // BUG Десь тут
    // Чомусь не дочікується закриття Purple
    delete prpl;
}

void Messenger::addPersonButton(QPushButton *butt) {
    lay->insertWidget(properties.reverseOrder ? (lay->count() - 4) : 1, butt);
}

void Messenger::on_settingsBtn_clicked(bool checked) {
    if (checked){
        persman->uncheck();
        ui->displayStack->setCurrentWidget(ui->settingsPage);
    } else {
        if (!persman->check())
            ui->displayStack->setCurrentIndex(0);
    }
}
