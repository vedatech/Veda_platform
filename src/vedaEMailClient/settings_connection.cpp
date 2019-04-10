#include "settings_connection.h"
#include "ui_settings_connection.h"

settings_connection::settings_connection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_connection)
{
    ui->setupUi(this);
}

settings_connection::~settings_connection()
{
    delete ui;
}
