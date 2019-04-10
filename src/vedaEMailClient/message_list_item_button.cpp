#include "message_list_item_button.h"
#include "ui_message_list_item_button.h"

message_list_item_button::message_list_item_button(QString author, QString short_description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message_list_item_button)
{
    ui->setupUi(this);
    ui->author_label->setText(author);
    ui->description_label->setText(short_description);
}

message_list_item_button::~message_list_item_button()
{
    delete ui;
}

message_list_item::message_list_item(QString author, QString short_description, int index_in_vector, QWidget *parent) :
    QPushButton(parent)
{
    lay = new (std::nothrow) QGridLayout(this);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    content = new (std::nothrow) message_list_item_button(author, short_description, this);
    setFixedHeight(content->height());
    lay->addWidget(content);
    index = index_in_vector;
}

message_list_item::~message_list_item()
{
    delete content;
    delete lay;
}

int message_list_item::get_index()
{
    return index;
}

void message_list_item::set_author(const QString &author)
{
    content->ui->author_label->setText(author);
}

QString message_list_item::get_author()
{
    return content->ui->author_label->text();
}

std::string message_list_item::get_wid()
{
    return wid;
}

void message_list_item::set_wid(std::string wid)
{
    this->wid = wid;
}
