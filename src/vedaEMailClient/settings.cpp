#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

    //beta
//    ui->connection_Button->hide();
//    ui->information_Button->hide();
//    ui->general_Button->hide();
}

settings::~settings()
{
    delete ui;
}
