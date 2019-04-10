#include "settings_account.h"
#include "ui_settings_account.h"

settings_account::settings_account(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_account)
{
    ui->setupUi(this);
}

settings_account::~settings_account()
{
    delete ui;
}
