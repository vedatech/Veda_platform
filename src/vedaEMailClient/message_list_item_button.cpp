#include "message_list_item_button.h"
#include "ui_message_list_item_button.h"

message_list_item_button::message_list_item_button(QString author, QString short_description, QString mSize, bool incoming, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::message_list_item_button)
{
    ui->setupUi(this);

    setStyleSheet("background-color: white;");

    ui->AuthorButton->setText(author);
    ui->DescrButton->setText(short_description);
    ui->sizeButton->setText(mSize);

    if (incoming){
        ui->AuthorButton->setCursor(Qt::PointingHandCursor);
        ui->DescrButton->setCursor(Qt::PointingHandCursor);
        ui->sizeButton->setCursor(Qt::PointingHandCursor);
        ui->pushButton->setCursor(Qt::PointingHandCursor);
        if (mSize.isEmpty()){
            ui->sizeButton->hide();
        }
    }
    else {
        ui->sizeButton->hide();
    }

    ui->AuthorButton->setFocusPolicy(Qt::NoFocus);
    ui->DescrButton->setFocusPolicy(Qt::NoFocus);
    ui->sizeButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);

    ui->line->setStyleSheet("background-color: black;");
}

message_list_item_button::~message_list_item_button()
{
    delete ui;
}

message_list_item::message_list_item(QString author, QString short_description, QString _rawTime, QString mSize, int index_in_vector, bool incoming, QWidget *parent) :
    QWidget(parent),
    index(index_in_vector),
    rawTime(_rawTime)
{
    content = new (std::nothrow) message_list_item_button(author, short_description, mSize, incoming, this);
    setFixedHeight(content->height());

    lay = new (std::nothrow) QGridLayout(this);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(content);

    if (incoming){
        connect(content->ui->AuthorButton, SIGNAL(clicked()), this, SIGNAL(clicked()));
        connect(content->ui->DescrButton, SIGNAL(clicked()), this, SIGNAL(clicked()));
        connect(content->ui->sizeButton, SIGNAL(clicked()), this, SIGNAL(clicked()));
        connect(content->ui->pushButton, SIGNAL(clicked()), this, SIGNAL(clicked()));
    }

    buttonStyleSheetBold = "QPushButton {"
                           "color: black;"
                           "border: none;"
                           "border-radius: none;"
//                            "padding: none;"
//                            "margin: none;"
                       "}"
                       "QPushButton:hover {"
                           "color: black;"
                           "border: none;"
                           "border-radius: none;"
//                            "padding: none;"
//                            "margin: none;"
                       "}"
                       "QPushButton:pressed {"
                           "color: black;"
                           "border: none;"
                           "border-radius: none;"
//                            "padding: none;"
//                            "margin: none;"
                       "}"
                       "QPushButton:checked {"
                           "color: black;"
                           "border: none;"
                           "border-radius: none;"
//                            "padding: none;"
//                            "margin: none;"
                       "}";

    buttonStyleSheetUnBold = "QPushButton {"
                             "color: gray;"
                             "border: none;"
                             "border-radius: none;"
 //                            "padding: none;"
 //                            "margin: none;"
                         "}"
                         "QPushButton:hover {"
                             "color: gray;"
                             "border: none;"
                             "border-radius: none;"
 //                            "padding: none;"
 //                            "margin: none;"
                         "}"
                         "QPushButton:pressed {"
                             "color: gray;"
                             "border: none;"
                             "border-radius: none;"
 //                            "padding: none;"
 //                            "margin: none;"
                         "}"
                         "QPushButton:checked {"
                             "color: gray;"
                             "border: none;"
                             "border-radius: none;"
 //                            "padding: none;"
 //                            "margin: none;"
                         "}";

    setBold();
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
    content->ui->AuthorButton->setText(author);
}

QString message_list_item::get_author()
{
    return content->ui->AuthorButton->text();
}

std::string message_list_item::get_wid()
{
    return wid;
}

QString message_list_item::getTime()
{
    return content->ui->DescrButton->text();
}

void message_list_item::set_wid(std::string wid)
{
    this->wid = wid;
}

void message_list_item::setBold()
{
    if (bold) return;
//    QFont font = content->ui->AuthorButton->font();
//    font.setBold(true);
//    content->ui->AuthorButton->setFont(font);
//    content->ui->DescrButton->setFont(font);
//    bold = true;
    content->ui->AuthorButton->setStyleSheet(buttonStyleSheetBold);
    content->ui->DescrButton->setStyleSheet(buttonStyleSheetBold);
    content->ui->sizeButton->setStyleSheet(buttonStyleSheetBold);
    content->ui->pushButton->setStyleSheet(buttonStyleSheetBold);
    bold = true;
}

void message_list_item::unsetBold()
{
    if (!bold) return;
//    QFont font = content->ui->AuthorButton->font();
//    font.setBold(false);
//    content->ui->AuthorButton->setFont(font);
//    content->ui->DescrButton->setFont(font);
//    bold = false;
    content->ui->AuthorButton->setStyleSheet(buttonStyleSheetUnBold);
    content->ui->DescrButton->setStyleSheet(buttonStyleSheetUnBold);
    content->ui->sizeButton->setStyleSheet(buttonStyleSheetUnBold);
    content->ui->pushButton->setStyleSheet(buttonStyleSheetUnBold);
    bold = false;
}

QString message_list_item::getRawTime()
{
    return rawTime;
}

//bool HoverFilter::eventFilter(QObject *obj, QEvent *ev)
//{
//    if (ev->type() == QEvent::HoverEnter){
//        w->setCursor(Qt::PointingHandCursor);
//    }
//    else if (ev->type() == QEvent::HoverLeave) {
//        w->setCursor(Qt::ArrowCursor);
//    }

//    return QObject::eventFilter(obj,ev);
//}
