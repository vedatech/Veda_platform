#ifndef QR_WIDGET_H
#define QR_WIDGET_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class qr_widget;
}

class qr_widget : public QWidget
{
    Q_OBJECT

public:
    explicit qr_widget(QWidget *parent = nullptr);
    ~qr_widget();

private:
    Ui::qr_widget *ui;
    virtual void paintEvent(QPaintEvent *e);
};

#endif // QR_WIDGET_H
