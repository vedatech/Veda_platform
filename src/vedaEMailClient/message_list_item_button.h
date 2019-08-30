#ifndef MESSAGE_LIST_ITEM_BUTTON_H
#define MESSAGE_LIST_ITEM_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

namespace Ui {
//class HoverFilter;
class message_list_item_button;
class message_list_item;
}

//class HoverFilter: public QObject
//{
//    Q_OBJECT

//public:
//    explicit HoverFilter(QPushButton *applier = nullptr):
//        w(applier){};

//protected:
//    bool eventFilter(QObject *obj, QEvent *ev);

//private:
//    QPushButton *w;
//    bool left;

//signals:
//    void clicked();
//};

class message_list_item_button : public QWidget
{
    Q_OBJECT

public:
    explicit message_list_item_button(QString author, QString short_description, QString mSize, bool incoming, QWidget *parent = nullptr);
    ~message_list_item_button();
    Ui::message_list_item_button *ui;
//    HoverFilter *ahf,
//                *dhf;

private:
};

class message_list_item: public QWidget
{
    Q_OBJECT

public:
    explicit message_list_item(QString author, QString short_description, QString _rawTime, QString mSize, int index_in_vector, bool incoming, QWidget *parent = nullptr);
    ~message_list_item();
    int get_index();
    void set_author(const QString &author);
    QString get_author();
    std::string get_wid();
    QString getTime();
    void set_wid(std::string wid);
    void setBold();
    void unsetBold();
    QString getRawTime();

private:
    QGridLayout *lay;
    message_list_item_button *content;
    int index;
    std::string wid;
    bool bold = false;
    QString rawTime;
    QString buttonStyleSheetBold;
    QString buttonStyleSheetUnBold;

signals:
    void clicked();

};

#endif // MESSAGE_LIST_ITEM_BUTTON_H
