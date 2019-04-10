#ifndef BALANCE_H
#define BALANCE_H

#include <QWidget>
#include "qr_widget.h"

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

private:
    qr_widget *widget;
};

#endif // BALANCE_H
