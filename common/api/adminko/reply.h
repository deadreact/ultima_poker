#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_admin
  {

   /*************************************************************************************************/
   /*************************************** RPC LEVEL 3 *********************************************/
   /*************************************************************************************************/

   namespace ns_reply
   {

    struct authorization
    {
     virtual ~authorization() { }

     GBP_DECLARE_ENUM(
      e_authorization
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
        (not_registered)
     )
     virtual void login(
      gbp::ns_api::ns_admin::ns_reply::authorization::e_authorization result
      , gbp::ns_api::ns_admin::e_admin_type admin_type) = 0;

    };

    struct user_managment
    {
     virtual ~user_managment() { }

     GBP_DECLARE_ENUM(
      e_user_managment
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
        (bad_id)
        (email_busy)
        // "афилейт с таким Netrefer ID уже есть"
        (netrefer_affiliate_id_exists)
        // "афилейт с таким логином уже есть"
        (affiliate_nickname_exists)
     )

     virtual void set_email_verified(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result) = 0;

     virtual void users_list(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      // отфильтрованные юзеры в кол-ве limit (см. request)
      , const std::vector<user_info_t>& users_list
      // всего ботов/людей
      , const std::pair<gbp_i64, gbp_i64>& counts
      // то же значение, что было в запросе
      , gbp_i32 from
      // всего юзеров по фильтру
      , gbp_i32 count) = 0;

     virtual void set_chat_blocked(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp_i64 user_id, bool current_chat_blocked_status) = 0;

     virtual void block_user(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result, gbp_i64 user_id) = 0;

     virtual void unblock_user(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result, gbp_i64 user_id) = 0;

     virtual void remove_user(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result, gbp_i64 user_id) = 0;

     virtual void user_profile(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , const user_profile_t& profile) = 0;

     virtual void change_profile(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result, gbp_i64 user_id) = 0;

     virtual void disconnect_user(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result) = 0;

     virtual void get_user_bonuses(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , const std::vector<gbp::ns_api::ns_special_offers::user_bonuses>& bonuses) = 0;

     virtual void user_bonuses_set_pause_state(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp::ns_api::ns_special_offers::e_type type_
      , bool current_state_paused) = 0;

     virtual void user_bonuses_cancel(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp::ns_api::ns_special_offers::e_type type_
      , gbp::ns_api::money current_amount_left) = 0;

     virtual void user_bonuses_add_amount(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp::ns_api::ns_special_offers::e_type type_
      , gbp::ns_api::money current_amount) = 0;

     virtual void user_bonuses_add_amount_transferred(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp::ns_api::ns_special_offers::e_type type_
      , gbp::ns_api::money current_amount_transferred) = 0;

     virtual void affiliates_get_list(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , const std::vector<gbp::ns_api::affiliate_item_t>& list) = 0;
     // 'ok', 'unknown_error', 'netrefer_affiliate_id_exists', 'affiliate_nickname_exists'
     virtual void affiliates_add(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result) = 0;
     virtual void affiliates_remove(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result) = 0;

     virtual void give_user_prize(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result, gbp_i64 user_id) = 0;

     virtual void set_user_verified_state(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp::ns_api::e_verifying status
      , gbp_i64 user_id) = 0;

     virtual void disconnect_all_online_users(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result) = 0;

     virtual void reset_first_deposit(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result, gbp_i64 user_id) = 0;

     virtual void make_payment_in(
      gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result
      , gbp_i64 user_id
      , gbp::ns_api::money first_deposit_amount
      , gbp::ns_api::money all_user_money) = 0;

     virtual void set_tournaments_money(gbp::ns_api::ns_admin::ns_reply::user_managment::e_user_managment result) = 0;

    };

    struct miscellaneous
    {
     virtual ~miscellaneous() { }

     GBP_DECLARE_ENUM(
      e_miscellaneous_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     // first - всего записей
     virtual void get_money_transfers(
      gbp::ns_api::ns_admin::ns_reply::miscellaneous::e_miscellaneous_result result,
      const std::pair<gbp_i64, std::vector<ns_api::ns_admin::money_transfer_item_t>>&items_pair
      ) = 0;
     // отключение всех бот-админов
     virtual void all_bot_admins_disconnect(gbp::ns_api::ns_admin::ns_reply::miscellaneous::e_miscellaneous_result result) = 0;
     virtual void make_verification_action(
      gbp::ns_api::ns_admin::ns_reply::miscellaneous::e_miscellaneous_result result
      , gbp::ns_api::ns_admin::e_verification_action action) = 0;
     virtual void get_countries_list(
      gbp::ns_api::ns_admin::ns_reply::miscellaneous::e_miscellaneous_result result
      , const std::vector<gbp::ns_api::country_item>& countries) = 0;
     virtual void reset_admin_pass(gbp::ns_api::ns_admin::ns_reply::miscellaneous::e_miscellaneous_result result) = 0;
    };

    struct hardware
    {
     virtual ~hardware() { }

     GBP_DECLARE_ENUM(
      e_get_hardwares_list
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     // получить всех юзеров по указанному железу
     virtual void get_hardwares_list(
      gbp::ns_api::ns_admin::ns_reply::hardware::e_get_hardwares_list result
      , const std::vector<ns_api::ns_admin::hardware_t>& lst) = 0;

     GBP_DECLARE_ENUM(
      e_block_user_hardware
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
        (bad_id)
     )
     // блокировать вход конкретного юзера с указанного железа
     virtual void block_user_hardware(
      gbp::ns_api::ns_admin::ns_reply::hardware::e_block_user_hardware result
      , hardware_t::e_status status
      , const std::string& hardware_id) = 0;

    };

    struct cashes
    {
     virtual ~cashes() { }

     GBP_DECLARE_ENUM(
      e_cashes_gain
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     //	ответ на запрос общей прибыли (с кешей)
     virtual void get_cashes_gain(gbp::ns_api::ns_admin::ns_reply::cashes::e_cashes_gain result, gbp_i64 gain) = 0;

     GBP_DECLARE_ENUM(
      e_add_cash
      , gbp_u8
      , friend
      , (ok)
        (invalid_fields)
        (unknown_error)
     )
     // добавили кеш-стол
     virtual void add_cash(gbp::ns_api::ns_admin::ns_reply::cashes::e_add_cash result) = 0;

     GBP_DECLARE_ENUM(
      e_remove_cash
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     //	удалили стол
     virtual void remove_cash(
      gbp::ns_api::ns_admin::ns_reply::cashes::e_remove_cash result
      , const std::vector<gbp_i64>& cash_ids) = 0;

     GBP_DECLARE_ENUM(
      e_cashes_list
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     // фильтровать список кеш-столов
     virtual void cashes_list(
      gbp::ns_api::ns_admin::ns_reply::cashes::e_cashes_list result
      , const std::vector<cash_t>& lst) = 0;

    };

    struct tournaments
    {
     virtual ~tournaments() { }

     GBP_DECLARE_ENUM(
      e_get_tournaments_gain
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     //	ответ на запрос общей прибыли (с турниров)
     virtual void get_tournaments_gain(gbp::ns_api::ns_admin::ns_reply::tournaments::e_get_tournaments_gain result, gbp_i64 gain) = 0;

     GBP_DECLARE_ENUM(
      e_get_tournaments
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     // ответ на tournament_game_filter(...)
     // получить список турниров
     virtual void get_tournaments(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_get_tournaments result
      , const std::vector<gbp::ns_api::ns_admin::tournament_t>& list) = 0;

     GBP_DECLARE_ENUM(
      e_add_tournament
      , gbp_u8
      , friend
      , (ok)
        // неправильное время по utc на компе с админкой
        (wrong_utc_time)
        // вектор процетов выплат пуст
        (payments_empty)
        // вектор завершения турнирных раундов пуст
        (round_ends_empty)
        /**
         * неправильные выплаты
         * Размер вектора 0, либо он меньше, чем макс. игроков в турнире */
        (wrong_win_payments)
        // spin-sng с таким бай-ином уже запущен
        (spin_sng_already_exists)
        // неправильный ID spin-sng пула
        (wrong_spin_sng_pool_id)
        // список уровней пула sng слишком мал
        (too_little_spin_sng_levels)
        (unknown_error)
     )
     // добавить турнир
     virtual void add_tournament(gbp::ns_api::ns_admin::ns_reply::tournaments::e_add_tournament result) = 0;

     GBP_DECLARE_ENUM(
      e_remove_tournament
      , gbp_u8
      , friend
      , (ok)
        (invalid_id)
        /**
         * удаляемый турнир запущен или на
         * регистрации (т.е. удалять нельзя) */
        (already_started)
        (unknown_error)
     )
     // удалить турнир
     virtual void remove_tournament(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_remove_tournament result
      , const std::vector<gbp_i64>& tournament_ids) = 0;

     GBP_DECLARE_ENUM(
      e_activate_tournament
      , gbp_u8
      , friend
      , (ok)
        (invalid_id)
        (unknown_error)
        // турнир удалён в момент активации (время начала регистрации вышло)
        (deleted)
     )
     // активировать турнир
     virtual void activate_tournament(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_activate_tournament result
      , gbp_i64 tournament_id) = 0;

     GBP_DECLARE_ENUM(
      e_get_free_qualifiers_list
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )
     virtual void get_free_qualifiers_list(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_get_free_qualifiers_list result
      , const std::vector<gbp::ns_api::qualifier_t>& ids) = 0;

     GBP_DECLARE_ENUM(
      e_set_tournament_qualifiers
      , gbp_u8
      , friend
      , (ok)
        (invalid_id)
        (not_main)
        (unknown_error)
     )

     virtual void set_tournament_qualifiers(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_set_tournament_qualifiers result
      , gbp_i64 tournament_id
      , const std::vector<gbp::ns_api::qualifier_t>& qualifiers) = 0;

     GBP_DECLARE_ENUM(
      e_set_chain_state
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void set_chain_state(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_set_chain_state result
      , gbp_i64 tournament_id
      , bool chain_is_available) = 0;

     GBP_DECLARE_ENUM(
      e_set_tournament_visibility
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void set_tournament_visibility(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_set_tournament_visibility result
      , gbp_i64 tournament_id
      , bool visibility_state) = 0;

     GBP_DECLARE_ENUM(
      e_tourn_finished_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_tournaments_finished(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_tourn_finished_result result
      , const std::vector<gbp::ns_api::ns_admin::tourn_finished_item>& items
      , gbp_i32 all_items_count) = 0;

     // статистика по отдельному завершённому турниру
     virtual void get_finished_tourn_info(
      gbp::ns_api::ns_admin::ns_reply::tournaments::e_tourn_finished_result result
      , const gbp::ns_api::ns_admin::tourn_finished_info_t& info) = 0;

    };

    /***************************************************************************/

    struct user_payments
    {
     virtual ~user_payments() { }

     GBP_DECLARE_ENUM(
      e_user_payments_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_payments_in_list(
      gbp::ns_api::ns_admin::ns_reply::user_payments::e_user_payments_result status
      , const std::vector<gbp::ns_api::ns_admin::ns_payments::payment_in>& list
      // кол-во записей по фильтру
      , gbp_u32 count) = 0;

     virtual void get_payments_out_list(
      gbp::ns_api::ns_admin::ns_reply::user_payments::e_user_payments_result status
      , const std::vector<gbp::ns_api::payment_out>& list
      // кол-во записей по фильтру
      , gbp_u32 count) = 0;

     virtual void set_payment_out_status(
      gbp::ns_api::ns_admin::ns_reply::user_payments::e_user_payments_result status
      , gbp_i64 id
      , gbp::ns_api::e_payment_out_status p_status) = 0;

     virtual void get_user_payments_stat(
      gbp::ns_api::ns_admin::ns_reply::user_payments::e_user_payments_result status
      , const std::vector<gbp::ns_api::ns_admin::user_payment_t>& payments) = 0;

    };

    /***************************************************************************/

    struct tourn_templ_management
    {
     virtual ~tourn_templ_management() { }

     GBP_DECLARE_ENUM(
      e_template_management_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_template_list(
      gbp::ns_api::ns_admin::ns_reply::tourn_templ_management::e_template_management_result status
      , gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , const std::vector<gbp::ns_api::ns_admin::template_t>& list) = 0;

     virtual void insert_template(
      gbp::ns_api::ns_admin::ns_reply::tourn_templ_management::e_template_management_result status
      , gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_) = 0;

     virtual void get_template(
      gbp::ns_api::ns_admin::ns_reply::tourn_templ_management::e_template_management_result status
      , gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , const gbp::ns_api::ns_admin::template_t& templ_result) = 0;

     virtual void remove_template(
      gbp::ns_api::ns_admin::ns_reply::tourn_templ_management::e_template_management_result status
      , gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_) = 0;

     virtual void update_template(
      gbp::ns_api::ns_admin::ns_reply::tourn_templ_management::e_template_management_result status
      , gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_) = 0;

     virtual void apply_sort_indexes(
      gbp::ns_api::ns_common::admin_template::e_tournament_template_type type_
      , e_template_management_result status) = 0;
    };

    struct sit_n_go_templates_restore
    {
     virtual ~sit_n_go_templates_restore() { }

     GBP_DECLARE_ENUM(
      e_sit_n_go_templates_restore_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_sit_n_go_restore_templates_list(
      gbp::ns_api::ns_admin::ns_reply::sit_n_go_templates_restore::e_sit_n_go_templates_restore_result status
      , const std::vector<gbp::ns_api::ns_common::sit_n_go_template_restore>& list) = 0;

     virtual void add_sit_n_go_restore_template(gbp::ns_api::ns_admin::ns_reply::sit_n_go_templates_restore::e_sit_n_go_templates_restore_result status) = 0;
     virtual void remove_sit_n_go_restore_template(gbp::ns_api::ns_admin::ns_reply::sit_n_go_templates_restore::e_sit_n_go_templates_restore_result status) = 0;
     virtual void start_sit_n_go_restore_templates(gbp::ns_api::ns_admin::ns_reply::sit_n_go_templates_restore::e_sit_n_go_templates_restore_result status) = 0;

    };

    struct spin_sng_pools
    {
     virtual ~spin_sng_pools() { }

     GBP_DECLARE_ENUM(
      e_spin_sng_pools_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_spin_sng_pools(
      gbp::ns_api::ns_admin::ns_reply::spin_sng_pools::e_spin_sng_pools_result status
      , const std::vector<gbp::ns_api::ns_admin::spin_sng_pool_t>& list) = 0;
     virtual void add_spin_sng_pool(gbp::ns_api::ns_admin::ns_reply::spin_sng_pools::e_spin_sng_pools_result status) = 0;
     virtual void show_spin_sng_pool_info(
      gbp::ns_api::ns_admin::ns_reply::spin_sng_pools::e_spin_sng_pools_result status
      , const gbp::ns_api::ns_admin::spin_sng_pool_info_t& info) = 0;
     virtual void stop_spin_sng_pool(gbp::ns_api::ns_admin::ns_reply::spin_sng_pools::e_spin_sng_pools_result status) = 0;

    };

    struct tickets
    {
     virtual ~tickets() { }

     GBP_DECLARE_ENUM(
      e_tickets_result
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_tickets_list(
      gbp::ns_api::ns_admin::ns_reply::tickets::e_tickets_result status
      , const std::vector<gbp::ns_api::ns_special_offers::ticket>& tickets) = 0;

     virtual void add_ticket(gbp::ns_api::ns_admin::ns_reply::tickets::e_tickets_result status) = 0;

     virtual void remove_tickets(gbp::ns_api::ns_admin::ns_reply::tickets::e_tickets_result status) = 0;

    };

    struct special_offers
    {
     virtual ~special_offers() { }

     GBP_DECLARE_ENUM(
      e_special_offers
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_special_offers_states(
      gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status
      , const std::vector<gbp::ns_api::ns_special_offers::special_offers_state>& states) = 0;

     virtual void start_soffer_rake_race_cash(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;
     virtual void start_soffer_rake_race_tournament(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;
     virtual void start_soffer_nodeposit_bonus(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;
     virtual void start_soffer_first_deposit_bonus(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;
     virtual void start_soffer_random_prize(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;
     virtual void start_soffer_status_for_deposit(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;
     virtual void start_soffer_status_rake_back(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status) = 0;

     virtual void get_soffer_rake_race_cash(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_rake_race_cash& info) = 0;
     virtual void get_soffer_rake_race_tournament(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_rake_race_tournament& info) = 0;
     virtual void get_soffer_nodeposit_bonus(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_nodeposit_bonus& info) = 0;
     virtual void get_soffer_first_deposit_bonus(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_first_deposit_bonus& info) = 0;
     virtual void get_soffer_random_prize(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_random_prize& info) = 0;
     virtual void get_soffer_status_for_deposit(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_status_for_deposit& info) = 0;
     virtual void get_soffer_status_rake_back(gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status, const ns_special_offers::soffer_status_rake_back& info) = 0;

     virtual void get_special_offer_times(
      gbp::ns_api::ns_admin::ns_reply::special_offers::e_special_offers status
      // first - время старта, second - время остановки
      , const std::pair<std::vector<ns_api::time_s>, std::vector<ns_api::time_s>>& pair_) = 0;

    };

    struct admin_statistic
    {
     virtual ~admin_statistic() { }

     GBP_DECLARE_ENUM(
      e_admin_stat
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void get_admin_stat(
      gbp::ns_api::ns_admin::ns_reply::admin_statistic::e_admin_stat status
      , const std::vector<ns_api::ns_admin::admin_statistic_t>& stat) = 0;
     virtual void get_users_stat(
      gbp::ns_api::ns_admin::ns_reply::admin_statistic::e_admin_stat status
      , const std::vector<ns_api::ns_admin::user_statistic_t>& stat) = 0;

    };

    struct autocontrol
    {
     virtual ~autocontrol() { }

     GBP_DECLARE_ENUM(
      e_autocontrol
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void autocontrol_set_enabled(gbp::ns_api::ns_admin::ns_reply::autocontrol::e_autocontrol status, bool current_state) = 0;
     virtual void autocontrol_get_day_fin_plan(
      gbp::ns_api::ns_admin::ns_reply::autocontrol::e_autocontrol status
      , const gbp::ns_api::fin_plan_day_item& day_item
      , const std::vector<gbp::ns_api::money>& days_amounts
      , bool autocontrol_state) = 0;
     virtual void autocontrol_update_day_fin_plan(gbp::ns_api::ns_admin::ns_reply::autocontrol::e_autocontrol status) = 0;
     virtual void autocontrol_get_fin_plan_stat(
      gbp::ns_api::ns_admin::ns_reply::autocontrol::e_autocontrol status
      , const gbp::ns_api::ns_admin::ns_autocontrol::fin_plan_stat_t& stat) = 0;

    };

    struct vip_system
    {
     virtual ~vip_system() { }

     GBP_DECLARE_ENUM(
      e_vip_system
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void vip_system_get_statuses(
      gbp::ns_api::ns_admin::ns_reply::vip_system::e_vip_system status
      , const std::vector<gbp::ns_vip_system::status_info>& statuses
      , bool apply_rakebacks_in_process
      , bool apply_vip_statuses_in_process) = 0;

     virtual void vip_system_update_vip_status(gbp::ns_api::ns_admin::ns_reply::vip_system::e_vip_system status) = 0;

     virtual void vip_system_apply_rakebacks(gbp::ns_api::ns_admin::ns_reply::vip_system::e_vip_system status) = 0;

     virtual void vip_system_apply_vip_statuses(gbp::ns_api::ns_admin::ns_reply::vip_system::e_vip_system status) = 0;

    };

    struct admins
    {
     virtual ~admins() { }

     GBP_DECLARE_ENUM(
      e_admins
      , gbp_u8
      , friend
      , (ok)
        (unknown_error)
     )

     virtual void add_new_admin(gbp::ns_api::ns_admin::ns_reply::admins::e_admins result) = 0;
     virtual void remove_admin(gbp::ns_api::ns_admin::ns_reply::admins::e_admins result) = 0;
     virtual void get_admins(
      gbp::ns_api::ns_admin::ns_reply::admins::e_admins result
      , const std::vector<gbp::ns_api::ns_admin::admin_item_t>& admins) = 0;

    };

   } // namespace ns_reply
  } // namespace ns_admin
 } // namespace ns_api
} // namespace gbp