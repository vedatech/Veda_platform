#ifndef OUTCOMING_LIST_ITEM_LABEL_H
#define OUTCOMING_LIST_ITEM_LABEL_H

#include <QWidget>

namespace Ui {
class outcoming_list_item_label;
}

class outcoming_list_item_label : public QWidget
{
    Q_OBJECT

public:
    explicit outcoming_list_item_label(QString recipient, QString time, QWidget *parent = nullptr);
    ~outcoming_list_item_label();
    void setRecipient(QString recipient);
    void setWID(std::string wid);
    QString getWID();

private:
    Ui::outcoming_list_item_label *ui;
    QString _wid;
};

#endif // OUTCOMING_LIST_ITEM_LABEL_H
