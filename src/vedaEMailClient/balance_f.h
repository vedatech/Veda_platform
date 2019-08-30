#ifndef BALANCE_H
#define BALANCE_H

#include <QWidget>
#include "qr_widget.h"
#include <QClipboard>

namespace Ui {
class balance_f;
}

class balance_f : public QWidget
{
    Q_OBJECT

public:
    explicit balance_f(QWidget *parent = nullptr);
    ~balance_f();
    Ui::balance_f *ui;

private slots:
    void on_pushButton_2_clicked();

private:
    qr_widget *widget;
    QClipboard *clipboard;
};

#endif // BALANCE_H
