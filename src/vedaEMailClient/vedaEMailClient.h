#ifndef VEDAEMAILCLIENT_H
#define VEDAEMAILCLIENT_H

//-----------------------------------------------------------------------------

#include <QWidget>
#include <QGridLayout>
#include <QThread>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QRegExp>
#include <QStandardPaths>
#include <QFileDialog>
//#include <QFontMetrics>
//#include <QHash>
#ifdef WINDOWS
#include <QTextCodec>
#endif

//-----------------------------------------------------------------------------


#include "stdlib.h"

//-----------------------------------------------------------------------------

#include "libvedawallet.h"
#include "json.hpp"
#include "vedainiwrapper.hpp"
#include "token.pb.h"

//-----------------------------------------------------------------------------

#include "QrCode.hpp"
#include "QrSegment.hpp"
#include "BitBuffer.hpp"

//-----------------------------------------------------------------------------

#ifdef WINDOWS
#include "winsock2.h"
#endif

//-----------------------------------------------------------------------------

#include "begin_f.h"
#include "ui_begin_f.h"

#include "registration.h"
#include "ui_registration.h"

#include "main_menu.h"
#include "ui_main_menu.h"

#include "message_list.h"
#include "ui_message_list.h"

#include "message_list_item_button.h"

#include "new_letter.h"
#include "ui_new_letter.h"

#include "read_letter.h"
#include "ui_read_letter.h"

#include "settings.h"
#include "ui_settings.h"

#include "settings_information.h"
#include "ui_settings_information.h"

#include "settings_account.h"
#include "ui_settings_account.h"

#include "settings_connection.h"
#include "ui_settings_connection.h"

#include "settings_general.h"
#include "ui_settings_general.h"

#include "settings_security.h"
#include "ui_settings_security.h"

#include "balance_f.h"
#include "ui_balance_f.h"

#include "loading_screen.h"

#include "outcoming_list_item_label.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "fileattachment.h"
#include "ui_fileattachment.h"

#define PATH_TO_BIN "bin"
#define PATH_TO_ETC "../etc"
#define PATH_TO_WALLETS_DIR "../etc/wallets"
#define PATH_TO_INI_FILE "../etc/vedaemail.ini"
#define PATH_TO_CURRENT_VERSION_FOLDER "2706191100"
#define PATH_TO_VERSIONS_FOLDER "Versions"

#define INI_FILE_CURRENT_NODE_SECTION "CURRENT_NODE"
#define INI_FILE_WALLETS_SECTION "WALLETS"
#define INI_FILE_ADDRESS_BOOK_SECTION "ADDRESS_BOOK"
#define INI_FILE_UNKNOWN_MAIL "unknown"
#define INI_FILE_CURRENT_NODE_LOCAL_IP "127.0.0.1"
#define INI_FILE_CURRENT_NODE_GERMAN_IP "116.203.80.155" //"159.69.190.3"
#define INI_FILE_AUTO_LOG_OUT_SECTION "AUTO_LOG_OUT"
#define INI_FILE_AUTO_LOG_OUT_NAME_FIELD "status"

#define READ_MESSAGE_LIST_SECTION "READ"
#define READ_MESSAGE_LIST_VALUE "1"
//#define INI_FILE_START_MONEY_SECTION "START_MONEY"
//#define INI_FILE_START_MONEY_TRUE "true"
//#define INI_FILE_START_MONEY_FALSE "false"


#define DEFAULT_WID_SIZE 64
#define DEFAULT_WID_SIZE_WITH_P 66
#define ERROR_INTERVAL 10000
#define LETTERS_PER_PAGE 10
#define EMPTY_TOKEN_COLOR_NAME " "
#define EMPTY_TOKEN_COLOR_NAME_2 "blank"
#define AUTO_RENEW_TIMER_PERIOD 3 * 60  * 1000
#define VDN_PER_MSG "1"
#define MARKED_TOKENS_PER_COMISSION_TOKEN 10
#define MESSAGES_AVAILABLE_MINIMUM_COUNT_TO_ALLOW_GET_MORE 10
#define MAX_GET_MORE_MSGS_BTN_PRESSES 2
#define PASS_MIN_LEN 16
#define SECRET_MIN_LEN 8
#define INCOMING_BUTTON_DEFAULT_TEXT "Incoming"
#define MAX_FILE_SIZE 52428800
#define REG_2_CODE_SIZE 6

#define SET_PREFERRED_NODE_BUTTON_EDIT_TEXT "Change"
#define SET_PREFERRED_NODE_BUTTON_APPLY_TEXT "Apply"

#define JSON_CERT_FIO       "FIO"
#define JSON_CERT_CITY      "CITY"
#define JSON_CERT_STATE     "STATE"
#define JSON_CERT_COUNTRY   "COUNTRY"
#define JSON_CERT_EMAIL     "EMAIL"
#define JSON_CERT_ORG       "O"
#define JSON_CERT_ORG_UNIT  "OU"

#define JSON_BALANCE_MASS "MASS"
#define JSON_BALANCE_STATUS "STATUS"
#define JSON_BALANCE_TYPE "TYPE"

#ifndef VEDA_PROTO_BUF
#define JSON_TOKEN_GENESIS                  "GENESIS"
#define JSON_TOKEN_GENESIS_VID              "VID"
#define JSON_TOKEN_GENESIS_TIMES            "TIMES"
#define JSON_TOKEN_GENESIS_TIMET            "TIMET"
#define JSON_TOKEN_GENESIS_AUTHOR_ID        "AUTHOR_ID"
#define JSON_TOKEN_GENESIS_NOMINAL          "NOMINAL"
#define JSON_TOKEN_GENESIS_COLOR_NAME          "COLOR_NAME"
#define JSON_TOKEN_GENESIS_COLOR_HASH          "COLOR_HASH"
#define JSON_TOKEN_GENESIS_COLOR_CNT           "COLOR_CNT"
#define JSON_TOKEN_AUTHOR_SIGN              "AUTHOR_SIGN"
#define JSON_TOKEN_USER                     "USER"
#define JSON_TOKEN_USER_TID                 "TID"
#define JSON_TOKEN_USER_TID_HASH            "TID_HASH"
#define JSON_TOKEN_USER_OWNER_ID            "OWNER_ID"
#define JSON_TOKEN_OWNER_SIGN               "OWNER_SIGN"
#else
#define JSON_FILE_NAME "NAME"
#endif

#define JSON_TRANSACTION_BODY                           "BODY"
#define JSON_TRANSACTION_BODY_TID                       "TID"
#define JSON_TRANSACTION_BODY_NUM                       "NUM"
#define JSON_TRANSACTION_BODY_TIME                      "TIME"
#define JSON_TRANSACTION_BODY_SENDER                    "SENDER"
#define JSON_TRANSACTION_BODY_RECIPIENT                 "RECIPIENT"
#define JSON_TRANSACTION_BODY_VID                       "VID"          // transfer and NEW for EXCHAHGE
#define JSON_TRANSACTION_BODY_VID_CMS                   "VID_CMS"
#define JSON_TRANSACTION_BODY_VID_OLD                   "VID_OLD"  // for EXCHAHGE
#define JSON_TRANSACTION_BODY_TYPE                      "TYPE"
#define JSON_TRANSACTION_BODY_CID                       "CID"
#define JSON_TRANSACTION_BODY_TID_REF                   "TID_REF"
#define JSON_TRANSACTION_BODY_DATA                      "DATA"
#define JSON_TRANSACTION_SENDER_SIGN                    "SENDER_SIGN"
#define JSON_TRANSACTION_RECIPIENT_SIGN                 "RECIPIENT_SIGN"
#define JSON_TRANSACTION_NODE_ID                        "NODE_ID"
#define JSON_TRANSACTION_NODE_SIGN                      "NODE_SIGN"
#define JSON_TRANSACTION_EXTENSION                      "EXTENSION"
#define JSON_TRANSACTION_EXTENSION_CONTRACT             "CONTRACT"
#define JSON_TRANSACTION_EXTENSION_CONTRACT_METODS      "METODS"
#define JSON_TRANSACTION_EXTENSION_CONTRACT_BODY        "BODY"
#define JSON_TRANSACTION_ROUND                          "ROUND"
#define JSON_TRANSACTION_INDEX                          "INDEX"
#define JSON_TRANSACTION_TRNCNTROUND                    "TRN_COUNT_IN_ROUND"

#define TRN_TRANSFER            2
#define TRN_EXCHANGE            3
#define TRN_TRANSFER_MY         4
#define TRN_CREATE_MARKED_TKN   5

#define JSON_TRN_TOKEN_IN_NOMINAL       "NOMINAL"
#define JSON_TRN_TOKEN_IN_TID_TRN       "TID"
#define JSON_TRN_TOKEN_IN_HASH_TRN      "HASH"

#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a > b ? b : a

namespace Ui {
class InactivityFilter;
class doubleClickFilter;
class vedaEMailClient;
class server_functions;
}

using json = nlohmann::json;






enum GetMailByWIDType{
    Incoming,
    Outcoming,
    IniFileWalletsSection
};







class InactivityFilter: public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
signals:
    void activity();
};







class doubleClickFilter: public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void doubleClick();
};









class server_functions : public QObject
{
    Q_OBJECT
public:
    explicit server_functions(QObject *parent = nullptr);
    ~server_functions();

public slots:
    void reg_1(uint64_t engine,
               char *dir,
               char *ip,
               char *pass, int pass_len,
               char *secret, int secret_len,
               char *cert, int cert_len,
               char *mailToCheck);

    void reg_2(char *email_code,
               int code_len);

    void auth(uint64_t engine,
              char *dir,
              char *ip,
              char *cryptobox_name, int cryptobox_name_len,
              char *pass, int pass_len,
              char *secret, int secret_len);
    void get_tokens(uint64_t wstk, bool with_trn_history_anyway = false);
    void send_tokens(uint64_t wstk,
                     int type,
                     char *wid_recipient,
                     char *token_mas, uint64_t token_mas_len,
                     char *token_mas_comission, uint64_t token_mas_comission_len,
                     char *data = 0, uint64_t data_len = 0);
    void get_email(uint64_t wstk, char *wid, const int index, GetMailByWIDType type);
    void get_wid_list_by_email(uint64_t wstk, char *email);
    void create_empty_messages(uint64_t wstk,
                               char *color_text,
                               int tokens_to_create_count,
                               char *WIDR,
                               char *com_mass,
                               int com_mas_len);
    void nulify_wstk();

signals:
    void reg_1_res(int error,
                   bool success,
                   uint64_t wstk);

    void reg_2_res(int error);

    void auth_res(uint64_t wstk,
                  int error,
                  char *token_mas,
                  uint64_t token_mas_len,
                  char *balance,
                  uint64_t token_cnt,
                  uint64_t marked_token_cnt,
                  char *trn_list, int trn_list_size);
    void get_tokens_res(int res, char *trns = NULL);
    void send_tokens_res(int res);
    void get_email_res(QString email, const int index, GetMailByWIDType type, bool from_server = true);
    void get_wid_list_by_email_res(json WIDs, int res = LIBVEDAWALLET_NOERROR);
    void create_empty_messages_res(int res);

private:
    uint64_t wstk;
};











class vedaEMailClient : public QWidget
{
    Q_OBJECT

public:
    explicit vedaEMailClient(QWidget *parent = nullptr);
    ~vedaEMailClient();

private:
    //Enums--------------------------------------------------------------------
    enum renewFrontendFrom{
        fromRefresh,
        fromAuth
    };
//    enum MessageList_Categiroes{
//        Incoming,
//        Outcoming
//    };








    //Structs------------------------------------------------------------------
    typedef struct _TOKEN_DATA{
        std::string vid;
        json j_data;
    }TOKEN_DATA;






    //Fields-------------------------------------------------------------------
    Ui::vedaEMailClient *ui;
//    begin_f *begin_form;
//    registration *registration_form;
//    main_menu *main_menu_form;
//    message_list *incoming_message_list_form,
//                 *outcoming_message_list_form;
//    new_letter *new_letter_form;
//    read_letter *read_letter_form;
//    settings *settings_form;
//    settings_information *settings_information_form;
//    settings_account *settings_account_form;
//    settings_connection *settings_connection_form;
//    settings_general *settings_general_form;
//    settings_security *settings_security_form;
//    balance_f *balance_form;
//    loading_screen *loading_screen_form;
    MainWindow *mainWindow;




    QVector<QWidget*> main_menu_forms,
                      settings_forms;
    doubleClickFilter dblClickFilter;



    uint64_t engine;
    server_functions *server;
    QThread *server_thread;
    QTimer *error_timer,
           *autorenew_timer,
           *autoLogOutTimer;
//    QLabel *_error_label;

//    QStringList Wallets;
    QString current_wid;
    QString current_mail_address;
    int current_opened_wallet;
    QString recipient_wid;

    uint64_t wstk = 0;
    json j_tokens;
    json j_trns;
    uint64_t balance = 0;
    uint64_t letters_count;
    uint64_t tokens_count;
    QVector<uint64_t> incoming_letters_indexes,
                      clear_letters_indexes,
                      tokens_indexes;

    int incoming_pages_num;
//    int FirstPageLetterCount;
    int LastPageLetterCount;
//    int incomingLettersCount;

    int current_first_incoming_letter;
    int lettersToShow;
    int currentIncomingPage = -1;

    int messagesAvailable;

    int unReadIncomingMessages/* = 0*/;
    bool forceUpdateIncomingFirstPage;

    QVector<message_list_item*> incoming_message_list;
    QVector<message_list_item*> outcoming_message_list;

    QVector<fileAttachment*> incomingFileButtons,
                             outcomingFileButtons;

//    QHash<QString, QString> fileAttachments;

    QMap<QString, QString> TIDsNTimes;
    QMap<QString, QString> TIDsNRawTimes;
//    QMap<QString, QString> RawTimesNTIDs;
    bool ignore_get_tokens_on_refresh = false;
    bool readIniFileFirstTime = true;
    int getMoreTokensButtonClickCount = 0;

    int ini_file_wallets_section_replaces_count;
    int ini_file_address_book_section_replaces_count_by_incoming;
    int ini_file_address_book_section_replaces_count_by_outcoming;

    bool ini_file_address_book_section_replaces_by_incoming_was_from_server;
    bool ini_file_address_book_section_replaces_by_outcoming_was_from_server;

    bool allIncomingQueryiesEnded;
    bool allOutcomingQueryiesEnded;

    int CurrentOpenedIncomingMessage;

    int autoLogOutTimerMinuteRange = 0;

    int currentMainWindowIndex;

    bool loadingScreenIsOn;

    QVector<std::string> WIDsByMail;
    QStringList recipientLineEditCompletions;

//    bool NewMessageFileAttached = false;
    uint64_t overallAttachmentsSize;

    std::string currentReadMessageVID;

    int readMessageFilesAttachedCount;
    QStringList readMessageFilesAttachedNames;

    QString regMail;

    bool preferredNodeEditable = false;

    bool loggedIn = false;

    QVector<QLabel*> errorLabels;

//    bool attachmentsGood;




    //Ini file-----------------------------------------------------------------
    VedaIniWrapper *iniWrapper = nullptr;
//    std::vector<VedaIniEntry> mail_wallet_pairs;
    QStringList walletsRecievedStartMoney;
    QString current_node;
    QDir dir;
    QString executable_path;

    VedaIniWrapper *currentReadMessageStorage = nullptr;
    QVector<std::string> currentReadVIDs;





    //Methods------------------------------------------------------------------
    void connect_widgets();
    void toggle_loading_screen(int index, bool on);
    void updateUnknownWalletsEverywhere();
    void init_server_thread();
    void connect_server_thread();
    void read_ini_file();
    void verifyIniFile();
    void sortIniFile();
    void removeIniDuplicates();
    void read_wallets();
    void save_wallets(QString mail_address, QString wallet);
    void read_address_book(bool withEMailsVerification = false);
    void addWIDListToAddressBook(bool withVerification = true);
    void read_current_node();
    void readAutoLogOutSettings();
    void saveAutoLogOutSettings();
    void fillBeginFormCompleter();
    void fillDescriptionLabelMail();
    QString find_new_wid();
    void show_error_label(QLabel *error_label, QString error_text, bool error = true);
    char *QString_to_char_array(QString str, int *len = nullptr);
    char *std_string_to_char_array(std::string str, int *len = nullptr);
    char *jsonToCharArray(json j, int *len = nullptr);
    char *make_certificate(char *&mailToCheck, int *len = nullptr);
    bool wallet_file_name_is_valid(QString name);
    void fill_token_indexes_vectors();
    void show_current_balance_everywhere();
    void fill_incoming_message_list();
    void fillOutcomingMessageList();
//    int get_current_open_wallet_index(QString mail_str);
    bool isEmailValidAddress(string str);
    bool isWord(QString str, int minimumSize = 0);
    QString get_WID_from_begin(int *index = nullptr);
    void clear_dir_files_list(QStringList &list);
    char *get_token_buffer_by_vid(const std::string &vid, int &len);
#ifndef VEDA_PROTO_BUF
    json get_token_json_by_vid(const std::string &vid);
#else
    TokenPb::Token get_token_pb_by_vid(const std::string &vid);
#endif
    char *form_token_mas_by_index(int index, int *len = nullptr);
    char *get_vid_by_index(int index);
    bool get_wid_by_email_locally(QString email);
    void finish_sending_message();
    QString getTimeFromTransaction(std::string str_time);
    void get_mails_for_wallets_section();
    bool hasStartMoney();
    void startAutoRenewTimer();
    QString makeVDNFromNum(uint64_t num);
    void renew_frontend();
    void startAutoLogOutTimer();
    bool parseWIDsByMail();
    void sortMessageListByTime(GetMailByWIDType type);
    void readReadMessageList();
    void addMessageToReadMessageList(const std::string &vid);
    void saveReadMessageList();
    void updateIncomingButtonText();
//    QString sizeFromColor(std::string str);
    QString sizeFromNum(int num);
    QString cutFileName(const QString &fileName);
    void deleteUncompletedBox();
    bool sortSection(std::string sectionName);
    void savePreferredNode(QString IP);






    //Slots--------------------------------------------------------------------
private slots:
    //Clickers-----------------------------------------------------------------
    void begin_form_signIn_ButtonClicked();
    void begin_form_register_ButtonClicked();
    void registration_form_sign_up_ButtonClicked();
    void registration_form_back_ButtonClicked();
    void on_logOut_Button_clicked();
    void main_menu_form_new_ButtonClicked();
    void main_menu_form_incoming_ButtonClicked();
    void main_menu_form_outcoming_ButtonClicked();
    void message_list_itemClicked();
    void on_settings_Button_clicked();
    void settings_form_back_ButtonClicked();
//    void settings_form_account_ButtonClicked();
//    void settings_form_connection_ButtonClicked();
//    void settings_form_general_ButtonClicked();
//    void settings_form_information_ButtonClicked();
//    void settings_form_security_ButtonClicked();
    void on_balance_Button_clicked();
    void balance_form_back_ButtonClicked();
    void read_letter_form_decrypt_ButtonClicked();
    void new_letter_form_send_ButtonClicked();
    void on_refresh_Button_clicked();
    void settings_account_form_clear_local_address_book_ButtonClicked();
    void read_letter_form_reply_ButtonClicked();
    void incoming_message_list_form_next_ButtonClicked();
    void incoming_message_list_form_prev_ButtonClicked();
    void incoming_message_list_form_first_ButtonClicked();
    void incoming_message_list_form_last_ButtonClicked();
    void balance_form_getMoreMessages_ButtonClicked();
    void newLetterAttachButtonClicked();
    void readMessageFormDeleteButtonClicked();
//    void fileToSaveChoosen();
    void fileButtonClicked(bool incoming, const QString &fileName/*, const QString &fileNameWithPath*/);
    void reg2ConfirmButtonClicked();
    void reg2BackButtonClicked();
    void setPreferredNodeButtonClicked();
    void beginSettingsButtonClicked();






    //other--------------------------------------------------------------------
    void doubleClicked();
    void error_timeout();
    void begin_form_login_lineEdit_textChanged(const QString &text);
    void autoLogOut();
    void autoLogOutSettingsChanged();





    void email_by_wid_getter(const std::string wid, const int index, GetMailByWIDType type);
    void email_by_wid_placer(QString email, const int index, GetMailByWIDType type ,bool from_server = false);
    void emailByWIDIniFileDecrementer();
    void recipientCompleterRefresher();







public slots:
    void resetAutoLogOutTimer();







    //Signals------------------------------------------------------------------
signals:
    void get_email_by_wid(const std::string wid, const int index, GetMailByWIDType type);
    void got_email_locally(QString email, const int index, GetMailByWIDType type, bool from_server = false);
    void decrementEmailsGotByWIDsCount();
    void allIncomingEmailsPlaced();
    void allOutcomingEmailsPlaced();






    //Server-------------------------------------------------------------------

signals:
    void server_reg_1(uint64_t engine,
               char *dir,
               char *ip,
               char *pass, int pass_len,
               char *secret, int secret_len,
               char *cert, int cert_len,
               char *mailToCheck);

    void server_reg_2(char *email_code,
               int code_len);

    void server_auth(uint64_t engine,
                     char *dir,
                     char *ip,
                     char *cryptobox_name, int cryptobox_name_len,
                     char *pass, int pass_len,
                     char *secret, int secret_len);
    void server_get_tokens(uint64_t wstk, bool with_trn_history_anyway = false);
    void server_send_tokens(uint64_t wstk,
                            int type,
                            char *wid_recipient,
                            char *token_mas,
                            uint64_t token_mas_len,
                            char *token_mas_comission,
                            uint64_t token_mas_comission_len,
                            char *data = 0, uint64_t data_len = 0);
    void server_get_email(uint64_t wstk, char *wid, const int index, GetMailByWIDType type);
    void server_get_wid_list_by_email(uint64_t wstk, char *email);
    void server_create_empty_messages(uint64_t wstk,
                                      char *color_text,
                                      int tokens_to_create_count,
                                      char *WIDR,
                                      char *com_mass,
                                      int com_mas_len);
    void server_nulify_wstk();

public slots:
    void server_from_reg_1(int error, bool res, uint64_t _wstk);
    void server_from_reg_2(int error);
    void server_from_auth(uint64_t _wstk,
                          int error,
                          char *token_mas,
                          uint64_t token_mas_len,
                          char *__balance,
                          uint64_t _token_cnt,
                          uint64_t marked_token_cnt,
                          char *trn_list, int trn_list_size);
    void server_from_get_tokens(int res, char *trns = NULL);
    void server_from_send_tokens(int res);
    void server_from_get_wid_list_by_email(json WIDs, int res = LIBVEDAWALLET_NOERROR);
    void server_from_create_empty_messages(int res);
};


#endif // VEDAEMAILCLIENT_H
