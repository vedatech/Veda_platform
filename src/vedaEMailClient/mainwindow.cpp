#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAutoFillBackground(true);

    cb = QApplication::clipboard();

    QList<QPushButton *> allPButtons = findChildren<QPushButton *>();
    for (int i = 0, limit = allPButtons.size(); i < limit; i++) {
        allPButtons[i]->setFocusPolicy(Qt::NoFocus);
    }

    setStyleSheet("QPushButton {background-color: rgb(0, 162, 230);"
                               "color: white;"
                               "border: 3px solid white;"
                               "padding: 5px;"
                               "border-radius: 10px;}"
                  "QPushButton:hover{background-color:  rgb(0, 196, 255,95);"
                                    "color:white;"
                                    "border: 3px solid white;"
                                    "padding: 5px;"
                                    "border-radius: 10px;}"
                  "QPushButton:pressed{background-color:  rgb(0, 196, 255,0);"
                                      "color:white;"
                                      "border: 3px solid white;"
                                      "padding: 5px;"
                                      "border-radius: 10px;}"
                  "QPushButton:checked{background-color:  rgb(0, 196, 255,0);"
                                      "color:white;"
                                      "border: 3px solid white;"
                                      "padding: 5px;"
                                      "border-radius: 10px;}"
                  "QPushButton:disabled{border: none; color: gray}"

                  "QLineEdit{color: black;}"

                  "QComboBox{color: black;"
                            "border-radius: 10px;}"
                  "QComboBox::down-arrow {background-color: white;"
                                         "border-top-left-radius: 10px;"
                                         "border-top-right-radius: 10px;}"
                  "QComboBox:drop-down {background-color: white;"
                                       "color: black;"
                                       "border-bottom-left-radius: 10px;"
                                       "border-bottom-right-radius: 10px;}"
              #ifdef WINDOWS
//                  "QScrollBar:vertical{background: none;"
//                                      "border: none;"
//                                      "padding: none;}"
                  "QScrollBar::handle {background-color: gray;"
                                      "min-height: 70px;}"
                  "QScrollBar::add-line:vertical {border: none;"
                                                 "background: none;}"
                  "QScrollBar::sub-line:vertical {border: none;"
                                                 "background: none;}"
                  "QScrollBar::add-line:horizontal {border: none;"
                                                 "background: none;}"
                  "QScrollBar::sub-line:horizontal {border: none;"
                                                 "background: none;}"
              #endif
                  );

    //hide error labels
//    ui->RegErrorLabel->hide();
//    ui->BeginErrLabel->hide();
//    ui->NewLetterErrLabel->hide();
//    ui->BalanceErrLabel->hide();
    ui->reg2ErrorLabel->clear();

    //layouts
    incLay = new (std::nothrow) QVBoxLayout(this);
    outLay = new (std::nothrow) QVBoxLayout(this);

    incLay->setContentsMargins(0,0,0,0);
    outLay->setContentsMargins(0,0,0,0);

    incLay->setSpacing(0);
    outLay->setSpacing(0);

    incFileLay = new (std::nothrow) QVBoxLayout(this);
    outFileLay = new (std::nothrow) QVBoxLayout(this);

    incFileLay->setContentsMargins(0,0,0,0);
    outFileLay->setContentsMargins(0,0,0,0);

    incFileLay->setSpacing(0);
    outFileLay->setSpacing(0);

//    QString scrollBarStyleSheet = "QScrollBar:vertical{min-width: 30px;"
//                                                      "background: none;"
//                                                      "border: none;"
//                                                      "padding: none;}"
//                                  "QScrollBar::handle {background-color: rgb(0, 162, 230);"
//                                                      "border-radius: 15px;"
//                                                      "min-height: 70px;}"
//                                  "QScrollBar::add-line:vertical {border: none;"
//                                                                 "background: none;}"
//                                  "QScrollBar::sub-line:vertical {border: none;"
//                                                                 "background: none;}";

    ui->IncomingscrollAreaWidgetContents->setLayout(incLay);
    ui->IncomingscrollAreaWidgetContents->setStyleSheet("background-color: lightgray;"/* + scrollBarStyleSheet*/);
    ui->OutcomingscrollAreaWidgetContents->setLayout(outLay);
    ui->OutcomingscrollAreaWidgetContents->setStyleSheet("background-color: lightgray;"/* + scrollBarStyleSheet*/);

    ui->newLetterScrollAreaWidgetContents->setLayout(outFileLay);
    ui->newLetterScrollAreaWidgetContents->setStyleSheet("background-color: lightgray;"/* + scrollBarStyleSheet*/);
    ui->readMessageScrollAreaWidgetContents->setLayout(incFileLay);
    ui->readMessageScrollAreaWidgetContents->setStyleSheet("background-color: lightgray;"/* + scrollBarStyleSheet*/);

    incLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    incFileLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    outFileLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    timer = new (std::nothrow) QTimer(this);
    timer->setInterval(LOADING_SCREEN_INTERVAL);
    connect(timer, SIGNAL(timeout()), this, SLOT(loadingTimeOut()));

    //:/Resources/1024.png
    dark.load(":/Resources/ico-pin-active.png");
    light.load(":/Resources/ico-pin-inactive.png");

    //new letter button icon
//    newMessageButtonIcon = new (std::nothrow) QIcon("");

    //Hide unneeded
    ui->city_lineEdit->hide();
    ui->state_lineEdit->hide();
    ui->change_password_Button->hide();
    ui->button4Page10->hide();

    ui->button0Page3->hide();
    ui->button1Page3->hide();
    ui->button2Page3->hide();
    ui->button3Page3->hide();

    ui->label5Page10->hide();
    ui->label6Page10->hide();
    ui->label7Page10->hide();
    ui->label8Page10->hide();
    ui->label9Page10->hide();
    ui->label10Page10->hide();

//    ui->newLetterAttachButton->hide();
    ui->button2Page6->hide();
    ui->button3Page6->hide();

//    ui->button2Page8->hide();
//    ui->label2Page8->hide();
    ui->button0Page8->hide();

    ui->button4Page3->hide();
    ui->button7Page3->hide();
//    ui->label0Page2->hide();



    //Mikhail






    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidgetPage3->setCurrentIndex(1);
//    statePage3 = 0;//"1"  "2"







    //background
//    QPixmap bkgnd(":/Resources/splash-background.png");
//    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
//    QPalette p = palette(); //copy current, not create new
//    p.setBrush(QPalette::Background, bkgnd);
//    setPalette(p);

    //page0

    //label Veda
    QImage picterFromLabel1;
    //picterFromLabel1.load("D:\\QTProject\\vedaEMailClient\\vedaEMailClient\\Images\\splash-logo-veda.png");
    picterFromLabel1.load(":/Resources/splash-logo-veda.png");
    ui->labe1PageBegin->setPixmap(QPixmap::fromImage(picterFromLabel1,Qt::AutoColor));
    ui->labe1PageBegin->setText("");

    //signIn_Button
//    ui->signIn_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255,95); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,0); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    //ui->signIn_Button->setStyleSheet("background-color: red");
    //ui->signIn_Button->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 196, 229); color:white; border-radius: 10px;"));
    //ui->signIn_Button->setStyleSheet("color:green");

    //register_Button
//    ui->register_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255,95); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,0); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");

    //labe2PageBegin
    ui->labe2PageBegin->setText("OR");
    ui->labe2PageBegin->setStyleSheet(QString::fromUtf8("color:white; "));

    //текст под веда
    ui->labe3PageBegin->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->labe4PageBegin->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->labe5PageBegin->setStyleSheet(QString::fromUtf8("color:white; "));

    //lineEdit
    ui->login_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->password_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->secret_lineEdit->setStyleSheet("border-radius: 5px;");


    //page1

    //veda label
    ui->label0Page1->setPixmap(QPixmap::fromImage(picterFromLabel1,Qt::AutoColor));
    ui->label0Page1->setText("");

    ui->label1Page1->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label1Page1->hide();

    //labels
    ui->label2Page1->setText("");
    ui->label3Page1->setText("");
    ui->label4Page1->setText("");
    ui->label5Page1->setText("");
    ui->label6Page1->setText("");
    ui->label7Page1->setText("");
    //picterFromLabel1.load("D:\\QTProject\\vedaEMailClient\\vedaEMailClient\\Images\\splash-logo-veda.png");

    //ui->progressBarPage1->setStyleSheet( "QProgressBar::chunk {background: blue; margin: 2px; }");


    //page2

    //label Veda
    ui->label1Page2->setPixmap(QPixmap::fromImage(picterFromLabel1,Qt::AutoColor));
    ui->label1Page2->setText("");

    //label Qr
//    QImage picterQr;
//    picterQr.load(":/Resources/ico-qr.png");
    //picterQr = picterQr.scaled(ui->label0Page2->size(), Qt::IgnoreAspectRatio);  //растягивание картинки по лэйблу
//    picterQr = picterQr.scaled(ui->label0Page2->size(), Qt::KeepAspectRatioByExpanding); //сохранение пропорций картинки по большему размеру
//    ui->label0Page2->setPixmap(QPixmap::fromImage(picterQr,Qt::AutoColor));
//    ui->label0Page2->setText("");
    ui->label0Page2->setStyleSheet("color: white;");

    //text labels
    ui->label2Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label3Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label4Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label5Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label6Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label7Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label8Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label9Page2->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label10Page2->setStyleSheet(QString::fromUtf8("color:white; "));

    //buttons
//    ui->back_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255,95); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,0); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->CopyAddressButton->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255,95); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,0); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");


    //page3

    //label Veda
//    ui->label0Page3->setStyleSheet();
//    ui->widget0Page3->setStyleSheet("background-color: rgb(0, 196, 255,0); border-radius: 10px;");
//    ui->label1Page3->setStyleSheet("color: white;");
    //load(":/Resources/splash-logo-veda.png");
    ui->label0Page3->setPixmap(QPixmap::fromImage(QImage(":/Resources/logo-veda-white.png"),Qt::AutoColor));
    ui->label0Page3->setText("");

    //left Buttons
//    ui->new_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
//    ui->incoming_Button->setStyleSheet(" background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;");
//    ui->outcoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
    //ui->incoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");


    //buttons
    ui->balance_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 0px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 0px solid white; padding: 5px; border-radius: 10px;}");

    ui->refresh_Button->setStyleSheet("QPushButton{border-image: url(:/Resources/Refresh2.png) stretch; background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{border-image: url(:/Resources/Refresh2.png) stretch; background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{border-image: url(:/Resources/Refresh2.png) stretch; background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->refresh_Button->setText("");

    ui->settings_Button->setStyleSheet("QPushButton{border-image: url(:/Resources/Sitting2.png) stretch; background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{border-image: url(:/Resources/Sitting2.png) stretch; background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{border-image: url(:/Resources/Sitting2.png) stretch; background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->settings_Button->setText("");
    ui->logOut_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 0px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 0px solid white; padding: 5px; border-radius: 10px;}");

    //labels
    ui->messages_available_word_label->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->message_available_count_label->setStyleSheet(QString::fromUtf8("color:white; "));

    ui->button1Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");
    ui->button2Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");
    ui->button3Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");
    ui->button0Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");

    ui->button4Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");
    ui->button5Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");
    ui->button6Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");
    ui->button7Page3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230, 0); color:black; border: 0px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  lightGray; color:black; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  gray; color:white; border: 1px solid white; padding: 5px; border-radius: 10px;}");

    //stackedWidgetPage3
    ui->stackedWidgetPage3->setStyleSheet("background-color: lightgray; color:white; padding: 5px; border-radius: 5px;");

    //lines

    //ui->line0Page3->setLineWidth(20);
    //ui->line1Page3->setLineWidth(2);

    //text and line edit
    ui->lineEditPage3->setStyleSheet("border-radius: 10px; border: 1px solid black; color:black; background-color: white;");
    ui->newLetterPlainTextEdit->setStyleSheet("QPlainTextEdit {border-radius: 5px; background-color: white; border: 1px solid black; color: black; padding: 10px;}"
                                              "QScrollBar::handle {background-color: gray;}"
                                          #ifdef WINDOWS
                                              "QScrollBar::add-line:vertical {border: none;"
                                                                             "background: none;}"
                                              "QScrollBar::sub-line:vertical {border: none;"
                                                                             "background: none;}"
                                              "QScrollBar::add-line:horizontal {border: none;"
                                                                             "background: none;}"
                                              "QScrollBar::sub-line:horizontal {border: none;"
                                                                             "background: none;}"
                                          #endif
                                              );
    ui->readLetterPlainTextEdit->setStyleSheet("QPlainTextEdit {border-radius: 5px; background-color: white; border: 1px solid black; color: black; padding: 10px;}"
                                               "QScrollBar::handle {background-color: gray;}"
                                           #ifdef WINDOWS
                                               "QScrollBar::add-line:vertical {border: none;"
                                                                              "background: none;}"
                                               "QScrollBar::sub-line:vertical {border: none;"
                                                                              "background: none;}"
                                               "QScrollBar::add-line:horizontal {border: none;"
                                                                              "background: none;}"
                                               "QScrollBar::sub-line:horizontal {border: none;"
                                                                              "background: none;}"
                                           #endif
                                               );
    ui->newLetterPlainTextEdit->document()->setDocumentMargin(30);
    ui->readLetterPlainTextEdit->document()->setDocumentMargin(30);





    //page3 (6)

    //labels
    ui->label0Page6->setStyleSheet(QString::fromUtf8("color:black; "));
    ui->label1Page6->setStyleSheet(QString::fromUtf8("color:black; "));

    //buttons
    ui->button0Page6->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->newLetterAttachButton->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->button2Page6->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->button3Page6->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");

    //text and line edit
    ui->lineEditPage6->setStyleSheet("border-radius: 10px; border: 1px solid black; color:black; background-color: white;");


    //page3 (8)

    //labels
    ui->label0Page8->setStyleSheet(QString::fromUtf8("color:black; "));
    ui->label1Page8->setStyleSheet(QString::fromUtf8("color:black; "));
//    ui->label2Page8->setStyleSheet(QString::fromUtf8("color:black; "));

    //buttons
//    ui->button0Page8->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->button1Page8->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->button2Page8->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
    ui->decryptButton->setStyleSheet("QPushButton{"
                                                 "background-color: rgb(0, 162, 230);"
                                                 "color:white;"
                                                 "border: 1px solid white;"
                                                 "padding: 5px;"
                                                 "border-radius: 10px;"
                                                 "}"
                                     "QPushButton:hover{"
                                                       "background-color:  rgb(0, 220, 255);"
                                                       "color:white;"
                                                       "border: 3px solid white;"
                                                       "padding: 5px;"
                                                       "border-radius: 10px;"
                                                       "}"
                                     "QPushButton:pressed{"
                                                         "background-color:  rgb(0, 196, 255,80);"
                                                         "color:white;"
                                                         "border: 3px solid white;"
                                                         "padding: 5px;"
                                                         "border-radius: 10px;"
                                                        "}"
                                     "QPushButton:disabled{"
                                                          "background-color:  rgb(0, 196, 255,80);"
                                                          "color:gray;"
                                                          "border: 3px solid white;"
                                                          "padding: 5px;"
                                                          "border-radius: 10px;"
                                                         "}");

    //page4
    //vedalabel
    ui->reg2LogoLabel->setPixmap(QPixmap::fromImage(picterFromLabel1,Qt::AutoColor));
    ui->reg2LogoLabel->setText("");

    //labels
    ui->label4Page4->setStyleSheet("color:white; ");
    ui->label6Page4->setStyleSheet("color:white; ");
    ui->reg2MailLabel->setStyleSheet("color:white; ");

    //page9

    //veda label
    ui->label0Page9->setPixmap(QPixmap::fromImage(picterFromLabel1,Qt::AutoColor));
    ui->label0Page9->setText("");

    //buttons
//    ui->sign_up_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255,95); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,0); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->back_Button_3->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255,95); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,0); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");

    //country_comboBox
    ui->country_comboBox->setStyleSheet("border-radius: 5px;");

    //lineEdits
//    ui->mail_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->MailLineEdit1->setStyleSheet("border-top-left-radius: 5px;"
                                     "border-bottom-left-radius: 5px;");
    ui->MailLineEdit3->setStyleSheet("border-top-right-radius: 5px;"
                                     "border-bottom-right-radius: 5px;");

    ui->MailLineEdit2->setStyleSheet("border-radius: none");
    ui->MailDogLineEdit->setStyleSheet("border-radius: none");
    ui->MailDotLineEdit1->setStyleSheet("border-radius: none");

    ui->MailDogLineEdit->setFocusPolicy(Qt::NoFocus);
    ui->MailDotLineEdit1->setFocusPolicy(Qt::NoFocus);

//    ui->lineEdit_2->setStyleSheet("border-radius: 5px;");
    ui->password_lineEdit_3->setStyleSheet("border-radius: 5px;");
    ui->secret_lineEdit_3->setStyleSheet("border-radius: 5px;");
    ui->first_name_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->last_name_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->city_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->state_lineEdit->setStyleSheet("border-radius: 5px;");
    ui->confirmSecretLineEdit->setStyleSheet("border-radius: 5px;");
    ui->confirmPasswordLineEdit->setStyleSheet("border-radius: 5px;");

    //page10

    //veda label
    ui->label0Page10->setPixmap(QPixmap::fromImage(picterFromLabel1,Qt::AutoColor));
    ui->label0Page10->setText("");

    //buttons
//    ui->back_Button_4->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->change_password_Button->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->button0Page10->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->button1Page10->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->button2Page10->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->button3Page10->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");
//    ui->button4Page10->setStyleSheet("QPushButton{background-color: rgb(0, 162, 230); color:white; border: 1px solid rgb(0, 162, 230); padding: 5px; border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 220, 255); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255,80); color:white; border: 3px solid white; padding: 5px; border-radius: 10px;}");

    //button flafs
//    ui->buttonRus->setStyleSheet("border-image: url(:/flagRus.png) stretch; ");
//    ui->buttonRus->setText("");
//    ui->buttonUsa->setStyleSheet("border-image: url(:/flagUsa.png) stretch; ");
//    ui->buttonUsa->setText("");
//    ui->buttonChi->setStyleSheet("border-image: url(:/flagChi.png) stretch; ");
//    ui->buttonChi->setText("");
    ui->buttonRus->hide();
    ui->buttonUsa->hide();
    ui->buttonChi->hide();


    //labels
    ui->label1Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label2Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label3Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label4Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label5Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label6Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label7Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label8Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label9Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label10Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label11Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label12Page10->setStyleSheet(QString::fromUtf8("color:white; "));
    ui->label13Page10->hide();/*setStyleSheet(QString::fromUtf8("color:white; "));*/
    ui->label_3->setStyleSheet("color: white;");

    //lineEdits
    ui->preferredNodeWordLineEdit->setStyleSheet("border-top-left-radius: 5px;"
                                                 "border-bottom-left-radius: 5px;"
//                                                 "background-color: rgb(0, 162, 230);"
                                                 "border-top: 3px solid white;"
                                                 "border-left: 3px solid white;"
                                                 "border-bottom: 3px solid white;"
                                                 );
    ui->preferredNodeLineEdit1->setStyleSheet(/*"border-top-left-radius: 5px;"
                                              "border-bottom-left-radius: 5px;"*/
                                              "border-radius: none;"
//                                              "background-color: rgb(0, 162, 230);"
                                              "border-top: 3px solid white;"
                                              "border-bottom: 3px solid white;"
                                              );
    ui->preferredNodeLineEdit4->setStyleSheet("border-top-right-radius: 5px;"
                                              "border-bottom-right-radius: 5px;"
//                                              "background-color: rgb(0, 162, 230);"
                                              "border-right: 3px solid white;"
                                              "border-top: 3px solid white;"
                                              "border-bottom: 3px solid white;"
                                              );
    ui->preferredNodeLineEdit2->setStyleSheet("border-radius: none;"
//                                              "background-color: rgb(0, 162, 230);"
                                              "border-top: 3px solid white;"
                                              "border-bottom: 3px solid white;"
                                              );
    ui->preferredNodeLineEdit3->setStyleSheet("border-radius: none;"
//                                              "background-color: rgb(0, 162, 230);"
                                              "border-top: 3px solid white;"
                                              "border-bottom: 3px solid white;"
                                              );

    ui->preferredNodeDotLineEdit1->setStyleSheet("border-radius: none;"
//                                                 "background-color: rgb(0, 162, 230);"
                                                 "border-top: 3px solid white;"
                                                 "border-bottom: 3px solid white;"
                                                 );
    ui->preferredNodeDotLineEdit2->setStyleSheet("border-radius: none;"
//                                                 "background-color: rgb(0, 162, 230);"
                                                 "border-top: 3px solid white;"
                                                 "border-bottom: 3px solid white;"
                                                 );
    ui->preferredNodeDotLineEdit3->setStyleSheet("border-radius: none;"
//                                                 "background-color: rgb(0, 162, 230);"
                                                 "border-top: 3px solid white;"
                                                 "border-bottom: 3px solid white;"
                                                 );

    intValidator.setTop(255);
    intValidator.setBottom(0);

    ui->preferredNodeLineEdit1->setValidator(&intValidator);
    ui->preferredNodeLineEdit2->setValidator(&intValidator);
    ui->preferredNodeLineEdit3->setValidator(&intValidator);
    ui->preferredNodeLineEdit4->setValidator(&intValidator);

    ui->preferredNodeWordLineEdit->setFocusPolicy(Qt::NoFocus);
    ui->preferredNodeDotLineEdit1->setFocusPolicy(Qt::NoFocus);
    ui->preferredNodeDotLineEdit2->setFocusPolicy(Qt::NoFocus);
    ui->preferredNodeDotLineEdit3->setFocusPolicy(Qt::NoFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCompleterOnLoginLineEdit(const QStringList &list)
{
    if (nullptr != LoginCompleter){
        delete LoginCompleter;
        LoginCompleter = nullptr;
    }

    LoginCompleter = new (std::nothrow) QCompleter(list, ui->login_lineEdit);
    LoginCompleter->setCompletionMode(QCompleter::PopupCompletion);
    QAbstractItemView *popup = LoginCompleter->popup();
    popup->setStyleSheet("color: black;");
    ui->login_lineEdit->setCompleter(LoginCompleter);
}

void MainWindow::setCompleterOnRecipientLineEdit(const QStringList &list)
{
    if (nullptr != RecipientCompleter){
        delete RecipientCompleter;
        RecipientCompleter = nullptr;
    }

    RecipientCompleter = new (std::nothrow) QCompleter(list, ui->lineEditPage6);
    RecipientCompleter->setCompletionMode(QCompleter::PopupCompletion);
    QAbstractItemView *popup = RecipientCompleter->popup();
    popup->setStyleSheet("color: black;");
    ui->lineEditPage6->setCompleter(RecipientCompleter);
}

void MainWindow::ShowAllCompletionsOnLoginLineEdit()
{
    if (nullptr != LoginCompleter){
        LoginCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        LoginCompleter->complete();
        LoginCompleter->setCompletionMode(QCompleter::PopupCompletion);
    }
}

void MainWindow::ShowAllCompletionsOnRecipientLineEdit()
{
    if (nullptr != RecipientCompleter){
        RecipientCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        RecipientCompleter->complete();
        RecipientCompleter->setCompletionMode(QCompleter::PopupCompletion);
    }
}

void MainWindow::CompleteOnRecipientLineEdit()
{
    if (nullptr != RecipientCompleter){
        ui->lineEditPage6->setFocus();
        RecipientCompleter->setCompletionPrefix(ui->lineEditPage6->text());
        RecipientCompleter->complete();
    }
}

void MainWindow::addIncomingListItem(QWidget *item)
{
    incLay->insertWidget(incLay->count() - 1, item);
}

void MainWindow::removeIncomingListItem(QWidget *item)
{
    incLay->removeWidget(item);
}

void MainWindow::addOutcomingListItem(QWidget *item)
{
    outLay->insertWidget(outLay->count() - 1, item);
}

void MainWindow::removeOutomingListItem(QWidget *item)
{
    outLay->removeWidget(item);
}

void MainWindow::addIncomingFile(QWidget *item)
{
    incFileLay->insertWidget(incFileLay->count() - 1, item);
}

void MainWindow::removeIncomingFile(QWidget *item)
{
    incFileLay->removeWidget(item);
}

void MainWindow::addOutcomingFile(QWidget *item)
{
    outFileLay->insertWidget(outFileLay->count() - 1, item);
}

void MainWindow::removeOutcomingFile(QWidget *item)
{
    outFileLay->removeWidget(item);
}

void MainWindow::toggleLoadingScreen(int index, bool on)
{
//    qDebug()<<ui->stackedWidget->currentIndex();
    /*if (isLoading && on){
        return;
    }
    else*/ if (on){
//        isLoading = true;
//        pageIndexBeforeLoadingScreenStart = ui->stackedWidget->currentIndex();
        ui->label2Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->stackedWidget->setCurrentIndex(1);
        valueTimer = 0;
        timer->start();
    }
    else {
//        ui->stackedWidget->setCurrentIndex(pageIndexBeforeLoadingScreenStart);
//        isLoading = false;
        ui->stackedWidget->setCurrentIndex(index);
        timer->stop();
    }

    //    qDebug()<<ui->stackedWidget->currentIndex();
}

void MainWindow::setNewLetterAttachButtonDefault()
{
    ui->newLetterAttachButton->setText(NEW_LETTER_ATTACH_BUTTON_DEFAULT_NAME);
    //32
    QFont font = ui->newLetterAttachButton->font();
    font.setPixelSize(32);
    ui->newLetterAttachButton->setFont(font);
}

void MainWindow::setNewLetterAttachButtonText(const QString &text)
{
    ui->newLetterAttachButton->setText(text);
    //14
    QFont font = ui->newLetterAttachButton->font();
    font.setPixelSize(14);
    font.setBold(true);
    ui->newLetterAttachButton->setFont(font);
}

void MainWindow::hideReadMessageSaveFilesSection()
{
//    ui->label2Page8->hide();
    ui->button0Page8->hide();
}

void MainWindow::showReadMessageSaveFilesSection(const QStringList &fileNames)
{
//    if (fileNames.isEmpty()) return;

    ///\todo for

//    ui->label2Page8->show();
    ui->button0Page8->show();
    ui->button0Page8->setText(fileNames[0]);
}

//void MainWindow::on_signIn_Button_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(3);
////    ui->stackedWidgetPage3->setCurrentIndex(1);

//    //lodingPage();

//}

//void MainWindow::lodingPage()
//{
//    timer = new QTimer();
//    timer->setSingleShot(true);
//    timer->start(250);
//    connect(timer, SIGNAL(timeout()), this, SLOT(lodingSlot()));
//}

//void MainWindow::lodingSlot()
//{
//    //ui->progressBarPage1->setRange(0,100); //пределы прогресса
//    //ui->progressBarPage1->setValue(0); //значение прогресса


//    QImage imageBlack;
//    QImage imageWhite;            ;
//    //picterFromLabel1.load("D:\\QTProject\\vedaEMailClient\\vedaEMailClient\\Images\\splash-logo-veda.png");
//    imageBlack.load(":/Resources/ico-pin-active.png");
//    imageWhite.load(":/Resources/ico-pin-inactive.png");

//    if(timerLoading)
//    {
//        valueTimer += 1;
//        timer = new QTimer();
//        timer->setSingleShot(true);
//        timer->start(250);
//        //ui->progressBarPage1->setValue(valueTimer/30); //значение прогресса

//        if(valueTimer % 8 == 1)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 2)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 3)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 4)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 5)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 6)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 7)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//        }
//        if(valueTimer % 8 == 0)
//        {
//            ui->label2Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label3Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label4Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label5Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label6Page1->setPixmap(QPixmap::fromImage(imageWhite,Qt::AutoColor));
//            ui->label7Page1->setPixmap(QPixmap::fromImage(imageBlack,Qt::AutoColor));
//        }




//        connect(timer, SIGNAL(timeout()), this, SLOT(lodingSlot()));
//    }
//    else
//    {
//        //valueTimer = 0;
//        ui->stackedWidget->setCurrentIndex(0);
//    }
//}

void MainWindow::resizeEvent(QResizeEvent *evt)  //изменяет фонофый рисунок под размер
{
    QPixmap bkgnd(":/Resources/splash-background-with-watermark.png");
    bkgnd = bkgnd.scaled(size(), Qt::IgnoreAspectRatio);
    QPalette p = palette(); //copy current, not create new
    p.setBrush(QPalette::Background, bkgnd);
    setPalette(p);

    QWidget::resizeEvent(evt); //call base implementation
}

//void MainWindow::on_new_Button_clicked()
//{
////    ui->stackedWidgetPage3->setCurrentIndex(0);
////    statePage3 = 0;
//    //style buttons
//    //ui->new_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
//    //ui->incoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
//    //ui->outcoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");


////    ui->new_Button->setStyleSheet(" background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;");
////    ui->incoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
////    ui->outcoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");

//}

//void MainWindow::on_incoming_Button_clicked()
//{
////    ui->stackedWidgetPage3->setCurrentIndex(1);
////    statePage3 = 1;

////    ui->new_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
////    ui->incoming_Button->setStyleSheet(" background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;");
////    ui->outcoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");

//}

//void MainWindow::on_outcoming_Button_clicked()
//{
////    ui->stackedWidgetPage3->setCurrentIndex(2);
////    statePage3 = 2;

////    ui->new_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
////    ui->incoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
////    ui->outcoming_Button->setStyleSheet(" background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;");

//}

//void MainWindow::on_back_Button_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(3);
////    ui->stackedWidgetPage3->setCurrentIndex(1);
////    statePage3 = 1;

////    ui->new_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
////    ui->incoming_Button->setStyleSheet(" background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;");
////    ui->outcoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");

//}

//void MainWindow::on_logOut_Button_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(0);
//}

//void MainWindow::on_register_Button_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(9);
//}

//void MainWindow::on_back_Button_3_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(0);
//}

//void MainWindow::on_back_Button_4_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(3);
////    ui->stackedWidgetPage3->setCurrentIndex(1);
////    statePage3 = 1;

////    ui->new_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");
////    ui->incoming_Button->setStyleSheet(" background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;");
////    ui->outcoming_Button->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255,0); color:white; padding: 5px;  border: 1px solid white;  border-radius: 10px;} QPushButton:hover{background-color:  rgb(0, 196, 255, 80); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;} QPushButton:pressed{background-color:  rgb(0, 196, 255); color:white; border: 3px solid white; padding: 5px;  border: 3px solid white;  border-radius: 10px;}");

//}

//void MainWindow::on_settings_Button_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(10);
//}

//void MainWindow::on_balance_Button_clicked()
//{
////    ui->stackedWidget->setCurrentIndex(2);
//}

//void MainWindow::on_button8Page3_clicked()
//{
////    ui->stackedWidgetPage3->setCurrentIndex(3);
////    statePage3 = 3;
//}

//void MainWindow::on_button9Page3_clicked()
//{
////    ui->stackedWidgetPage3->setCurrentIndex(3);
//    //    statePage3 = 3;
//}

void MainWindow::loadingTimeOut()
{
    if (1 == (++valueTimer % 8)){
        ui->label2Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
    }
    else if (2 == (valueTimer % 8)) {
        ui->label2Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
    }
    else if (3 == (valueTimer % 8)) {
        ui->label2Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
    }
    else if (4 == (valueTimer % 8)) {
        ui->label2Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
    }
    else if (5 == (valueTimer % 8)) {
        ui->label2Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
    }
    else if (6 == (valueTimer % 8)) {
        ui->label2Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
    }
    else if (7 == (valueTimer % 8)) {
        ui->label2Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
    }
    else {  //if (0 == (valueTimer % 8))
        ui->label2Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label3Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label4Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label5Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label6Page1->setPixmap(QPixmap::fromImage(light,Qt::AutoColor));
        ui->label7Page1->setPixmap(QPixmap::fromImage(dark,Qt::AutoColor));
    }
}

void MainWindow::on_CopyAddressButton_clicked()
{
    cb->setText(ui->label10Page2->text());
}

//void MainWindow::on_button0Page10_clicked()
//{
////    qDebug()<<"i'm clicked";
//}
