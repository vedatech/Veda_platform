#include "settings_information.h"
#include "ui_settings_information.h"

settings_information::settings_information(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_information)
{
    ui->setupUi(this);
}

settings_information::~settings_information()
{
    delete ui;
}
