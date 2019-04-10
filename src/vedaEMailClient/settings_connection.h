#ifndef SETTINGS_CONNECTION_H
#define SETTINGS_CONNECTION_H

#include <QWidget>

namespace Ui {
class settings_connection;
}

class settings_connection : public QWidget
{
    Q_OBJECT

public:
    explicit settings_connection(QWidget *parent = nullptr);
    ~settings_connection();
    Ui::settings_connection *ui;

private:
};

#endif // SETTINGS_CONNECTION_H
