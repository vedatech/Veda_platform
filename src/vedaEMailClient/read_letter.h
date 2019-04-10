#ifndef READ_LETTER_H
#define READ_LETTER_H

#include <QWidget>

namespace Ui {
class read_letter;
}

class read_letter : public QWidget
{
    Q_OBJECT

public:
    explicit read_letter(QWidget *parent = nullptr);
    ~read_letter();
    Ui::read_letter *ui;
    void set_index(int index);
    int get_index();

private:
    int _index;
};

#endif // READ_LETTER_H
