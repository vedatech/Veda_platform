#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);

    //в данном приложении не актуально
    ui->connection_Button->hide();
}

settings::~settings()
{
    delete ui;
}
