#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCompleter>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QClipboard>
#include <QAbstractItemView>
#include <QPlainTextEdit>
#include <QIntValidator>

#define LOADING_SCREEN_INTERVAL 250
#define NEW_LETTER_ATTACH_BUTTON_DEFAULT_NAME "📎"
#define READ_MESSAGE_SAVE_BUTTON_TEXT_PREFIX "🖫 "

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;

    void setCompleterOnLoginLineEdit(const QStringList &list);
    void setCompleterOnRecipientLineEdit(const QStringList &list);

    void ShowAllCompletionsOnLoginLineEdit();
    void ShowAllCompletionsOnRecipientLineEdit();
    void CompleteOnRecipientLineEdit();

    void addIncomingListItem(QWidget *item);
    void removeIncomingListItem(QWidget *item);

    void addOutcomingListItem(QWidget *item);
    void removeOutomingListItem(QWidget *item);

    void addIncomingFile(QWidget *item);
    void removeIncomingFile(QWidget *item);

    void addOutcomingFile(QWidget *item);
    void removeOutcomingFile(QWidget *item);

    void toggleLoadingScreen(int index, bool on);

    void setNewLetterAttachButtonDefault();
    void setNewLetterAttachButtonText(const QString &text);

    void hideReadMessageSaveFilesSection();
    void showReadMessageSaveFilesSection(const QStringList &fileNames);

private slots:
//    void on_signIn_Button_clicked();
//    void lodingSlot();

//    void on_new_Button_clicked();

//    void on_incoming_Button_clicked();

//    void on_outcoming_Button_clicked();

//    void on_back_Button_clicked();

//    void on_logOut_Button_clicked();

//    void on_register_Button_clicked();

//    void on_back_Button_3_clicked();

//    void on_back_Button_4_clicked();

//    void on_settings_Button_clicked();

//    void on_balance_Button_clicked();

//    void on_button8Page3_clicked();

//    void on_button9Page3_clicked();

    void loadingTimeOut();

    void on_CopyAddressButton_clicked();

//    void on_button0Page10_clicked();

private:
    void resizeEvent(QResizeEvent *evt);


     QTimer *timer;
     int valueTimer;
     int pageIndexBeforeLoadingScreenStart;
//     void lodingPage();
//     int statePage3;
     QCompleter *LoginCompleter = nullptr,
                *RecipientCompleter = nullptr;
     QVBoxLayout *incLay,
                 *outLay;
     QVBoxLayout *incFileLay,
                 *outFileLay;
     QImage dark, light;
     bool isLoading = false;
     QClipboard *cb;

     QIcon *newMessageButtonIcon;
     QIntValidator intValidator;
};

#endif // MAINWINDOW_H
