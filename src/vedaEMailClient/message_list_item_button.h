#ifndef MESSAGE_LIST_ITEM_BUTTON_H
#define MESSAGE_LIST_ITEM_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

namespace Ui {
class message_list_item_button;
class message_list_item;
}

class message_list_item_button : public QWidget
{
    Q_OBJECT

public:
    explicit message_list_item_button(QString author, QString short_description, QWidget *parent = nullptr);
    ~message_list_item_button();
    Ui::message_list_item_button *ui;

private:
};

class message_list_item: public QPushButton
{
    Q_OBJECT

public:
    explicit message_list_item(QString author, QString short_description, int index_in_vector, QWidget *parent = nullptr);
    ~message_list_item();
    int get_index();
    void set_author(const QString &author);
    QString get_author();
    std::string get_wid();
    void set_wid(std::string wid);

private:
    QGridLayout *lay;
    message_list_item_button *content;
    int index;
    std::string wid;

};

#endif // MESSAGE_LIST_ITEM_BUTTON_H
