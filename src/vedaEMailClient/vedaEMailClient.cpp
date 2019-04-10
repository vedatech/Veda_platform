#include "vedaEMailClient.h"
#include "ui_vedaEMailClient.h"

vedaEMailClient::vedaEMailClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vedaEMailClient)
{
    ui->setupUi(this);
    ui->description_label->hide();
    setWindowTitle("VEDA EMail client");

    if (LIBVEDAWALLET_NOERROR != veda_wallet_init_library(engine)){
        exit(-1);
    }

    init_widgets();

    connect_widgets();

    init_server_thread();

    connect_server_thread();

    executable_path = QCoreApplication::applicationDirPath();
    QDir::setCurrent(executable_path);
    iniWrapper = new (std::nothrow) VedaIniWrapper(PATH_TO_INI_FILE, true);
    read_ini_file();

    error_timer = new (std::nothrow) QTimer(this);
    error_timer->setSingleShot(true);
    error_timer->setInterval(ERROR_INTERVAL);
    connect(error_timer, SIGNAL(timeout()),
            this, SLOT(error_timeout()));




    //
    message_list_item *item1 = new (std::nothrow) message_list_item("author1", "new message", 0, main_menu_form);
    incoming_message_list_form->additem(item1);
    incoming_message_list << item1;
    connect(item1, SIGNAL(clicked()),
            this, SLOT(message_list_itemClicked()));
    //




    show_begin();
}

vedaEMailClient::~vedaEMailClient()
{
    delete ui;

    delete server;
    if (server_thread->isRunning()){
    server_thread->exit();
    }
    server_thread->deleteLater();

    if (0 != wstk) veda_wallet_free(wstk);
    veda_wallet_stop_library(engine);
}

template<typename T> void vedaEMailClient::init_widget(T *&widget, QWidget *parent)
{
    QWidget *content_widget;
    QGridLayout *content_layout;

    if (this == parent){
        content_widget = this->ui->content_widget;
        content_layout = this->ui->content_layout;
    }
    else if (main_menu_form == parent){
        content_widget = main_menu_form->ui->content_widget;
        content_layout = main_menu_form->ui->content_layout;
    }
    else if (settings_form == parent){
        content_widget = settings_form->ui->content_widget;
        content_layout = settings_form->ui->content_layout;
    }
    else return;

    widget = new (std::nothrow) T(content_widget);
    if (nullptr == widget) exit(-1);
    content_layout->addWidget(widget);
    widget->hide();

    if (main_menu_form == parent){
        main_menu_forms << widget;
    }
    else if (settings_form == parent){
        settings_forms << widget;
    }
}

void vedaEMailClient::init_widgets()
{
    loading_screen_form = new (std::nothrow) loading_screen(this);
    if (nullptr == loading_screen_form) exit(-1);
    ui->gridLayout->addWidget(loading_screen_form);
    loading_screen_form->hide();

    init_widget<begin_f>(begin_form, this);
    init_widget<main_menu>(main_menu_form, this);
    init_widget<settings>(settings_form, this);
    init_widget<registration>(registration_form, this);

    init_widget<message_list>(incoming_message_list_form, main_menu_form);
    init_widget<message_list>(outcoming_message_list_form, main_menu_form);
    init_widget<new_letter>(new_letter_form, main_menu_form);
    init_widget<read_letter>(read_letter_form, main_menu_form);

    init_widget<settings_information>(settings_information_form, settings_form);
    init_widget<settings_account>(settings_account_form, settings_form);
    init_widget<settings_connection>(settings_connection_form, settings_form);
    init_widget<settings_general>(settings_general_form, settings_form);
    init_widget<settings_security>(settings_security_form, settings_form);

    init_widget<balance_f>(balance_form, this);

}


void vedaEMailClient::connect_widgets()
{
    connect(this, SIGNAL( get_email_by_wid(const std::string, const int)),
            this, SLOT(email_by_wid_getter(const std::string, const int)));



    connect(this, SIGNAL(got_email_locally(QString, const int, bool)),
            this, SLOT(email_by_wid_placer(QString, const int, bool)));



    connect(begin_form->ui->signIn_Button, SIGNAL(clicked()),
            this,                          SLOT(begin_form_signIn_ButtonClicked()));
    connect(begin_form->ui->register_Button, SIGNAL(clicked()),
            this,                            SLOT(begin_form_register_ButtonClicked()));
    begin_form->ui->login_lineEdit->installEventFilter(&dblClickFilter);
    connect(&dblClickFilter, SIGNAL(doubleClick()),
            this, SLOT(doubleClicked()));
    connect(begin_form->ui->login_lineEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(begin_form_login_lineEdit_textChanged()));



    connect(registration_form->ui->sign_up_Button, SIGNAL(clicked()),
            this,                                  SLOT(registration_form_sign_up_ButtonClicked()));
    connect(registration_form->ui->back_Button, SIGNAL(clicked()),
            this,                               SLOT(registration_form_back_ButtonClicked()));



    connect(main_menu_form->ui->new_Button, SIGNAL(clicked()),
            this, SLOT(main_menu_form_new_ButtonClicked()));
    connect(main_menu_form->ui->incoming_Button, SIGNAL(clicked()),
            this, SLOT(main_menu_form_incoming_ButtonClicked()));
    connect(main_menu_form->ui->outcoming_Button, SIGNAL(clicked()),
            this, SLOT(main_menu_form_outcoming_ButtonClicked()));


    connect(settings_form->ui->back_Button, SIGNAL(clicked()),
            this, SLOT(settings_form_back_ButtonClicked()));
    connect(settings_form->ui->account_Button, SIGNAL(clicked()),
            this, SLOT(settings_form_account_ButtonClicked()));
    connect(settings_form->ui->connection_Button, SIGNAL(clicked()),
            this, SLOT(settings_form_connection_ButtonClicked()));
    connect(settings_form->ui->general_Button, SIGNAL(clicked()),
            this, SLOT(settings_form_general_ButtonClicked()));
    connect(settings_form->ui->information_Button, SIGNAL(clicked()),
            this, SLOT(settings_form_information_ButtonClicked()));
    connect(settings_form->ui->security_Button, SIGNAL(clicked()),
            this, SLOT(settings_form_security_ButtonClicked()));



    connect(balance_form->ui->back_Button, SIGNAL(clicked()),
            this, SLOT(balance_form_back_ButtonClicked()));



    connect(read_letter_form->ui->decrypt_Button, SIGNAL(clicked()),
            this, SLOT(read_letter_form_decrypt_ButtonClicked()));



    connect(new_letter_form->ui->send_Button, SIGNAL(clicked()),
            this, SLOT(new_letter_form_send_ButtonClicked()));
    new_letter_form->ui->recipient_lineEdit->installEventFilter(&dblClickFilter);
    connect(&dblClickFilter, SIGNAL(doubleClick()),
            this, SLOT(doubleClicked()));
    connect(new_letter_form->ui->recipient_lineEdit, SIGNAL(editingFinished()),
            this, SLOT(new_letter_form_recipient_lineEdit_editingFinished()));
}

void vedaEMailClient::show_begin()
{
    begin_form->show();
    toggle_main_menu_upper_buttons(false);

    begin_form->ui->password_lineEdit->setText("1234567890123456");
    begin_form->ui->secret_lineEdit->setText("12345678");
}

void vedaEMailClient::switch_main_menu_content(QWidget *to)
{
    for (int i = 0, limit = main_menu_forms.size(); i < limit; i++){
        main_menu_forms[i]->hide();
    }
    to->show();
}

void vedaEMailClient::switch_settings_content(QWidget *to)
{
    for (int i = 0, limit = settings_forms.size(); i < limit; i++){
        settings_forms[i]->hide();
    }
    to->show();
}

void vedaEMailClient::toggle_main_menu_upper_buttons(bool on)
{
    if (on){
        ui->balance_Button->show();
        ui->logOut_Button->show();
        ui->refresh_Button->show();
        ui->settings_Button->show();
        ui->message_available_count_label->show();
        ui->messages_available_word_label->show();
    }
    else{
        ui->balance_Button->hide();
        ui->logOut_Button->hide();
        ui->refresh_Button->hide();
        ui->settings_Button->hide();
        ui->message_available_count_label->hide();
        ui->messages_available_word_label->hide();
    }
}

void vedaEMailClient::toggle_loading_screen()
{
    if (!loading_screen_form->isVisible()){
        ui->widget->hide();
        loading_screen_form->_show();
    }
    else {
        loading_screen_form->_hide();
        ui->widget->show();
    }
}

void vedaEMailClient::init_server_thread()
{
    server = new (std::nothrow) server_functions();
    if (nullptr == server) exit(-1);
    server_thread = new (std::nothrow) QThread(this);
    if (nullptr == server_thread) exit(-1);

    server->moveToThread(server_thread);
    server_thread->start();
}

void vedaEMailClient::connect_server_thread()
{
    qRegisterMetaType<uint64_t>("uint64_t");
    qRegisterMetaType<json>("json");

    connect(this, SIGNAL(server_reg_1(uint64_t, char *, char *, char *, int, char *, int, char *, int)),
            server, SLOT(       reg_1(uint64_t, char *, char *, char *, int, char *, int, char *, int)));
    connect(server, SIGNAL(    reg_1_res(int, bool, uint64_t)),
            this, SLOT(server_from_reg_1(int, bool, uint64_t)));



    connect(this, SIGNAL(server_reg_2(char *, int)),
            server, SLOT(reg_2(char *, int)));
    connect(server, SIGNAL(reg_2_res(int)),
            this, SLOT(server_from_reg_2(int)));



    connect(this, SIGNAL(server_auth(uint64_t, char *, char *, char *, int, char *, int, char *, int)),
            server, SLOT(       auth(uint64_t, char *, char *, char *, int, char *, int, char *, int)));
    connect(server, SIGNAL(    auth_res(uint64_t, int, char *, uint64_t, char *, uint64_t, uint64_t)),
            this, SLOT(server_from_auth(uint64_t, int, char *, uint64_t, char *, uint64_t, uint64_t)));



    connect(this, SIGNAL(server_get_tokens(uint64_t)),
            server, SLOT(get_tokens(uint64_t)));
    connect(server, SIGNAL(get_tokens_res(int)),
            this, SLOT(server_from_get_tokens(int)));



    connect(this, SIGNAL(server_send_tokens(uint64_t, int, char *, char *, uint64_t, char *, uint64_t, char *, uint64_t)),
            server, SLOT(       send_tokens(uint64_t, int, char *, char *, uint64_t, char *, uint64_t, char *, uint64_t)));
    connect(server, SIGNAL(    send_tokens_res(int)),
            this, SLOT(server_from_send_tokens(int)));



    connect(this, SIGNAL(server_get_email(uint64_t, char *, const int)),
            server, SLOT(       get_email(uint64_t, char *, const int)));
    connect(server, SIGNAL(  get_email_res(QString, const int, bool)),
            this, SLOT(email_by_wid_placer(QString, const int, bool)));



    connect(this, SIGNAL(server_get_wid_list_by_email(uint64_t, char *)),
            server, SLOT(       get_wid_list_by_email(uint64_t, char *)));
    connect(server, SIGNAL(    get_wid_list_by_email_res(json, int, bool)),
            this, SLOT(server_from_get_wid_list_by_email(json, int, bool)));
}

void vedaEMailClient::read_ini_file()
{
    if (!dir.cd(PATH_TO_ETC)){
        dir.mkdir(PATH_TO_ETC);
    }
    dir.cd(executable_path);
    if (!dir.cd(PATH_TO_WALLETS_DIR)){
        dir.mkdir(PATH_TO_WALLETS_DIR);
    }
    dir.cd(executable_path);

    read_wallets();

    read_address_book();

    read_current_node();

    ///\todo отсортировать адресную книгу, а также проверить ее содержимое на хуйню
}

void vedaEMailClient::read_wallets()
{
    //1. Считать данные ini - уже считаны при открытии ini файла

    //2. Считать реальные файлы в папке
    if (dir.cd(PATH_TO_WALLETS_DIR)){
        QStringList dirList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        clear_dir_files_list(dirList);

        //3. Удалить из ini несуществующие
        //3.5. Также переименовать в unknown адреса, не соответствующие шаблону почты
        if (!iniWrapper->datas.empty()){
            bool found_first = false;
            for (int i = 0, limit = iniWrapper->datas.size(); i < limit;){


                QString ___del = QString::fromStdString(iniWrapper->datas[i].value);


                if (0 == strcmp(INI_FILE_WALLETS_SECTION,iniWrapper->datas[i].index.c_str())){
                    found_first = true;

                    if (!dirList.contains(QString::fromStdString(iniWrapper->datas[i].value))){
                        limit--;
                        iniWrapper->datas.erase(iniWrapper->datas.begin() + i);
                    }
                    else {
                        if (!isEmailValidAddress(iniWrapper->datas[i].name)){
                            iniWrapper->datas[i].name = INI_FILE_UNKNOWN_MAIL;
                        }

                        //превентивно чистить dirList чтобы оставить только неучтенные в ini
                        dirList.removeOne(QString::fromStdString(iniWrapper->datas[i].value));

                        i++;
                    }
                }
                else if (found_first){
                    break;
                }
                else {
                    i++;
                }
            }
        }

        //4. Удалить из ini дубликаты
        if (!iniWrapper->datas.empty()){
            std::string name, value;
            for (int i = 0, limit = iniWrapper->datas.size(); i < limit; i++){
                if (0 != strcmp(INI_FILE_WALLETS_SECTION,iniWrapper->datas[i].index.c_str())){
                    continue;
                }

                name = iniWrapper->datas[i].name;
                value = iniWrapper->datas[i].value;
                for (int j = i + 1; j < iniWrapper->datas.size();) {
                    if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
                        if ((name == iniWrapper->datas[j].name) && (value == iniWrapper->datas[j].value)){

                            limit--;
                            iniWrapper->datas.erase(iniWrapper->datas.begin() + j);
                        }
                        else j++;
                    }
                    else break;
                }
            }
        }

        //5. Добавить в ini неучтенные как unknown (WID)
        if (!dirList.isEmpty()){
            //5.1. Найти индекс вставки
            int insertPlace = 0;
            if (!iniWrapper->datas.empty()) {
                bool section_found = false;
                for (; insertPlace < iniWrapper->datas.size(); insertPlace++){
                    if (0 != strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[insertPlace].index.c_str())){
                        if (section_found) {
                            break;
                        }
                        else {
                            continue;
                        }
                    }
                    else {
                        section_found = true;
                    }
                }
            }

            auto it = iniWrapper->datas.begin();
            std::advance(it, insertPlace);
            VedaIniEntry temp;
            temp.index = INI_FILE_WALLETS_SECTION;
            temp.name = INI_FILE_UNKNOWN_MAIL;
            for (int i = 0; i < dirList.size(); i++){
                temp.value = dirList[i].toLocal8Bit().data();
                iniWrapper->datas.insert(it++, temp);
            }
        }
    }

    //6. Отсортировать ini по одинаковым Email'ам:
    if (!iniWrapper->datas.empty()){
        std::string mail;
        int size = iniWrapper->datas.size();
        int same_name_entries;

        for (int i = 0; i < size; i += (1 + same_name_entries)){
            if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
                mail = iniWrapper->datas[i].name;
                same_name_entries = 0;
                for (int j = i + 1; j < size; j++){
                    if (iniWrapper->datas[j].name == mail){
                        if ((j - i) > ++same_name_entries){
                            iniWrapper->datas.insert(iniWrapper->datas.begin() + i + 1 + same_name_entries,
                                                     iniWrapper->datas[j]);
                            iniWrapper->datas.erase(iniWrapper->datas.begin() + j + 1);
                        }
                    }
                }
            }
        }
    }

    //7. Перезаписать ini
    iniWrapper->writeConfigFile();

    //8. Заполнить completer
    if (!iniWrapper->datas.empty()){
        QStringList completions;
        QString last;
        bool first_same_mail = true;
        for (int i = 0; i < iniWrapper->datas.size(); i++){
            if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
                completions << QString::fromStdString(iniWrapper->datas[i].name);
                if (last == completions.last()){
                    if (first_same_mail){
                        completions[completions.size() - 2] = last + " (" +
                                QString::fromStdString(iniWrapper->datas[i - 1].value) + ")";
                        first_same_mail = false;
                    }
                        completions.last() += " (" +
                                QString::fromStdString(iniWrapper->datas[i].value) + ")";
                }
                else if (QString(INI_FILE_UNKNOWN_MAIL) == completions.last()) {
                    completions.last() += " (" +
                            QString::fromStdString(iniWrapper->datas[i].value) + ")";
                }
                else {
                    last = completions.last();
                    first_same_mail = true;
                }
            }
        }





        //
//        completions.clear();
//        completions << "2N7FWMTMBPJ9RCK6MTC2QDXQAUODXTPVAPVQOQX7MVWPZLHTS9YYWSWZMH45TMD8";
//        completions << "29LEM1Q5HJBVYUDJQD79PFFWFKYYN3EBMU41N6VX4NLA7RYTK9CWSNJQQYKPX8I7";
        //







        begin_form->set_completer(completions);
    }
}

void vedaEMailClient::read_address_book()
{
    //1. Считать ini - считано конструктором ini

    //2. Удалить лажу
    if (!iniWrapper->datas.empty()){
        for (int i = 0;i < iniWrapper->datas.size(); i++){
            if (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str())){
                //лажа - если имя - не email или соответствующий адрес - не адрес
                if (!isEmailValidAddress(iniWrapper->datas[i].name) ||
                        !wallet_file_name_is_valid(QString::fromStdString(iniWrapper->datas[i].value))){
                    iniWrapper->datas.erase(iniWrapper->datas.begin() + i);
                }
            }
        }
    }

    //3. Отсортировать
    if (!iniWrapper->datas.empty()){
        bool sorted = true;
        int i = 0;
        for (; i < iniWrapper->datas.size(); i++){
            if (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str()))
                break;
        }

        if (i == iniWrapper->datas.size()){//секции нет
            //все хорошо
        }
        else if (i == iniWrapper->datas.size() - 1){
            //тоже все хорошо
        }
        else {
            for (; i < iniWrapper->datas.size() - 1; i++){
                if (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str())){
                    if (iniWrapper->datas[i].name > iniWrapper->datas[i + 1].name){
                        sorted = false;
                    }
                }
            }
        }

        if (!sorted){
            ///\todo отсортировать
        }
    }

    iniWrapper->writeConfigFile();

    //доставить completer в создание сообщения
    std::vector<VedaIniEntry> entries;
    QStringList to_completer;
    if (0 < iniWrapper->getAllEntrysByName(INI_FILE_ADDRESS_BOOK_SECTION, entries)){
        for (int i = 0; i < entries.size(); i++){
            to_completer << QString::fromStdString(entries[i].name);
        }
    }

    new_letter_form->set_completer(to_completer);
}

void vedaEMailClient::read_current_node()
{
    std::vector<VedaIniEntry> vect;
    int res;
    if (0 == (res = iniWrapper->getAllEntrysByName(INI_FILE_CURRENT_NODE_SECTION, vect))){
        current_node = "116.203.21.103";
    }
    else if (1 == res){
        current_node = QString::fromStdString(vect[0].name);
    }
    else {
        /// \todo очистить лишнее
    }
}

void vedaEMailClient::save_new_mail_wallet_pair(QString mail_address, QString wallet)
{
    if (wallet.isEmpty()) return;

    //1. Найти место для вставки
    int i = 0;

    //Вставить в начало секции
    for (; i < iniWrapper->datas.size(); i++){
        if (strcmp(INI_FILE_WALLETS_SECTION,iniWrapper->datas[i].index.c_str())) break;
    }

    VedaIniEntry temp;
    temp.name = mail_address.toLocal8Bit().data();
    temp.value = wallet.toLocal8Bit().data();
    temp.index = INI_FILE_WALLETS_SECTION;
    iniWrapper->datas.insert(iniWrapper->datas.begin() + i, temp);
    iniWrapper->writeConfigFile();
}

QString vedaEMailClient::find_new_wid()
{
    if (!dir.cd(PATH_TO_WALLETS_DIR)) return "";
    QStringList dirList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    clear_dir_files_list(dirList);

    //найти WID, не существующий в ini
    QString WID;
    for (int i = 0; i < iniWrapper->datas.size(); i++) {
        if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
            WID = QString::fromStdString(iniWrapper->datas[i].value);
            if (dirList.contains(WID)){
                dirList.removeOne(WID);
            }
        }
    }

    if (1 == dirList.size()){//условие правильного нахождения несуществующего WID
        return dirList[0];
    }
    else{
        return "";
    }
}

//QString vedaEMailClient::get_email_by_wid(const std::string wid)
//{
//    QString ret;

//    //1. Найти локально
//    std::vector<VedaIniEntry> local;
//    if (0 != iniWrapper->getAllEntrysByName(INI_FILE_ADDRESS_BOOK_SECTION, local)){
//        for (int i = 0; i < local.size(); i++){
//            if (wid == iniWrapper->datas[i].value){
//                return QString::fromStdString(iniWrapper->datas[i].name);
//            }
//        }
//    }

//    ///\todo разбить на до и после обращения к серверу
//    //2. Если локальный поиск успеха не дал, обратиться к серверу
//    char *wid_ch = std_string_to_char_array(wid);
//    if (NULL == wid_ch){
//        return ret;
//    }

//    char *email_ch = NULL;
//    int email_ch_len = 0;
//    if (LIBVEDAWALLET_NOERROR != veda_wallet_get_email(wstk,
//                                                       wid_ch,
//                                                       &email_ch,
//                                                       email_ch_len)){
//        free(wid_ch);
//        return ret;
//    }
//    free(wid_ch);
//    ret = email_ch;

//    //3. добавить в адресную книгу
//    std::string section_name_string = INI_FILE_ADDRESS_BOOK_SECTION;
//    for (int i = 0; i < iniWrapper->datas.size(); i++){
//        if (iniWrapper->datas[i].index != section_name_string) continue;


//    }

//    return ret;
//}

//void vedaEMailClient::write_wallets()
//{
//    for (int i = 0; i < iniWrapper->datas.size(); i++){
//        if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
//            iniWrapper->datas[i].clear();
//        }
//    }

//    for (int i = 0; i < mail_wallet_pairs.size(); i++){
//        iniWrapper->setStringValue(INI_FILE_WALLETS_SECTION,
//                                   mail_wallet_pairs[i].name.c_str(),
//                                   mail_wallet_pairs[i].value.c_str());
//    }
//    iniWrapper->writeConfigFile();
//}

void vedaEMailClient::show_error_label(QLabel *error_label, QString error_text)
{
    _error_label = error_label;
    _error_label->setText("<FONT COLOR=#F00000>" +
                         error_text +
                         "</FONT>");
    _error_label->show();
    error_timer->start();
}

char *vedaEMailClient::QString_to_char_array(QString str, int *len)
{
    if (str.isEmpty())
        return NULL;

    int *l;
    int ll;
    if (nullptr == len){
        l = &ll;
    }
    else {
        l = len;
    }

    *l =  str.toLocal8Bit().size();
    char *ret = (char *)malloc(*l + 1);
    if (!ret)
        return NULL;

    memset(ret, 0x0, *l + 1);
    memcpy(ret, str.toLocal8Bit().data(), *l);
    return ret;
}

char *vedaEMailClient::std_string_to_char_array(string str, int *len)
{
    if (str.empty()){
        return NULL;
    }

    int *l;
    int ll;
    if (nullptr == len){
        l = &ll;
    }
    else {
        l = len;
    }

    *l = str.size();
    char *ret = (char *)malloc(*l + 1);
    if (!ret){
        return NULL;
    }

    memset(ret, 0x0, *l + 1);
    memcpy(ret, str.c_str(), *l);
    return ret;
}

char *vedaEMailClient::make_certificate(int *len)
{
    char *res = NULL;
    std::string temp;
    int *l;
    int ll;
    json j;

    if (nullptr == len){
        l = &ll;
    }
    else {
        l = len;
    }

    j[JSON_CERT_FIO] = std::string(registration_form->ui->first_name_lineEdit->text().toLocal8Bit().data()) +
               std::string(" ") +
               std::string(registration_form->ui->last_name_lineEdit->text().toLocal8Bit().data());
    j[JSON_CERT_CITY] = std::string(registration_form->ui->city_lineEdit->text().toLocal8Bit().data());
    j[JSON_CERT_STATE] = std::string(registration_form->ui->state_lineEdit->text().toLocal8Bit().data());
    j[JSON_CERT_COUNTRY] = std::string(registration_form->ui->country_comboBox->currentText().toLocal8Bit().data());
    j[JSON_CERT_EMAIL] = std::string(registration_form->ui->mail_lineEdit->text().toLocal8Bit().data()) +
                         std::string(registration_form->ui->mail_postfix_lineEdit->text().toLocal8Bit().data());
    j[JSON_CERT_ORG] = std::string(" ");
    j[JSON_CERT_ORG_UNIT] = std::string(" ");

    temp = j.dump();
    *l = temp.size();
    res = (char *)malloc(*l + 1);
    if (!res) return res;
    memcpy(res, temp.c_str(), *l);
    res[*l] = 0x0;
    return res;
}

bool vedaEMailClient::wallet_file_name_is_valid(QString name)
{
    return (DEFAULT_WID_SIZE == name.size());
}

void vedaEMailClient::fill_token_indexes_vectors()
{
    incoming_letters_indexes.clear();
    clear_letters_indexes.clear();
    tokens_indexes.clear();

    int index = -1;
    bool status = false;

    char *vid_ch = NULL;
    char *token_ch = NULL;
    int token_ch_len = 0;
    json j;
    std::string color_name;
    std::string empty_token_compare_pattern = EMPTY_TOKEN_COLOR_NAME;
    for (auto it = j_tokens[JSON_BALANCE_MASS].begin(); it != j_tokens[JSON_BALANCE_MASS].end(); ++it){

        index++;

        try {
            status = j_tokens[JSON_BALANCE_MASS][it.key()][JSON_BALANCE_STATUS].get<bool>();
        } catch (...) {
            qDebug()<<"json parse exception";
            continue;
        }
        if (!status) {
            continue;
        }

        try {
            if (j_tokens[JSON_BALANCE_MASS][it.key()][JSON_BALANCE_TYPE].get<int>() == 0) {
                tokens_indexes <<index;
            }
            else {
//                vid_ch = std_string_to_char_array(it.key());
//                if (NULL == vid_ch){
//                    qDebug()<<"Low memory";
//                    return;
//                }

//                if (LIBVEDAWALLET_NOERROR != veda_wallet_get_token_json_by_id(wstk,
//                                                                              vid_ch,
//                                                                              &token_ch,
//                                                                              token_ch_len)){
//                    qDebug()<<"cannot get token info";
//                    free(vid_ch);
//                    return;
//                }
//                free(vid_ch);
//                vid_ch = NULL;

//                j.clear();
//                j = json::parse(std::string(token_ch, token_ch_len));
//                free(token_ch);
//                token_ch = NULL;
//                token_ch_len = 0;

//                if (j.empty()){
//                    qDebug()<<"parse json failure";
//                    return;
//                }
                j = get_token_json_by_vid(it.key());
                if (j.empty()){
                    qDebug()<<"parse json failure";
                    return;
                }

                color_name = j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_COLOR_NAME].get<std::string>();
                if (color_name == empty_token_compare_pattern){
                    clear_letters_indexes << index;
                }
                else {
                    incoming_letters_indexes << index;
                }
            }
        }
        catch(...){
            qDebug()<<"json parse exception";
            continue;
        }
    }
}

void vedaEMailClient::show_current_balance_everywhere()
{
    ui->message_available_count_label->setText(QString::number(clear_letters_indexes.size()));
    ///\todo минимальное из количества комиссионных токенов и количества болванок
}

void vedaEMailClient::fill_incoming_message_list()
{
    //удалить предыдущие
    message_list_item *temp;
    if (!incoming_message_list.isEmpty()){
        for(int i = 0, limit = incoming_message_list.size(); i < limit; i++){
            temp = incoming_message_list.last();
            incoming_message_list.removeLast();
            delete temp;
        }
    }
    //заполнить новыми
    ///\todo проверка допустимости загрузки списка

    if (incoming_letters_indexes.isEmpty()){
        return;
    }
    int iterator_index = 0;
    int current_index_to_find = incoming_letters_indexes[current_first_incoming_letter];
    int current_letters_vector_index = 0;
    int letters_found = 0;
    TOKEN_DATA td;

    int letters_number_to_find = (LETTERS_PER_PAGE >= incoming_letters_indexes.size()) ?
                incoming_letters_indexes.size() : LETTERS_PER_PAGE;

    for (auto it = j_tokens[JSON_BALANCE_MASS].begin();
         (it != j_tokens[JSON_BALANCE_MASS].end()) &&
         (letters_found < letters_number_to_find);
         ++it, iterator_index++){

        if (iterator_index < current_index_to_find) continue;

        current_letters_vector_index = current_first_incoming_letter + ++letters_found;
        if (letters_found < letters_number_to_find){
            current_index_to_find = incoming_letters_indexes[current_letters_vector_index];
        }
//        td.vid = it.key();
//        td.j_data = it.value();

        json token_j;

//        char *vid_ch = std_string_to_char_array(td.vid);
//        if (NULL == vid_ch){
//            qDebug()<<"Low memory";
//            return;
//        }

//        char *token_ch = NULL;
//        int token_ch_len = 0;
//        if (LIBVEDAWALLET_NOERROR != veda_wallet_get_token_json_by_id(wstk,
//                                                                      vid_ch,
//                                                                      &token_ch,
//                                                                      token_ch_len)){
//            qDebug()<<"cannot get token info";
//            free(vid_ch);
//            return;
//        }
//        free(vid_ch);

//        token_j = json::parse(std::string(token_ch, token_ch_len));
//        free(token_ch);
//        if (token_j.empty()){
//            qDebug()<<"parse json failure";
//            return;
//        }
        token_j = get_token_json_by_vid(it.key());
        if (token_j.empty()){
            qDebug()<<"parse json failure";
            return;
        }

        std::string author;
        QString short_description;
        try {
            author = token_j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_AUTHOR_ID].get<std::string>();
            short_description = QString::fromStdString(token_j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_COLOR_NAME].get<std::string>());
        } catch (...) {
            qDebug()<<"parse json exception";
        }

//        char *author_WID = QString_to_char_array(author);
//        if (NULL == author_WID){
//            qDebug()<<"Low memory";
//            return;
//        }

//        char *author_mail_ch = NULL;
//        int author_mail_ch_len = 0;
//        if (LIBVEDAWALLET_NOERROR != veda_wallet_get_email(wstk, author_WID, &author_mail_ch, author_mail_ch_len)){
////            qDebug()<<"error getting email";
////            free(author_WID);
////            return;
//            author = INI_FILE_UNKNOWN_MAIL;
//        }
//        else {
//            author = author_mail_ch;
//            free(author_mail_ch);
//        }

        temp = new (std::nothrow) message_list_item(INI_FILE_UNKNOWN_MAIL, short_description, current_letters_vector_index - 1, main_menu_form);
        temp->set_wid(author);
        incoming_message_list_form->additem(temp);
        incoming_message_list << temp;
        emit get_email_by_wid(author, incoming_message_list.size() - 1);

        connect(temp, SIGNAL(clicked()), this, SLOT(message_list_itemClicked()));
    }
}

int vedaEMailClient::get_current_open_wallet_index(QString mail_str)
{
    QString address;
    //Если описание расширенное
    if (mail_str.contains("(")){
        address = mail_str.split("(")[1]; //взять все после "("
        address.chop(1); //удалить ")"

        std::string add(address.toStdString());
        for (int i = 0; i < mail_wallet_pairs.size(); i++){
            if (add == mail_wallet_pairs[i].value){
                return i;
            }
        }
        return -1;
    }
    else {
        std::string ml(mail_str.toStdString());
        for (int i = 0; i < mail_wallet_pairs.size(); i++){
            if (ml == mail_wallet_pairs[i].name){
                return i;
            }
        }
        return -1;
    }
}

bool vedaEMailClient::isEmailValidAddress(std::string str)
{
    QRegExp EmailRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
    return QString::fromStdString(str).contains(EmailRegExp);
}

QString vedaEMailClient::get_WID_from_begin(int *index)
{
    std::string temp;

    QString login_string = begin_form->ui->login_lineEdit->text();
    if (login_string.contains("(")){//логин содержит WID в скобках
        login_string.chop(1);
        temp = login_string.split("(")[1].toLocal8Bit().data();

        if (nullptr != index){
            for (int i = 0;i < iniWrapper->datas.size(); i++) {
                if (temp == iniWrapper->datas[i].value){
                    *index = i;
                    return QString::fromStdString(temp);
                }
            }
        }
    }
    else {//логин содержит только mail адрес
        temp = login_string.toLocal8Bit().data();
        int i = 0;
        for (; i < iniWrapper->datas.size(); i++) {
            if (temp == iniWrapper->datas[i].name){
                if (nullptr != index) {
                    *index = i;
                    break;
                }
            }
        }
        if (i == iniWrapper->datas.size()){
            //не было найдено совпадения
            ///\todo возможно, стоит попробовать обратиться к серверу чтобы получить адрес
            return "";
        }
        else {
            return QString::fromStdString(iniWrapper->datas[i].value);
        }
    }
}

void vedaEMailClient::clear_dir_files_list(QStringList &list)
{
    for (int i = 0; i < list.size();){
        if (DEFAULT_WID_SIZE != list[i].size()){
            list.removeAt(i);
        }
        else i++;
    }
}

char *vedaEMailClient::get_token_buffer_by_vid(const string &vid)
{
    char *vid_ch = NULL;
    int vid_ch_len = vid.size() + 1;
    vid_ch = (char *)malloc(vid_ch_len);
    if (!vid_ch){
        return NULL;
    }
    memset(vid_ch, 0x0, vid_ch_len);
    memcpy(vid_ch, vid.c_str(), vid_ch_len - 1);

    char *token_ch = NULL;
    int token_ch_len = 0;
    if (LIBVEDAWALLET_NOERROR != veda_wallet_get_token_json_by_id(wstk,
                                                                  vid_ch,
                                                                  &token_ch,
                                                                  token_ch_len)){
        qDebug()<<"cannot get token info";
        free(vid_ch);
        return NULL;
    }
    else {
        free(vid_ch);
        return token_ch;
    }
}

json vedaEMailClient::get_token_json_by_vid(const string &vid)
{
    char *token_ch = get_token_buffer_by_vid(vid);
    if (NULL == token_ch){
        return json();
    }
    else {
        json j = json::parse(std::string(token_ch));
        free(token_ch);
        return j;
    }
}

char *vedaEMailClient::form_token_mas_by_index(int index, int *len)
{
    if (j_tokens.empty()){
        return NULL;
    }

    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    std::advance(it, index);
    std::string vid = it.key();

    json j = get_token_json_by_vid(vid);
    if (j.empty()){
        qDebug()<<"Get token failure";
    }

    json j_buf;
    j_buf[vid][JSON_TRN_TOKEN_IN_NOMINAL] = j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_NOMINAL];
    j_buf[vid][JSON_TRN_TOKEN_IN_TID_TRN] = j[JSON_TOKEN_USER][JSON_TOKEN_USER_TID];
    j_buf[vid][JSON_TRN_TOKEN_IN_HASH_TRN] = j[JSON_TOKEN_USER][JSON_TOKEN_USER_TID_HASH];

    vid = j_buf.dump();
    return std_string_to_char_array(vid, len);
}

char *vedaEMailClient::get_vid_by_index(int index)
{
    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    std::advance(it, index);
    return std_string_to_char_array(it.key());
}

bool vedaEMailClient::get_wid_by_email_locally(QString email)
{
    std::vector<VedaIniEntry> list;
    if (0 == iniWrapper->getAllEntrysByName(INI_FILE_ADDRESS_BOOK_SECTION, list)){
        return false;
    }
    else {
        std::string emailString = email.toLocal8Bit().data();
        for (int i = 9; i < list.size(); i++){ //тут опечатка. исправить, когда отладишь сервер
            if (emailString == list[i].name){
                recipient_wid = QString::fromStdString(list[i].value);
                return true;
            }
        }
        return false;
    }
}

void vedaEMailClient::finish_sending_message(bool withRecipientWID)
{
    //остальное относится к случаю - если там адрес wid

    //изменить COLOR_NAME
    char *vid = get_vid_by_index(clear_letters_indexes[0]);
    if (NULL == vid){
        qDebug()<<"Low memory";
        return;
    }

//    char *my_wid = QString_to_char_array(current_wid);
//    if (NULL == my_wid){
//        free(vid);
//        qDebug()<<"Low memory";
//        return;
//    }
    //для того, чтобы адресат расшифровал, надо передать его WID
    char *WIDr = NULL;
    if (!withRecipientWID){
     WIDr = QString_to_char_array(new_letter_form->ui->recipient_lineEdit->text());
    }
    else {
        WIDr = QString_to_char_array(recipient_wid);
    }
    if (NULL == WIDr){
        qDebug()<<"Low memory";
        return;
    }


    int color_name_len = 0;
    char *color_name = QString_to_char_array(new_letter_form->ui->message_textEdit->toPlainText(), &color_name_len);
    if (NULL == color_name){
        free(WIDr);
        free(vid);
        qDebug()<<"Low memory";
        return;
    }

    if (LIBVEDAWALLET_NOERROR != veda_wallet_edit_marked_token_by_id(wstk,
                                                                     vid,
                                                                     WIDr,
                                                                     color_name,
                                                                     color_name_len)){
        free(color_name);
        free(WIDr);
        free(vid);
        qDebug()<<"Error editing letter text";
        return;
    }
    else {
        free(color_name);
//        free(WIDr);
        free(vid);
    }





    //отправить
    int token_mas_len = 0;
    char *token_mas = form_token_mas_by_index(clear_letters_indexes[0], &token_mas_len);
    if (NULL == token_mas){
        free(WIDr);
        qDebug()<<"Low memory";
        return;
    }

    int com_mas_len = 0;
    char *com_mas = form_token_mas_by_index(tokens_indexes[0], &com_mas_len);
    if (NULL == com_mas){
        free(token_mas);
        free(WIDr);
        qDebug()<<"Low memory";
        return;
    }

    emit server_send_tokens(wstk,
                            TRN_TRANSFER, ///\todo определить на свой ли кошелек идет отправка
                            WIDr,
                            token_mas,
                            token_mas_len,
                            com_mas,
                            com_mas_len);

    toggle_loading_screen();
}

void vedaEMailClient::begin_form_signIn_ButtonClicked()
{
    char *curnode = QString_to_char_array(current_node);
    if (NULL == curnode){
        qDebug()<<"low memory";
        return;
    }

    current_wid = get_WID_from_begin(&current_opened_wallet);
    if (current_wid.isEmpty()){
        free(curnode);
        qDebug()<<"cannot recieve wallet address";
        return;
    }

    int cryptobox_name_len;
    char *cryptobox_name = QString_to_char_array(current_wid, &cryptobox_name_len);
    if (NULL == cryptobox_name){
        free(curnode);
        qDebug()<<"low memory";
        return;
    }

    int pass_len;
    char *pass = QString_to_char_array(begin_form->ui->password_lineEdit->text(), &pass_len);
    if (NULL == pass){
        free(cryptobox_name);
        free(curnode);
        qDebug()<<"low memory";
        return;
    }

    int secret_len;
    char *secret = QString_to_char_array(begin_form->ui->secret_lineEdit->text(), &secret_len);
    if (NULL == secret){
        free(pass);
        free(cryptobox_name);
        free(curnode);
        qDebug()<<"low memory";
        return;
    }

    toggle_loading_screen();

    emit server_auth(engine,
                     PATH_TO_WALLETS_DIR,
                     curnode,
                     cryptobox_name,
                     cryptobox_name_len,
                     pass,
                     pass_len,
                     secret,
                     secret_len);

}

void vedaEMailClient::begin_form_register_ButtonClicked()
{
    begin_form->hide();
    registration_form->show();

    registration_form->ui->city_lineEdit->setText("w");
    registration_form->ui->mail_lineEdit->setText("w");
    registration_form->ui->state_lineEdit->setText("w");
    registration_form->ui->secret_lineEdit->setText("12345678");
    registration_form->ui->first_name_lineEdit->setText("w");
    registration_form->ui->last_name_lineEdit->setText("w");
    registration_form->ui->password_lineEdit->setText("1234567890123456");
}

void vedaEMailClient::registration_form_sign_up_ButtonClicked()
{
    char *curnode = QString_to_char_array(current_node);

    int pass_len;
    char *pass = QString_to_char_array(registration_form->ui->password_lineEdit->text(), &pass_len);
    if (NULL == pass){
        qDebug()<<"low memory";
        return;
    }

    int secret_len;
    char *secret = QString_to_char_array(registration_form->ui->secret_lineEdit->text(), &secret_len);
    if (NULL == secret){
        free(pass);
        qDebug()<<"low memory";
        return;
    }

    int cert_len;
    char *certificate = make_certificate(&cert_len);
    if (NULL == secret){
        free(pass);
        free(secret);
        qDebug()<<"low memory";
        return;
    }

    emit server_reg_1(engine,
                      PATH_TO_WALLETS_DIR,
                      curnode,
                      pass,
                      pass_len,
                      secret,
                      secret_len,
                      certificate,
                      cert_len);

    toggle_loading_screen();
}

void vedaEMailClient::registration_form_back_ButtonClicked()
{
    registration_form->hide();
    begin_form->show();
}

void vedaEMailClient::on_logOut_Button_clicked()
{
    veda_wallet_free(wstk);

    main_menu_form->hide();
    show_begin();
}

void vedaEMailClient::main_menu_form_new_ButtonClicked()
{
//    new_letter_form->ui->send_Button->setDisabled(true);
    switch_main_menu_content(new_letter_form);
}

void vedaEMailClient::main_menu_form_incoming_ButtonClicked()
{
    switch_main_menu_content(incoming_message_list_form);
}

void vedaEMailClient::main_menu_form_outcoming_ButtonClicked()
{
    switch_main_menu_content(outcoming_message_list_form);
}

void vedaEMailClient::message_list_itemClicked()
{
    message_list_item *temp = (message_list_item*)sender();

    //0. Вписать автора
    read_letter_form->ui->from_lineEdit->setText(temp->get_author());

    //1. Найти нужный токен и запросить его инфо
    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    int index_needed = incoming_letters_indexes[temp->get_index()];
    read_letter_form->set_index(index_needed);
    std::advance(it, index_needed);

//    char *vid_ch = std_string_to_char_array(it.key());
//    if (NULL == vid_ch){
//        qDebug()<<"Low memory";
//        return;
//    }

//    char *token_ch = NULL;
//    int token_ch_len = 0;
//    if (LIBVEDAWALLET_NOERROR != veda_wallet_get_token_json_by_id(wstk,
//                                                                  vid_ch,
//                                                                  &token_ch,
//                                                                  token_ch_len)){
//        qDebug()<<"cannot get token info";
//        free(vid_ch);
//        return;
//    }

//    json j = json::parse(std::string(token_ch, token_ch_len));
//    free(token_ch);
//    if (j.empty()){
//        qDebug()<<"parse json failure";
//        return;
//    }
    json j = get_token_json_by_vid(it.key());
    if (j.empty()){
        qDebug()<<"parse json failure";
        return;
    }

    //2. Взять поле с текстом
    try {
        read_letter_form->ui->message_textEdit->setText(
                    QString::fromStdString(
                        j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_COLOR_NAME].get<std::string>()
                        ));
    } catch (...) {
        qDebug()<<"taking data from token exception";
    }

    read_letter_form->ui->decrypt_Button->setEnabled(true);
    switch_main_menu_content(read_letter_form);
}

void vedaEMailClient::on_settings_Button_clicked()
{
//    if (ui->settings_Button->isChecked()){
        main_menu_form->hide();
        toggle_main_menu_upper_buttons(false);
        settings_form->show();
        switch_settings_content(settings_information_form);
//    }
//    else {
//        settings_form->hide();
//        main_menu_form->show();
//        toggle_main_menu_upper_buttons(true);
        //    }
}

void vedaEMailClient::settings_form_back_ButtonClicked()
{
    settings_form->hide();
    main_menu_form->show();
    toggle_main_menu_upper_buttons(true);
}

void vedaEMailClient::settings_form_account_ButtonClicked()
{
    switch_settings_content(settings_account_form);
}

void vedaEMailClient::settings_form_connection_ButtonClicked()
{
    switch_settings_content(settings_connection_form);
}

void vedaEMailClient::settings_form_general_ButtonClicked()
{
    switch_settings_content(settings_general_form);
}

void vedaEMailClient::settings_form_information_ButtonClicked()
{
    switch_settings_content(settings_information_form);
}

void vedaEMailClient::settings_form_security_ButtonClicked()
{
    switch_settings_content(settings_security_form);
}

void vedaEMailClient::on_balance_Button_clicked()
{
    main_menu_form->hide();
    toggle_main_menu_upper_buttons(false);
    balance_form->show();
}

void vedaEMailClient::balance_form_back_ButtonClicked()
{
    balance_form->hide();
    main_menu_form->show();
    toggle_main_menu_upper_buttons(true);
}

void vedaEMailClient::read_letter_form_decrypt_ButtonClicked()
{
    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    std::advance(it, read_letter_form->get_index());
//    char *vid_ch = QString_to_char_array(
//                QString::fromStdString(
//                    it.key()
//                ));
//    if (NULL == vid_ch){
//        qDebug()<<"Low memory";
//        return;
//    }

    char *token_j_buf = NULL;
    int token_j_buf_len = 0;
//    if (LIBVEDAWALLET_NOERROR != veda_wallet_get_token_json_by_id(wstk,
//                                                                  vid_ch,
//                                                                  &token_j_buf,
//                                                                  token_j_buf_len)){
//        free(vid_ch);
//        qDebug()<<"get token failure";
//        return;
//    }
//    free(vid_ch);
    token_j_buf = get_token_buffer_by_vid(it.key());
    if (NULL == token_j_buf){
        qDebug()<<"get token failure";
        return;
    }

    char *decrypted = NULL;
    int decrypted_len = 0;
    if (LIBVEDAWALLET_NOERROR != veda_wallet_decrypt_marked_token(wstk,
                                                                  token_j_buf,
                                                                  &decrypted,
                                                                  decrypted_len)){
        qDebug()<<"decrypt failure";
        free(token_j_buf);
    }
    else {
        read_letter_form->ui->message_textEdit->setText(decrypted);
        free(token_j_buf);
        free(decrypted);
    }

    read_letter_form->ui->decrypt_Button->setDisabled(true);
}

void vedaEMailClient::new_letter_form_send_ButtonClicked()
{
    //если там мыло
    if (isEmailValidAddress(new_letter_form->ui->recipient_lineEdit->text().toLocal8Bit().data())){
        if (!get_wid_by_email_locally(new_letter_form->ui->recipient_lineEdit->text())){
            char *email = QString_to_char_array(new_letter_form->ui->recipient_lineEdit->text());
            emit server_get_wid_list_by_email(wstk,
                                              email);
            toggle_loading_screen();
        }
        else {
            json j = json::parse(std::string(recipient_wid.toLocal8Bit().data()));
            server_from_get_wid_list_by_email(j);
        }

        return;
    }

    finish_sending_message();
}

void vedaEMailClient::doubleClicked()
{
    if (begin_form->isVisible()) begin_form->show_all_completions();
    else if (new_letter_form->isVisible()) new_letter_form->show_all_completions();
}

void vedaEMailClient::error_timeout()
{
    _error_label->hide();
}

void vedaEMailClient::begin_form_login_lineEdit_textChanged()
{
    ///\todo сделать подгон текста под размер если не будет сделано наоборот
}

void vedaEMailClient::new_letter_form_recipient_lineEdit_editingFinished()
{
//    if (!get_wid_by_email_locally(new_letter_form->ui->recipient_lineEdit->text())){
//        char *email = QString_to_char_array(new_letter_form->ui->recipient_lineEdit->text());
//        emit server_get_wid_list_by_email(wstk,
//                                          email);
//    }
}

void vedaEMailClient::email_by_wid_getter(const string wid, const int index)
{
    QString ret;

    //1. Найти локально
    std::vector<VedaIniEntry> local;
    if (0 != iniWrapper->getAllEntrysByName(INI_FILE_ADDRESS_BOOK_SECTION, local)){
        for (int i = 0; i < local.size(); i++){
            if (wid == local[i].value){
                emit got_email_locally(QString::fromStdString(local[i].name), index);
                return;
//                return QString::fromStdString(iniWrapper->datas[i].name);
            }
        }
    }

    //2. Если локальный поиск успеха не дал, обратиться к серверу
    char *wid_ch = std_string_to_char_array(wid);
    if (NULL == wid_ch){
//        return ret;
        emit got_email_locally(QString(), index);
        return;
    }

    toggle_loading_screen();
    emit server_get_email(wstk, wid_ch, index);

//    char *email_ch = NULL;
//    int email_ch_len = 0;
//    if (LIBVEDAWALLET_NOERROR != veda_wallet_get_email(wstk,
//                                                       wid_ch,
//                                                       &email_ch,
//                                                       email_ch_len)){
//        free(wid_ch);
////        return ret;
//    }
//    free(wid_ch);
//    ret = email_ch;

    //3. добавить в адресную книгу
//    std::string section_name_string = INI_FILE_ADDRESS_BOOK_SECTION;
//    for (int i = 0; i < iniWrapper->datas.size(); i++){
//        if (iniWrapper->datas[i].index != section_name_string) continue;


//    }

//    return ret;
}

void vedaEMailClient::email_by_wid_placer(QString email, const int index, bool from_server)
{
    if (index >= incoming_message_list.size()) return;
    std::string wid = incoming_message_list[index]->get_wid();
    incoming_message_list[index]->set_author(email);

    if (!from_server) return;

    //Если пришло от сервера, добавить в адресную книгу в соответствующее алфавитному порядку место
    toggle_loading_screen();
    std::string email_string = email.toLocal8Bit().data();
    VedaIniEntry temp;
    temp.index = INI_FILE_ADDRESS_BOOK_SECTION;
    temp.name = email_string;
    temp.value = wid;
    int i = 0;
    for (; i < iniWrapper->datas.size(); i++){
        if (0 == strcmp(iniWrapper->datas[i].index.c_str(), INI_FILE_ADDRESS_BOOK_SECTION)){
            break;
        }
    }

    if (i == iniWrapper->datas.size()){
        //если этой секции в ini раньше не было
        iniWrapper->datas.push_back(temp);
    }
    else{//если секция есть
        for (; i < iniWrapper->datas.size(); i++) {
            if (iniWrapper->datas[i].name > email_string){
                auto it = iniWrapper->datas.begin();
                std::advance(it,i);
                iniWrapper->datas.insert(it,temp);
            }
            else{
                continue;
            }
        }
    }

    iniWrapper->writeConfigFile();
}

void vedaEMailClient::server_from_reg_1(int error, bool res, uint64_t _wstk)
{
    toggle_loading_screen();

    if (res){
        wstk = _wstk;

        char *code = QString_to_char_array("leha-kartoha");
        if (NULL == code){
            veda_wallet_free(wstk);
            qDebug()<<"low memory";
            return;
        }

        emit server_reg_2(code, strlen(code));

        toggle_loading_screen();
    }
    else {
        show_error_label(registration_form->ui->error_label, "error code: " + QString::number(error));
    }
}

void vedaEMailClient::server_from_reg_2(int error)
{
    if (LIBVEDAWALLET_NOERROR == error){
        registration_form->hide();
        begin_form->show();

        save_new_mail_wallet_pair(registration_form->ui->mail_lineEdit->text() +
                                  registration_form->ui->mail_postfix_lineEdit->text(),
                                  find_new_wid());
    }
    else {
        show_error_label(registration_form->ui->error_label, "error code: " + QString::number(error));
    }

    toggle_loading_screen();
}

void vedaEMailClient::server_from_auth(uint64_t _wstk, int error, char *token_mas, uint64_t token_mas_len, char *__balance, uint64_t _token_cnt, uint64_t marked_token_cnt)
{
    toggle_loading_screen();

    if (LIBVEDAWALLET_NOERROR == error){
        wstk = _wstk;
        if (NULL != token_mas){
            j_tokens = json::parse(std::string(token_mas, token_mas_len));
        }
        if (__balance) {
            balance = (uint64_t)atoll(__balance);
        tokens_count = _token_cnt;
        }
        letters_count = marked_token_cnt;





        //
//        ui->description_label->setText(begin_form->ui->login_lineEdit->text());
        //
        ui->description_label->setText("vedauser@vedanet.io");







        ui->description_label->show();
        ui->description_spacer_label->hide();

        fill_token_indexes_vectors();
        fill_incoming_message_list();
        show_current_balance_everywhere();

        toggle_main_menu_upper_buttons(true);
        main_menu_form->show();
        switch_main_menu_content(incoming_message_list_form);
        begin_form->hide();
    }
    else {
        show_error_label(begin_form->ui->error_label, "error code: " + QString::number(error));
    }
}

void vedaEMailClient::server_from_get_tokens(int res)
{
    toggle_loading_screen();

    if ((LIBVEDAWALLET_NOERROR == res) || ignore_get_tokens_on_refresh){
        ignore_get_tokens_on_refresh = false;
        bool need_to_refresh_balance = true;

        char *token_mas = NULL;
        uint64_t token_mas_len = 0;
        if (j_tokens.empty()){
            if (LIBVEDAWALLET_NOERROR != veda_wallet_form_token_mas(wstk,
                                                                    &token_mas,
                                                                    token_mas_len)){
                //баланса не было и нет
                need_to_refresh_balance = false;
            }
        }
        else {//баланс был
            std::string token_string = j_tokens.dump();
            token_mas_len = token_string.size();
            token_mas = (char *)malloc(token_mas_len + 1);
            if ((NULL == token_mas) && (!token_string.empty())){
                return;
            }
            memset(token_mas, 0x0, token_mas_len + 1);
            memcpy(token_mas, token_string.c_str(), token_mas_len);
        }

        if (need_to_refresh_balance){
            char *__balance = NULL;
            char *token_mas_out = NULL;
            uint64_t token_mas_len_out = 0;
            int res = LIBVEDAWALLET_NOERROR;
            if (LIBVEDAWALLET_NOERROR != (res = veda_wallet_refresh_balance(wstk,
                                                                            token_mas,
                                                                            token_mas_len,
                                                                            &token_mas_out,
                                                                            token_mas_len_out,
                                                                            &__balance,
                                                                            tokens_count,
                                                                            letters_count))){
                j_tokens.clear();
                incoming_letters_indexes.clear();
                tokens_indexes.clear();
                tokens_count = 0;
                letters_count = 0;
                balance = 0;
            }
            else {
                if (0 == token_mas_len_out){
                    token_mas_out = token_mas;
                    token_mas_len_out = token_mas_len;
                }
                else if (token_mas) free(token_mas);
            }

            balance = (uint64_t)atoll(__balance);
            free(__balance);

            j_tokens = json::parse(std::string(token_mas_out, token_mas_len_out));
            free(token_mas_out);

            fill_token_indexes_vectors();
            fill_incoming_message_list();
            show_current_balance_everywhere();
        }
    }
}

void vedaEMailClient::server_from_send_tokens(int res)
{
    ///\todo возможно форма результата, или форма результата только в случае, если произошла ошибка
    /// такую форму надо сделать по аналогии с экраном загрузки скрывающую все остальное и только
    /// с кнопкой "ОК"

    toggle_loading_screen();
    ignore_get_tokens_on_refresh = true;
    ui->refresh_Button->click();
    switch_main_menu_content(incoming_message_list_form);
}

void vedaEMailClient::server_from_get_wid_list_by_email(json WIDs, int res, bool from_server)
{
    if (from_server){
        toggle_loading_screen();
    }
    if (LIBVEDAWALLET_NOERROR == res){
        if (!WIDs.empty()){
            try {
                recipient_wid = QString::fromStdString(WIDs.begin().value().get<std::string>()); ///\todo обработать случай нескольких почт на одно мыло
            } catch (...) {
                qDebug()<<"error parsing json";
                return;
            }

            //теперь надо продолжить отправку сообщения
            finish_sending_message(true);
        }
    }
    else {
        ///\todo show_error_label
    }
}

void vedaEMailClient::on_refresh_Button_clicked()
{
    toggle_loading_screen();
    emit server_get_tokens(wstk);
}

bool doubleClickFilter::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonDblClick){
        emit doubleClick();
    }

    return QObject::eventFilter(obj, ev);
}
