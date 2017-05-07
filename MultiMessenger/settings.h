#ifndef SETTINGS_H
#define SETTINGS_H

#include "programsettings.h"

#include <QAbstractButton>
#include <QFile>
#include <QWidget>
#include <QSettings>

class AccountManager;

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    AccountManager * getAccMan();

private slots:
    void display(ProgramSettings);

    void on_resultBox_clicked(QAbstractButton *button);

public slots:
    void defaults();
    void restore();
    void reset();
    void save();

signals:
    void finished(int);

private:
    Ui::Settings *ui;
    ProgramSettings current;
    QFile *f;
    QSettings sett;
};

#endif // SETTINGS_H
