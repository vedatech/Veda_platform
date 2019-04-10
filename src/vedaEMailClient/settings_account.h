#ifndef SETTINGS_ACCOUNT_H
#define SETTINGS_ACCOUNT_H

#include <QWidget>

namespace Ui {
class settings_account;
}

class settings_account : public QWidget
{
    Q_OBJECT

public:
    explicit settings_account(QWidget *parent = nullptr);
    ~settings_account();
    Ui::settings_account *ui;

private:
};

#endif // SETTINGS_ACCOUNT_H
