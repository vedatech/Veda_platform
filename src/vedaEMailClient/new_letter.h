#ifndef NEW_LETTER_H
#define NEW_LETTER_H

#include <QWidget>
#include <QCompleter>

namespace Ui {
class new_letter;
}

class new_letter : public QWidget
{
    Q_OBJECT

public:
    explicit new_letter(QWidget *parent = nullptr);
    ~new_letter();
    Ui::new_letter *ui;
    void set_completer(const QStringList &list);
    void show_all_completions();
    void clear();

private slots:
    void on_recipient_lineEdit_textChanged(const QString &arg1);

private:
    bool recipient_is_ok;
    QCompleter *completer = nullptr;
};

#endif // NEW_LETTER_H
