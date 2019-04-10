#include "vedaEMailClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    vedaEMailClient w;
    w.show();

    return a.exec();
}
