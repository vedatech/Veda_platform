#ifndef SETTINGS_GENERAL_H
#define SETTINGS_GENERAL_H

#include <QWidget>

namespace Ui {
class settings_general;
}

class settings_general : public QWidget
{
    Q_OBJECT

public:
    explicit settings_general(QWidget *parent = nullptr);
    ~settings_general();
    Ui::settings_general *ui;

private slots:
    void on_pushButton_clicked();

private:
};

#endif // SETTINGS_GENERAL_H
