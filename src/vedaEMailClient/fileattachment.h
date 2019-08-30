#ifndef FILEATTACHMENT_H
#define FILEATTACHMENT_H

#include <QWidget>
#define SAVE_SYMBOL "🖫"
#define DELETE_SYMBOL "X"
#define MAX_FILE_NAME_LEN 64

namespace Ui {
class fileAttachment;
}

class fileAttachment : public QWidget
{
    Q_OBJECT

public:
    explicit fileAttachment(bool incoming, QString fileName, QString fileNameWithPath, uint64_t fileSize, QWidget *parent = nullptr);
    ~fileAttachment();
    bool isIncoming();
    QString getFileName();
    QString getFileNameWithPath();
    uint64_t getFileSize();

private:
    Ui::fileAttachment *ui;
    bool _incoming;
    QString _fileName;
    QString _fileNameWithPath;
    QString delSaveButtonDelStyleSheet;
    QString delSaveButtonSaveStyleSheet;
    uint64_t _size;

private slots:
    void clickEmitter();

signals:
    void clicked(bool incoming, const QString &fileName/*, const QString &fileNameWithPath*/);
};

#endif // FILEATTACHMENT_H
