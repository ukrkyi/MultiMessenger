#ifndef PERSONWIDGET_H
#define PERSONWIDGET_H

#include <QTextTableFormat>
#include <QUuid>
#include <QWidget>

namespace Ui {
class personWidget;
}

class Person;

class EnterPressHandler : public QObject
{
    Q_OBJECT
public:
    EnterPressHandler(QObject*parent):QObject(parent){}

protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void enterPressed();
};

class personWidget : public QWidget
{
    Q_OBJECT

public:
    explicit personWidget(Person*,QWidget *parent = 0);
    ~personWidget();

public slots:
    void updateImage(QImage);
    void updateName(QString);
    void updateRelations();
    void displayMessage(QUuid, QUuid);

private slots:
    void on_editButton_clicked();

    void on_sendButton_clicked();

    void on_methodBox_currentIndexChanged(int);

signals:
    void editingReq(QUuid);
    void msgSend(QUuid, QString);
    void mainRelChanged(QUuid);

private:
    Ui::personWidget *ui;
    QUuid displayedRel;
    const QString timeFormat = "(HH:mm:ss)";
    QTextTableFormat m_fmt;
    QTextTableCellFormat time_fmt, in_fmt, out_fmt;
    QUuid person;
};

#endif // PERSONWIDGET_H
