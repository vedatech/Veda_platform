#include "balance_f.h"
#include "ui_balance_f.h"

balance_f::balance_f(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::balance_f)
{
    ui->setupUi(this);
    widget = new (std::nothrow)qr_widget(this);

    ui->QRhorizontalLayout->insertWidget(1,widget);

    clipboard = QApplication::clipboard();
}

balance_f::~balance_f()
{
    delete ui;
}


void balance_f::on_pushButton_2_clicked()
{
    clipboard->setText(ui->WID_lineEdit->text());
}
