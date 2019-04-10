#ifndef LIBVEDAWALLET_H
#define LIBVEDAWALLET_H

/*!
    \file
    \brief Функции кошелька
    \details Заголовочный файл для использования библиотеки API-функций работы с VEDA PLATFORM
    \author Veda team
    \version 1.0
    \date Июнь 2018 года

*/

#include "stdint.h"
#include "inttypes.h"
#include "libvedawallet_global.h"


#define LIBVEDAWALLET_NOERROR                0                         ///< Функция отработала без ошибок
#define LIBVEDAWALLET_ERROR                  20000                     ///< Неизвестная ошибка
#define LIBVEDAWALLET_ERROR_PARAMS           LIBVEDAWALLET_ERROR + 1   ///< Неверные входные параметры
#define LIBVEDAWALLET_ERROR_OPEN             LIBVEDAWALLET_ERROR + 2   ///< Ошибка открытия контейнера
#define LIBVEDAWALLET_ERROR_READING          LIBVEDAWALLET_ERROR + 3   ///< Ошибка чтения данных из контейнера
#define LIBVEDAWALLET_ERROR_WRITING          LIBVEDAWALLET_ERROR + 4   ///< Ошибка записи данных в контейнер
#define LIBVEDAWALLET_ERROR_MEMORY           LIBVEDAWALLET_ERROR + 5   ///< Ошибка выделения памяти
#define LIBVEDAWALLET_ERROR_CRYPTLIBS        LIBVEDAWALLET_ERROR + 6   ///< Ошибка инициализации библиотеки
#define LIBVEDAWALLET_ERROR_CRYPTINIT        LIBVEDAWALLET_ERROR + 7   ///< Ошибка инициализации криптоалгоритма
#define LIBVEDAWALLET_ERROR_CRYPTPGEN        LIBVEDAWALLET_ERROR + 8   ///< Ошибка генерации
#define LIBVEDAWALLET_ERROR_CRYPT            LIBVEDAWALLET_ERROR + 9   ///< Ошибка криптоалгоритма
#define LIBVEDAWALLET_ERROR_ELNFOUND         LIBVEDAWALLET_ERROR + 10  ///< Ошибка элемент не найден
#define LIBVEDAWALLET_ERROR_SEND             LIBVEDAWALLET_ERROR + 11  ///< Ошибка при отправке данных
#define LIBVEDAWALLET_ERROR_RECV             LIBVEDAWALLET_ERROR + 12  ///< Ошибка при получении данных
#define LIBVEDAWALLET_ERROR_RECVBADCODE      LIBVEDAWALLET_ERROR + 13  ///< Ошибка при получении - получен код ошибки
#define LIBVEDAWALLET_ERROR_TRNCREATE        LIBVEDAWALLET_ERROR + 14  ///< Ошибка при создании транзакции
#define LIBVEDAWALLET_ERROR_JSON_INCORRECT   LIBVEDAWALLET_ERROR + 15  ///< Ошибка - неверная структура JSON
#define LIBVEDAWALLET_ERROR_EXCEPTION        LIBVEDAWALLET_ERROR + 16  ///< Ошибка - исключение
#define LIBVEDAWALLET_ERROR_GET_SAT          LIBVEDAWALLET_ERROR + 17  ///< Ошибка получение списка соседей с узла
#define LIBVEDAWALLET_ERROR_SQL              LIBVEDAWALLET_ERROR + 18  ///< Ошибка формирования SQL запроса
#define LIBVEDAWALLET_ERROR_SQL_PARAMS       LIBVEDAWALLET_ERROR + 19  ///< Ошибка параметров SQL
#define LIBVEDAWALLET_ERROR_SQL_WRITE        LIBVEDAWALLET_ERROR + 20  ///< Ошибка записи в БД
#define LIBVEDAWALLET_ERROR_SQL_READ         LIBVEDAWALLET_ERROR + 21  ///< Ошибка чтения из БД
#define LIBVEDAWALLET_ERROR_SQL_OPEN         LIBVEDAWALLET_ERROR + 22  ///< Ошибка открытия БД
#define LIBVEDAWALLET_ERROR_COMPARE          LIBVEDAWALLET_ERROR + 23  ///< Ошибка сравнения
#define LIBVEDAWALLET_ERROR_SYSTEM           LIBVEDAWALLET_ERROR + 24  ///< Ошибка системная



/// \brief Регистрация кошелька этап 1
/// \param[in]  wstk дескриптор стека кошелька
/// \param[in]  pass пароль
/// \param[in]  pass_len длина пароля
/// \param[in]  secret кодовое слово
/// \param[in]  secret_len длина кодового слова
/// \param[in]  cert поля сертификата в формате json (строка)
/// \param[in]  cert_len длина поля cert
/// \param[out] error код результата
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_register_1(uint64_t wstk,
                                      const char *pass, int pass_len,
                                      const char *secret, int secret_len,
                                      const char *cert, int cert_len,
                                      int &error);


/// \brief Регистрация кошелька этап 2
/// \param[in] wstk дескриптор стека кошелька
/// \param[in] email_code код подтверждения
/// \param[in] code_len длина кода подтверждения
/// \param[out] error код результата
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
/// В случае ошибки необходимо вызвать функции veda_wallet_CryptoBoxFree и veda_wallet_CryptoBoxDropBox
///
DLL_IMPORT int veda_wallet_register_2(uint64_t wstk, const char *email_code, int code_len, int &error);


/// \brief Открыть локальный кошелек
/// \param[in]  wstk дескриптор стека кошелька
/// \param[in]  cryptobox_name имя файла криптоконтейнера - освободить память функцией veda_wallet_CryptoBoxFree (при logout)
/// \param[in]  cryptobox_name_len длина имени файла криптоконтейнера
/// \param[in]  pass пароль
/// \param[in]  pass_len длина пароля
/// \param[in]  secret кодовое слово
/// \param[in]  secret_len длина кодового слова
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
DLL_IMPORT int veda_wallet_open(uint64_t wstk,
                              const char *cryptobox_name, int cryptobox_name_len,
                              const char *pass, int pass_len,
                              const char *secret, int secret_len);

/// \brief Авторизация кошелька
/// \param[in]  wstk дескриптор стека кошелька
/// \param[in]  cryptobox_name имя файла криптоконтейнера - освободить память функцией veda_wallet_CryptoBoxFree (при logout)
/// \param[in]  cryptobox_name_len длина имени файла криптоконтейнера
/// \param[in]  pass пароль
/// \param[in]  pass_len длина пароля
/// \param[in]  secret кодовое слово
/// \param[in]  secret_len длина кодового слова
/// \param[out] error код результата
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_authorization(uint64_t wstk,
                                         const char *cryptobox_name, int cryptobox_name_len,
                                         const char *pass, int pass_len,
                                         const char *secret, int secret_len,
                                         int &error);


/// \brief Формирование массива токенов из криптоконтейнера
/// \param[in] wstk дескриптор стека кошелька
/// \param[out] token_mas json массив токенов - освободить как станет ненужным функцией veda_wallet_free_param (при logout)
/// \param[out] token_mas_len размер массива токенов
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_form_token_mas(uint64_t wstk,
                                          char **token_mas,
                                          uint64_t &token_mas_len);


/// \brief Проверка токенов и запрос баланса
/// \param[in] wstk дескриптор стека кошелька
/// \param[in] token_mas_in json массив токенов
/// \param[in] token_mas_len_in размер массива токенов
/// \param[out] token_mas_out json массив токенов - освободить как станет ненужным функцией veda_wallet_free_param (при logout)
/// \param[out] token_mas_len_out размер массива токенов
/// \param[out] balance баланс кошелька - освободить как станет ненужным функцией veda_wallet_free_param
/// \param[out] token_cnt количество немаркированных токенов
/// \param[out] marked_token_cnt количество маркированных токенов
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_init_balance(uint64_t wstk,
                                        const char *token_mas_in,
                                        uint64_t token_mas_len_in,
                                        char **token_mas_out,
                                        uint64_t &token_mas_len_out,
                                        char **balance,
                                        uint64_t &token_cnt,
                                        uint64_t  &marked_token_cnt);


/// \brief Обновление баланса кошелька
/// \param[in] wstk дескриптор стека кошелька
/// \param[in] token_mas_in json массив токенов
/// \param[in] token_mas_len_in размер массива токенов
/// \param[out] token_mas_out json массив токенов - освободить как станет ненужным функцией veda_wallet_free_param (при logout)
/// \param[out] token_mas_len_out размер массива токенов
/// \param[out] balance баланс кошелька - освободить как станет ненужным функцией veda_wallet_free_param
/// \param[out] token_cnt количество немаркированных токенов
/// \param[out] marked_token_cnt количество маркированных токенов
/// \return код ошибки, в случае успеха LIBVEDAWALLET_NOERROR
/// LIBVEDAWALLET_ERROR_ELNFOUND - нет токенов
/// Если LIBVEDAWALLET_NOERROR == 0 и (token_mas_out == 0 или token_mas_len_out == 0),
/// то работаем с массивом token_mas_in - он актуальный, иначе работаем с token_mas_out
DLL_IMPORT int veda_wallet_refresh_balance(uint64_t wstk,
                                           const char *token_mas_in,
                                           uint64_t token_mas_len_in,
                                           char **token_mas_out,
                                           uint64_t &token_mas_len_out,
                                           char **balance,
                                           uint64_t  &token_cnt,
                                           uint64_t  &marked_token_cnt);


/// \brief Получение номера последней исходящей транзакиис аккаунта (с кошелька)
/// \param[in] wstk дескриптор стека кошелька
/// \param[in] wid_sender адрес отправителя (64 байта)
/// \param[out] num_trn счётчик исходящих транзакций с аккаунта (с кошелька)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_cnt_trn_out(uint64_t wstk,
                                           const char *wid_sender,
                                           uint64_t &num_trn);


/// \brief Инициализация библиотеки VEDA_WALLET
/// вызывается один раз перед выполнением основных инструкий программы (например, в начале функции main())
/// \param[out] hlib дескриптор библиотеки
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_init_library(uint64_t &hlib);


/// \brief Освобождение библиотеки VEDA_WALLET
/// вызывается один раз перед завершением программы (например, в конце функции main())
/// \param[in, out] hlib дескриптор библиотеки
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_stop_library(uint64_t &hlib);


/// \brief Инициализация кошелька
/// перед инициализацией кошелька убедитесь, что была проведена инициализация библиотеки
/// veda_wallet_init_crypto_engine
/// \param[in]  hlib дескриптор библиотеки
/// \param[in]  dir  директория хранения криптоконтейнеров
/// \param[in]  ip   ip-адрес НОДы (из настроек приложения)
/// \param[out] wstk дескриптор стека кошелька
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_init(uint64_t hlib, const char *dir, const char *ip, uint64_t &wstk);


/// \brief Освобождение кошелька
/// \param[in] wstk дескриптор стека кошелька
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_free(uint64_t &wstk);


/// \brief Освобождение дескриптора криптоконтейнера
/// \param[in] wstk дескриптор стека кошелька
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_CryptoBoxFree(uint64_t wstk);


/// \brief Удаление файла криптоконтейнера
/// \param[in] wstk дескриптор стека кошелька (8 байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_CryptoBoxDropBox(uint64_t wstk);


/// \brief Закрытие соединения с нодой
/// \param[in] wstk дескриптор стека кошелька
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_close_connection(uint64_t wstk);


/// \brief Освобождение памяти
/// \param [in] param Строка (память выделена в *param)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_free_param_str(char **param);


/// \brief Получение списка транзакций по ID кошелька
/// \param[in] wstk дескриптор стека кошелька
/// \param[out] trn_list массив транзакций в виде json - освободить как станет ненужным veda_wallet_free_param_str
/// \param[out] trn_list_size размер массива транзакций (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_trn_list_by_wid(uint64_t wstk, char **trn_list, int &trn_list_size);


/// \brief Получение списка транзакций из своего хранилища
/// \param[in] wstk дескриптор стека кошелька
/// \param[out] trn_list массив транзакций в виде json - освободить как станет ненужным veda_wallet_free_param_str
/// \param[out] trn_list_size размер массива транзакций (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_trn_list_from_local_db(uint64_t wstk, char **trn_list, int &trn_list_size);


/// \brief Получение json-описания транзакции по ID
/// \param[in] wstk дескриптор стека кошелька
/// \param[in] id   идентификатор транзакции
/// \param[out] jtrn     транзакция в виде json - освободить как станет ненужным veda_wallet_free_param_str
/// \param[out] jtrn_len размер jtrn (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_trn_json_by_tid(uint64_t wstk, const char *tid, char **jtrn, int &jtrn_len);


/// \brief Получение json-описания токена по ID
/// \param[in] wstk дескриптор стека кошелька
/// \param[in] id   идентификатор токена
/// \param[out] jtoken токен в виде json - освободить как станет ненужным veda_wallet_free_param_str
/// \param[out] jtoken_len размер jtoken (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_token_json_by_id(uint64_t wstk, const char *id, char **jtoken, int &jtoken_len);


/// \brief Получение комиссии
/// \param[in] trn_type тип транзакции
/// \param[in] trn_nominal номинал, участвующий в транзакции
/// \param[out] comission комиссия
/// \param[out] precent процент от общего номинала, участвующего в транзакции, который составляет комиссия
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_comission(int trn_type,
                                         uint64_t trn_nominal,
                                         uint64_t &comission,
                                         uint64_t &percent);


/// \brief Создание и отправка транзакции (внутри отправка транзакции и отправка токенов)
/// \param[in] wstk         дескриптор стека кошелька
/// \param[in] num_trn      счётчик исходящих транзакций с аккаунта (с кошелька)
/// \param[in] type         тип транзакции
/// \param[in] wid_recipient    адрес получателя (64 байта)
/// \param[in] token_mas        json массив токенов
/// \param[in] token_mas_len    размер массива токенов (байт)
/// \param[in] token_mas_comission      json массив комиссионных токенов
/// \param[in] token_mas_comission_len  размер массива комиссионных токенов (байт)
/// \param[in] data                     строка для помещения в транзакцию
/// \param[in] data_len                 размер строки (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_send_transaction(uint64_t wstk,
                                            uint64_t &num_trn,
                                            int   type,
                                            const char *wid_recipient,
                                            const char *token_mas, uint64_t token_mas_len,
                                            const char *token_mas_comission, uint64_t token_mas_comission_len,
                                            const char *data = 0, uint64_t data_len = 0);


/// \brief Получение своих токенов из сети
/// \param[in] wstk дескриптор стека кошелька
/// \return код ошибки, в случае успеха (токены получены) VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_tokens(uint64_t wstk);


/// \brief Получение сертификата в виде json-массива
/// \param[in]  wstk     дескриптор стека кошелька
/// \param[out] jcert    json-массив полей сертификата - освободить как станет ненужным veda_wallet_free_param_str
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_X509_attributes(uint64_t wstk, char **jcert);


/// \brief Изменение пароля и кодового слова кошелька
/// \param[in] wstk          дескриптор стека кошелька
/// \param[in] password      новый пароль
/// \param[in] passwordlen   длина пароля
/// \param[in] codeword      кодовое слово
/// \param[in] codewordlen   длина кодового слова
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_change_wallet_password_codeword(uint64_t wstk, const char *password, int passwordlen, const char *codeword, int codewordlen);


/// \brief Изменение пароля и кодового слова кошелька
/// \param[in] wstk          дескриптор стека кошелька
/// \param[in] letter        текст пользовательского обращения в службу поддержки
/// \param[in] letter_size   длина текста
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_send_letter_to_support(uint64_t wstk, const char *letter, int letter_size);


/// \brief Проверка на валидность криптоконтейнера
/// \param[in] cryptobox_path       путь к криптоконтейнеру
/// \param[in] cryptobox_path_len   длина пути
/// \param[in] password             пароль
/// \param[in] passwordlen          длина пароля
/// \param[in] codeword             кодовое слово
/// \param[in] codewordlen          длина кодового слова
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_is_cryptobox_valid(const char *cryptobox_path, int cryptobox_path_len,
                                              const char *password, int passwordlen,
                                              const char *codeword, int codewordlen);


/// \brief Функция добавляет запись в БД SQLITE
/// \param[in] pwstk        дескриптор стека кошелька
/// \param[in] jext         JSON массив с котнрактом и методами
/// \param[in] jext_len     длина jext
/// \param[in] jcms         JSON массив с токенами
/// \param[in] jcms_len     длина jcms
/// \param[in] num_trn      счётчик исходящих транзакций с аккаунта (с кошелька)
/// \param[in] data         поле data - текстовое поле в транзакции
/// \param[in] data_len     размер data (байт)
/// \return Код ошибки, или 0
///
DLL_IMPORT int veda_wallet_deploy_contract(uint64_t wstk,
                                           const char *jext, int jext_len,
                                           const char *jcms, int jcms_len,
                                           uint64_t num_trn,
                                           const char *data = 0, int data_len = 0);


/// \brief Функция получения списка контрактов из сети
/// \param[in]  wstk         дескриптор стека кошелька
/// \param[out] jmas         JSON массив со списком контрактов (см формат)
/// \param[out] jmas_len     длина jmas
/// \return Код ошибки, или 0
///
DLL_IMPORT int veda_wallet_get_contract_list(uint64_t wstk, char **jmas, int &jmas_len);


/// \brief Отправка транзакции на контракт (внутри отправка транзакции и отправка токенов)
/// \param[in] wstk         дескриптор стека кошелька
/// \param[in] num_trn      счётчик исходящих транзакций с аккаунта (с кошелька)
/// \param[in] type         тип транзакции (TRN_RUNCONTRACT)
/// \param[in] wid_recipient    адрес получателя (64 байта) - CID
/// \param[in] token_mas        json массив токенов
/// \param[in] token_mas_len    размер массива токенов (байт)
/// \param[in] token_mas_comission      json массив комиссионных токенов
/// \param[in] token_mas_comission_len  размер массива комиссионных токенов (байт)
/// \param[in] token_mas_comission      json массив методов
/// \param[in] token_mas_comission_len  размер массива методов (байт)
/// \param[in] data                     поле data - текстовое поле в транзакции
/// \param[in] data_len                 размер data (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_run_contract(uint64_t wstk,
                                        uint64_t &num_trn,
                                        int   type,
                                        const char *wid_recipient,
                                        const char *token_mas, uint64_t token_mas_len,
                                        const char *token_mas_comission, uint64_t token_mas_comission_len,
                                        const char *extension, uint64_t extension_len,
                                        const char *data = 0, uint64_t data_len = 0);


/// \brief Отправка транзакции на контракт (внутри отправка транзакции и отправка токенов)
/// \param[in] wstk         дескриптор стека кошелька
/// \param[in] num_trn      счётчик исходящих транзакций с аккаунта (с кошелька)
/// \param[in] type         тип транзакции (TRN_EXCHANGE)
/// \param[in] wid_recipient    адрес получателя (64 байта) - CID
/// \param[in] jtoken_mas       json массив старых токенов
/// \param[in] jtokenmas_len    размер массива токенов (байт)
/// \param[in] jnominal         json массив новых номиналов
/// \param[in] jnominal_len     размер массива новых номиналов (байт)
/// \param[in] data             поле data - текстовое поле в транзакции
/// \param[in] data_len         размер data (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_exchange_token(uint64_t wstk,
                                          uint64_t &num_trn,
                                          int   type,
                                          const char *wid_recipient,
                                          const char *jtoken_mas, int jtokenmas_len,
                                          const char *jnominal, int jnominal_len,
                                          const char *data = 0, int data_len = 0);


/// \brief Создание цветных токенов (внутри отправка транзакции и отправка токенов)
/// \param[in] wstk                 дескриптор стека кошелька
/// \param[in] num_trn              счётчик исходящих транзакций с аккаунта (с кошелька)
/// \param[in] name_token           название токена
/// \param[in] count                количество токенов
/// \param[in] wid_recipient        адрес получателя (64 байта)
/// \param[in] token_cms_mas        json массив комиссионных токенов
/// \param[in] token_cms_mas_len    размер token_cms_mas (байт)
/// \param[in] cypher               признак шифрования поля color
/// \param[in] rsakey               публичный ключ RSA получателя токенов
/// \param[in] rsakeylen            размер ключа (байт)
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR

DLL_IMPORT int veda_wallet_create_marked_tokens(uint64_t wstk,
                                                uint64_t &num_trn,
                                                char *name_token,
                                                int count,
                                                char *wid_recipient,
                                                char *token_cms_mas,
                                                int   token_cms_mas_len,
                                                int   cypher = 0,
                                                char *rsakey = 0x0,
                                                int   rsakeylen = 0);


/// \brief Дешифрование маркированного токена
/// \param[in] wstk             дескриптор стека кошелька
/// \param[in] token            токен, json-массив
/// \param[out] data            дешифрованные данные
/// \param[out] data_len        размер данных
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_decrypt_marked_token(uint64_t wstk, const char *token, char **data, int &data_len);


/// \brief Вызов статического метода контракта
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] _CID                     идентификатор контракта
/// \param[in] _metod_call              наименование метода
/// \param[in] _metod_call_len          рамзер _metod_call
/// \param[in] _params                  параметры метода _metod_call
/// \param[in] _params_len              размер _params
/// \param[in] _answer_contract         ответ
/// \param[in] _answer_contract_len     размер _answer_contract
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_execute_contract_func(uint64_t wstk,
                                                 const char *_CID,
                                                 const char *_metod_call, uint64_t _metod_call_len,
                                                 const char *_params, uint64_t _params_len,
                                                 char **_answer_contract, uint64_t *_answer_contract_len);


/// \brief Получение RSA ключа шифрования по wid
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] wid                      идентификатор кошелька (адрес получателя)
/// \param[out] rsapubkey               RSA ключ
/// \param[out] rsapubkey_len           размер rsapubkey
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_rsa_pub_key(uint64_t wstk, const char *wid, char **rsapubkey, int &rsapubkey_len);


/// \brief Получение публичного GOST ключа по wid
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] wid                      идентификатор кошелька (адрес)
/// \param[out] gostpubkey              GOST ключ
/// \param[out] gostpubkey_len          размер gostpubkey
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_gost_pub_key(uint64_t wstk, const char *wid, char **gostpubkey, int &gostpubkey_len);


/// \brief Изменение цветных полей токенов
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] id                       идентификатор токена
/// \param[in] recipient                WID получателя
/// \param[in] color_name               поле color_name
/// \param[in] color_name_len     		размер color_name
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_edit_marked_token_by_id(uint64_t wstk, const char *id, const char *recipient, char *color_name, int color_name_len);



/// \brief Проверка подписи токена
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] id                       идентификатор токена
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_check_token_sign(uint64_t wstk, const char *id);


/// \brief Получение EMAIL по wid
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] wid                      идентификатор кошелька (адрес)
/// \param[out] email                   email-адрес
/// \param[out] email_len               размер email
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_email(uint64_t wstk, const char *wid, char **email, int &email_len);


/// \brief Получение списка wid по EMAIL
/// \param[in] wstk                     дескриптор стека кошелька
/// \param[in] email                    email-адрес
/// \param[out] widlist                 список адресов (json)
/// \param[out] widlist_len             размер widlist
/// \return код ошибки, в случае успеха VEDAWALLET_NOERROR
///
DLL_IMPORT int veda_wallet_get_wid_list_by_email(uint64_t wstk, const char *email, char **widlist, int &widlist_len);

#endif // LIBVEDAWALLET_H
