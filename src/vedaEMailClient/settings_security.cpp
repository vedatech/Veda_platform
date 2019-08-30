#include "settings_security.h"
#include "ui_settings_security.h"

settings_security::settings_security(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_security)
{
    ui->setupUi(this);
    ui->spinBox->setDisabled(true);

    //beta
    ui->dualFA_Button->hide();
}

settings_security::~settings_security()
{
    delete ui;
}

void settings_security::toggle_autologout(bool on)
{
    if (on){
        ui->spinBox->setEnabled(true);
        autologout = true;
    }
    else {
        ui->spinBox->setDisabled(true);
        autologout = false;
    }
}

//void settings_security::on_pushButton_clicked()
//{
//    toggle_autologout(!autologout);
//}
