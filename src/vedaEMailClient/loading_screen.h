#ifndef LOADING_SCREEN_H
#define LOADING_SCREEN_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class loading_screen;
}

class loading_screen : public QWidget
{
    Q_OBJECT

public:
    explicit loading_screen(QWidget *parent = nullptr);
    ~loading_screen();

    void _show();
    void _hide();

private:
    Ui::loading_screen *ui;
    QMovie *gif;
};

#endif // LOADING_SCREEN_H
