#ifndef MESSENGER_H
#define MESSENGER_H

#include <QDir>
#include <QFile>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <qpurple.h>

namespace Ui {
class Messenger;
}

class AccountManager;
class PersonManager;

class Messenger : public QMainWindow
{
    Q_OBJECT

public:
    explicit Messenger(QWidget *parent = 0);
    ~Messenger();

signals:
    void purpleConnect();

public slots:
    void addPersonButton(QPushButton *);
    void displayStart();
    void purpleInit(bool);

private slots:
    void on_settingsBtn_clicked(bool checked);

private:
    Ui::Messenger *ui;
    QVBoxLayout * lay;
    QPurple *prpl;
    QDir configDir;
    QFile* accountsFile, *personFile, *relationsFile, *messageCache;
    AccountManager* accman;
    PersonManager* persman;
};

#endif // MESSENGER_H
