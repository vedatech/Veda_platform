#include "vedaEMailClient.h"

server_functions::server_functions(QObject *parent) : QObject(parent)
{
    wstk = 0;
}

server_functions::~server_functions()
{

}

void server_functions::reg_1(uint64_t engine, char *dir, char *ip, char *pass, int pass_len, char *secret, int secret_len, char *cert, int cert_len, char *mailToCheck)
{
    int error = 0;
    bool res = true;
    int res2 = 0;

//    char *WIDList = NULL;
//    int WIDListLen = 0;
//    json WIDListJ;

    if (wstk != 0){
        veda_wallet_free(wstk);
        wstk = 0;
    }


    if (LIBVEDAWALLET_NOERROR != (res2 = veda_wallet_init(engine,
                                                  dir,
                                                  ip,
                                                  wstk))){
        res = false;
    }

//    if (LIBVEDAWALLET_NOERROR == (res2 = veda_wallet_get_wid_list_by_email(wstk,
//                                             mailToCheck,
//                                             &WIDList,
//                                                                           WIDListLen))){

//        try {
//            WIDListJ = json::parse(std::string(WIDList, WIDListLen));
//        } catch (...) {
//            qDebug()<<"error parsing json";
//        }
//    }
//    else {

//    }
    if (mailToCheck){
        free(mailToCheck);
    }

    if (res){
        if (res && LIBVEDAWALLET_NOERROR != ( res2 = veda_wallet_register_1(wstk,
                                                                            pass,
                                                                            pass_len,
                                                                            secret,
                                                                            secret_len,
                                                                            cert,
                                                                            cert_len,
                                                                            error)))
            res = false;
    }

    free(ip);
    free(pass);
    free(secret);
    free(cert);
    emit reg_1_res(res2, res, wstk);
}

void server_functions::reg_2(char *email_code, int code_len)
{
    int error = 0;
    int ret;
//    bool res = true;

    if (LIBVEDAWALLET_NOERROR != (ret = veda_wallet_register_2(wstk,
                                                        email_code,
                                                        code_len,
                                                        error))){
//        res = false;
//        veda_wallet_CryptoBoxDropBox(wstk); //уже делается внутри библиотечной функции
    }

    veda_wallet_free(wstk);
    free(email_code);
    emit reg_2_res(ret);
}

void server_functions::auth(uint64_t engine, char *dir, char *ip, char *cryptobox_name, int cryptobox_name_len, char *pass, int pass_len, char *secret, int secret_len)
{
    int error = 0,
            ires = 0;
    bool emitted = false;
    bool ignore_init_balance = false;
    uint64_t wstk = 0;
    char *token_mas = NULL;
    uint64_t token_mas_len = 0;
    char *token_mas_out = NULL;
    uint64_t token_mas_len_out = 0;
    char *balance = NULL;
    uint64_t token_cnt = 0;
    uint64_t marked_token_cnt = 0;
    char *trn_list = NULL;
    int trn_list_size = 0;

    //1. инициализировать кошелек
    if (LIBVEDAWALLET_NOERROR != veda_wallet_init(engine,
                                                  dir,
                                                  ip,
                                                  wstk)){
        emit auth_res(0x0,
                      0x0,
                      NULL,
                      0,
                      NULL,
                      0,
                      0,
                      trn_list,
                      trn_list_size);
        emitted = true;
    }

    //2. авторизоваться
    if (!emitted){
        if (LIBVEDAWALLET_NOERROR != (ires = veda_wallet_authorization(wstk,
                                                                       cryptobox_name,
                                                                       cryptobox_name_len,
                                                                       pass,
                                                                       pass_len,
                                                                       secret,
                                                                       secret_len,
                                                                       error))){
            veda_wallet_free(wstk);
            wstk = 0;
            emit auth_res(0x0,
                          ires,
                          NULL,
                          0,
                          NULL,
                          0,
                          0,
                          trn_list,
                          trn_list_size);
            emitted = true;
        }
    }

    //3. запросить из сети данные о новых токенах
    if (!emitted){
        veda_wallet_get_tokens(wstk);

        //4. сформировать массив токенов
        if (LIBVEDAWALLET_NOERROR != (ires = veda_wallet_form_token_mas(wstk,
                                                                        &token_mas,
                                                                        token_mas_len))){
            if (LIBVEDAWALLET_ERROR_ELNFOUND == ires){

            }
            else{

            }
            ignore_init_balance = true;
        }
    }

    if (!emitted && !ignore_init_balance){
        if (LIBVEDAWALLET_NOERROR != veda_wallet_init_balance(wstk,
                                                              token_mas,
                                                              token_mas_len,
                                                              &token_mas_out,
                                                              token_mas_len_out,
                                                              &balance,
                                                              token_cnt,
                                                              marked_token_cnt)){
            if (token_mas) free(token_mas);
            token_mas = NULL;
        }
        if (token_mas) free(token_mas);
//        emit auth_res(wstk,
//                      0x0,
//                      token_mas_out,
//                      token_mas_len_out,
//                      balance,
//                      token_cnt,
//                      marked_token_cnt,
//                      trn_list,
//                      trn_list_size);
//        emitted = true;
    }

    if (!emitted) {
        if (LIBVEDAWALLET_NOERROR != veda_wallet_get_trn_list_from_local_db(wstk,
                                                                            &trn_list,
                                                                            trn_list_size)){
           veda_wallet_get_trn_list_by_wid(wstk, &trn_list, trn_list_size);
        }

        emit auth_res(wstk,
                      0x0,
                      token_mas_out,
                      token_mas_len_out,
                      balance,
                      token_cnt,
                      marked_token_cnt,
                      trn_list,
                      trn_list_size);
        emitted = true;
    }




        free(ip);
        free(cryptobox_name);
        free(pass);
        free(secret);
}

void server_functions::get_tokens(uint64_t wstk, bool with_trn_history_anyway)
{
    int res = veda_wallet_get_tokens(wstk);
    char *trns = NULL;
    int len = 0;

    if ((LIBVEDAWALLET_NOERROR == res) || with_trn_history_anyway){
        if (LIBVEDAWALLET_NOERROR != veda_wallet_get_trn_list_from_local_db(wstk, &trns, len)){
            veda_wallet_get_trn_list_by_wid(wstk, &trns, len);
        }
    }

    emit get_tokens_res(res, trns);
}

void server_functions::send_tokens(uint64_t wstk, int type, char *wid_recipient, char *token_mas, uint64_t token_mas_len, char *token_mas_comission, uint64_t token_mas_comission_len, char *data, uint64_t data_len)
{
    uint64_t tr_num = 0;
    int res;
    res = veda_wallet_send_transaction(wstk,
                                       tr_num,
                                       type,
                                       wid_recipient,
                                       token_mas, token_mas_len,
                                       token_mas_comission, token_mas_comission_len,
                                       data, data_len);
    emit send_tokens_res(res);
    free(wid_recipient);
    free(token_mas);
    free(token_mas_comission);
}

void server_functions::get_email(uint64_t wstk, char *wid, const int index, GetMailByWIDType type)
{
    char *email = NULL;
    int email_len = 0;

    if (LIBVEDAWALLET_NOERROR == veda_wallet_get_email(wstk, wid, &email, email_len)){
        emit get_email_res(QString(email), index, type);
        free(email);
    }
    else {
        emit get_email_res(QString(), index, type);
    }
    free(wid);
}

void server_functions::get_wid_list_by_email(uint64_t wstk, char *email)
{
    char *wid_list = NULL;
    int wid_list_len = 0;
    int res;
    if (LIBVEDAWALLET_NOERROR == (res = veda_wallet_get_wid_list_by_email(wstk, email, &wid_list, wid_list_len))){
        json ret;
        try {
            ret = json::parse(std::string(wid_list));
            free(wid_list);
        } catch (...) {
            qDebug()<<"error parsing json";
        }

        emit get_wid_list_by_email_res(ret, LIBVEDAWALLET_NOERROR);
    }
    else {
        emit get_wid_list_by_email_res(json(), res);
    }
    free(email);
}

void server_functions::create_empty_messages(uint64_t wstk, char *color_text, int tokens_to_create_count, char *WIDR, char *com_mass, int com_mas_len)
{
    int res = LIBVEDAWALLET_NOERROR;
    uint64_t num_trn = 0;

    res = veda_wallet_create_marked_tokens(wstk,
                                           num_trn,
                                           color_text,
                                           tokens_to_create_count,
                                           WIDR,
                                           com_mass,
                                           com_mas_len);
    emit create_empty_messages_res(res);
    free(color_text);
    free(WIDR);
    free(com_mass);
}

void server_functions::nulify_wstk()
{
    wstk = 0;
}
