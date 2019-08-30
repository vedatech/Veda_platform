#include "outcoming_list_item_label.h"
#include "ui_outcoming_list_item_label.h"

outcoming_list_item_label::outcoming_list_item_label(QString recipient, QString time, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outcoming_list_item_label)
{
    ui->setupUi(this);
    ui->recipient_label->setText(recipient);
    ui->time_label->setText(time);
}

outcoming_list_item_label::~outcoming_list_item_label()
{
    delete ui;
}

void outcoming_list_item_label::setRecipient(QString recipient)
{
    ui->recipient_label->setText(recipient);
}

void outcoming_list_item_label::setWID(std::string wid)
{
    _wid = QString::fromStdString(wid);
}

QString outcoming_list_item_label::getWID()
{
    return _wid;
}
