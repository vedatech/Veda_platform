#include "registration.h"
#include "ui_registration.h"

registration::registration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registration)
{
    ui->setupUi(this);

    //в данном приложении не актуально
    ui->city_lineEdit->hide();
    ui->state_lineEdit->hide();
}

registration::~registration()
{
delete ui;
}

void registration::on_mail_lineEdit_textChanged(const QString &arg1)
{
//    if ()
}