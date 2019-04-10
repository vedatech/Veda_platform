#include "settings_general.h"
#include "ui_settings_general.h"

settings_general::settings_general(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_general)
{
    ui->setupUi(this);
}

settings_general::~settings_general()
{
    delete ui;
}

void settings_general::on_pushButton_clicked()
{
    ui->comboBox->showPopup();
}
