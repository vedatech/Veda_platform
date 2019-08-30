#include "read_letter.h"
#include "ui_read_letter.h"

read_letter::read_letter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::read_letter)
{
    ui->setupUi(this);

    //beta
    ui->delete_Button->hide();
    ui->file_Button->hide();
}

read_letter::~read_letter()
{
    delete ui;
}

void read_letter::set_index(int index)
{
    _index = index;
}

int read_letter::get_index()
{
    return _index;
}
