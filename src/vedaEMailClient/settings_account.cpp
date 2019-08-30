#include "settings_account.h"
#include "ui_settings_account.h"

settings_account::settings_account(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_account)
{
    ui->setupUi(this);

    //beta
    ui->clear_local_address_book_Button->hide();
}

settings_account::~settings_account()
{
    delete ui;
}
