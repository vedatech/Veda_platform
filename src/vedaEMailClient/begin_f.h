#ifndef BEGIN_F_H
#define BEGIN_F_H

#include <QWidget>
#include <QCompleter>
#include <QDebug>

namespace Ui {
class begin_f;
}

class begin_f : public QWidget
{
    Q_OBJECT

public:
    explicit begin_f(QWidget *parent = nullptr);
    ~begin_f();
    Ui::begin_f *ui;
    void set_completer(const QStringList &list);
    void show_all_completions();

private slots:

    void on_login_lineEdit_returnPressed();

    void on_password_lineEdit_returnPressed();

    void on_secret_lineEdit_returnPressed();

private:
    QCompleter *completer = nullptr;
};

#endif // BEGIN_F_H
