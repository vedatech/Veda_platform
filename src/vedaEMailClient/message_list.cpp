#include "message_list.h"
#include "ui_message_list.h"

message_list::message_list(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message_list)
{
    ui->setupUi(this);

    layout = new (std::nothrow) QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    ui->scrollAreaWidgetContents->setLayout(layout);
    layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

message_list::~message_list()
{
    delete ui;
}

void message_list::additem(QWidget *item)
{
    layout->insertWidget(0, item);
}

void message_list::removeitem(QWidget *item)
{
    layout->removeWidget(item);
}
