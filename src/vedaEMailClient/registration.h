#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>

namespace Ui {
class registration;
}

class registration : public QWidget
{
    Q_OBJECT

public:
    explicit registration(QWidget *parent = nullptr);
    ~registration();
    Ui::registration *ui;

private slots:
//    void on_mail_lineEdit_textChanged(const QString &arg1);

private:
};

#endif // REGISTRATION_H
