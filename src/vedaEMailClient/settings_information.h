#ifndef SETTINGS_INFORMATION_H
#define SETTINGS_INFORMATION_H

#include <QWidget>

namespace Ui {
class settings_information;
}

class settings_information : public QWidget
{
    Q_OBJECT

public:
    explicit settings_information(QWidget *parent = nullptr);
    ~settings_information();
    Ui::settings_information *ui;

private:
};

#endif // SETTINGS_INFORMATION_H
