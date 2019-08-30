#include "fileattachment.h"
#include "ui_fileattachment.h"

fileAttachment::fileAttachment(bool incoming, QString fileName, QString fileNameWithPath, uint64_t fileSize, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileAttachment),
    _incoming(incoming),
    _fileName(fileName),
    _fileNameWithPath(fileNameWithPath),
    _size(fileSize)
{
    ui->setupUi(this);

    ui->DelSaveButton->setFocusPolicy(Qt::NoFocus);
    ui->fileNameButton->setFocusPolicy(Qt::NoFocus);
    ui->rightButton->setFocusPolicy(Qt::NoFocus);

    delSaveButtonSaveStyleSheet = "QPushButton {"
                                     "color: black;"
                                     "border: none;"
                                     "border-radius: none;"
                                     "background-image: url(:/Resources/ico-save.png);"// no-repeat;"// stretch;"
                                     "background-repeat: no-repeat;"
          //                            "padding: none;"
          //                            "margin: none;"
                                 "}"
                                 "QPushButton:hover {"
                                     "color: black;"
                                     "border: none;"
                                     "border-radius: none;"
                                     "background-image: url(:/Resources/ico-save.png)"// no-repeat;"// stretch;"
                                     "background-repeat: no-repeat;"
          //                            "padding: none;"
          //                            "margin: none;"
                                 "}"
                                 "QPushButton:pressed {"
                                     "color: black;"
                                     "border: none;"
                                     "border-radius: none;"
                                     "background-image: url(:/Resources/ico-save.png)"// no-repeat;"// stretch;
                                     "background-repeat: no-repeat;"
          //                            "padding: none;"
          //                            "margin: none;"
                                 "}"
                                 "QPushButton:checked {"
                                     "color: black;"
                                     "border: none;"
                                     "border-radius: none;"
                                     "background-image: url(:/Resources/ico-save.png)"// no-repeat;"// stretch;"
                                     "background-repeat: no-repeat;"
          //                            "padding: none;"
          //                            "margin: none;"
                                 "}";

    delSaveButtonDelStyleSheet = "QPushButton {"
                                  "color: black;"
                                  "border: none;"
                                  "border-radius: none;"
       //                            "padding: none;"
       //                            "margin: none;"
                              "}"
                              "QPushButton:hover {"
                                  "color: black;"
                                  "border: none;"
                                  "border-radius: none;"
       //                            "padding: none;"
       //                            "margin: none;"
                              "}"
                              "QPushButton:pressed {"
                                  "color: black;"
                                  "border: none;"
                                  "border-radius: none;"
       //                            "padding: none;"
       //                            "margin: none;"
                              "}"
                              "QPushButton:checked {"
                                  "color: black;"
                                  "border: none;"
                                  "border-radius: none;"
       //                            "padding: none;"
       //                            "margin: none;"
                              "}";

    if (incoming){
//        ui->DelSaveButton->setText(SAVE_SYMBOL);
        ui->DelSaveButton->setText("");
        ui->DelSaveButton->setStyleSheet(delSaveButtonSaveStyleSheet);
//        connect(ui->fileNameButton, SIGNAL(clicked()), this, SLOT(clickEmitter()));
//        ui->fileNameButton->setCursor(Qt::PointingHandCursor);
//        connect(ui->rightButton, SIGNAL(clicked()), this, SLOT(clickEmitter()));
//        ui->rightButton->setCursor(Qt::PointingHandCursor);
    }
    else {
        ui->DelSaveButton->setText(DELETE_SYMBOL);
        ui->DelSaveButton->setStyleSheet(delSaveButtonDelStyleSheet);
    }

    if (MAX_FILE_NAME_LEN >= fileName){
        ui->fileNameButton->setText(fileName);
    }
    else {
        //пока не парсится полный случай приизнаков расширений
        QString cut;
        cut = fileName;
        cut.chop((fileName.size() - MAX_FILE_NAME_LEN) + 2);
        cut.append("...");
        ui->fileNameButton->setText(cut);
    }

    connect(ui->DelSaveButton, SIGNAL(clicked()), this, SLOT(clickEmitter()));
    ui->DelSaveButton->setCursor(Qt::PointingHandCursor);

    setStyleSheet("background-color: lightgray; color: black; ");
    ui->line->setStyleSheet("background-color: lightgray;");

//    ui->DelSaveButton->setStyleSheet(delSaveButtonDelStyleSheet);
    ui->fileNameButton->setStyleSheet("QPushButton {"
                                      "color: black;"
                                      "border: none;"
                                      "border-radius: none;"
                                      "text-align: left;"
           //                            "padding: none;"
           //                            "margin: none;"
                                  "}"
                                  "QPushButton:hover {"
                                      "color: black;"
                                      "border: none;"
                                      "border-radius: none;"
           //                            "padding: none;"
           //                            "margin: none;"
                                  "}"
                                  "QPushButton:pressed {"
                                      "color: black;"
                                      "border: none;"
                                      "border-radius: none;"
           //                            "padding: none;"
           //                            "margin: none;"
                                  "}"
                                  "QPushButton:checked {"
                                      "color: black;"
                                      "border: none;"
                                      "border-radius: none;"
           //                            "padding: none;"
           //                            "margin: none;"
                                  "}");
    ui->rightButton->setStyleSheet("QPushButton {"
                                   "color: black;"
                                   "border: none;"
                                   "border-radius: none;"
        //                            "padding: none;"
        //                            "margin: none;"
                               "}"
                               "QPushButton:hover {"
                                   "color: black;"
                                   "border: none;"
                                   "border-radius: none;"
        //                            "padding: none;"
        //                            "margin: none;"
                               "}"
                               "QPushButton:pressed {"
                                   "color: black;"
                                   "border: none;"
                                   "border-radius: none;"
        //                            "padding: none;"
        //                            "margin: none;"
                               "}"
                               "QPushButton:checked {"
                                   "color: black;"
                                   "border: none;"
                                   "border-radius: none;"
        //                            "padding: none;"
        //                            "margin: none;"
                               "}");
}

fileAttachment::~fileAttachment()
{
    delete ui;
}

bool fileAttachment::isIncoming()
{
    return _incoming;
}

QString fileAttachment::getFileName()
{
    return _fileName;
}

QString fileAttachment::getFileNameWithPath()
{
    return _fileNameWithPath;
}

uint64_t fileAttachment::getFileSize()
{
    return _size;
}

void fileAttachment::clickEmitter()
{
    emit clicked(_incoming, _fileName/*, _fileNameWithPath*/);
}
