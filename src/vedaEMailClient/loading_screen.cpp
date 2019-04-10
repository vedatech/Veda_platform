#include "loading_screen.h"
#include "ui_loading_screen.h"

loading_screen::loading_screen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loading_screen)
{
    ui->setupUi(this);
    gif = new (std::nothrow) QMovie(":/Resources/loading.gif");
    ui->label->setMovie(gif);
}

loading_screen::~loading_screen()
{
    delete ui;
    delete gif;
}

void loading_screen::_show()
{
    show();
    gif->start();
}

void loading_screen::_hide()
{
    hide();
    gif->stop();
}
