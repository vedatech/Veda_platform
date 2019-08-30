#include "vedaEMailClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InactivityFilter filter;
    a.installEventFilter(&filter);

#ifdef WINDOWS
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1,1), &wsaData);
//    QFont font("MS Shell Dlg 2",11,1,false);
//    a.setFont(font);
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#elif MACOS
    QFont font("MS Shell Dlg 2", 11);
    a.setFont(font);
#endif

    vedaEMailClient w;

    QObject::connect(&filter, SIGNAL(activity()), &w, SLOT(resetAutoLogOutTimer()));

    w.show();

    return a.exec();
}
