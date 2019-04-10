#include "new_letter.h"
#include "ui_new_letter.h"

new_letter::new_letter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::new_letter)
{
    ui->setupUi(this);
}

new_letter::~new_letter()
{
    delete ui;
}

void new_letter::set_completer(const QStringList &list)
{
    if (nullptr != completer){
        delete completer;
        completer = nullptr;
    }

    completer = new (std::nothrow) QCompleter(list, ui->recipient_lineEdit);
    ui->recipient_lineEdit->setCompleter(completer);
}

void new_letter::show_all_completions()
{
    if (nullptr != completer){
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        completer->complete();
        completer->setCompletionMode(QCompleter::PopupCompletion);
    }
}

void new_letter::on_recipient_lineEdit_textChanged(const QString &arg1)
{

}
