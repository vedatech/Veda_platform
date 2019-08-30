#include "vedaEMailClient.h"
#include "ui_vedaEMailClient.h"

vedaEMailClient::vedaEMailClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vedaEMailClient)
{
    ui->setupUi(this);
    setWindowTitle("VEDA EMail client");
    setWindowIcon(QIcon(":/Resources/veda-icon-512.png"));

//#ifdef MACOS
//    setWindowIcon(QIcon(":/Resources/ico-coin.png"));
//#else
//#endif

    if (LIBVEDAWALLET_NOERROR != veda_wallet_init_library(engine)){
        exit(-1);
    }

    mainWindow = new (std::nothrow) MainWindow(this);
    if (nullptr == mainWindow) exit(-1);
    ui->AppLayout->addWidget(mainWindow);

    connect_widgets();

    init_server_thread();

    connect_server_thread();

    //QCoreApplication::applicationDirPath();
//    QDir::setCurrent(executable_path);
    read_ini_file();

    error_timer = new (std::nothrow) QTimer(this);
    error_timer->setSingleShot(true);
    error_timer->setInterval(ERROR_INTERVAL);
    connect(error_timer, SIGNAL(timeout()),
            this, SLOT(error_timeout()));

    autorenew_timer = new (std::nothrow) QTimer(this);
    autorenew_timer->setSingleShot(true);
    connect(autorenew_timer, SIGNAL(timeout()),
            this, SLOT(on_refresh_Button_clicked()));

    autoLogOutTimer = new (std::nothrow) QTimer(this);
    autoLogOutTimer->setSingleShot(true);
    connect(autoLogOutTimer, SIGNAL(timeout()),
            this, SLOT(autoLogOut()));
}

vedaEMailClient::~vedaEMailClient()
{
//    saveReadMessageList();

    delete ui;

    delete server;
    server_thread->exit();
    server_thread->deleteLater();

    if (0 != wstk) veda_wallet_free(wstk);
    veda_wallet_stop_library(engine);
}

void vedaEMailClient::connect_widgets()
{
    qRegisterMetaType<GetMailByWIDType>("GetMailByWIDType");

    connect(this, SIGNAL( get_email_by_wid(const std::string, const int, GetMailByWIDType)),
            this, SLOT(email_by_wid_getter(const std::string, const int, GetMailByWIDType)));

    connect(this, SIGNAL(got_email_locally(QString, const int, GetMailByWIDType, bool)),
            this, SLOT(email_by_wid_placer(QString, const int, GetMailByWIDType, bool)));

    connect(this, SIGNAL(allIncomingEmailsPlaced()),
            this, SLOT(recipientCompleterRefresher()));
    connect(this, SIGNAL(allOutcomingEmailsPlaced()),
            this, SLOT(recipientCompleterRefresher()));



    connect(mainWindow->ui->register_Button, SIGNAL(clicked()),
            this, SLOT(begin_form_register_ButtonClicked()));

    connect(mainWindow->ui->back_Button_3, SIGNAL(clicked()),
            this, SLOT(registration_form_back_ButtonClicked()));

    connect(mainWindow->ui->logOut_Button, SIGNAL(clicked()),
            this, SLOT(on_logOut_Button_clicked()));

    connect(mainWindow->ui->new_Button, SIGNAL(clicked()),
            this, SLOT(main_menu_form_new_ButtonClicked()));

    connect(mainWindow->ui->incoming_Button, SIGNAL(clicked()),
            this, SLOT(main_menu_form_incoming_ButtonClicked()));

    connect(mainWindow->ui->outcoming_Button, SIGNAL(clicked()),
            this, SLOT(main_menu_form_outcoming_ButtonClicked()));

    connect(mainWindow->ui->settings_Button, SIGNAL(clicked()),
            this, SLOT(on_settings_Button_clicked()));

    connect(mainWindow->ui->back_Button_4, SIGNAL(clicked()),
            this, SLOT(settings_form_back_ButtonClicked()));

    connect(mainWindow->ui->balance_Button, SIGNAL(clicked()),
            this, SLOT(on_balance_Button_clicked()));

    connect(mainWindow->ui->back_Button, SIGNAL(clicked()),
            this, SLOT(balance_form_back_ButtonClicked()));

    connect(mainWindow->ui->sign_up_Button, SIGNAL(clicked()),
            this, SLOT(registration_form_sign_up_ButtonClicked()));

    connect(mainWindow->ui->signIn_Button, SIGNAL(clicked()),
            this, SLOT(begin_form_signIn_ButtonClicked()));

    mainWindow->ui->login_lineEdit->installEventFilter(&dblClickFilter);
    connect(&dblClickFilter, SIGNAL(doubleClick()),
            this, SLOT(doubleClicked()));

    connect(mainWindow->ui->login_lineEdit, SIGNAL(textChanged(const QString&)),
                        this, SLOT(begin_form_login_lineEdit_textChanged(const QString&)));

    connect(mainWindow->ui->GetMoreMessagesButton, SIGNAL(clicked()),
            this, SLOT(balance_form_getMoreMessages_ButtonClicked()));

    connect(mainWindow->ui->decryptButton, SIGNAL(clicked()),
            this, SLOT(read_letter_form_decrypt_ButtonClicked()));

    connect(mainWindow->ui->button1Page8, SIGNAL(clicked()),
            this, SLOT(read_letter_form_reply_ButtonClicked()));

    connect(mainWindow->ui->button0Page6, SIGNAL(clicked()),
            this, SLOT(new_letter_form_send_ButtonClicked()));

    mainWindow->ui->lineEditPage6->installEventFilter(&dblClickFilter);

    //button4-7Page3
    connect(mainWindow->ui->button4Page3, SIGNAL(clicked()),
            this, SLOT(incoming_message_list_form_first_ButtonClicked()));

    connect(mainWindow->ui->button5Page3, SIGNAL(clicked()),
            this, SLOT(incoming_message_list_form_prev_ButtonClicked()));

    connect(mainWindow->ui->button6Page3, SIGNAL(clicked()),
            this, SLOT(incoming_message_list_form_next_ButtonClicked()));

    connect(mainWindow->ui->button7Page3, SIGNAL(clicked()),
            this, SLOT(incoming_message_list_form_last_ButtonClicked()));

    connect(mainWindow->ui->button0Page10, SIGNAL(clicked()),
            this, SLOT(autoLogOutSettingsChanged()));
    connect(mainWindow->ui->button1Page10, SIGNAL(clicked()),
            this, SLOT(autoLogOutSettingsChanged()));
    connect(mainWindow->ui->button2Page10, SIGNAL(clicked()),
            this, SLOT(autoLogOutSettingsChanged()));
    connect(mainWindow->ui->button3Page10, SIGNAL(clicked()),
            this, SLOT(autoLogOutSettingsChanged()));

    connect(mainWindow->ui->refresh_Button, SIGNAL(clicked()),
            this, SLOT(on_refresh_Button_clicked()));

    connect(mainWindow->ui->newLetterAttachButton, SIGNAL(clicked()),
            this, SLOT(newLetterAttachButtonClicked()));

    connect(mainWindow->ui->button2Page8, SIGNAL(clicked()),
            this, SLOT(readMessageFormDeleteButtonClicked()));

//    connect(mainWindow->ui->button0Page8, SIGNAL(clicked()),
//            this, SLOT(fileToSaveChoosen()));

    connect(mainWindow->ui->reg2ConfirmButton, SIGNAL(clicked()),
            this, SLOT(reg2ConfirmButtonClicked()));

    connect(mainWindow->ui->reg2BackButton, SIGNAL(clicked()),
            this, SLOT(reg2BackButtonClicked()));

    connect(mainWindow->ui->setPreferredNodeButton, SIGNAL(clicked()),
            this, SLOT(setPreferredNodeButtonClicked()));

    connect(mainWindow->ui->beginSettingsButton, SIGNAL(clicked()),
            this, SLOT(beginSettingsButtonClicked()));
}

void vedaEMailClient::toggle_loading_screen(int index, bool on)
{
    mainWindow->toggleLoadingScreen(index, on);
}

void vedaEMailClient::updateUnknownWalletsEverywhere()
{
    //1. Подсказки вначале
    fillBeginFormCompleter();

    //2. Почта во внутреннюю переменную и на description label
    fillDescriptionLabelMail();
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

    connect(this, SIGNAL(server_reg_1(uint64_t, char *, char *, char *, int, char *, int, char *, int, char *)),
            server, SLOT(       reg_1(uint64_t, char *, char *, char *, int, char *, int, char *, int, char *)));
    connect(server, SIGNAL(    reg_1_res(int, bool, uint64_t)),
            this, SLOT(server_from_reg_1(int, bool, uint64_t)));



    connect(this, SIGNAL(server_reg_2(char *, int)),
            server, SLOT(reg_2(char *, int)));
    connect(server, SIGNAL(reg_2_res(int)),
            this, SLOT(server_from_reg_2(int)));



    connect(this, SIGNAL(server_auth(uint64_t, char *, char *, char *, int, char *, int, char *, int)),
            server, SLOT(       auth(uint64_t, char *, char *, char *, int, char *, int, char *, int)));
    connect(server, SIGNAL(    auth_res(uint64_t, int, char *, uint64_t, char *, uint64_t, uint64_t, char *, int)),
            this, SLOT(server_from_auth(uint64_t, int, char *, uint64_t, char *, uint64_t, uint64_t, char *, int)));



    connect(this, SIGNAL(server_get_tokens(uint64_t, bool)),
            server, SLOT(get_tokens(uint64_t, bool)));
    connect(server, SIGNAL(get_tokens_res(int, char *)),
            this, SLOT(server_from_get_tokens(int, char *)));



    connect(this, SIGNAL(server_send_tokens(uint64_t, int, char *, char *, uint64_t, char *, uint64_t, char *, uint64_t)),
            server, SLOT(       send_tokens(uint64_t, int, char *, char *, uint64_t, char *, uint64_t, char *, uint64_t)));
    connect(server, SIGNAL(    send_tokens_res(int)),
            this, SLOT(server_from_send_tokens(int)));



    connect(this, SIGNAL(server_get_email(uint64_t, char *, const int, GetMailByWIDType)),
            server, SLOT(       get_email(uint64_t, char *, const int, GetMailByWIDType)));
    connect(server, SIGNAL(  get_email_res(QString, const int, GetMailByWIDType, bool)),
            this, SLOT(email_by_wid_placer(QString, const int, GetMailByWIDType, bool)));
    connect(this, SIGNAL(decrementEmailsGotByWIDsCount()),
            this, SLOT(emailByWIDIniFileDecrementer()));



    connect(this, SIGNAL(server_get_wid_list_by_email(uint64_t, char *)),
            server, SLOT(       get_wid_list_by_email(uint64_t, char *)));
    connect(server, SIGNAL(    get_wid_list_by_email_res(json, int)),
            this, SLOT(server_from_get_wid_list_by_email(json, int)));



    connect(this, SIGNAL(server_create_empty_messages(uint64_t, char *, int, char *, char *, int)),
            server, SLOT(create_empty_messages(uint64_t, char *, int, char *, char *, int)));
    connect(server, SIGNAL(create_empty_messages_res(int)),
            this, SLOT(server_from_create_empty_messages(int)));



    connect(this, SIGNAL(server_nulify_wstk()), server, SLOT(nulify_wstk()));
}

void vedaEMailClient::read_ini_file()
{
    if (readIniFileFirstTime){
        executable_path = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];
//        qDebug()<<"work dir path is " << executable_path;
        if (!dir.cd(executable_path)){
            dir.mkdir(executable_path);
            dir.cd(executable_path);
        }
        if (!dir.cd(PATH_TO_VERSIONS_FOLDER)){
            dir.mkdir(PATH_TO_VERSIONS_FOLDER);
            dir.cd(PATH_TO_VERSIONS_FOLDER);
        }
        if (!dir.cd(PATH_TO_CURRENT_VERSION_FOLDER)){
            dir.mkdir(PATH_TO_CURRENT_VERSION_FOLDER);
            dir.cd(PATH_TO_CURRENT_VERSION_FOLDER);
        }
        if (!dir.cd(PATH_TO_BIN)){
            dir.mkdir(PATH_TO_BIN);
            dir.cd(PATH_TO_BIN);
        }
        executable_path = dir.absolutePath();
        dir.setCurrent(executable_path);

        if (!dir.cd(PATH_TO_ETC)){
            dir.mkdir(PATH_TO_ETC);
        }
        dir.cd(executable_path);
        if (!dir.cd(PATH_TO_WALLETS_DIR)){
            dir.mkdir(PATH_TO_WALLETS_DIR);
        }
        dir.cd(executable_path);
        readIniFileFirstTime = false;
    }

    if (nullptr == iniWrapper){
        iniWrapper = new (std::nothrow) VedaIniWrapper(PATH_TO_INI_FILE, true);
    }

    verifyIniFile();

    read_wallets();

    read_address_book(true);

    read_current_node();

    readAutoLogOutSettings();

//    read_start_money();

    ///\todo отсортировать адресную книгу, а также проверить ее содержимое на хуйню
}

void vedaEMailClient::verifyIniFile()
{
    sortIniFile();

    removeIniDuplicates();

    ///\todo другие необходимые проверки


    iniWrapper->writeConfigFile();
}

void vedaEMailClient::sortIniFile()
{
    QVector<std::string> sections;
    sections << INI_FILE_CURRENT_NODE_SECTION;
    sections << INI_FILE_AUTO_LOG_OUT_SECTION;
    sections << INI_FILE_ADDRESS_BOOK_SECTION;
    sections << INI_FILE_WALLETS_SECTION;

    QVector<uint64_t> section_indexes;
    for (int sectCount = 0, sectLimit = sections.size(); sectCount < sectLimit; sectCount++){

        //1. Узнать, требуется ли сортировка
        bool needToSort = false;
        section_indexes.clear();
        for (uint64_t i = 0, limit = iniWrapper->datas.size(); i < limit; i++) {
            if (iniWrapper->datas[i].index == sections[sectCount]){
                section_indexes << i;
            }
        }

        if (1 == section_indexes.size() || 0 == section_indexes.size()) continue;

        for (int i = 0, limit = section_indexes.size() - 1; i < limit; i++) {
            if ((section_indexes[i + 1] - section_indexes[i]) > 1){
                needToSort = true;
                break;
            }
        }

        if (!needToSort) continue;
        else {
            //2. Если сортировка требуется, отсортировать по месту первого нахождения элемента этой секции в инишнике
            VedaIniEntry temp;
            auto itBegin = iniWrapper->datas.begin();
            for (int i = 1, limit = section_indexes.size(); i < limit; i++) {

                if ((section_indexes[i] - section_indexes[i - 1]) > 1){ //место, в котором разрыв секции
                    temp.clear();
                    temp = iniWrapper->datas[section_indexes[i]];
                    iniWrapper->datas.erase(itBegin + section_indexes[i]);
                    iniWrapper->datas.insert(itBegin + section_indexes[i - 1] + 1, temp);
                    section_indexes[i] = section_indexes[i - 1] + 1;
                }
            }
        }
    }
}

void vedaEMailClient::removeIniDuplicates()
{
    if (!iniWrapper->datas.empty()){
        std::string name, value;

        //секции считаются отсортированными, так что при смене секции цикл должен завершаться
        std::string sectionName;

        for (int i = 0, limit = iniWrapper->datas.size(); i < limit; i++){
            name = iniWrapper->datas[i].name;
            value = iniWrapper->datas[i].value;
            sectionName = iniWrapper->datas[i].index;
            for (int j = i + 1; j < iniWrapper->datas.size();) {
                if (iniWrapper->datas[j].index != sectionName) break;

                if ((name == iniWrapper->datas[j].name) && (value == iniWrapper->datas[j].value)){
                    limit--;
                    iniWrapper->datas.erase(iniWrapper->datas.begin() + j);
                }
                else j++;

            }
        }
    }
}

void vedaEMailClient::read_wallets()
{
    //1. Считать данные ini - уже считаны при открытии ini файла

    //2. Считать реальные файлы в папке
    if (dir.cd(PATH_TO_WALLETS_DIR)){
        QStringList dirList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        clear_dir_files_list(dirList);
        dir.cd(executable_path);

        //3. Удалить из ini несуществующие
        //3.5. Также переименовать в unknown адреса, не соответствующие шаблону почты
        if (!iniWrapper->datas.empty()){
            bool found_first = false;
            for (int i = 0, limit = iniWrapper->datas.size(); i < limit;){

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

        //4. удалить дубликаты - сделано в другом месте

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
                if (iniWrapper->datas.empty()
                        ||
                    (insertPlace == iniWrapper->datas.size()) //если секции не было
                     )
                {
                    iniWrapper->datas.push_back(temp);
                    insertPlace++;
                }
                else {
                    iniWrapper->datas.insert(it++, temp);
                }
            }
        }
    }

    //6. Отсортировать ini по одинаковым Email'ам:
//    if (!iniWrapper->datas.empty()){
//        std::string mail;
//        int size = iniWrapper->datas.size();
//        int same_name_entries = 0;

//        for (int i = 0; i < size; i += (1 + same_name_entries)){
//            if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
//                mail = iniWrapper->datas[i].name;
//                same_name_entries = 0;
//                for (int j = i + 1; j < size; j++){
//                    if (0 != strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[j].index.c_str())) break;
//                    if (iniWrapper->datas[j].name == mail){
//                        if ((j - i) > ++same_name_entries){
//                            iniWrapper->datas.insert(iniWrapper->datas.begin() + i + 1 + same_name_entries,
//                                                     iniWrapper->datas[j]);
//                            iniWrapper->datas.erase(iniWrapper->datas.begin() + j + 1);
//                            j++;
//                        }
//                    }
//                }
//            }
//        }
//    }
    sortSection(std::string(INI_FILE_WALLETS_SECTION));

    //7. Перезаписать ini
    iniWrapper->writeConfigFile();

    //8. Заполнить completer
    fillBeginFormCompleter();
}

void vedaEMailClient::read_address_book(bool withEMailsVerification)
{
    //1. Считать ini - считано конструктором ini

    if (withEMailsVerification){
        //2. Удалить лажу
        if (!iniWrapper->datas.empty()){
            bool needToWrite = false;
            for (int i = 0;i < iniWrapper->datas.size(); i++){
                if (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str())){
                    //лажа - если имя - не email или соответствующий адрес - не адрес
                    if (!isEmailValidAddress(iniWrapper->datas[i].name) ||
                            !wallet_file_name_is_valid(QString::fromStdString(iniWrapper->datas[i].value))){
                        iniWrapper->datas.erase(iniWrapper->datas.begin() + i);
                        needToWrite = true;
                    }
                }
            }
            if (needToWrite){
                iniWrapper->writeConfigFile();
            }
        }
    }

    //отсортировать по одинаковым email'ам
    if (sortSection(std::string(INI_FILE_ADDRESS_BOOK_SECTION))){
        iniWrapper->writeConfigFile();
    }

    //доставить completer в создание сообщения
    recipientLineEditCompletions.clear();
    std::vector<VedaIniEntry> entries;
    if (0 < iniWrapper->getAllEntrysByName(INI_FILE_ADDRESS_BOOK_SECTION, entries)){
        std::string lastEMail;
        int toCompleterLastIndex = -1;
        for (uint64_t i = 0, limit = entries.size(); i < limit; i++) {
            if (entries[i].name == lastEMail){//найдена почта с более чем одним адресом
                recipientLineEditCompletions[toCompleterLastIndex++] = QString::fromStdString(
                                entries[i - 1].name + " (" + entries[i - 1].value + ")"
                            );
                recipientLineEditCompletions.push_back(QString::fromStdString(
                                entries[i].name + " (" + entries[i].value + ")"
                            ));
            }
            else {
                lastEMail = entries[i].name;
                recipientLineEditCompletions.push_back(QString::fromStdString(
                                           entries[i].name
                            ));
                toCompleterLastIndex++;
            }
        }
    }

    mainWindow->setCompleterOnRecipientLineEdit(recipientLineEditCompletions);
}

void vedaEMailClient::addWIDListToAddressBook(bool withVerification)
{
    int i = 0;
    int limit = iniWrapper->datas.size();

    if (withVerification){

        if (!iniWrapper->datas.empty()){

            bool section_found = false;
            for (;
                 (i < limit) && (!WIDsByMail.isEmpty());
                 i++) {
                if (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str())){
                    section_found = true;
                    //вставлять в inifile нужно только новые аккаунты
                    WIDsByMail.removeOne(iniWrapper->datas[i].value);
                }
                else if (section_found) {
                    break;
                }
            }

        }

        if (WIDsByMail.isEmpty()){
            //не нужно ничего вставлять
            return;
        }
    }

    //1. Найти секцию
    for (i = 0; i < limit; i++) {
        if (0 == strcmp(iniWrapper->datas[i].index.c_str(), INI_FILE_ADDRESS_BOOK_SECTION)){
            break;
        }
    }

    if (!withVerification) {
    //проверить тот случай, когда на самом деле нужная запись уже легла другим способом

    }

    //2. Вставить
    VedaIniEntry temp;
    temp.index = INI_FILE_ADDRESS_BOOK_SECTION;
    temp.name = mainWindow->ui->lineEditPage6->text().toLocal8Bit().data();
    if (i == limit){//секции не было
        for (int j = 0, toInsert = WIDsByMail.size(); j < toInsert; j++) {
            temp.value = WIDsByMail[j];
            iniWrapper->datas.push_back(temp);
        }
    }
    else {//секция была
        //0. Найти место внутри секции для вставки
        for (;
             (i < limit)
             &&
             (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str()));
             i++) {
            if (iniWrapper->datas[i].name < temp.name) continue;
            else break;
        }

        //1. Вставить
        auto it = iniWrapper->datas.begin();
        for (int j = 0, toInsert = WIDsByMail.size(); j < toInsert; j++) {
            temp.value = WIDsByMail[j];
            if (i == limit){
                iniWrapper->datas.push_back(temp);
            }
            else {
                iniWrapper->datas.insert(it + i++, temp);
            }
        }
    }

    iniWrapper->writeConfigFile();
}

void vedaEMailClient::read_current_node()
{
    std::vector<VedaIniEntry> vect;
    int res;
    if (1 != (res = iniWrapper->getAllEntrysByName(INI_FILE_CURRENT_NODE_SECTION, vect))){

        if (1 < res){
            bool ignore_first = true;
            for (int i = 0, limit = iniWrapper->datas.size(); i < limit; i++){
                if (0 == strcmp(INI_FILE_CURRENT_NODE_SECTION, iniWrapper->datas[i].index.c_str())){
                    if (!ignore_first){
                        iniWrapper->datas.erase(iniWrapper->datas.begin() + i--);
                    }
                    else {
                        ignore_first = false;
                        current_node = QString::fromStdString(vect[1].name);
                    }
                }
            }
        }
        else {
            current_node = INI_FILE_CURRENT_NODE_GERMAN_IP; //INI_FILE_CURRENT_NODE_LOCAL_IP

            VedaIniEntry temp;
            temp.index = INI_FILE_CURRENT_NODE_SECTION;
            temp.name = temp.value = INI_FILE_CURRENT_NODE_GERMAN_IP;

            iniWrapper->datas.push_back(temp);
        }
        iniWrapper->writeConfigFile();

    }
    else {
        current_node = QString::fromStdString(vect[0].name);
    }

    QStringList nums = current_node.split(".");
    if (nums.size() != 4) return;

    mainWindow->ui->preferredNodeLineEdit1->setText(nums[0]);
    mainWindow->ui->preferredNodeLineEdit2->setText(nums[1]);
    mainWindow->ui->preferredNodeLineEdit3->setText(nums[2]);
    mainWindow->ui->preferredNodeLineEdit4->setText(nums[3]);
}

void vedaEMailClient::readAutoLogOutSettings()
{
    if (!iniWrapper->datas.empty()){
        bool First = true;
        bool only = true;
        for (int i = 0, limit = iniWrapper->datas.size(); i < limit; i++) {
            if (0 == strcmp(INI_FILE_AUTO_LOG_OUT_SECTION, iniWrapper->datas[i].index.c_str())){
                if (First){
                    First = false;
                    autoLogOutTimerMinuteRange = QString::fromStdString(iniWrapper->datas[i].value).toInt();
                }
                else {
                    only = false;
                    iniWrapper->datas.erase(iniWrapper->datas.begin() + i--);
                    limit--;
                }
            }
            else if (!First) {
                break;
            }
        }
        if (First){
            autoLogOutTimerMinuteRange = 0;
        }
        if (!only){
            iniWrapper->writeConfigFile();
        }
    }
    else {
        autoLogOutTimerMinuteRange = 0;
    }

    if (0 == autoLogOutTimerMinuteRange){
        mainWindow->ui->button0Page10->setChecked(true);
    }
    else if (1 == autoLogOutTimerMinuteRange){
        mainWindow->ui->button1Page10->setChecked(true);
    }
    else if (2 == autoLogOutTimerMinuteRange){
        mainWindow->ui->button2Page10->setChecked(true);
    }
    else {
        mainWindow->ui->button3Page10->setChecked(true);
    }
}

void vedaEMailClient::saveAutoLogOutSettings()
{
    //считаем что запись одна, так как лишние были удалены при запуске программы
    //секции отсортированы

    //0. Найти место для вставки
    int insertIndex = 0;
    if (!iniWrapper->datas.empty()){
        for (int limit = iniWrapper->datas.size(); insertIndex < limit; insertIndex++) {
            if (0 == strcmp(INI_FILE_AUTO_LOG_OUT_SECTION, iniWrapper->datas[insertIndex].index.c_str())){
                break;
            }
        }
    }

    //1. Вставить
    if (insertIndex == iniWrapper->datas.size()){
        VedaIniEntry temp;
        temp.index = INI_FILE_AUTO_LOG_OUT_SECTION;
        temp.name = INI_FILE_AUTO_LOG_OUT_NAME_FIELD;
        temp.value = QString::number(autoLogOutTimerMinuteRange).toLocal8Bit().data();
        iniWrapper->datas.push_back(temp);
    }
    else {
        iniWrapper->datas[insertIndex].value = QString::number(autoLogOutTimerMinuteRange).toLocal8Bit().data();
    }
    iniWrapper->writeConfigFile();
}

void vedaEMailClient::fillBeginFormCompleter()
{
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

        mainWindow->setCompleterOnLoginLineEdit(completions);
    }
}

void vedaEMailClient::fillDescriptionLabelMail()
{
    std::string stdWID = current_wid.toLocal8Bit().data();
    for (int i = 0; i < iniWrapper->datas.size(); i++){
        if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
            if (iniWrapper->datas[i].value == stdWID){
                current_mail_address = QString::fromStdString(
                                                        iniWrapper->datas[i].name
                                                             );
                mainWindow->ui->mainMenuMaillabel->setText(current_mail_address);
                mainWindow->ui->label11Page10->setText(current_mail_address);
            }
        }
    }
}

void vedaEMailClient::save_wallets(QString mail_address, QString wallet)
{
    if (wallet.isEmpty()) return;

    //1. Найти место для вставки
    int i = 0;

    //Вставить в конец секции
    for (; i < iniWrapper->datas.size(); i++){
        if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())) break;
    }
    for (; i < iniWrapper->datas.size(); i++){
        if (0 != strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())) break;
    }

    VedaIniEntry temp;
    temp.name = mail_address.toLocal8Bit().data();
    temp.value = wallet.toLocal8Bit().data();
    temp.index = INI_FILE_WALLETS_SECTION;

    if (i == iniWrapper->datas.size()){
        iniWrapper->datas.push_back(temp);
    }
    else {
        iniWrapper->datas.insert(iniWrapper->datas.begin() + i, temp);
    }
    iniWrapper->writeConfigFile();
}

QString vedaEMailClient::find_new_wid()
{
    if (!dir.cd(PATH_TO_WALLETS_DIR)) return "";
    QStringList dirList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    clear_dir_files_list(dirList);
    dir.cd(executable_path);

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

void vedaEMailClient::show_error_label(QLabel *error_label, QString error_text, bool error)
{
    if (!errorLabels.contains(error_label)){
        errorLabels << error_label;
    }

    if (error){
        error_label->setText("<FONT COLOR=#F00000>" +
                              error_text +
                              "</FONT>");
    }
    else {
        error_label->setText("<FONT COLOR=#00F000>" +
                              error_text +
                              "</FONT>");
    }
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

char *vedaEMailClient::jsonToCharArray(json j, int *len)
{
    return std_string_to_char_array(j.dump(), len);
}

char *vedaEMailClient::make_certificate(char *&mailToCheck, int *len)
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

    mailToCheck = QString_to_char_array(
                QString::fromStdString(std::string(mainWindow->ui->MailLineEdit1->text().toLocal8Bit().data()) +
                                       std::string(mainWindow->ui->MailDogLineEdit->text().toLocal8Bit().data()) +
                                       std::string(mainWindow->ui->MailLineEdit2->text().toLocal8Bit().data()) +
                                       std::string(mainWindow->ui->MailDotLineEdit1->text().toLocal8Bit().data()) +
                                       std::string(mainWindow->ui->MailLineEdit3->text().toLocal8Bit().data()))
                  );
    if (NULL == mailToCheck){
        return NULL;
    }

    j[JSON_CERT_FIO] = std::string(mainWindow->ui->first_name_lineEdit->text().toLocal8Bit().data()) +
               std::string(" ") +
               std::string(mainWindow->ui->last_name_lineEdit->text().toLocal8Bit().data());
    j[JSON_CERT_CITY] = std::string(" ");
    j[JSON_CERT_STATE] = std::string(" ");
    j[JSON_CERT_COUNTRY] = std::string(mainWindow->ui->country_comboBox->currentText().toLocal8Bit().data());
    j[JSON_CERT_EMAIL] = mailToCheck;
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
    unReadIncomingMessages = 0;

    int index = -1;
    bool status = false;

//    incoming_letters_indexes.fill(0, TIDsNTimes.size());

#ifndef VEDA_PROTO_BUF
    json j;
#else
    TokenPb::Token token;
#endif
    std::string color_name;
    std::string empty_token_compare_pattern = EMPTY_TOKEN_COLOR_NAME;
    std::string empty_token_compare_pattern_2 = EMPTY_TOKEN_COLOR_NAME_2;
    QString TID;
    QMap<QString, int> RawTimesNIndexes;
//    int indexToRemove;
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
#ifndef VEDA_PROTO_BUF
            j = get_token_json_by_vid(it.key());
            if (j.empty()){
                qDebug()<<"parse json failure";
                continue;
            }
#else
            token = get_token_pb_by_vid(it.key());
            if (!token.has_user() || !token.has_genesis()){
                qDebug()<<"protobuf parse exception";
                continue;
            }
#endif

            if (j_tokens[JSON_BALANCE_MASS][it.key()][JSON_BALANCE_TYPE].get<int>() == 0) {
                tokens_indexes << index;
            }
            else {
#ifndef VEDA_PROTO_BUF
                color_name = j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_COLOR_NAME].get<std::string>();
#else
                color_name = token.genesis().color_text();
#endif
                if ((color_name == empty_token_compare_pattern) ||
                        (color_name == empty_token_compare_pattern_2)){
                    clear_letters_indexes << index;
                }
                else {
                    TID = QString::fromStdString(
            #ifndef VEDA_PROTO_BUF
                                j[JSON_TOKEN_USER][JSON_TOKEN_USER_TID].get<std::string>()
            #else
                                token.user().tid()
            #endif
                                );
                    RawTimesNIndexes.insert(TIDsNRawTimes[TID], index);
//                    incoming_letters_indexes << index;
                    if (!currentReadVIDs.contains(it.key())){
                        unReadIncomingMessages++;
                    }
                }
            }
        }
        catch(...){
            qDebug()<<"json parse exception";
            continue;
        }
    }

    //сделать список входящих отсортированным по убыванию времени
    for (auto it = RawTimesNIndexes.begin(); it != RawTimesNIndexes.end(); it++) {
        incoming_letters_indexes.push_front(it.value());
    }

    incoming_pages_num = incoming_letters_indexes.size() / LETTERS_PER_PAGE;
    if (0 < (LastPageLetterCount = incoming_letters_indexes.size() % LETTERS_PER_PAGE)) {
        incoming_pages_num++;
    }
    else {
        LastPageLetterCount = LETTERS_PER_PAGE;
    }

    //IncomingButton text
    updateIncomingButtonText();
}

void vedaEMailClient::show_current_balance_everywhere()
{
    messagesAvailable = MIN(clear_letters_indexes.size(), tokens_indexes.size());

    mainWindow->ui->message_available_count_label->setText(QString::number(messagesAvailable));
    ///\todo минимальное из количества комиссионных токенов и количества болванок

    if (0 == messagesAvailable){
        mainWindow->ui->new_Button->setDisabled(true);
    }
    else {
        mainWindow->ui->new_Button->setEnabled(true);
    }

    if ((MESSAGES_AVAILABLE_MINIMUM_COUNT_TO_ALLOW_GET_MORE > messagesAvailable)
            &&
            (tokens_indexes.size() > (messagesAvailable + 1))
            &&
            (MAX_GET_MORE_MSGS_BTN_PRESSES > getMoreTokensButtonClickCount))//чтобы в результате возможных сообщений не стало меньше
    {
        mainWindow->ui->GetMoreMessagesButton->setEnabled(true);
    }
    else {
        mainWindow->ui->GetMoreMessagesButton->setDisabled(true);
    }

    mainWindow->ui->label2Page2->setText(makeVDNFromNum(balance));
    mainWindow->ui->label6Page10->setText(makeVDNFromNum(balance));
    mainWindow->ui->label6Page2->setText(QString::number(messagesAvailable));
    mainWindow->ui->label8Page2->setText(VDN_PER_MSG);
    mainWindow->ui->label10Page2->setText(current_wid);

    //settings
    mainWindow->ui->label11Page10->setText(current_mail_address);
    //m sent
    mainWindow->ui->label2Page10->setText(QString::number(outcoming_message_list.size()));
    //m recieved
    mainWindow->ui->label4Page10->setText(QString::number(incoming_letters_indexes.size()));
}

void vedaEMailClient::fill_incoming_message_list()
{
    ini_file_address_book_section_replaces_count_by_incoming = 0;
    ini_file_address_book_section_replaces_by_incoming_was_from_server = false;
    allIncomingQueryiesEnded = false;

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
    if (incoming_letters_indexes.isEmpty()){
        allIncomingQueryiesEnded = true;
        return;
    }

    json::iterator it;
#ifndef VEDA_PROTO_BUF
    json token_j;
#else
    TokenPb::Token token;
#endif
    std::string author;
    QString TID;
    QString time;
    QString rawTime;
//    std::string color;
    int attachmentsSize;
    for(int i = 0; i < lettersToShow; i++){
        it = j_tokens[JSON_BALANCE_MASS].begin();
        std::advance(it, incoming_letters_indexes[current_first_incoming_letter + i]);

#ifndef VEDA_PROTO_BUF
        token_j = get_token_json_by_vid(it.key());
        if (token_j.empty()){
            qDebug()<<"parse json failure";
            continue;
        }
#else
        token = get_token_pb_by_vid(it.key());
        if (!token.has_user() || !token.has_genesis()){
            qDebug()<<"parse protobuf failure";
            continue;
        }
#endif

        try {
#ifndef VEDA_PROTO_BUF
            author = token_j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_AUTHOR_ID].get<std::string>();
#else
            author = token.genesis().author_id();
#endif
            TID = QString::fromStdString(
            #ifndef VEDA_PROTO_BUF
                        token_j[JSON_TOKEN_USER][JSON_TOKEN_USER_TID].get<std::string>()
            #else
                        token.user().tid()
            #endif
                        );
            if (!TID.isEmpty()){
                time = TIDsNTimes[TID];
                rawTime = TIDsNRawTimes[TID];
            }

#ifndef VEDA_PROTO_BUF
            color = token_j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_COLOR_NAME].get<std::string>();
#else
//            color = token.genesis().color_text();
            attachmentsSize = 0;
            attachmentsSize += token.genesis().color_text_cnt();
            for (int i = 0, limit = token.genesis().color_file_size(); i < limit; i++) {
                attachmentsSize += token.genesis().color_file(i).len();
            }
#endif
            //            colorSize = color.size();

        } catch (...) {
            qDebug()<<"parse json exception";
            continue;
        }

        temp = new (std::nothrow) message_list_item(INI_FILE_UNKNOWN_MAIL,
                                                    time/* + "index in vector: " + QString::number(current_letters_vector_index - 1)*/,
                                                    rawTime,
                                                    sizeFromNum(attachmentsSize),
                                                    current_first_incoming_letter + i,
                                                    true,
                                                    mainWindow->ui->IncomingscrollAreaWidgetContents);
        temp->set_wid(author);

        if (currentReadVIDs.contains(it.key())){
            temp->unsetBold();
        }

        mainWindow->addIncomingListItem(temp);
        incoming_message_list << temp;
        ini_file_address_book_section_replaces_count_by_incoming++;

        connect(temp, SIGNAL(clicked()), this, SLOT(message_list_itemClicked()));
    }

    for (int i = 0, limit = incoming_message_list.size(); i < limit; i++) {
        emit get_email_by_wid(incoming_message_list[i]->get_wid(), i, Incoming);
    }

    if (incoming_message_list.isEmpty()){
        allIncomingQueryiesEnded = true;
    }
}

void vedaEMailClient::fillOutcomingMessageList()
{
    ini_file_address_book_section_replaces_count_by_outcoming = 0;
    ini_file_address_book_section_replaces_by_outcoming_was_from_server = false;
    allOutcomingQueryiesEnded = false;

    //удалить предыдущие
    message_list_item *temp;
    if (!outcoming_message_list.isEmpty()){
        for (int i = 0, limit = outcoming_message_list.size(); i < limit; i++){
            temp = outcoming_message_list.last();
            outcoming_message_list.removeLast();
            delete temp;
        }
    }
    TIDsNTimes.clear();
    TIDsNRawTimes.clear();

    //заполнить новыми
    int i = 0;
    int outcoming_index = 0;
    auto it = j_trns.begin();
    QString wid_sender;
    QString wid_recipient;
    std::string rawTime;
    QString time;
    QString TID;
    for (; it != j_trns.end(); it++, i++){
        try {
            //определить что исходящая
            wid_sender = QString::fromStdString(
                        it.value()[JSON_TRANSACTION_BODY][JSON_TRANSACTION_BODY_SENDER].get<std::string>()
                        );

            wid_recipient = QString::fromStdString(
                        it.value()[JSON_TRANSACTION_BODY][JSON_TRANSACTION_BODY_RECIPIENT].get<std::string>()
                        );

            rawTime = it.value()[JSON_TRANSACTION_BODY][JSON_TRANSACTION_BODY_TIME].get<std::string>();
            time = getTimeFromTransaction(rawTime);
            //если входящая - заполнить вектор времен
            if (wid_sender != current_wid
                    ||
                wid_sender == wid_recipient) //если сам себе
            {
                TID = QString::fromStdString(
                        it.value()[JSON_TRANSACTION_BODY][JSON_TRANSACTION_BODY_TID].get<std::string>()
                            );
                TIDsNTimes.insert(TID, time);
                TIDsNRawTimes.insert(TID, QString::fromStdString(rawTime));
//                RawTimesNTIDs.insert(QString::fromStdString(rawTime), TID);

//                if (wid_sender != wid_recipient){
//                    //если не сам себе, не надо заполнять как исходящую
//                    continue;
//                }
                continue;
            }
//            else {

//            }


//            time = getTimeFromTransaction(it.value()[JSON_TRANSACTION_BODY][JSON_TRANSACTION_BODY_TIME].get<std::string>());


        } catch (...) {
            qDebug()<<"json exception";
            continue;
        }

        temp = new (std::nothrow) message_list_item(INI_FILE_UNKNOWN_MAIL,
                                                    time,
                                                    QString::fromStdString(rawTime),
                                                    "",
                                                    -1,
                                                    false,
                                                    mainWindow->ui->OutcomingscrollAreaWidgetContents);
//        temp->setStyleSheet("border: 2px solid lightgreen;");
        temp->set_wid(
                    std::string(
                        wid_recipient.toLocal8Bit().data(), wid_recipient.toLocal8Bit().size()));
        temp->unsetBold();
        mainWindow->addOutcomingListItem(temp);
        outcoming_message_list << temp;
        ini_file_address_book_section_replaces_count_by_outcoming++;
//        emit get_email_by_wid(wid_recipient, outcoming_index++, Outcoming);
    }

    sortMessageListByTime(Outcoming);

    for (int i = 0, limit = outcoming_message_list.size(); i < limit; i++) {
        emit get_email_by_wid(outcoming_message_list[i]->get_wid(), i, Outcoming);
    }

    if (0 == outcoming_index){
        allOutcomingQueryiesEnded = true;
    }
}

//int vedaEMailClient::get_current_open_wallet_index(QString mail_str)
//{
//    QString address;
//    //Если описание расширенное
//    if (mail_str.contains("(")){
//        address = mail_str.split("(")[1]; //взять все после "("
//        address.chop(1); //удалить ")"

//        std::string add(address.toStdString());
//        for (int i = 0; i < mail_wallet_pairs.size(); i++){
//            if (add == mail_wallet_pairs[i].value){
//                return i;
//            }
//        }
//        return -1;
//    }
//    else {
//        std::string ml(mail_str.toStdString());
//        for (int i = 0; i < mail_wallet_pairs.size(); i++){
//            if (ml == mail_wallet_pairs[i].name){
//                return i;
//            }
//        }
//        return -1;
//    }
//}

bool vedaEMailClient::isEmailValidAddress(std::string str)
{
    QRegExp EmailRegExp("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$");
    return QString::fromStdString(str).contains(EmailRegExp);
}

bool vedaEMailClient::isWord(QString str, int minimumSize)
{
    QRegExp wordRegExp("\\w");
    if (0 != minimumSize){
        if (str.size() < minimumSize) return false;
    }

    return str.contains(wordRegExp);
}

QString vedaEMailClient::get_WID_from_begin(int *index)
{
    std::string temp;

    QString login_string = mainWindow->ui->login_lineEdit->text();
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
            return QString();
        }
    }
    else {//логин содержит только mail адрес
        temp = login_string.toLocal8Bit().data();
        int i = 0;
        for (; i < iniWrapper->datas.size(); i++) {
            if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
                if (temp == iniWrapper->datas[i].name){
                    if (nullptr != index) {
                        *index = i;
                        break;
                    }
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
        if ((DEFAULT_WID_SIZE_WITH_P == list[i].size())
                && (list[i].contains("_p"))){
            list[i].chop(2);
            i++;
        }
        else {
            list.removeAt(i);
        }
    }
}

char *vedaEMailClient::get_token_buffer_by_vid(const string &vid, int &len)
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
    if (LIBVEDAWALLET_NOERROR != veda_wallet_get_token_json_by_id(wstk,
                                                                  vid_ch,
                                                                  &token_ch,
                                                                  len)){
        qDebug()<<"cannot get token info";
        free(vid_ch);
        return NULL;
    }
    else {
        free(vid_ch);
        return token_ch;
    }
}


#ifndef VEDA_PROTO_BUF
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
#else

TokenPb::Token vedaEMailClient::get_token_pb_by_vid(const string &vid)
{
    TokenPb::Token ret;
    int bufLen = 0;

    char *token_ch = get_token_buffer_by_vid(vid, bufLen);
    if (NULL != token_ch){
        ret.ParseFromArray(token_ch, bufLen);
        free(token_ch);
    }
    return ret;

//        serialize:
//        ret.ByteSizeLong() потому что надо сразу выделить память
//        ret.SerializeToArray()

        //verify field existence
//        ret.genesis().color_name().empty();

        //setter:
//        ret.mutable_user()->set_tid()
        //getter:
//        ret.user().tid()
}
#endif

char *vedaEMailClient::form_token_mas_by_index(int index, int *len)
{
    if (j_tokens.empty()){
        return NULL;
    }

    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    std::advance(it, index);
    std::string vid = it.key();
#ifndef VEDA_PROTO_BUF
    json j;
#else
    TokenPb::Token token;
#endif

#ifndef VEDA_PROTO_BUF
    j = get_token_json_by_vid(vid);
    if (j.empty()){
        qDebug()<<"Get token failure";
        return NULL;
    }
#else
    token = get_token_pb_by_vid(vid);
    if (!token.has_user() || !token.has_genesis()){
        qDebug()<<"parse protobuf failure";
        return NULL;
    }
#endif

    json j_buf;

#ifndef VEDA_PROTO_BUF
    j_buf[vid][JSON_TRN_TOKEN_IN_NOMINAL] = j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_NOMINAL];
    j_buf[vid][JSON_TRN_TOKEN_IN_TID_TRN] = j[JSON_TOKEN_USER][JSON_TOKEN_USER_TID];
    j_buf[vid][JSON_TRN_TOKEN_IN_HASH_TRN] = j[JSON_TOKEN_USER][JSON_TOKEN_USER_TID_HASH];
#else
    j_buf[vid][JSON_TRN_TOKEN_IN_NOMINAL] = token.genesis().nominal();
    j_buf[vid][JSON_TRN_TOKEN_IN_TID_TRN] = token.user().tid();
    j_buf[vid][JSON_TRN_TOKEN_IN_HASH_TRN] = token.user().tid_hash();
#endif

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
        WIDsByMail.clear();
        for (uint64_t i = 0; i < list.size(); i++){
            if (emailString == list[i].name){
                WIDsByMail << list[i].value;
            }

        }
        return !WIDsByMail.isEmpty();
//        if (!WIDsByMail.isEmpty()) return true;
//        return false;
    }
}

void vedaEMailClient::finish_sending_message()
{
    //0. Пресечь попытку отправить самому себе
    if (recipient_wid == current_wid){
        show_error_label(mainWindow->ui->NewLetterErrLabel, "Recipient must be another Veda User");
        return;
    }

    //изменить COLOR_NAME
    char *vid = get_vid_by_index(clear_letters_indexes[0]);
    if (NULL == vid){
        qDebug()<<"Low memory";
        return;
    }

    //для того, чтобы адресат расшифровал, надо передать его WID
    char *WIDr = NULL;

    WIDr = QString_to_char_array(recipient_wid);
    if (NULL == WIDr){
        qDebug()<<"Low memory";
        return;
    }
//    qDebug()<<"WIDr is set to         " << WIDr << " " << strlen(WIDr);

    char *jfileList = NULL;
    int jfileListLen = 0;
    if (0 < overallAttachmentsSize){
//        json file, filesInfo;
//        file[JSON_FILE_NAME] = std::string(
//                    fileToAttachName.toLocal8Bit().data()
//                    );
//        filesInfo.push_back(file);
//        jfileList = jsonToCharArray(filesInfo, &jfileListLen);

        json file, filesInfo;
        for (int i = 0, limit = outcomingFileButtons.size(); i < limit; i++){
            file[JSON_FILE_NAME] = std::string(
                        outcomingFileButtons[i]->getFileNameWithPath().toLocal8Bit().data()
                        );
            filesInfo.push_back(file);
            jfileList = jsonToCharArray(filesInfo, &jfileListLen);
        }
    }
//    else {//text
    int color_name_len = 0;
    char *color_name = NULL;

    if (mainWindow->ui->newLetterPlainTextEdit->toPlainText().isEmpty()){
//        color_name = QString_to_char_array("Attachments\n        ↓", &color_name_len);
    }
    else {
        color_name = QString_to_char_array(mainWindow->ui->newLetterPlainTextEdit->toPlainText(), &color_name_len);
        if (NULL == color_name){
            free(WIDr);
            free(vid);
            qDebug()<<"Low memory";
            return;
        }
    }



    if (LIBVEDAWALLET_NOERROR != veda_wallet_edit_marked_token_by_id(wstk,
                                                                     vid,
                                                                     WIDr,
                                                                     color_name,
                                                                     color_name_len,
                                                                     jfileList,
                                                                     jfileListLen)){
        free(color_name);
        free(WIDr);
        free(vid);
        if (jfileList) free(jfileList);
        qDebug()<<"Error editing letter text";
        return;
    }
    else {
        free(color_name);
        free(vid);
        if (jfileList) free(jfileList);
    }

//        qDebug()<<"WIDr after editing is  " << WIDr << " " << strlen(WIDr);
//    }

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

//    qDebug()<<"WIDr before sending is " << WIDr << " " << strlen(WIDr);

    emit server_send_tokens(wstk,
                            TRN_TRANSFER, ///\todo определить на свой ли кошелек идет отправка
                            WIDr,
                            token_mas,
                            token_mas_len,
                            com_mas,
                            com_mas_len);
    startAutoRenewTimer();
    startAutoLogOutTimer();
    toggle_loading_screen(3, true);
}

QString vedaEMailClient::getTimeFromTransaction(string str_time)
{
    QString raw = QString::fromStdString(str_time);
    QStringList raw_list = raw.split(".");
    if (0 == raw_list.size()){
        return QString("");
    }
    else if (2 > raw_list.size()) {
        raw_list << QString::number(0);
    }

    bool ok = false;
    unsigned long secs = raw_list[0].toULong(&ok);
    if (!ok) return QString("");

    unsigned long msecs = raw_list[1].toULong(&ok);
    if (!ok) return QString("");

    struct tm *ptm;
    ptm = localtime((time_t*)&secs);

    char c_time[40] = {0x0};
    strftime(c_time, sizeof(c_time), "%Y-%m-%d %H:%M:%S", ptm);

    msecs /= 1000;
    char c_ret[128] = {0x0};
    sprintf(c_ret,"%s.%ld ", c_time, (long)msecs);

    raw = c_ret;

    //может в итоге не будет нужно, но сейчас есть:
    //удаление миллисекунд
    raw.chop(raw.size() - raw.lastIndexOf("."));

    return raw;
}

void vedaEMailClient::get_mails_for_wallets_section()
{
    //дойти до секции
    int i = 0;
    for (; i < iniWrapper->datas.size(); i++) {
        if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())) break;
    }

    if (i < iniWrapper->datas.size()){//секция есть
        int section_begin = i;

        //сосчитать, сколько замен требуется, они понадобятся для сигнала об окончании замен
        QVector<int> indexesToEmit;
        ini_file_wallets_section_replaces_count = 0;
        for (; i < iniWrapper->datas.size(); i++){
            if (0 == strcmp(INI_FILE_WALLETS_SECTION, iniWrapper->datas[i].index.c_str())){
                if (0 == strcmp(INI_FILE_UNKNOWN_MAIL, iniWrapper->datas[i].name.c_str())){
                    ini_file_wallets_section_replaces_count++;
                    indexesToEmit << i;
                }
            }
            else break;
        }

        //теперь отправить сигналы запроса почтового адреса
        int limit = indexesToEmit.size();
        if (0 < limit){
            for (i = 0; i < limit; i++){
                emit get_email_by_wid(iniWrapper->datas[indexesToEmit[i]].value, indexesToEmit[i], IniFileWalletsSection);
            }
        }
    }
}

bool vedaEMailClient::hasStartMoney()
{
    return walletsRecievedStartMoney.contains(current_wid);
}

void vedaEMailClient::startAutoRenewTimer()
{
//    qDebug()<<"autorenew timer started";
    autorenew_timer->start(AUTO_RENEW_TIMER_PERIOD);
}

QString vedaEMailClient::makeVDNFromNum(uint64_t num)
{
    QString ret = QString::number(num);
//    QString rret = ret.insert(ret.size() - 6, ",");
    if (ret.size() <= 6){
        return QString::number(0);
    }
    else {
        ret.chop(6);
        return ret;
    }
}

void vedaEMailClient::renew_frontend()
{
    fillOutcomingMessageList();

    fill_token_indexes_vectors();
    if (1 == incoming_pages_num || 0 == incoming_pages_num){
        mainWindow->ui->button4Page3->hide();
        mainWindow->ui->button5Page3->hide();
        mainWindow->ui->button6Page3->hide();
        mainWindow->ui->button7Page3->hide();
    }
    else {
        mainWindow->ui->button4Page3->show();
        mainWindow->ui->button5Page3->show();
        mainWindow->ui->button6Page3->show();
        mainWindow->ui->button7Page3->show();
    }

    incoming_message_list_form_first_ButtonClicked();
    show_current_balance_everywhere();
}

void vedaEMailClient::startAutoLogOutTimer()
{
    if (0 < autoLogOutTimerMinuteRange){
//        qDebug()<<"autologout started for " << autoLogOutTimerMinuteRange << " minutes";
        autoLogOutTimer->start(1000 * 60 * autoLogOutTimerMinuteRange);
    }
    else {
//        qDebug()<<"autologout turned off";
        autoLogOutTimer->stop();
    }
}

bool vedaEMailClient::parseWIDsByMail()
{
    if (1 == WIDsByMail.size()){
        recipient_wid = QString::fromStdString(WIDsByMail[0]);
        return true;
    }
    else if (1 < WIDsByMail.size()) {
//        вставить в completions для newletter
        QString strToFind = mainWindow->ui->lineEditPage6->text();
        if (recipientLineEditCompletions.contains(strToFind)){
            recipientLineEditCompletions.removeOne(strToFind);
        }

        QString strToInsert;
        for (int i = 0; i < WIDsByMail.size(); i++) {
            strToInsert = strToFind + " (" + QString::fromStdString(WIDsByMail[i]) + ")";
            if (!recipientLineEditCompletions.contains(strToInsert)){
                recipientLineEditCompletions.push_back(strToInsert);
            }
        }

        mainWindow->setCompleterOnRecipientLineEdit(recipientLineEditCompletions);
        show_error_label(mainWindow->ui->NewLetterErrLabel, "This Veda account have more than one adresses, choose one");
        mainWindow->CompleteOnRecipientLineEdit();
//        read_address_book(false);
    }
    else { //нет аккаунтов с таким email'ом
        show_error_label(mainWindow->ui->NewLetterErrLabel, "This Veda account does not exist");
    }

    /*
            show_error_label(mainWindow->ui->NewLetterErrLabel, "This Veda account have more than one adresses, choose one");
            mainWindow->CompleteOnRecipientLineEdit();
    */
    return false;
}

void vedaEMailClient::sortMessageListByTime(GetMailByWIDType type)
{
    //выбрать список
    QVector<message_list_item*> &list = Incoming == type ? incoming_message_list : outcoming_message_list;

    //отсортировать по времени в порядке возрастания
    QMap<QString, int> map;
    for (int i = 0, limit = list.size(); i < limit; i++) {
        map.insert(list[i]->getRawTime(), i);
    }

    //переставить
    QVector<message_list_item*> newList;
    message_list_item* temp;
    while (!map.isEmpty()){
        temp = list[map.last()];
        newList << temp;
        map.remove(map.lastKey());

        Incoming == type ?
        mainWindow->removeIncomingListItem(temp) :
        mainWindow->removeOutomingListItem(temp);
    }

    list = newList;

    for (int i = 0, limit = list.size(); i < limit; i++) {

        Incoming == type ?
        mainWindow->addIncomingListItem(list[i]) :
        mainWindow->addOutcomingListItem(list[i]);
    }
}

void vedaEMailClient::readReadMessageList()
{
    if (nullptr != currentReadMessageStorage){
        delete currentReadMessageStorage;
        currentReadMessageStorage = nullptr;
    }

    char *fileName = QString_to_char_array(QString(PATH_TO_WALLETS_DIR) + "/" + current_wid + ".tmp");
    currentReadMessageStorage = new (std::nothrow) VedaIniWrapper(fileName);
    free(fileName);

    currentReadVIDs.clear();
    for (uint64_t i = 0, limit = currentReadMessageStorage->datas.size(); i < limit; i++) {
        currentReadVIDs << currentReadMessageStorage->datas[i].name;
    }
}

void vedaEMailClient::addMessageToReadMessageList(const std::string &vid)
{
    currentReadVIDs << vid;

    VedaIniEntry temp;
    temp.index = READ_MESSAGE_LIST_SECTION;
    temp.value = READ_MESSAGE_LIST_VALUE;
    temp.name = vid;

    int placeToInsert = 0;
    for (uint64_t limit = currentReadMessageStorage->datas.size(); placeToInsert < limit; placeToInsert++) {
        if (currentReadMessageStorage->datas[placeToInsert].name < vid){
            continue;
        }
        else if (currentReadMessageStorage->datas[placeToInsert].name == vid){
            placeToInsert = -1;
            break;
        }
        else {
            break;
        }
    }

    if (-1 == placeToInsert) {
        return;
    }
    else if (placeToInsert == currentReadMessageStorage->datas.size()){
        currentReadMessageStorage->datas.push_back(temp);
    }
    else {
        auto it = currentReadMessageStorage->datas.begin();
        currentReadMessageStorage->datas.insert(it + placeToInsert, temp);
    }

    //только если сообщение было новым
    //1. Сразу учесть это в файле
    currentReadMessageStorage->writeConfigFile();

    //2. Учесть это в кнопке
    unReadIncomingMessages--;
    updateIncomingButtonText();
}

void vedaEMailClient::saveReadMessageList()
{
    if (nullptr != currentReadMessageStorage){
        currentReadMessageStorage->writeConfigFile();
    }
}

void vedaEMailClient::updateIncomingButtonText()
{
    if (0 < unReadIncomingMessages){
        mainWindow->ui->incoming_Button->setText(QString(INCOMING_BUTTON_DEFAULT_TEXT)
                                                 + " ("
                                                 + QString::number(unReadIncomingMessages)
                                                 + ")");
    }
    else {
        mainWindow->ui->incoming_Button->setText(INCOMING_BUTTON_DEFAULT_TEXT);
    }
}

QString vedaEMailClient::sizeFromNum(int num)
{
    if (0 > num) return QString("");

    QString ret;
    uint64_t s = num;

    if (0 == (s >> 10)){
        ret = QString::number(s) + " B";
        return ret;
    }

    s >>= 10;
    if (0 == (s >> 10)){
        ret = QString::number(s) + " KB";
        return ret;
    }

    s >>= 10;
    if (0 == (s >> 10)){
        ret = QString::number(s) + " MB";
        return ret;
    }

    s >>= 10;
    if (0 == (s >> 10)){
        ret = QString::number(s) + " GB";
        return ret;
    }

    s >>= 10;
    if (0 == (s >> 10)){
        ret = QString::number(s) + " TB";
        return ret;
    }

    return QString("");
}

//QString vedaEMailClient::sizeFromColor(string str)
//{
//    QString ret;
//    uint64_t s = str.size();

//    if (0 == (s >> 10)){
//        ret = QString::number(s) + " B";
//        return ret;
//    }

//    s >>= 10;
//    if (0 == (s >> 10)){
//        ret = QString::number(s) + " KB";
//        return ret;
//    }

//    s >>= 10;
//    if (0 == (s >> 10)){
//        ret = QString::number(s) + " MB";
//        return ret;
//    }

//    s >>= 10;
//    if (0 == (s >> 10)){
//        ret = QString::number(s) + " GB";
//        return ret;
//    }

//    s >>= 10;
//    if (0 == (s >> 10)){
//        ret = QString::number(s) + " TB";
//        return ret;
//    }

//    return QString("");
//}

QString vedaEMailClient::cutFileName(const QString &fileName)
{
    return fileName.split("/").last();
}

void vedaEMailClient::deleteUncompletedBox()
{
    veda_wallet_CryptoBoxDropBox(wstk);
    veda_wallet_CryptoBoxFree(wstk);
    emit server_nulify_wstk();
}

bool vedaEMailClient::sortSection(string sectionName)
{
    if (!iniWrapper->datas.empty()){
        bool wasSorted = false;
        std::string mail;
        int size = iniWrapper->datas.size();
        int same_name_entries = 0;

        for (int i = 0; i < size; i += (1 + same_name_entries)){
            if (0 == strcmp(sectionName.c_str(), iniWrapper->datas[i].index.c_str())){
                mail = iniWrapper->datas[i].name;
                same_name_entries = 0;
                for (int j = i + 1; j < size; j++){
                    if (0 != strcmp(sectionName.c_str(), iniWrapper->datas[j].index.c_str())) break;
                    if (iniWrapper->datas[j].name == mail){
                        if ((j - i) > ++same_name_entries){
                            iniWrapper->datas.insert(iniWrapper->datas.begin() + i + same_name_entries,
                                                     iniWrapper->datas[j]);
                            iniWrapper->datas.erase(iniWrapper->datas.begin() + j + 1);
//                            j--;
                            wasSorted = true;
                        }
                    }
                }
            }
        }

        return wasSorted;
    }
    else {
        return false;
    }
}

void vedaEMailClient::savePreferredNode(QString IP)
{
    int index = 0;
    int limit = 0;
    if (0 < (limit = iniWrapper->datas.size())){
        while (index < limit) {
            if (0 == strcmp(iniWrapper->datas[index].index.c_str(), INI_FILE_CURRENT_NODE_SECTION))
                break;
        }
    }

    VedaIniEntry temp;
    temp.index = INI_FILE_CURRENT_NODE_SECTION;
    temp.name = temp.value = IP.toLocal8Bit().data();
    if (index == limit){
        iniWrapper->datas.push_back(temp);
    }
    else {
        iniWrapper->datas[index].name = iniWrapper->datas[index].value = temp.name;
    }
    iniWrapper->writeConfigFile();
}

void vedaEMailClient::begin_form_signIn_ButtonClicked()
{
    if (mainWindow->ui->login_lineEdit->text().isEmpty()){
        show_error_label(mainWindow->ui->BeginErrLabel, "Login is empty");
        return;
    }

    if (PASS_MIN_LEN > mainWindow->ui->password_lineEdit->text().size()){
        show_error_label(mainWindow->ui->BeginErrLabel, "Password is too short");
        return;
    }
    if (SECRET_MIN_LEN > mainWindow->ui->secret_lineEdit->text().size()){
        show_error_label(mainWindow->ui->BeginErrLabel, "Secret phrase is too short");
        return;
    }


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
    char *pass = QString_to_char_array(mainWindow->ui->password_lineEdit->text(), &pass_len);
    if (NULL == pass){
        free(cryptobox_name);
        free(curnode);
        qDebug()<<"low memory";
        return;
    }

    int secret_len;
    char *secret = QString_to_char_array(mainWindow->ui->secret_lineEdit->text(), &secret_len);
    if (NULL == secret){
        free(pass);
        free(cryptobox_name);
        free(curnode);
        qDebug()<<"low memory";
        return;
    }

    toggle_loading_screen(0, true);

    emit server_auth(engine,
                     PATH_TO_WALLETS_DIR,
                     curnode,
                     cryptobox_name,
                     cryptobox_name_len,
                     pass,
                     pass_len,
                     secret,
                     secret_len);
    startAutoRenewTimer();
    startAutoLogOutTimer();
}

void vedaEMailClient::begin_form_register_ButtonClicked()
{
    mainWindow->ui->stackedWidget->setCurrentIndex(9);
}

void vedaEMailClient::registration_form_sign_up_ButtonClicked()
{
//    if (!isWord(mainWindow->ui->MailLineEdit1->text(), 2)){
//        show_error_label(mainWindow->ui->RegErrorLabel, "Your Veda Account prefix is invalid or too short");
//        return;
//    }
//    if (!isWord(mainWindow->ui->MailLineEdit2->text())){
//        show_error_label(mainWindow->ui->RegErrorLabel, "Your Veda Account domain is invalid or too short");
//        return;
//    }
//    if (!isWord(mainWindow->ui->MailLineEdit3->text(), 2)){
//        show_error_label(mainWindow->ui->RegErrorLabel, "Your Veda Account suffix is invalid or too short");
//        return;
//    }
    if (!isEmailValidAddress(
                std::string(
                    QString(
                        mainWindow->ui->MailLineEdit1->text() +
                        mainWindow->ui->MailDogLineEdit->text() +
                        mainWindow->ui->MailLineEdit2->text() +
                        mainWindow->ui->MailDotLineEdit1->text() +
                        mainWindow->ui->MailLineEdit3->text()
                        ).toLocal8Bit().data()
                    ))){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your Veda Account name is not a valid email address");
        return;
    }
    if (PASS_MIN_LEN > mainWindow->ui->password_lineEdit_3->text().size()){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your password is too short");
        return;
    }
    if (mainWindow->ui->confirmPasswordLineEdit->text() != mainWindow->ui->password_lineEdit_3->text()){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your password confirm does not match your password");
        return;
    }
    if (SECRET_MIN_LEN > mainWindow->ui->secret_lineEdit_3->text().size()){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your secret phrase is too short");
        return;
    }
    if (mainWindow->ui->confirmSecretLineEdit->text() != mainWindow->ui->secret_lineEdit_3->text()){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your secret phrase confirm does not match your secret phrase");
        return;
    }
    if (!isWord(mainWindow->ui->first_name_lineEdit->text())){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your first name is invalid or empty");
        return;
    }
    if (!isWord(mainWindow->ui->last_name_lineEdit->text())){
        show_error_label(mainWindow->ui->RegErrorLabel, "Your last name is invalid or empty");
        return;
    }

    char *curnode = QString_to_char_array(current_node);

    int pass_len;
    char *pass = QString_to_char_array(mainWindow->ui->password_lineEdit_3->text(), &pass_len);
    if (NULL == pass){
        qDebug()<<"low memory";
        return;
    }

    int secret_len;
    char *secret = QString_to_char_array(mainWindow->ui->secret_lineEdit_3->text(), &secret_len);
    if (NULL == secret){
        free(pass);
        qDebug()<<"low memory";
        return;
    }

    int cert_len;
    char *mailToCheck = NULL;
    char *certificate = make_certificate(mailToCheck, &cert_len);
    if (NULL == certificate){
        free(pass);
        free(secret);
        qDebug()<<"low memory";
        return;
    }
    regMail = mailToCheck;

    emit server_reg_1(engine,
                      PATH_TO_WALLETS_DIR,
                      curnode,
                      pass,
                      pass_len,
                      secret,
                      secret_len,
                      certificate,
                      cert_len,
                      mailToCheck);

    toggle_loading_screen(9, true);

}

void vedaEMailClient::registration_form_back_ButtonClicked()
{
    mainWindow->ui->stackedWidget->setCurrentIndex(0);
}

void vedaEMailClient::on_logOut_Button_clicked()
{
    veda_wallet_free(wstk);
//    saveReadMessageList();
    autorenew_timer->stop();
    autoLogOutTimer->stop();
    getMoreTokensButtonClickCount = 0;
    currentIncomingPage = -1;
    loggedIn = false;

    mainWindow->ui->stackedWidget->setCurrentIndex(0);

//    main_menu_form->hide();
//    ui->description_label->clear();
//    show_begin();
}

void vedaEMailClient::main_menu_form_new_ButtonClicked()
{
    if (0 == mainWindow->ui->stackedWidgetPage3->currentIndex()) return;

    mainWindow->ui->lineEditPage6->clear();
    mainWindow->ui->newLetterPlainTextEdit->clear();

    while (!outcomingFileButtons.isEmpty()) {
        mainWindow->removeOutcomingFile(outcomingFileButtons.last());
        delete outcomingFileButtons.last();
        outcomingFileButtons.removeLast();
    }
    overallAttachmentsSize = 0;
//    attachmentsGood = true;

    mainWindow->ui->stackedWidgetPage3->setCurrentIndex(0);
}

void vedaEMailClient::main_menu_form_incoming_ButtonClicked()
{
    if (1 == mainWindow->ui->stackedWidgetPage3->currentIndex()) return;

    mainWindow->ui->stackedWidgetPage3->setCurrentIndex(1);
}

void vedaEMailClient::main_menu_form_outcoming_ButtonClicked()
{
    if (2 == mainWindow->ui->stackedWidgetPage3->currentIndex()) return;

    mainWindow->ui->stackedWidgetPage3->setCurrentIndex(2);
}

void vedaEMailClient::message_list_itemClicked()
{
    message_list_item *temp = (message_list_item*)sender();

    //0. Вписать автора
    mainWindow->ui->lineEditPage3->setText(temp->get_author());

    //1. Найти нужный токен и запросить его инфо
    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    CurrentOpenedIncomingMessage = incoming_letters_indexes[temp->get_index()];
//    read_letter_form->set_index(index_needed);
    std::advance(it, CurrentOpenedIncomingMessage);
    currentReadMessageVID = it.key();

    //Сделать письмо прочитанным
    temp->unsetBold();
    addMessageToReadMessageList(currentReadMessageVID);
#ifndef VEDA_PROTO_BUF
    json j;
#else
    TokenPb::Token token;
#endif

#ifndef VEDA_PROTO_BUF
    j = get_token_json_by_vid(currentReadMessageVID);
    if (j.empty()){
        qDebug()<<"parse json failure";
        return;
    }
#else
    token = get_token_pb_by_vid(currentReadMessageVID);
    if (!token.has_user() || !token.has_genesis()){
        qDebug()<<"parse protobuf failure";
        return;
    }
#endif

    //2. Взять поле с текстом
    try {
        mainWindow->ui->readLetterPlainTextEdit->clear();
#ifdef VEDA_PROTO_BUF
        if (token.genesis().color_text_cnt() == 1){
#endif
        mainWindow->ui->readLetterPlainTextEdit->setPlainText(
                    QString::fromStdString(
                #ifndef VEDA_PROTO_BUF
                        j[JSON_TOKEN_GENESIS][JSON_TOKEN_GENESIS_COLOR_NAME].get<std::string>()
                #else
                        token.genesis().color_text()
                #endif
                        ));
#ifdef VEDA_PROTO_BUF
        }
#endif

    } catch (...) {
        qDebug()<<"taking data from token exception";
        return;
    }

#ifdef VEDA_PROTO_BUF
    //3. Проверить вложенные файлы
    readMessageFilesAttachedNames.clear();
    readMessageFilesAttachedCount = 0;

    fileAttachment *tempFA;
    while (!incomingFileButtons.isEmpty()) {
        tempFA = incomingFileButtons.last();
        mainWindow->removeIncomingFile(tempFA);
        delete tempFA;
        incomingFileButtons.removeLast();
    }

    if (0 < (readMessageFilesAttachedCount = token.genesis().color_file_size())){
        for (int i = 0; i < readMessageFilesAttachedCount; i++) {
            readMessageFilesAttachedNames << QString::fromStdString(
                                                 token.genesis().color_file(i).name()
                                                 );

            tempFA = new fileAttachment(true, readMessageFilesAttachedNames.last(), QString(),
                                        token.genesis().color_file(i).len(),
                                        mainWindow->ui->readMessageScrollAreaWidgetContents);
            mainWindow->addIncomingFile(tempFA);
            incomingFileButtons << tempFA;
            connect(tempFA, SIGNAL(clicked(bool, const QString &)),
                    this, SLOT(fileButtonClicked(bool, const QString &)));
        }

        //обновить кнопку
//        mainWindow->showReadMessageSaveFilesSection(readMessageFilesAttachedNames);
    }
    else {
//        mainWindow->hideReadMessageSaveFilesSection();
    }
#endif

    if (mainWindow->ui->readLetterPlainTextEdit->toPlainText().isEmpty()){
//        mainWindow->ui->decryptButton->setDisabled(true);
        mainWindow->ui->decryptButton->hide();
    }
    else {
        mainWindow->ui->decryptButton->setEnabled(true);
        mainWindow->ui->decryptButton->show();
    }
    mainWindow->ui->stackedWidgetPage3->setCurrentIndex(3);
}

void vedaEMailClient::on_settings_Button_clicked()
{
    mainWindow->ui->stackedWidget->setCurrentIndex(10);
}

void vedaEMailClient::settings_form_back_ButtonClicked()
{
    if (loggedIn){
        mainWindow->ui->stackedWidget->setCurrentIndex(3);
        mainWindow->ui->stackedWidgetPage3->setCurrentIndex(1);
    }
    else {
        mainWindow->ui->stackedWidget->setCurrentIndex(0);
        mainWindow->ui->stackedWidgetPage3->setCurrentIndex(1);
    }
}

void vedaEMailClient::on_balance_Button_clicked()
{
    mainWindow->ui->stackedWidget->setCurrentIndex(2);
}

void vedaEMailClient::balance_form_back_ButtonClicked()
{
    mainWindow->ui->stackedWidget->setCurrentIndex(3);
    mainWindow->ui->stackedWidgetPage3->setCurrentIndex(1);
}

void vedaEMailClient::read_letter_form_decrypt_ButtonClicked()
{
    auto it = j_tokens[JSON_BALANCE_MASS].begin();
    std::advance(it, CurrentOpenedIncomingMessage);

//    char *token_j_buf = NULL;
//    int token_j_buf_len = 0;

//    token_j_buf = get_token_buffer_by_vid(it.key(), token_j_buf_len);
//    if (NULL == token_j_buf){
//        qDebug()<<"get token failure";
//        return;
//    }
    char *vid_ch = std_string_to_char_array(it.key());
    if (NULL == vid_ch){
        qDebug()<<"low memory";
        return;
    }

    char *decrypted = NULL;
    int decrypted_len = 0;
//    if (LIBVEDAWALLET_NOERROR != veda_wallet_decrypt_marked_token(wstk,
    if (LIBVEDAWALLET_NOERROR != veda_wallet_decrypt_marked_token_text(wstk,
//                                                                  token_j_buf,
//                                                                  token_j_buf_len,
                                                                       vid_ch,
                                                                  &decrypted,
                                                                  decrypted_len)){
        qDebug()<<"decrypt failure";
//        free(token_j_buf);
        free(vid_ch);
    }
    else {
        mainWindow->ui->readLetterPlainTextEdit->setPlainText(decrypted);
//        free(token_j_buf);
        free(vid_ch);
        free(decrypted);
    }

    mainWindow->ui->decryptButton->setDisabled(true);
}

void vedaEMailClient::new_letter_form_send_ButtonClicked()
{
    //если что-то не заполнено - ничего не делать
    if (mainWindow->ui->lineEditPage6->text().isEmpty() ||
            (mainWindow->ui->newLetterPlainTextEdit->toPlainText().isEmpty() && (0 == overallAttachmentsSize))){
        return;
    }

//    if (!attachmentsGood){
//        show_error_label(mainWindow->ui->NewLetterErrLabel, "Attachments size must be less then 50MB");
//        return;
//    }

    //если там мыло
    if (isEmailValidAddress(mainWindow->ui->lineEditPage6->text().toLocal8Bit().data())){
//        if (!get_wid_by_email_locally(mainWindow->ui->lineEditPage6->text())){
            char *email = QString_to_char_array(mainWindow->ui->lineEditPage6->text());
            emit server_get_wid_list_by_email(wstk,
                                              email);
            startAutoRenewTimer();
            startAutoLogOutTimer();
            toggle_loading_screen(3, true);
//        }
//        else {//если нашлось локально
//            finish_sending_message();
//        }

    }
    else if (mainWindow->ui->lineEditPage6->text().contains("(")) {//мыло с описанием адреса
        recipient_wid = mainWindow->ui->lineEditPage6->text().split("(")[1];//WID + ")"
        recipient_wid.chop(1);//WID
        finish_sending_message();
    }
    else {//адрес        
        recipient_wid = mainWindow->ui->lineEditPage6->text();
        finish_sending_message();
    }

}

void vedaEMailClient::doubleClicked()
{
    if (0 == mainWindow->ui->stackedWidget->currentIndex()){
        mainWindow->ShowAllCompletionsOnLoginLineEdit();
    }
    else if (0 == mainWindow->ui->stackedWidgetPage3->currentIndex()) {
        mainWindow->ShowAllCompletionsOnRecipientLineEdit();
    }
}

void vedaEMailClient::error_timeout()
{
//    _error_label->setText("");
    for (int i = 0, limit = errorLabels.size(); i < limit; i++) {
        errorLabels[i]->setText("");
    }
}

void vedaEMailClient::begin_form_login_lineEdit_textChanged(const QString &text)
{
    ///\todo сделать подгон текста под размер если не будет сделано наоборот
}

void vedaEMailClient::autoLogOut()
{
//    qDebug()<<"autologout";

    mainWindow->ui->logOut_Button->click();
}

void vedaEMailClient::autoLogOutSettingsChanged()
{
    int newOption;
    if (mainWindow->ui->button0Page10->isChecked()){
        newOption = 0;
    }
    else if (mainWindow->ui->button1Page10->isChecked()) {
        newOption = 1;
    }
    else if (mainWindow->ui->button2Page10->isChecked()) {
        newOption = 2;
    }
    else {
        newOption = 3;
    }

    if (newOption != autoLogOutTimerMinuteRange){

//        qDebug()<<"autologout option changed to " << newOption << " minutes";

        autoLogOutTimerMinuteRange = newOption;
        saveAutoLogOutSettings();
        if (loggedIn){
            startAutoLogOutTimer();
        }
    }
}

void vedaEMailClient::email_by_wid_getter(const string wid, const int index, GetMailByWIDType type)
{
    QString ret;

    //1. Найти локально
    if (type != IniFileWalletsSection){
        std::vector<VedaIniEntry> local;
        if (0 != iniWrapper->getAllEntrysByName(INI_FILE_ADDRESS_BOOK_SECTION, local)){
            for (int i = 0; i < local.size(); i++){
                if (wid == local[i].value){
                    emit got_email_locally(QString::fromStdString(local[i].name), index, type);
                    return;
                    //                return QString::fromStdString(iniWrapper->datas[i].name);
                }
            }
        }
    }

    //2. Если локальный поиск успеха не дал, обратиться к серверу
    char *wid_ch = std_string_to_char_array(wid);
    if (NULL == wid_ch){
//        return ret;
        emit got_email_locally(QString(), index, type);
        return;
    }

    toggle_loading_screen(3, true);
    emit server_get_email(wstk, wid_ch, index, type);
    startAutoRenewTimer();
    startAutoLogOutTimer();

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

void vedaEMailClient::email_by_wid_placer(QString email, const int index, GetMailByWIDType type, bool from_server)
{
    std::string wid;
    if (Incoming == type){
        if ((index >= incoming_message_list.size()) || email.isEmpty()) return;
        wid = incoming_message_list[index]->get_wid();
        incoming_message_list[index]->set_author(email);
    }
    else if (Outcoming == type) {
        if ((index >= outcoming_message_list.size()) || email.isEmpty()) return;
        wid = outcoming_message_list[index]->get_wid();
        outcoming_message_list[index]->set_author(email);
    }
    else if (IniFileWalletsSection == type){
        if ((index >= iniWrapper->datas.size()) || email.isEmpty()) return;
        iniWrapper->datas[index].name = email.toLocal8Bit().data();
        emit decrementEmailsGotByWIDsCount();
        return;
    }

    if (from_server){

        //Если пришло от сервера, добавить в адресную книгу в соответствующее алфавитному порядку место
        toggle_loading_screen(3, false);
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

        bool pseudo_inserted = false;
        if (i == iniWrapper->datas.size()){
            //если этой секции в ini раньше не было
            iniWrapper->datas.push_back(temp);
        }
        else{//если секция есть
            bool inserted = false;
            auto it = iniWrapper->datas.begin();
            for (;
                 (i < iniWrapper->datas.size()) && !inserted;
                 i++) {
                if (iniWrapper->datas[i].name > email_string){
                    iniWrapper->datas.insert(it + i,temp);
                    inserted = true;
                }
                else if (iniWrapper->datas[i].name == email_string) {
                    int j = i;
                    //вставить среди записей соответствующих данному email в алфавитном порядке wid'ов
                    for (;
                         (j < iniWrapper->datas.size()) &&
                         (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[j].index.c_str())) &&
                         (iniWrapper->datas[j].name == email_string);
                         j++){
                        //случай, когда надо вставить в конец, рассмотрен ниже
                        if (iniWrapper->datas[j].value < wid) continue;
                        else if (iniWrapper->datas[j].value == wid) {
                            pseudo_inserted = inserted = true;
                            break;
                        }
                        else {
                            iniWrapper->datas.insert(it + j,temp);
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted && !pseudo_inserted){//если вышел из секции, найдя такой же email
                        if (j == iniWrapper->datas.size()){
                            iniWrapper->datas.push_back(temp);
                            inserted = true;
                        }
                        else {
                            iniWrapper->datas.insert(it + j, temp);
                            inserted = true;
                        }
                    }
                }
                else{
                    continue;
                }
            }

            //наконец, если секция была в конце и надо вставить в самый конец
            if (!inserted) iniWrapper->datas.push_back(temp);
        }
    }

    //отправить сигнал надо в конце чтобы все хорошо прочиталось
    if (Incoming == type){
        if (from_server){
//            qDebug()<<"incoming placed from server";
            ini_file_address_book_section_replaces_by_incoming_was_from_server = true;
        }
        if (0 == --ini_file_address_book_section_replaces_count_by_incoming){
//            qDebug()<<"incoming to place: " << ini_file_address_book_section_replaces_count_by_incoming;
//            qDebug()<<"all incoming placed";
            allIncomingQueryiesEnded = true;
            emit allIncomingEmailsPlaced();
        }
    }
    else if (Outcoming == type) {
        if (from_server){
//            qDebug()<<"outcoming placed from server";
            ini_file_address_book_section_replaces_by_outcoming_was_from_server = true;
        }
        if (0 == --ini_file_address_book_section_replaces_count_by_outcoming){
//            qDebug()<<"outcoming to place: " << ini_file_address_book_section_replaces_count_by_outcoming;
//            qDebug()<<"all outcoming placed";
            allOutcomingQueryiesEnded = true;
            emit allOutcomingEmailsPlaced();
        }
    }
//    if (!pseudo_inserted) iniWrapper->writeConfigFile();
}

void vedaEMailClient::emailByWIDIniFileDecrementer()
{
    if (0 == --ini_file_wallets_section_replaces_count){
        updateUnknownWalletsEverywhere();
        iniWrapper->writeConfigFile();
    }
}

void vedaEMailClient::recipientCompleterRefresher()
{
    if (allIncomingQueryiesEnded && allOutcomingQueryiesEnded){
        if (ini_file_address_book_section_replaces_by_incoming_was_from_server ||
                ini_file_address_book_section_replaces_by_outcoming_was_from_server){
            iniWrapper->writeConfigFile();
            read_address_book();
        }
    }
}

void vedaEMailClient::resetAutoLogOutTimer()
{
//    qDebug()<<"activity";
    startAutoLogOutTimer();
}

void vedaEMailClient::server_from_reg_1(int error, bool res, uint64_t _wstk)
{
//    toggle_loading_screen();

    toggle_loading_screen(9, false);

    if (res){
        wstk = _wstk;

//        char *code = QString_to_char_array("leha-kartoha");
//        if (NULL == code){
//            veda_wallet_free(wstk);
//            qDebug()<<"low memory";
//            return;
//        }

//        emit server_reg_2(code, strlen(code));
        mainWindow->ui->reg2MailLabel->setText(regMail);
        mainWindow->ui->reg2CodeLineEdit->clear();
        mainWindow->ui->reg2CodeLineEdit->setFocus();

        mainWindow->ui->stackedWidget->setCurrentIndex(4);

    }
    else {

//        if ((20011 == error) || (20012 == res) || (32 == error)){
//            show_error_label(mainWindow->ui->RegErrorLabel, "error code: " + QString::number(error));
//        }
//        else {
            show_error_label(mainWindow->ui->RegErrorLabel, "error code: " + QString::number(error));
//        }

    }
}

void vedaEMailClient::server_from_reg_2(int error)
{
    toggle_loading_screen(4, false);

    if (LIBVEDAWALLET_NOERROR == error){
        registration_form_back_ButtonClicked();

        save_wallets(mainWindow->ui->MailLineEdit1->text() +
                     mainWindow->ui->MailDogLineEdit->text() +
                     mainWindow->ui->MailLineEdit2->text() +
                     mainWindow->ui->MailDotLineEdit1->text() +
                     mainWindow->ui->MailLineEdit3->text(),
                     find_new_wid());

        mainWindow->ui->first_name_lineEdit->clear();
        mainWindow->ui->last_name_lineEdit->clear();
        mainWindow->ui->password_lineEdit_3->clear();
        mainWindow->ui->secret_lineEdit_3->clear();
        mainWindow->ui->confirmPasswordLineEdit->clear();
        mainWindow->ui->confirmSecretLineEdit->clear();
        mainWindow->ui->MailLineEdit1->clear();
        mainWindow->ui->MailLineEdit2->clear();
        mainWindow->ui->MailLineEdit3->clear();

        mainWindow->ui->reg2CodeLineEdit->clear();

        read_wallets();
        mainWindow->ui->back_Button_3->click();
        show_error_label(mainWindow->ui->BeginErrLabel, "Account successfully registered", false);
    }
    else {
        //reDo reg1
        registration_form_sign_up_ButtonClicked();

        show_error_label(mainWindow->ui->reg2ErrorLabel, "Invalid code from email (error code: " + QString::number(error) + ")");
    }
}

void vedaEMailClient::server_from_auth(uint64_t _wstk, int error, char *token_mas, uint64_t token_mas_len, char *__balance, uint64_t _token_cnt, uint64_t marked_token_cnt, char *trn_list, int trn_list_size)
{
    toggle_loading_screen(0, false);

    if (LIBVEDAWALLET_NOERROR == error){
        wstk = _wstk;
        if (NULL != token_mas){
            j_tokens = json::parse(std::string(token_mas, token_mas_len));
            free(token_mas);
        }
        if (__balance) {
            balance = (uint64_t)atoll(__balance);
            free(__balance);
            tokens_count = _token_cnt;
        }
        letters_count = marked_token_cnt;
        if (NULL != trn_list){
            j_trns = json::parse(std::string(trn_list, trn_list_size));
            free(trn_list);
        }

        if (mainWindow->ui->login_lineEdit->text().contains("(")){
            current_mail_address = mainWindow->ui->login_lineEdit->text().split("(")[0];
            current_mail_address.chop(1);
        }
        else {
            current_mail_address = mainWindow->ui->login_lineEdit->text();
        }

        mainWindow->ui->mainMenuMaillabel->setText(current_mail_address);

        mainWindow->ui->login_lineEdit->clear();
        mainWindow->ui->password_lineEdit->clear();
        mainWindow->ui->secret_lineEdit->clear();

        readReadMessageList();

        renew_frontend();

        mainWindow->ui->stackedWidget->setCurrentIndex(3);
        mainWindow->ui->stackedWidgetPage3->setCurrentIndex(1);
//        toggle_main_menu_upper_buttons(true);
//        main_menu_form->show();
//        switch_main_menu_content(incoming_message_list_form);
//        main_menu_form->ui->incoming_Button->click();
//        begin_form->hide();

        get_mails_for_wallets_section();

        loggedIn = true;
    }
    else {
        show_error_label(mainWindow->ui->BeginErrLabel, "error code: " + QString::number(error));
        autorenew_timer->stop();
        autoLogOutTimer->stop();
    }
}

void vedaEMailClient::server_from_get_tokens(int res, char *trns)
{
    toggle_loading_screen(currentMainWindowIndex, false);

    if ((LIBVEDAWALLET_NOERROR == res) || ignore_get_tokens_on_refresh){
        ignore_get_tokens_on_refresh = false;
        bool need_to_refresh_balance = true;

        char *token_mas = NULL;
        uint64_t token_mas_len = 0;
        if (j_tokens.empty()
                ||
            (j_tokens.count(JSON_BALANCE_MASS) && j_tokens[JSON_BALANCE_MASS].is_null())){
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

            //обновить транзакции
            if (NULL != trns){
                json j;
                try {
                    j = json::parse(trns);
                    j_trns = j;
                    free(trns);
                } catch (...) {

                }
            }

            forceUpdateIncomingFirstPage = (LIBVEDAWALLET_NOERROR == res);
            renew_frontend();
        }
    }
    else {
        if ((20011 == res) || (20012 == res) || (32 == res)){
            show_error_label(mainWindow->ui->refreshErrorLabel, "Connection is lost. Log out and log in please");
        }
        else if (20013 == res) {
            //не надо ничего делать, просто ничего нового нет
        }
        else {
            show_error_label(mainWindow->ui->refreshErrorLabel, "error code: " + QString::number(res));
        }
    }
}

void vedaEMailClient::server_from_send_tokens(int res)
{
    toggle_loading_screen(3, false);

    ///\todo возможно форма результата
    if (LIBVEDAWALLET_NOERROR == res){

        mainWindow->ui->lineEditPage6->clear();
        mainWindow->ui->newLetterPlainTextEdit->clear();
        /*    ui->message_textEdit->clear();
    ui->recipient_lineEdit->clear();*/
//        new_letter_form->clear();

        ignore_get_tokens_on_refresh = true;
        mainWindow->ui->refresh_Button->click();
        mainWindow->ui->incoming_Button->click();
    }
    else {
        if ((20011 == res) || (20012 == res) || (32 == res)){
            show_error_label(mainWindow->ui->NewLetterErrLabel, "Connection is lost. Log out and log in please");
        }
        else {
            show_error_label(mainWindow->ui->NewLetterErrLabel, "error code: " + QString::number(res));
        }
    }
}

void vedaEMailClient::server_from_get_wid_list_by_email(json WIDs, int res)
{
    toggle_loading_screen(3, false);

    if (LIBVEDAWALLET_NOERROR == res){
        if (!WIDs.empty()){
            try {
                //                recipient_wid = QString::fromStdString(WIDs.begin().value().get<std::string>()); ///\todo обработать случай нескольких почт на одно мыло
                WIDsByMail.clear();
                for (auto it = WIDs.begin(); it != WIDs.end(); it++) {
                    WIDsByMail << it.value().get<std::string>();
                    
                }
            } catch (...) {
                qDebug()<<"error parsing json";
                return;
            }
        }

        //обработать различные варианты количества аккаунтов, связанных с email'ом
        if (0 == WIDsByMail.size()){
            show_error_label(mainWindow->ui->NewLetterErrLabel, "This Veda account does not exist");
        }
        else if (1 == WIDsByMail.size()){
            recipient_wid = QString::fromStdString(WIDsByMail[0]);

            if (!recipientLineEditCompletions.contains(mainWindow->ui->lineEditPage6->text())){
                addWIDListToAddressBook(false);
                read_address_book();
            }

            finish_sending_message();
        }
        else { // > 1
            addWIDListToAddressBook();
            read_address_book();
            show_error_label(mainWindow->ui->NewLetterErrLabel, "This Veda account have more than one adresses, choose one");
            mainWindow->CompleteOnRecipientLineEdit();
        }

    }
    else {
        if ((20011 == res) || (20012 == res) || (32 == res)){
            show_error_label(mainWindow->ui->NewLetterErrLabel, "Connection is lost. Log out and log in please");
        }
        else if ((20025 == res) || (20010 == res)){
            show_error_label(mainWindow->ui->NewLetterErrLabel, "Recipient account does not exist");
        }
        else {
            show_error_label(mainWindow->ui->NewLetterErrLabel, "Recieving address information failure. Error code: " + QString::number(res));
        }
    }
}

void vedaEMailClient::server_from_create_empty_messages(int res)
{
    toggle_loading_screen(2, false);
    if (LIBVEDAWALLET_NOERROR == res){
        show_error_label(mainWindow->ui->BalanceErrLabel, "Your messages were successfully created They will appear on your account soon", false);
        mainWindow->ui->GetMoreMessagesButton->setDisabled(true);
        getMoreTokensButtonClickCount++;

        ignore_get_tokens_on_refresh = true;
        mainWindow->ui->refresh_Button->click();
    }
    else {
        if ((20011 == res) || (20012 == res) || (32 == res)){
            show_error_label(mainWindow->ui->BalanceErrLabel, "Connection is lost. Log out and log in please");
        }
        else {
            show_error_label(mainWindow->ui->BalanceErrLabel, "error code: " + QString::number(res));
        }
    }
}

void vedaEMailClient::on_refresh_Button_clicked()
{
    currentMainWindowIndex = mainWindow->ui->stackedWidget->currentIndex();
    toggle_loading_screen(currentMainWindowIndex, true);
    emit server_get_tokens(wstk, ignore_get_tokens_on_refresh);
    startAutoRenewTimer();
    startAutoLogOutTimer();
}

void vedaEMailClient::settings_account_form_clear_local_address_book_ButtonClicked()
{
    if (!iniWrapper->datas.empty()){
        for (int i = 0; i < iniWrapper->datas.size();){
            if (0 == strcmp(INI_FILE_ADDRESS_BOOK_SECTION, iniWrapper->datas[i].index.c_str())){
                iniWrapper->datas.erase(iniWrapper->datas.begin() + i);
            }
            else {
                i++;
            }
        }

        iniWrapper->writeConfigFile();
    }
}

void vedaEMailClient::read_letter_form_reply_ButtonClicked()
{
    mainWindow->ui->new_Button->click();

    mainWindow->ui->lineEditPage6->setText(mainWindow->ui->lineEditPage3->text());

    mainWindow->ui->newLetterPlainTextEdit->clear();

}

void vedaEMailClient::incoming_message_list_form_next_ButtonClicked()
{
    if (currentIncomingPage == (incoming_pages_num - 1)) return;

    if (currentIncomingPage == (incoming_pages_num - 2)){
        incoming_message_list_form_last_ButtonClicked();
    }
    else {
        currentIncomingPage++;
        lettersToShow = LETTERS_PER_PAGE;
        current_first_incoming_letter += LETTERS_PER_PAGE;
    }

    fill_incoming_message_list();
}

void vedaEMailClient::incoming_message_list_form_prev_ButtonClicked()
{
    if (0 == currentIncomingPage) return;

    if (1 == currentIncomingPage){
        incoming_message_list_form_first_ButtonClicked();
    }
    else {
        currentIncomingPage--;
        lettersToShow = LETTERS_PER_PAGE;
        current_first_incoming_letter -= LETTERS_PER_PAGE;
    }

    fill_incoming_message_list();
}

void vedaEMailClient::incoming_message_list_form_first_ButtonClicked()
{
    if (!forceUpdateIncomingFirstPage){
        if (0 == currentIncomingPage) return;
    }

    currentIncomingPage = 0;
    if (1 == incoming_pages_num){
        lettersToShow = incoming_letters_indexes.size();
    }
    else {
        lettersToShow = LETTERS_PER_PAGE;
    }
    current_first_incoming_letter = 0;

    fill_incoming_message_list();
}

void vedaEMailClient::incoming_message_list_form_last_ButtonClicked()
{
    if ((incoming_pages_num - 1) == currentIncomingPage) return;

    currentIncomingPage = incoming_pages_num - 1;

    lettersToShow = LastPageLetterCount;
    current_first_incoming_letter = incoming_letters_indexes.size() - lettersToShow;

//    mainWindow->ui->button7Page3->hide();
//    mainWindow->ui->button6Page3->hide();

    fill_incoming_message_list();
}

void vedaEMailClient::balance_form_getMoreMessages_ButtonClicked()
{
    char *color_text = std_string_to_char_array(" ");
    if (NULL == color_text){
        qDebug()<<"Low memory";
        return;
    }

    char *WIDR = QString_to_char_array(current_wid);
    if (NULL == WIDR){
        free(color_text);
        qDebug()<<"Low memory";
        return;
    }

    int com_mas_len = 0;
    char *com_mass = form_token_mas_by_index(tokens_indexes[0], &com_mas_len);

    emit server_create_empty_messages(wstk,
                                      color_text,
                                      MARKED_TOKENS_PER_COMISSION_TOKEN,
                                      WIDR,
                                      com_mass,
                                      com_mas_len);
    startAutoRenewTimer();
    startAutoLogOutTimer();
    toggle_loading_screen(2, true);
}

void vedaEMailClient::newLetterAttachButtonClicked()
{
    QString fileToAttachName = QFileDialog::getOpenFileName(this,
                                                            "Choose file to attach. File must be less than 50MB",
                                                            QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0],
            "");
    if (fileToAttachName.isEmpty()) return;

    QFile fileToAttach(fileToAttachName);
    if (!fileToAttach.open(QIODevice::ReadOnly)){
        show_error_label(mainWindow->ui->NewLetterErrLabel, "Opening file error");
        return;
    }

    uint64_t fileSize = 0;
    fileSize = MAX(0, fileToAttach.size());
    fileToAttach.close();

    if (0 == fileSize){
        show_error_label(mainWindow->ui->NewLetterErrLabel, "File is empty or corrupted");
        return;
    }
    else if (MAX_FILE_SIZE < (fileSize + overallAttachmentsSize)){
        show_error_label(mainWindow->ui->NewLetterErrLabel, "Attachments size must be less then 50MB");
//        attachmentsGood = false;
        return;
    }

    overallAttachmentsSize += fileSize;

    fileAttachment *tempFA;
    tempFA = new fileAttachment(false, cutFileName(fileToAttachName),
                                fileToAttachName, fileSize,
                                mainWindow->ui->newLetterScrollAreaWidgetContents);
    mainWindow->addOutcomingFile(tempFA);
    outcomingFileButtons << tempFA;
    connect(tempFA, SIGNAL(clicked(bool, const QString &)),
            this, SLOT(fileButtonClicked(bool, const QString &)));
}

void vedaEMailClient::readMessageFormDeleteButtonClicked()
{
    char *vid_ch = NULL;
    if (NULL == (vid_ch = std_string_to_char_array(currentReadMessageVID))){
        qDebug()<<"low memory";
        return;
    }

    int res = LIBVEDAWALLET_NOERROR;
    if (LIBVEDAWALLET_NOERROR != (res = veda_wallet_delete_token_by_id(wstk, vid_ch))){
        show_error_label(mainWindow->ui->NewLetterErrLabel, "Error deleting message, code: " + QString::number(res));
    }
    else {
        ignore_get_tokens_on_refresh = true;
        mainWindow->ui->refresh_Button->click();
        mainWindow->ui->incoming_Button->click();
    }
}

//void vedaEMailClient::fileToSaveChoosen()
//{
//    QString SaveLocation;
//    SaveLocation = QFileDialog::getSaveFileName(this,
//                                                "Save file as",
//                                                QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)[0] +
//                                                "/" +
//                                                mainWindow->ui->button0Page8->text(),
//                                                "");

//    if (!SaveLocation.isEmpty()){
//        QFile file(SaveLocation);
////        if (!file.open(QIODevice::WriteOnly)){
////            qDebug()<<"open file fail";
////            return;
////        }
////        else {
//        //            file.close();

//        char *data = NULL;
//        int dataLen = 0;

//        char *curVID = std_string_to_char_array(currentReadMessageVID);
//        if (NULL == curVID){
//            qDebug()<<"low memory";
//            return;
//        }

//        char *fileName = QString_to_char_array(mainWindow->ui->button0Page8->text());
//        if (NULL == fileName){
//            free(curVID);
//            qDebug()<<"low memory";
//            return;
//        }
//        int res = veda_wallet_decrypt_marked_token_file(wstk,
//                                                        curVID,
//                                                        fileName,
//                                                        &data,
//                                                        dataLen);
//        free(curVID);
//        free(fileName);
//        if (LIBVEDAWALLET_NOERROR != res){
//            qDebug()<<"decrypt file fail";
//            return;
//        }
//        else {
//            if (!file.open(QIODevice::WriteOnly)){
//                qDebug()<<"open file fail";
//            }
//            else {
//                file.write(data, dataLen);
//                file.close();
//            }
//            free(data);
//        }
////        }
//    }
//}

void vedaEMailClient::fileButtonClicked(bool incoming, const QString &fileName/*, const QString &fileNameWithPath*/)
{
    if (incoming){
        //клик по входящему файлу приведет к его сохранению
        QString SaveLocation;
        SaveLocation = QFileDialog::getSaveFileName(this,
                                                    "Save file as",
                                                    QStandardPaths::standardLocations(QStandardPaths::DownloadLocation)[0] +
                                                    "/" +
                                                    fileName,
                                                    "");

        if (!SaveLocation.isEmpty()){
            QFile file(SaveLocation);
            char *data = NULL;
            int dataLen = 0;

            char *curVID = std_string_to_char_array(currentReadMessageVID);
            if (NULL == curVID){
                qDebug()<<"low memory";
                return;
            }

            char *_fileName = QString_to_char_array(fileName);
            if (NULL == _fileName){
                free(curVID);
                qDebug()<<"low memory";
                return;
            }
            int res = veda_wallet_decrypt_marked_token_file(wstk,
                                                            curVID,
                                                            _fileName,
                                                            &data,
                                                            dataLen);
            free(curVID);
            free(_fileName);
            if (LIBVEDAWALLET_NOERROR != res){
                qDebug()<<"decrypt file fail";
                return;
            }
            else {
                if (!file.open(QIODevice::WriteOnly)){
                    qDebug()<<"open file fail";
                }
                else {
                    file.write(data, dataLen);
                    file.close();
                }
                free(data);
            }
        }
    }
    else { //outcoming
        //клик по исходящему файлу означает его удаление из списка прикрепленных
        fileAttachment *_sender = (fileAttachment*)sender();

        uint64_t fileSize = _sender->getFileSize();
        if (overallAttachmentsSize < fileSize){
            overallAttachmentsSize = 0;
        }
        else {
            overallAttachmentsSize -= fileSize;
        }

//        if (MAX_FILE_SIZE > overallAttachmentsSize){
////            attachmentsGood = true;
//        }

        mainWindow->removeOutcomingFile(_sender);
        outcomingFileButtons.removeOne(_sender);
        disconnect(_sender, SIGNAL(clicked(bool, const QString &)),
                   this, SLOT(fileButtonClicked(bool, const QString &)));
        delete _sender;
    }
}

void vedaEMailClient::reg2ConfirmButtonClicked()
{
    if (mainWindow->ui->reg2CodeLineEdit->text().size() != REG_2_CODE_SIZE){
        show_error_label(mainWindow->ui->reg2ErrorLabel, "Code size is wrong");
        return;
    }
    else {
        int codeLen = 0;
        char *code = QString_to_char_array(mainWindow->ui->reg2CodeLineEdit->text(), &codeLen);
        if (NULL == code){
            qDebug()<<"Low memory";
            return;
        }

        toggle_loading_screen(4, true);

        emit server_reg_2(code, codeLen);
    }
}

void vedaEMailClient::reg2BackButtonClicked()
{
    deleteUncompletedBox();

    mainWindow->ui->stackedWidget->setCurrentIndex(9);
}

void vedaEMailClient::setPreferredNodeButtonClicked()
{
    if (preferredNodeEditable){
        if (mainWindow->ui->preferredNodeLineEdit1->text().isEmpty() ||
            mainWindow->ui->preferredNodeLineEdit2->text().isEmpty() ||
            mainWindow->ui->preferredNodeLineEdit3->text().isEmpty() ||
            mainWindow->ui->preferredNodeLineEdit4->text().isEmpty()){
            show_error_label(mainWindow->ui->settingsErrorLabel, "invalid IP address");
            return;
        }

        mainWindow->ui->preferredNodeLineEdit1->setReadOnly(true);
        mainWindow->ui->preferredNodeLineEdit2->setReadOnly(true);
        mainWindow->ui->preferredNodeLineEdit3->setReadOnly(true);
        mainWindow->ui->preferredNodeLineEdit4->setReadOnly(true);
        savePreferredNode(
                    mainWindow->ui->preferredNodeLineEdit1->text() +
                    mainWindow->ui->preferredNodeDotLineEdit1->text() +
                    mainWindow->ui->preferredNodeLineEdit2->text() +
                    mainWindow->ui->preferredNodeDotLineEdit2->text() +
                    mainWindow->ui->preferredNodeLineEdit3->text() +
                    mainWindow->ui->preferredNodeDotLineEdit3->text() +
                    mainWindow->ui->preferredNodeLineEdit4->text()
                    );
        show_error_label(mainWindow->ui->settingsErrorLabel, "Changes are saved. Restart Veda Email to apply them", false);

        preferredNodeEditable = false;
        mainWindow->ui->setPreferredNodeButton->setText(SET_PREFERRED_NODE_BUTTON_EDIT_TEXT);
    }
    else {
        mainWindow->ui->preferredNodeLineEdit1->setReadOnly(false);
        mainWindow->ui->preferredNodeLineEdit2->setReadOnly(false);
        mainWindow->ui->preferredNodeLineEdit3->setReadOnly(false);
        mainWindow->ui->preferredNodeLineEdit4->setReadOnly(false);

        preferredNodeEditable = true;
        mainWindow->ui->setPreferredNodeButton->setText(SET_PREFERRED_NODE_BUTTON_APPLY_TEXT);
        mainWindow->ui->preferredNodeLineEdit1->setFocus();
    }
}

void vedaEMailClient::beginSettingsButtonClicked()
{
    on_settings_Button_clicked();
}

bool doubleClickFilter::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseButtonDblClick){
        emit doubleClick();
    }

    return QObject::eventFilter(obj, ev);
}

bool InactivityFilter::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::KeyPress ||
            ev->type() == QEvent::MouseMove ||
            ev->type() == QEvent::MouseButtonPress ||
            ev->type() == QEvent::MouseButtonRelease ||
            ev->type() == QEvent::MouseButtonDblClick)
        emit activity();

    return QObject::eventFilter(obj, ev);
}
