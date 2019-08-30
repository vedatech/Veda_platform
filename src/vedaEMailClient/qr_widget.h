#ifndef QR_WIDGET_H
#define QR_WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QVector>

namespace Ui {
class qr_widget;
}

class qr_widget : public QWidget
{
    Q_OBJECT

public:
    explicit qr_widget(QWidget *parent = nullptr);
    ~qr_widget();
    void paintQRCode(const QVector<bool> &blacks, const int side_size);

private:
    Ui::qr_widget *ui;
    virtual void paintEvent(QPaintEvent *e);
    void resizeQRpolygon(int remains = 0);

    int maxside;
    int xasset;
    int yasset;
};

#endif // QR_WIDGET_H
