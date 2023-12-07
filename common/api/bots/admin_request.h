#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_bots
  {
   namespace ns_admin_request
   { // bot_adminka -> bot_server

    /***************************** MISCELLANEOUS *******************************/

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     virtual void ping() = 0;
     // изменить собственный пароль
     virtual void change_own_password(const std::string& old_password, const std::string& new_password) = 0;
     // получение инфы по отдельному столу
     virtual void get_single_cash_info(gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     // получение инфы по характеру
     virtual void get_character_info(gbp_i64 character_id) = 0;
     // запрос фин. плана за сутки
     virtual void get_day_fin_plan(gbp_i32 day_number) = 0;
     // взять список стран
     virtual void get_countries_list() = 0;
     // высадить игрока (бота или человека) со стола
     virtual void player_close_table(gbp_i64 user_id, gbp_i64 tournament_id, gbp_i64 game_id) = 0;

    };

    /************************* DISTRIBUTION TEMPLATES **************************/

    struct distribution_templates
    {
     virtual ~distribution_templates() { }

     virtual void get_templates(const gbp::ns_api::ns_bots::ns_distr_templates::filter& filter) = 0;
     virtual void add_template(const gbp::ns_api::ns_bots::ns_distr_templates::d_template& template_) = 0;
     virtual void remove_template(gbp::ns_api::ns_bots::ns_distr_templates::e_game_type templates_game_type, gbp_i64 template_id) = 0;

     // назначить юзеру раздачу по шаблону
     virtual void set_distribution_template(const gbp::ns_api::ns_bots::ns_distr_templates::set_template_req_type& req_info) = 0;
    };

    /***************************** AUTHORIZATION *******************************/

    struct authorization
    {
     virtual ~authorization() { }

     // авторизироваться на бот-сервере
     virtual void login(const std::string& login, const std::string& pass, const std::string& mb_id, const std::string& hdd_id) = 0;

     // разлогиниться на бот-сервере
     virtual void logout() = 0;

    };

    /******************************* ACCOUNTS *********************************/

    struct accounts
    {
     virtual ~accounts() { }

     // запросить список аккаунтов
     virtual void get_accounts(const gbp::ns_api::ns_bots::accounts_filter& filter) = 0;
     // зарегистрировать новый аккаунт
     virtual void add_account(
      const std::string& login
      , const std::string& pass
      , gbp::ns_api::ns_bots::account_info::e_usertype usertype_) = 0;
     virtual void block_account(gbp_i64 user_id) = 0;
     virtual void unblock_account(gbp_i64 user_id) = 0;
     virtual void remove_account(gbp_i64 user_id) = 0;

     // запросить список железа по указанному id аккаунта
     virtual void get_hardwares(gbp_i64 account_id) = 0;

     /**
      * каждый раз когда юзер логинится с нового железа оно помещается в БД. Поэтому админ может 
      * заблокировать юзера по конкретному железу, а также разблокировать.
      * user_id - чтобы можно было заблокировать юзера в рантайме */
     virtual void block_hardware(gbp_i64 hardware_id, gbp_i64 user_id) = 0;
     virtual void activate_hardware(gbp_i64 hardware_id) = 0;
     // изменение названия точки доступа железа
     virtual void change_hardware_place_name(gbp_i64 hardware_id, const std::string& new_place_name) = 0;
    };

    /******************************** CASHES **********************************/

    struct cashes
    {
     virtual ~cashes() { }

     // получить список кешей
     virtual void get_cashes(const gbp::ns_api::ns_bots::cashes_filter& filter) = 0;
    };
    
    /***************************** TOURNAMENTS ********************************/

    struct tournaments
    {
     virtual ~tournaments() { }

     // получить список турниров
     virtual void get_tournaments(const gbp::ns_api::ns_bots::tournaments_filter& filter) = 0;
     // получить дополнительную информацию по турниру
     virtual void get_additional_tournament_stat(
      gbp_i64 tournament_id
      , const gbp::ns_api::ns_bots::tourn_stat_cashes_filter& cashes_filter) = 0;
    };

    /****************************** CASH RULES ********************************/

    struct cash_rules
    {
     virtual ~cash_rules() { }

     // получить общие правила поведения ботов за кешами
     virtual void get_cash_rules(gbp::ns_api::ns_bots::cash_rules::e_game_type type_) = 0;
     // установить общие правила поведения ботов в кешах
     virtual void set_cash_rules(const gbp::ns_api::ns_bots::cash_rules& rules) = 0;
     // взять правила поведения ботов в кешах на основе времени суток
     virtual void get_cashes_occupancy(gbp::ns_api::ns_bots::cash_rules::e_game_type type_) = 0;
     // установить правила поведения ботов в кешах на основе времени суток
     virtual void set_cashes_occupancy(const gbp::ns_api::ns_bots::cashes_occupancy& cashes_occupancy) = 0;
    };

    /*************************** TOURNAMENT RULES *****************************/

    struct tournament_rules
    {
     virtual ~tournament_rules() { }

     // получить общие правила поведения ботов в турнирах
     virtual void get_tournament_rules(gbp::ns_api::ns_bots::tournament_rules::e_buy_in_limit buy_in_limit) = 0;
     // установить общие правила поведения ботов в турнирах
     virtual void set_tournament_rules(const gbp::ns_api::ns_bots::tournament_rules& rules) = 0;
    };

    /**************************** SIT_N_GO RULES ******************************/

    struct sit_n_go_rules
    {
     virtual ~sit_n_go_rules() { }

     // получить общие правила поведения ботов в sit_n_go
     virtual void get_sit_n_go_rules(gbp::ns_api::ns_bots::sit_n_go_rules::e_sit_n_go_buy_in_limit buy_in_limit) = 0;
     // установить общие правила поведения ботов в sit_n_go
     virtual void set_sit_n_go_rules(const gbp::ns_api::ns_bots::sit_n_go_rules& rules) = 0;
    };

    /****************************** CHARACTERS ********************************/

    struct characters
    {
     virtual ~characters() { }

     // получить список характеров ботов
     virtual void get_characters() = 0;
     // получить дополнительную инфомацию по характеру
     virtual void get_character_info(gbp_i64 character_id) = 0;
     /**
      * получить список стран (с их кодами и количеством свободных ников).
      * Каждой стране должен соответствовать файл с перечнем ников */
     virtual void get_countries_info() = 0;
     // добавление нового характера бота
     virtual void add_character(const gbp::ns_api::ns_bots::character& character_) = 0;
     // удаление характера бота
     virtual void remove_character(gbp_i64 character_id) = 0;
     // обновить какие-то данные в характере
     virtual void update_character_info(const gbp::ns_api::ns_bots::character_update_info_t& info) = 0;
    };

    /************************ PLANNED DISTRIBUTIONS ***************************/

    struct planned_distributions
    {
     virtual ~planned_distributions() { }

     // взять все запланированные раздачи игры
     virtual void get_planned_distributions(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::planned_distributions_filter& filter) = 0;

     // добавление запланированной раздачи
     virtual void add_planned_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const gbp::ns_api::ns_bots::planned_distribution& new_planned_distribution) = 0;

     // удаление запланированной раздачи
     virtual void remove_planned_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 distribution_number) = 0;

     // получить список по истории шаблонных запл. раздач
     virtual void get_planned_distr_history_items(
      gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     virtual void remove_planned_distr_history_item(
      gbp_i64 tournament_id
      , bool is_sit_n_go
      , gbp_i64 game_id
      , gbp_i32 distr_number) = 0;

    };

    /********************************** LOCKS *********************************/

    struct locks
    {
     virtual ~locks() { }

     // блокировка таблицы целиком
     virtual void lock_table(gbp::ns_api::ns_bots::ns_locks::e_table table) = 0;
     virtual void unlock_table(gbp::ns_api::ns_bots::ns_locks::e_table table) = 0;

     /**
      * блокировка кешей.
      * Кеш может быть заблокирован и разблокирован в любой 
      * момент, пока есть в списке кешей.
      * Заблокирован в один момент времени может быть только один кеш, а разблокировано 
      * сразу несколько */
     virtual void lock_cash(gbp_i64 game_id) = 0;
     virtual void unlock_cashes(const std::vector<gbp_i64>& games_ids) = 0;
     
     // блокировка турнирных кешей
     virtual void lock_tournament_cash(gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     virtual void unlock_tournament_cashes(const std::unordered_map<gbp_i64, std::vector<gbp_i64>>& tournament_games) = 0;
    };

    /********************************** CHATS *********************************/

    struct chats
    {
     virtual ~chats() { }

     /**
      * админ открыл диалог списка чат-кешей.
      * Чтобы бот-сервер знал каким админам высылать индикации новых сообщений */
     virtual void open_main_chats_dialog() = 0;
     // админ закрыл диалог списка чат-кешей
     virtual void close_main_chats_dialog() = 0;
     // админ запрашивает список чат-кешей
     virtual void get_chat_cashes(gbp::ns_api::ns_bots::e_chat_cashes_filter filter) = 0;
     // админ запрашивает список сообщений по столу
     virtual void get_chat_messages(gbp_i64 chat_cash_id) = 0;
     // админ открывает чат-стол (на сервере также происходит блокировка чат-стола)
     virtual void chat_table_play_start(gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     // админ разблокирует чат-кеши (id турнира и id кеша)
     virtual void chats_table_play_stop(const std::vector<std::pair<gbp_i64, gbp_i64>>& chat_cashes_ids) = 0;
     // админ написал сообщение вместо бота
     virtual void add_chat_message(gbp_i64 game_id, const gbp::ns_api::ns_bots::chat_message_info& message_info) = 0;

    };

    /**************************** PLAY INSTEAD BOTS ***************************/

    struct play_instead_bots
    {
     virtual ~play_instead_bots() { }

     virtual void play_instead_bots_start(gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     virtual void play_instead_bots_stop(gbp_i64 tournament_id, gbp_i64 game_id) = 0;
     virtual void set_status(gbp_i64 tournament_id, gbp_i64 game_id, gbp::ns_api::ns_bots::e_play_instead_bots_status status) = 0;
     virtual void get_table_state_instead_bots(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // бот сделал ход
     virtual void user_action(
      gbp_i64 game_id
      , gbp_i64 bot_id
      , const std::pair<gbp::ns_api::ns_table::e_user_action, gbp::ns_api::money>& action) = 0;

     // сделал блайнд
     virtual void user_blind_done(
      gbp_i64 game_id
      , gbp_i64 bot_id
      , bool accepted) = 0;

     // "показать карты" 
     virtual void set_show_hand(
      gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

     // отослать сообщение
     virtual void send_chat_message(
      gbp_i64 game_id
      , gbp_i64 bot_id
      , const std::string& message) = 0;

     // запрос на пополнение в кеше
     virtual void add_game_money(
      gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

     // запрос на выход из ожидания      
     virtual void user_online(
      gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

     /********* турнирные *********/

     // админ делает аддон за бота
     virtual void make_addon(
      gbp_i64 bot_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // админ делает ребай за бота
     virtual void make_rebuy(
      gbp_i64 bot_id
      , gbp_i64 tournament_id
      , gbp_i64 game_id
      , bool is_double_rebuy) = 0;

     // вывести бота в online
     virtual void make_bot_online(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 bot_id) = 0;

    };

    /******************************** HISTORY *********************************/

    struct history
    {
     virtual ~history() { }

     // получить список историй
     virtual void get_history(const gbp::ns_api::ns_bots::history_filter& filter) = 0;
    };

    /********************************** CHASE *********************************/

    struct chase
    {
     virtual ~chase() { }

     // статистика наблюдений по людям
     virtual void get_chase_cashes_stat() = 0;
     virtual void get_chase_tournament_cashes_stat() = 0;
     virtual void get_chase_sit_n_go_cashes_stat() = 0;
     virtual void update_user_notes(const std::string& nickname, gbp_i64 user_id, const std::string& new_notes) = 0;
     virtual void update_user_task(gbp_i64 user_id, gbp_i64 tournament_id, gbp_i32 task_number, const std::string& task_text) = 0;
     virtual void get_tournament_prizes(gbp_i64 tournament_id, bool is_sit_n_go) = 0;
    };

    /******************************** STATISTIC ********************************/

    struct statistic
    {
     virtual ~statistic() { }

     virtual void get_ai_characters_stat() = 0;
    };

    /********************************* ERRORS **********************************/

    struct errors
    {
     virtual ~errors() { }

     virtual void get_errors(gbp_i32 last_count) = 0;
     virtual void clear_errors() = 0;

    };    

   } // ns_admin_request
  } // ns_bots
 } // ns_api
} // gbp
