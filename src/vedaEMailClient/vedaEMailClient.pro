#-------------------------------------------------
#
# Project created by QtCreator 2019-03-06T16:05:43
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VedaEMail
TEMPLATE = app

DESTDIR = ../../bin/

DEFINES +=  VEDA_PROTO_BUF

macx{
    DEFINES += MACOS
    QMAKE_CXXFLAGS += -fdeclspec
    ICON = ./Resources/veda_app_icon.icns
    #QMAKE_INFO_PLIST = Info.plist
}

win32{
    DEFINES += WINDOWS
    RC_ICONS += ./Resources/veda_app_icon.ico
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../libvedawallet
INCLUDEPATH += $$PWD/../libvedainiwrapper
INCLUDEPATH += $$PWD/QR_code_lib
INCLUDEPATH += $$PWD/../libvedatransactions
INCLUDEPATH +=  ../protobuf-3.6.1/src/

unix:!macx{
    DEFINES += LINUX_NON_MACOS
    LIBS += -L.. -L. -Llib -L../lib -L../../lib -lvedawallet -lvedainiwrapper -lprotobuf #-lvedatransactions
    LIBS += -Wl,-rpath,. -Wl,-rpath,.. -Wl,-rpath,lib -Wl,-rpath,../lib -Wl,-rpath,../../lib
    LIBS += -Wl,-rpath,$(DEFAULT_LIB_INSTALL_PATH)
    LIBS += -fpic
}
macx{
    LIBS += -L.. -L. -L./lib -L../lib -L../../lib -lvedawallet -lvedainiwrapper
    LIBS += -fpic
}

win32{
    LIBS += -L.. -L. -Llib -L../lib -L../../lib -lvedawallet -lvedainiwrapper
    LIBS += -lws2_32
    LIBS += -fpic
}

SOURCES += \
    ../libvedatransactions/token.pb.cc \
    fileattachment.cpp \
        main.cpp \
    mainwindow.cpp \
    vedaEMailClient.cpp \
    registration.cpp \
    main_menu.cpp \
    message_list_item_button.cpp \
    new_letter.cpp \
    message_list.cpp \
    read_letter.cpp \
    settings.cpp \
    settings_information.cpp \
    settings_account.cpp \
    settings_connection.cpp \
    settings_general.cpp \
    settings_security.cpp \
    qr_widget.cpp \
    loading_screen.cpp \
    server_functions.cpp \
    begin_f.cpp \
    balance_f.cpp \
    outcoming_list_item_label.cpp

HEADERS += \
    ../libvedatransactions/token.pb.h \
    fileattachment.h \
    mainwindow.h \
    vedaEMailClient.h \
    registration.h \
    main_menu.h \
    message_list_item_button.h \
    new_letter.h \
    message_list.h \
    read_letter.h \
    settings.h \
    settings_information.h \
    settings_account.h \
    settings_connection.h \
    settings_general.h \
    settings_security.h \
    qr_widget.h \
    loading_screen.h \
    begin_f.h \
    balance_f.h \
    outcoming_list_item_label.h

FORMS += \
    fileattachment.ui \
    mainwindow.ui \
    vedaEMailClient.ui \
    registration.ui \
    main_menu.ui \
    message_list_item_button.ui \
    new_letter.ui \
    message_list.ui \
    read_letter.ui \
    settings.ui \
    settings_information.ui \
    settings_account.ui \
    settings_connection.ui \
    settings_general.ui \
    settings_security.ui \
    qr_widget.ui \
    loading_screen.ui \
    begin_f.ui \
    balance_f.ui \
    outcoming_list_item_label.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
