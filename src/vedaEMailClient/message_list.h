#ifndef MESSAGE_LIST_H
#define MESSAGE_LIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSpacerItem>

namespace Ui {
class message_list;
}

class message_list : public QWidget
{
    Q_OBJECT

public:
    explicit message_list(QWidget *parent = nullptr);
    ~message_list();
    Ui::message_list *ui;
    void additem(QWidget *item);
    void removeitem(QWidget *item);

private:
    QVBoxLayout *layout;
};

#endif // MESSAGE_LIST_H
