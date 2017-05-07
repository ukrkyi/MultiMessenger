#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>

class QAbstractButton;

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    void accept();
    ~StartDialog();

private slots:
    void on_nextButton_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::StartDialog *ui;
};

#endif // STARTDIALOG_H
