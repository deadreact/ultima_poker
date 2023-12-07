#pragma once

#include "types.h"
#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_admin
  {

   /*************************************************************************************************/
   /*************************************** RPC LEVEL 1 *********************************************/
   /*************************************************************************************************/

   namespace ns_request
   {

    struct authorization
    {
     virtual ~authorization() { }

     /**
      * запрос на авторизацию.
      * Серверу ередаётся логин, пароль, id системной платы, id харда. */
     virtual void login(
      const std::string& login
      , const std::string& pass
      , const std::string& mb_id
      , const std::string& hdd_id) = 0;

    };

    struct user_managment
    {
     virtual ~user_managment() { }

     // верификация почты
     virtual void set_email_verified(gbp_i64 user_id, bool state) = 0;

     /**
      * взять список юзеров.
      * При нажатии "Применить" заполняется
      * структура фильтра, и отправляется на сервер,
      * в ответ вызывается users_list(...) с массивом информации по каждому игроку */
     virtual void users_list(const gbp::ns_api::ns_admin::user_info_filter_t& filter, gbp_i32 from, gbp_i8 limit) = 0;

     /**
      * блокировать юзера для сообщений стола,
      * true - чат заблокировать */
     virtual void set_chat_blocked(gbp_i64 user_id, bool status) = 0;

     // блокировать юзера по id по причине description
     virtual void block_user(
      gbp_i64 user_id,
      const std::string& description) = 0;

     // разблокировать юзера по id
     virtual void unblock_user(gbp_i64 user_id) = 0;

     // удалить(в БД статус "Удален") юзера по id по причине description
     virtual void remove_user(
      gbp_i64 user_id,
      const std::string& description) = 0;

     /**
      * получить профиль юзера с указанным id, в ответ
      * вызовется user_profile со структурой профиля */
     // запрос профили по id указанного юзера
     virtual void user_profile(gbp_i64 user_id) = 0;

     /**
      * при изменении профиля юзера, заполняется
      * структура юзера и отправляется на сервер для сохранения изменений */
     // изменить профиль
     virtual void change_profile(const gbp::ns_api::ns_admin::user_profile_t& profile) = 0;

     // отключить игрока
     virtual void disconnect_user(gbp_i64 user_id) = 0;

     // отыгрываемые бонусы игрока
     virtual void get_user_bonuses(gbp_i64 user_id) = 0;
     // приостановить отыгрываемость по акции
     virtual void user_bonuses_set_pause_state(gbp_i64 user_id, gbp::ns_api::ns_special_offers::e_type type_, bool state_paused) = 0;
     // обнуление остатка для отыгрыша
     virtual void user_bonuses_cancel(gbp_i64 user_id, gbp::ns_api::ns_special_offers::e_type type_) = 0;
     // добавление "начислено" по акции
     virtual void user_bonuses_add_amount(gbp_i64 user_id, gbp::ns_api::ns_special_offers::e_type type_, gbp::ns_api::money amount) = 0;
     // добавление "отыграно" по акции
     virtual void user_bonuses_add_amount_transferred(gbp_i64 user_id, gbp::ns_api::ns_special_offers::e_type type_, gbp::ns_api::money amount) = 0;

     // афилейты
     virtual void affiliates_get_list() = 0;
     virtual void affiliates_add(gbp_i64 netrefer_affiliate_id, const std::string& nickname) = 0;
     virtual void affiliates_remove(gbp_i64 id) = 0;

     /**
      * апиха начисления приза юзеру. Админ может начислить юзеру
      * деньги или билет на турнир. Выбирает в админке юзера, выбирает приз для
      * него и шлёт на игровой. Сервер выполняет действия в зависимости от выигрыша и
      * шлёт этому юзеру (если он онлайн) оповещение о том, что тот выиграл приз.
      * Все юзеры видят в лобби этого победителя с призом */
     virtual void give_user_prize(const gbp::ns_api::user_prize& prize) = 0;

     // установить состояние верификации юзера
     virtual void set_user_verified_state(gbp_i64 user_id, gbp::ns_api::e_verifying status) = 0;    

     // отключить всех залогиненых юзеров, которые имеют статус online
     virtual void disconnect_all_online_users() = 0;

     // сброс "первого депозита
     virtual void reset_first_deposit(gbp_i64 user_id) = 0;

     // пополнение денег аккаута (с помещением в таблицу payments_in и т.п.)
     virtual void make_payment_in(gbp_i64 user_id, gbp::ns_api::money money_, const std::string& bonuscode) = 0;

    };

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     // взять список транзакций переводов денег
     virtual void get_money_transfers(const gbp::ns_api::ns_admin::transfers_filter_t& filter) = 0;
     // отключение всех бот-админов
     virtual void all_bot_admins_disconnect() = 0;
     // действие по верификации
     virtual void make_verification_action(gbp_i64 user_id, gbp::ns_api::ns_admin::e_verification_action action) = 0;
     // взять список стран
     virtual void get_countries_list() = 0;
     // смена пароля админа
     virtual void reset_admin_pass(const std::string& email) = 0;

    };

    struct hardware
    {
     virtual ~hardware() { }

     // взять железки по фильтру
     virtual void get_hardwares_list(const gbp::ns_api::ns_admin::hardware_filter_t& filter) = 0;

     /**
      * блокировать вход юзера с указанного железа.
      * При блокировании юзера по железу на сервер отправляется id
      * записи железа и id юзера */
     virtual void block_user_hardware(
      bool block // true=block, false=unblock
      , const std::string& hardware_id
      , const std::string& description) = 0;

    };

    struct cashes
    {
     virtual ~cashes() { }

     /**
      * запрос суммы "Общей прибыли" вызывается периодически
      * (TODO: выяснить интервал) пока видимый диалог */
     virtual void get_cashes_gain() = 0;

     // создание новых кешей в количестве count
     virtual void add_cash(const gbp::ns_api::ns_admin::new_cash_t& new_cash_info, gbp_u16 count) = 0;

     // удаляем кеш по ID
     virtual void remove_cash(const std::vector<gbp_i64>& cash_ids) = 0;

     //	получить список кеш-столов
     virtual void cashes_list(const gbp::ns_api::ns_admin::cash_filter_t& filter) = 0;

    };

    struct tournaments
    {
     virtual ~tournaments() { }

     // запрос суммы "общей прибыли"
     virtual void get_tournaments_gain() = 0;

     // список турниров
     virtual void get_tournaments(const gbp::ns_api::ns_admin::tournament_filter_t& filter) = 0;

     /**
      * создаём, заполняем структуру с инфой по турниру
      * и отправляем на сервер для добавления */
     virtual void add_tournament(const gbp::ns_api::ns_common::tournament_template_t& tournament_info) = 0;

     // удалить турнир
     virtual void remove_tournament(const std::vector<gbp_i64>& tournament_ids) = 0;

     // активируем турнир
     virtual void activate_tournament(gbp_i64 tournament_id) = 0;

     // получить список "свободных" квалификаторов
     virtual void get_free_qualifiers_list() = 0;

     // установить турниру квалификаторы
     virtual void set_tournament_qualifiers(
      gbp_i64 tournament_id
      , const std::vector<gbp_i64>& qualifiers_ids) = 0;

     // установить состояние цепочки для sit_n_go (только для турниров на стадии регистрации)
     virtual void set_chain_state(gbp_i64 tournament_id, bool is_available) = 0;

     // вкл./выкл. видимость турнира
     virtual void set_tournament_visibility(gbp_i64 tournament_id, bool visibility) = 0;

     // список завершённых турниров
     virtual void get_tournaments_finished(const gbp::ns_api::ns_admin::tourn_finished_filter_t& filter, gbp_i32 from, gbp_i32 limit) = 0;

     // статистика по отдельному завершённому турниру
     virtual void get_finished_tourn_info(gbp_i64 item_id) = 0;

    };

    /****** заявки юзеров на выплаты  ******/

    struct user_payments
    {
     virtual ~user_payments() { }

     // выдача успешных пополнений игровых стеков (ввод денег)
     virtual void get_payments_in_list(const gbp::ns_api::ns_admin::ns_payments::user_payments_in_filter& filter) = 0;

     // выдача заявок (вывод денег)
     virtual void get_payments_out_list(const gbp::ns_api::ns_admin::ns_payments::user_payments_out_filter& filter) = 0;

     /**
      * установка статуса заявки.
      * payment_out_id - ID заявки;
      * user_id - ID юзера, подавшего заявку;
      * amount - деньги на выплату по заявке;
      * status_ - статус, установленный админом */
     virtual void set_payment_out_status(
      gbp_i64 payment_out_id
      , gbp_i64 user_id
      , ns_api::money amount
      , ns_api::e_payment_system payment_system
      , ns_api::e_payment_out_status status_) = 0;

     // инфа юзера по вводу/выводу денег
     virtual void get_user_payments_stat(gbp_i64 user_id) = 0;
    };

    struct admin_others
    {
     virtual ~admin_others() { }

     // ping
     virtual void ping() = 0;

    };

    struct tourn_templ_management
    {
     virtual ~tourn_templ_management() { }

     // получить список имён сохраненных шаблонов
     virtual void get_template_list(gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_) = 0;
     // сохранить шаблон в серверной БД
     virtual void insert_template(
      gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , const std::string& title
      , const std::string& template_data) = 0;
     // получить шаблон по иду
     virtual void get_template(
      gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , gbp_i64 template_id) = 0;
     // удалить шаблон по иду
     virtual void remove_template(
      gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , gbp_i64 template_id) = 0;
     // обновить шаблон
     virtual void update_template(
      gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , gbp_i64 template_id
      , const std::string& new_template_title
      , const std::string& new_template_data) = 0;
     // применение индексов сортировки (ID шаблона | индекс)
     virtual void apply_sort_indexes(
      gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , const std::unordered_map<gbp_i64, gbp_i64>& indexes) = 0;

    };

    // шаблоны для восстановления группы sit_n_go
    struct sit_n_go_templates_restore
    {
     virtual ~sit_n_go_templates_restore() { }

     virtual void get_sit_n_go_restore_templates_list() = 0;
     virtual void add_sit_n_go_restore_template(const gbp::ns_api::ns_common::sit_n_go_template_restore& item) = 0;
     virtual void remove_sit_n_go_restore_template(gbp_i64 template_id) = 0;
     virtual void start_sit_n_go_restore_templates() = 0;
    };

    struct tickets
    {
     virtual ~tickets() { }

     // получить список всех билетов
     virtual void get_tickets_list(const gbp::ns_api::ns_admin::get_tickets_filter_t& filter) = 0;

     // добавить новый билет
     virtual void add_ticket(const gbp::ns_api::ns_special_offers::ticket& new_ticket) = 0;

     // удаление билетов
     virtual void remove_tickets(const std::vector<gbp_i64>& ticket_ids) = 0;
    };

    struct special_offers
    {
     virtual ~special_offers() { }

     // получить состояния всех акций
     virtual void get_special_offers_states() = 0;

     // при старте также должно быть сохранение настроек
     virtual void start_soffer_rake_race_cash(const gbp::ns_api::ns_special_offers::soffer_rake_race_cash& soffer_new_settings) = 0;
     virtual void start_soffer_rake_race_tournament(const gbp::ns_api::ns_special_offers::soffer_rake_race_tournament& soffer_new_settings) = 0;
     virtual void start_soffer_nodeposit_bonus(const gbp::ns_api::ns_special_offers::soffer_nodeposit_bonus& soffer_new_settings) = 0;
     virtual void start_soffer_first_deposit_bonus(const gbp::ns_api::ns_special_offers::soffer_first_deposit_bonus& soffer_new_settings) = 0;
     virtual void start_soffer_random_prize(const gbp::ns_api::ns_special_offers::soffer_random_prize& soffer_new_settings) = 0;
     virtual void start_soffer_status_for_deposit(const gbp::ns_api::ns_special_offers::soffer_status_for_deposit& soffer_new_settings) = 0;
     virtual void start_soffer_status_rake_back(const gbp::ns_api::ns_special_offers::soffer_status_rake_back& soffer_new_settings) = 0;

     // настройки каждой акции
     virtual void get_soffer_rake_race_cash() = 0;
     virtual void get_soffer_rake_race_tournament() = 0;
     virtual void get_soffer_nodeposit_bonus() = 0;
     virtual void get_soffer_first_deposit_bonus() = 0;
     virtual void get_soffer_random_prize() = 0;
     virtual void get_soffer_status_for_deposit() = 0;
     virtual void get_soffer_status_rake_back() = 0;

     // времена акций
     virtual void get_special_offer_times(gbp::ns_api::ns_special_offers::e_type type_) = 0;
    };

    struct admin_statistic
    {
     virtual ~admin_statistic() { }

     virtual void get_admin_stat(const gbp::ns_api::ns_admin::admin_statistic_filter_t& filter) = 0;

     // статистика по юзерам
     virtual void get_users_stat() = 0;
    };

    struct autocontrol
    {
     virtual ~autocontrol() { }

     // включить/выключить автоконтроль
     virtual void autocontrol_set_enabled(bool state) = 0;

     // взять фин. план по номеру дня
     virtual void autocontrol_get_day_fin_plan(gbp_i32 day_number) = 0;

     // обновить фин. план дня
     virtual void autocontrol_update_day_fin_plan(const gbp::ns_api::fin_plan_day_item& day_item) = 0;

     // статистика за указанные дни
     virtual void autocontrol_get_fin_plan_stat(const std::vector<gbp_i32>& days) = 0;

    };

    struct vip_system
    {
     virtual ~vip_system() { }

     // список vip-статусов
     virtual void vip_system_get_statuses() = 0;

     // обновление vip-статуса
     virtual void vip_system_update_vip_status(const ns_vip_system::status_info& s_info) = 0;

     // применение к юзерам rakeback-бонусов
     virtual void vip_system_apply_rakebacks(const std::pair<ns_api::time_s, ns_api::time_s>& rakeback_times) = 0;

     // применение к юзерам новых статусов (в зависимости от достижений)
     virtual void vip_system_apply_vip_statuses() = 0;

    };

    struct admins
    {
     virtual ~admins() { }

     virtual void add_new_admin(const gbp::ns_api::ns_admin::new_admin_t& new_admin) = 0;
     virtual void remove_admin(gbp_i64 admin_id) = 0;
     virtual void get_admins() = 0;

    };

   } // namespace ns_request
  } // namespace ns_admin
 } // namespace ns_api
} // namespace gbp
