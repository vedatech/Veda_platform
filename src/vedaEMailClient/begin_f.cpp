#include "begin_f.h"
#include "ui_begin_f.h"

begin_f::begin_f(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::begin_f)
{
    ui->setupUi(this);
    ui->error_label->hide();
}

begin_f::~begin_f()
{
    delete ui;
}

void begin_f::set_completer(const QStringList &list)
{
    if (nullptr != completer){
        delete completer;
        completer = nullptr;
    }

    completer = new (std::nothrow) QCompleter(list, ui->login_lineEdit);
    ui->login_lineEdit->setCompleter(completer);
}

void begin_f::show_all_completions()
{
    if (nullptr != completer){
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        completer->complete();
        completer->setCompletionMode(QCompleter::PopupCompletion);
    }
}
