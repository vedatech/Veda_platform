#ifndef SETTINGS_SECURITY_H
#define SETTINGS_SECURITY_H

#include <QWidget>

#define AUTOLOGOUT_BUTTON_ON_TEXT "Auto Log out: On"
#define AUTOLOGOUT_BUTTON_OFF_TEXT "Auto Log out: Off"

namespace Ui {
class settings_security;
}

class settings_security : public QWidget
{
    Q_OBJECT

public:
    explicit settings_security(QWidget *parent = nullptr);
    ~settings_security();
    Ui::settings_security *ui;
    void toggle_autologout(bool on);

private slots:
    void on_pushButton_clicked();

private:
    bool autologout = false;

};

#endif // SETTINGS_SECURITY_H
