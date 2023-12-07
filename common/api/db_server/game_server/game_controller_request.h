#pragma once

#include "../../api_common.h"
#include "../../adminko/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_request
  {
   struct game_controller
   {
    virtual ~game_controller() { }

    // регистрация юзера
    virtual void register_user(const gbp::ns_api::user_register_info_t& ui) = 0;
    virtual void add_tournament(const gbp::ns_api::ns_common::add_new_tournament_request& info) = 0;
    virtual void delete_tournament(gbp_i64 tournament_id, bool is_main_tournament) = 0;
    virtual void add_finished_tournament(const gbp::ns_api::tournament_finished_t& item) = 0;
    virtual void get_tournaments_finished(
     const gbp::ns_api::ns_admin::tourn_finished_filter_t& filter
     , gbp_i64 admin_id
     , gbp_i32 from
     , gbp_i32 limit) = 0;
    virtual void get_finished_tourn_info(gbp_i64 item_id, gbp_i64 admin_id) = 0;
    virtual void user_logout(const gbp::ns_api::ns_common::disconnected_user_info& info) = 0;
    virtual void update_user_money(gbp_i64 user_id, gbp::ns_api::money new_user_money, bool is_freeroll) = 0;
    virtual void set_new_tournament_status(
     gbp_i64 tournament_id
     , gbp::ns_api::ns_tournament_types::e_tournament_status new_status
     , gbp::ns_api::time_s new_status_time
     , gbp::ns_api::money gain) = 0;
    virtual void set_user_status(gbp_i64 user_id, gbp::ns_api::e_user_status u_new_status) = 0;
    // статус цепочки в турнире
    virtual void set_tournament_chain_status(gbp_i64 tournament_id, bool status) = 0;
    virtual void get_cash_next_ids(const gbp::ns_api::ns_common::get_cash_next_ids_request_type& req_type) = 0;
    // установка всех статусов юзеров в offline
    virtual void cancel_all_users_statuses() = 0;
    // новый админский шаблон
    virtual void insert_admin_template(
     gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
     , const std::string& title
     , const std::string& template_data) = 0;
    virtual void remove_admin_template(gbp_i64 template_id) = 0;
    virtual void update_admin_template(gbp_i64 template_id, const std::string& title, const std::string& template_data) = 0;
    // обновить "призы" по платежу юзера
    virtual void update_payment_in_prizes(gbp_i64 user_id, gbp::ns_api::money additional_money, const std::string& prizes) = 0;
    // обновление всего_введено/всего_выведено/на_выплату
    virtual void update_user_payments(
     gbp_i64 user_id
     , gbp::ns_api::money payments_all_in
     , gbp::ns_api::money payments_all_out
     , gbp::ns_api::money payments_all_wait_out) = 0;
    // обновление статуса верификации юзера
    virtual void update_user_verified_state(gbp_i64 user_id, gbp::ns_api::e_verifying status) = 0;
    // обновление верификации почты юзера
    virtual void set_email_verified(gbp_i64 user_id, bool state) = 0;
    // обновление заметок по юзеру
    virtual void set_user_notes(gbp_i64 user_id, const std::string& user_notes) = 0;
    // бан по чату
    virtual void set_chat_blocked(gbp_i64 user_id, bool status) = 0;
    // активация юзера
    virtual void user_activation(gbp_i64 user_id) = 0;
    // для сортировки админских шаблонов
    virtual void apply_sort_indexes(const std::unordered_map<gbp_i64, gbp_i64>& indexes) = 0;

    // обновить деньги юзеров за последний час (user_id: money)
    virtual void update_users_last_hour_money(const std::unordered_map<gbp_i64, gbp::ns_api::money>& info) = 0;
    // перевод денег другому юзеру
    virtual void send_money_to_user(const std::string& sender_nickname, const std::string& receiver_nickname, gbp::ns_api::money money_) = 0;
    // добавление sit_n_go-шаблона для восстановления
    virtual void add_sit_n_go_restore_template(
     gbp_i64 admin_id
     , const gbp::ns_api::ns_common::sit_n_go_template_restore& item) = 0;
    // добавление нового флеш-кеша
    // удаление sit_n_go-шаблона для восстановления
    virtual void remove_sit_n_go_restore_template(gbp_i64 id) = 0;
    // обновление суточной статистики
    virtual void update_day_statistic(const gbp::ns_api::ns_common::stat_item& stat_item) = 0;
    // сохранение состояния кеш-стола для восстановления денег юзерам
    virtual void save_cash_users_restore_states(gbp_i64 game_id, const std::string& json_users) = 0;
    // сохранение состояния турнира для восстановления денег юзерам
    virtual void save_tourn_users_restore_states(gbp_i64 tournament_id, const std::string& json_users) = 0;
    // очистка сохранённого состояния стола по кешу
    virtual void clear_cash_users_restore_states(gbp_i64 game_id) = 0;
    // очистка сохранённого состояния стола по турниру
    virtual void clear_tourn_users_restore_states(gbp_i64 tournament_id) = 0;
    // приз турнира в БД (для восстановления денег)
    virtual void update_tournament_prize(gbp_i64 tournament_id, gbp::ns_api::money prize) = 0;
    // обнуление первого депозита
    virtual void reset_first_deposit(gbp_i64 user_id) = 0;
    // установить квалификаторы для турнира
    virtual void set_tournament_qualifiers(
     gbp_i64 tournament_id
     , const std::vector<gbp_i64>& qualifiers_ids) = 0;
    // обновление по таблице бонусов
    virtual void update_user_bonus_actions_info(
     const gbp::ns_api::ns_common::user_bonuses_info& info
     , const std::string& json_tickets) = 0;
    // обновление бонусов игрока
    virtual void update_user_bonuses(std::int64_t user_id, const std::vector<ns_api::ns_special_offers::user_bonuses>& bonuses) = 0;
    // аффилейты
    virtual void affiliates_add(gbp_i64 admin_id, gbp_i64 netrefer_affiliate_id, const std::string& nickname) = 0;
    virtual void affiliates_remove(gbp_i64 id) = 0;
    // автоконтроль, обновление фин. плана дня
    virtual void autocontrol_update_day_fin_plan(const gbp::ns_api::fin_plan_day_item& fin_item) = 0;
    // обновление vip-статуса
    virtual void update_vip_status(const ns_vip_system::status_info& s_info) = 0;
    // внесение нового бонуса юзера
    virtual void add_vip_bonus(const ns_vip_system::bonus_item& b_item) = 0;
    // обновление информации по рейкам игрока
    virtual void update_user_rake_info(gbp_i64 user_id, const ns_vip_system::user_rake_info& rake_info) = 0;
    // обновление vip-статуса юзера
    virtual void update_user_vip_status(gbp_i64 user_id, ns_vip_system::e_status vip_status) = 0;
    // обновление доходов с юзера
    virtual void update_user_proceeds_money(gbp_i64 user_id, ns_api::money tourn_money, ns_api::money cash_money) = 0;
    // обновление номера раздачи в кеше
    virtual void update_cash_distr_number(gbp_i64 game_id, gbp_i64 distr_number) = 0;

    virtual void get_countries() = 0;
    virtual void affiliates_get_list() = 0;
    virtual void get_users() = 0;    
    virtual void get_cashes() = 0;
    virtual void get_tournaments() = 0;
    virtual void get_sit_n_go_restore_templates() = 0;
    virtual void get_emails() = 0;
    virtual void get_admin_templates() = 0;
    virtual void get_common_statistic() = 0;
    virtual void get_special_offers() = 0;
    virtual void get_autocontrol_financial_plans() = 0;
    virtual void get_vip_system() = 0;

   }; // game_controller
  } // ns_request
 } // ns_db_server
} // namespace gbp