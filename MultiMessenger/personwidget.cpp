#include "personwidget.h"
#include "ui_personwidget.h"

#include <QTextTable>

#include "account.h"
#include "message.h"
#include "person.h"
#include "relation.h"

personWidget::personWidget(Person* pers, QWidget* parent) : QWidget(parent),
                                                            ui(new Ui::personWidget),
                                                            person(pers->id) {
    ui->setupUi(this);
    EnterPressHandler* enterHandler = new EnterPressHandler(this);
    ui->msgBox->installEventFilter(enterHandler);
    connect(enterHandler, &EnterPressHandler::enterPressed, ui->sendButton, &QPushButton::click);
    ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(":/ico/user")));
    m_fmt.setBorder(0);
    m_fmt.setColumnWidthConstraints({ QTextLength(QTextLength::FixedLength, QFontMetricsF(ui->messagesView->font()).width("(00:00:00) ")),
        QTextLength(QTextLength::PercentageLength, 100) });
    time_fmt.setPadding(3);
    time_fmt.setFontPointSize(10);
    time_fmt.setFontWeight(800);
    in_fmt.setBackground(QBrush(QColor("LightSkyBlue")));
    in_fmt.setPadding(3);
    in_fmt.setFontPointSize(10);
    out_fmt.setBackground(QBrush(QColor("SpringGreen")));
    out_fmt.setPadding(3);
    out_fmt.setFontPointSize(10);
    connect(pers, &Person::newMessage, this, &personWidget::displayMessage);
    connect(pers, &Person::avatarChanged, this, &personWidget::updateImage);
    connect(pers, &Person::nameChanged, this, &personWidget::updateName);
    connect(this, &personWidget::mainRelChanged, pers, &Person::changeMainRel);
    updateName(pers->name);
    updateImage(pers->avatar);
    QList<Message*> list = pers->getMessages();
    for (QList<Message *>::iterator it = list.begin(),
                                    end = list.end();
         it != end; it++) {
        displayMessage((*it)->relation, (*it)->id);
    }
    updateRelations();
}

personWidget::~personWidget() {
    delete ui;
}

void personWidget::updateImage(QImage img) {
    ui->imageLabel->setPixmap(QPixmap::fromImage(img));
}

void personWidget::updateName(QString name) {
    ui->nameLabel->setText("<span style=\" font-size:14pt;\">" + name + "</span>");
}

void personWidget::updateRelations()
{
    Person *pers = Person::items[person];
    QList<Relation*> rels = pers->getConnections();
    for (QList<Relation *>::iterator it = rels.begin(),
                                     end = rels.end();
         it != end; it++) {
        ui->methodBox->addItem((*it)->getAccount()->getIcon(), "", (*it)->id);
    }
    if (!pers->mainRelation.isNull())
        ui->methodBox->setCurrentIndex(ui->methodBox->findData(pers->mainRelation));
}

void personWidget::displayMessage(QUuid relid, QUuid msgid) {
    QTextCursor curs(ui->messagesView->textCursor());
    curs.movePosition(QTextCursor::End);
    if (displayedRel != relid) {
        curs.insertHtml("<div style=\"font-size: 8pt; color:gray; margin-left: 40px;\">"
            + Relation::items[relid]->getAccount()->getAccountType() + "</div>");
        curs.insertHtml("<hr />");
        displayedRel = relid;
    }
    Message* msg = Message::items[msgid];
    QString time = msg->time.toString(timeFormat), text = msg->text;
    QTextTable* t = curs.insertTable(1, 2, m_fmt);
    t->cellAt(0, 0).setFormat(time_fmt);
    if (msg->type == Message::Incoming) {
        t->cellAt(0, 1).setFormat(in_fmt);
    } else {
        t->cellAt(0, 1).setFormat(out_fmt);
    }
    curs.insertText(time);
    curs.movePosition(QTextCursor::NextCell);
    curs.insertHtml(text);
}

void personWidget::on_editButton_clicked() {
    emit editingReq(person);
}

bool EnterPressHandler::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return) {
            emit enterPressed();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void personWidget::on_sendButton_clicked() {
    QString msg = ui->msgBox->toHtml();
    msg = msg.mid(msg.indexOf("<p"), msg.indexOf("</body>") - msg.indexOf("<p"));
    ui->msgBox->clear();
    emit msgSend(ui->methodBox->currentData().toUuid(), msg);
}

void personWidget::on_methodBox_currentIndexChanged(int) {
    emit mainRelChanged(ui->methodBox->currentData().toUuid());
}
