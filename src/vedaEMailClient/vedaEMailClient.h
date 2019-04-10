#ifndef VEDAEMAILCLIENT_H
#define VEDAEMAILCLIENT_H

//-----------------------------------------------------------------------------

#include <QWidget>
#include <QGridLayout>
#include <QThread>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QFontMetrics>
#include <QRegExp>

//-----------------------------------------------------------------------------

#include "stdlib.h"

//-----------------------------------------------------------------------------

#include "libvedawallet.h"
#include "json.hpp"
#include "vedainiwrapper.hpp"

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

#define PATH_TO_ETC "../etc"
#define PATH_TO_WALLETS_DIR "../etc/wallets"
#define PATH_TO_INI_FILE "../etc/vedaemail.ini"

#define INI_FILE_CURRENT_NODE_SECTION "CURRENT_NODE"
#define INI_FILE_WALLETS_SECTION "WALLETS"
#define INI_FILE_ADDRESS_BOOK_SECTION "ADDRESS_BOOK"
#define INI_FILE_UNKNOWN_MAIL "unknown"

#define DEFAULT_WID_SIZE 64
#define ERROR_INTERVAL 10000
#define LETTERS_PER_PAGE 10
#define EMPTY_TOKEN_COLOR_NAME " "

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

#define TRN_TRANSFER            2
#define TRN_EXCHANGE            3
#define TRN_TRANSFER_MY         4
#define TRN_CREATE_MARKED_TKN   5

#define JSON_TRN_TOKEN_IN_NOMINAL       "NOMINAL"
#define JSON_TRN_TOKEN_IN_TID_TRN       "TID"
#define JSON_TRN_TOKEN_IN_HASH_TRN      "HASH"

namespace Ui {
class doubleClickFilter;
class vedaEMailClient;
class server_functions;
}

using json = nlohmann::json;








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
               char *cert, int cert_len);

    void reg_2(char *email_code,
               int code_len);

    void auth(uint64_t engine,
              char *dir,
              char *ip,
              char *cryptobox_name, int cryptobox_name_len,
              char *pass, int pass_len,
              char *secret, int secret_len);
    void get_tokens(uint64_t wstk);
    void send_tokens(uint64_t wstk,
                     int type,
                     char *wid_recipient,
                     char *token_mas, uint64_t token_mas_len,
                     char *token_mas_comission, uint64_t token_mas_comission_len,
                     char *data = 0, uint64_t data_len = 0);
    void get_email(uint64_t wstk, char *wid, const int index);
    void get_wid_list_by_email(uint64_t wstk, char *email);

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
                  uint64_t marked_token_cnt);
    void get_tokens_res(int res);
    void send_tokens_res(int res);
    void get_email_res(QString email, const int index, bool from_server = true);
    void get_wid_list_by_email_res(json WIDs, int res = LIBVEDAWALLET_NOERROR, bool from_server = false);

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
    enum MessageList_Categiroes{
        Incoming,
        Outcoming
    };








    //Structs------------------------------------------------------------------
    typedef struct _TOKEN_DATA{
        std::string vid;
        json j_data;
    }TOKEN_DATA;






    //Fields-------------------------------------------------------------------
    Ui::vedaEMailClient *ui;
    begin_f *begin_form;
    registration *registration_form;
    main_menu *main_menu_form;
    message_list *incoming_message_list_form,
                 *outcoming_message_list_form;
    new_letter *new_letter_form;
    read_letter *read_letter_form;
    settings *settings_form;
    settings_information *settings_information_form;
    settings_account *settings_account_form;
    settings_connection *settings_connection_form;
    settings_general *settings_general_form;
    settings_security *settings_security_form;
    balance_f *balance_form;
    loading_screen *loading_screen_form;




    QVector<QWidget*> main_menu_forms,
                      settings_forms;
    doubleClickFilter dblClickFilter;



    uint64_t engine;
    server_functions *server;
    QThread *server_thread;
    QTimer *error_timer;
    QLabel *_error_label;

//    QStringList Wallets;
    QString current_wid;
    QString current_mail_address;
    int current_opened_wallet;
    QString recipient_wid;

    uint64_t wstk = 0;
    json j_tokens;
    uint64_t balance;
    uint64_t letters_count;
    uint64_t tokens_count;
    QVector<uint64_t> incoming_letters_indexes,
                      clear_letters_indexes,
                      tokens_indexes;
    uint64_t current_first_incoming_letter = 0;
    QVector<message_list_item*> incoming_message_list,
                                outcoimng_message_list;
    bool ignore_get_tokens_on_refresh = false;





    //Ini file-----------------------------------------------------------------
    VedaIniWrapper *iniWrapper;
    std::vector<VedaIniEntry> mail_wallet_pairs;
    QString current_node;
    QDir dir;
    QString executable_path;





    //Methods------------------------------------------------------------------
    template<typename T> void init_widget(T *&widget, QWidget *parent);
    void init_widgets();
    void connect_widgets();
    void show_begin();
    void switch_main_menu_content(QWidget *to);
    void switch_settings_content(QWidget *to);
    void toggle_main_menu_upper_buttons(bool on);
    void toggle_loading_screen();




    void init_server_thread();
    void connect_server_thread();
    void read_ini_file();
    void read_wallets();
    void read_address_book();
    void read_current_node();
    void save_new_mail_wallet_pair(QString mail_address, QString wallet);
    QString find_new_wid();
//    QString get_email_by_wid(const string wid);
//    void write_wallets();
    void show_error_label(QLabel *error_label,
                           QString error_text);
    char *QString_to_char_array(QString str, int *len = nullptr);
    char *std_string_to_char_array(std::string str, int *len = nullptr);
    char *make_certificate(int *len = nullptr);
    bool wallet_file_name_is_valid(QString name);
    void fill_token_indexes_vectors();
    void show_current_balance_everywhere();
    void fill_incoming_message_list();
    int get_current_open_wallet_index(QString mail_str);
    bool isEmailValidAddress(string str);
    QString get_WID_from_begin(int *index = nullptr);
    void clear_dir_files_list(QStringList &list);
    char *get_token_buffer_by_vid(const std::string &vid);
    json get_token_json_by_vid(const std::string &vid);
    char *form_token_mas_by_index(int index, int *len = nullptr);
    char *get_vid_by_index(int index);
    bool get_wid_by_email_locally(QString email);
    void finish_sending_message(bool withRecipientWID = false);






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
    void settings_form_account_ButtonClicked();
    void settings_form_connection_ButtonClicked();
    void settings_form_general_ButtonClicked();
    void settings_form_information_ButtonClicked();
    void settings_form_security_ButtonClicked();
    void on_balance_Button_clicked();
    void balance_form_back_ButtonClicked();
    void read_letter_form_decrypt_ButtonClicked();
    void new_letter_form_send_ButtonClicked();
    void on_refresh_Button_clicked();






    //other--------------------------------------------------------------------
    void doubleClicked();
    void error_timeout();
    void begin_form_login_lineEdit_textChanged();
    void new_letter_form_recipient_lineEdit_editingFinished();





    void email_by_wid_getter(const std::string wid, const int index);
    void email_by_wid_placer(QString email, const int index, bool from_server = false);







    //Signals------------------------------------------------------------------
signals:
    void get_email_by_wid(const std::string wid, const int index);
    void got_email_locally(QString email, const int index, bool from_server = false);






    //Server-------------------------------------------------------------------

signals:
    void server_reg_1(uint64_t engine,
               char *dir,
               char *ip,
               char *pass, int pass_len,
               char *secret, int secret_len,
               char *cert, int cert_len);

    void server_reg_2(char *email_code,
               int code_len);

    void server_auth(uint64_t engine,
                     char *dir,
                     char *ip,
                     char *cryptobox_name, int cryptobox_name_len,
                     char *pass, int pass_len,
                     char *secret, int secret_len);
    void server_get_tokens(uint64_t wstk);
    void server_send_tokens(uint64_t wstk,
                            int type,
                            char *wid_recipient,
                            char *token_mas,
                            uint64_t token_mas_len,
                            char *token_mas_comission,
                            uint64_t token_mas_comission_len,
                            char *data = 0, uint64_t data_len = 0);
    void server_get_email(uint64_t wstk, char *wid, const int index);
    void server_get_wid_list_by_email(uint64_t wstk, char *email);

public slots:
    void server_from_reg_1(int error, bool res, uint64_t _wstk);
    void server_from_reg_2(int error);
    void server_from_auth(uint64_t _wstk,
                          int error,
                          char *token_mas,
                          uint64_t token_mas_len,
                          char *__balance,
                          uint64_t _token_cnt,
                          uint64_t marked_token_cnt);
    void server_from_get_tokens(int res);
    void server_from_send_tokens(int res);
    void server_from_get_wid_list_by_email(json WIDs, int res = LIBVEDAWALLET_NOERROR, bool from_server = false);
};


#endif // VEDAEMAILCLIENT_H
