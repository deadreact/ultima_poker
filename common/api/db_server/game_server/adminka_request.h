#pragma once

#include "../../api_common.h"
#include "../../adminko/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_request
  {
   struct adminka
   {
    virtual ~adminka() { }

    virtual void login(
     gbp_i64 admin_id
     , const std::string& login
     , const std::string& pass
     , const std::string& mb_id
     , const std::string& hdd_id) = 0;

    // смена пароля админа
    virtual void reset_admin_pass(gbp_i64 admin_id, const std::string& admin_email, const std::string& new_pass_orig) = 0;

    // заблокировать юзера
    virtual void block_user(gbp_i64 admin_id, gbp_i64 user_id, const std::string& description) = 0;

    // разблокировать юзера
    virtual void unblock_user(gbp_i64 admin_id, gbp_i64 user_id) = 0;

    // удалить юзера
    virtual void remove_user(gbp_i64 admin_id, gbp_i64 user_id, const std::string& description) = 0;

    // изменить профиль юзера
    virtual void change_profile(gbp_i64 admin_id, const ns_api::ns_admin::user_profile_t& u_profile) = 0;

    // получить список железа
    virtual void get_hardwares_list(gbp_i64 admin_id, const ns_api::ns_admin::hardware_filter_t& filter) = 0;

    // заблокировать железо юзера
    virtual void block_user_hardware(gbp_i64 admin_id, bool block, const std::string& hardware_id, const std::string& description) = 0;

    // создать кеш-стол
    virtual void add_cash(
     gbp_i64 admin_id
     , const gbp::ns_api::ns_common::game_settings& game_settings_
     , gbp_u16 count) = 0;

    // удалить кеш-стол
    virtual void remove_cash(gbp_i64 admin_id, gbp_i64 cash_id) = 0;

    // создать турнир
    virtual void add_tournament(gbp_i64 admin_id, const ns_api::ns_common::tournament_template_t& templ) = 0;

    /******* payments ********/

    // получить список транзакций по вводу денег
    virtual void get_payments_in_list(gbp_i64 admin_id, const ns_api::ns_admin::ns_payments::user_payments_in_filter& filter) = 0;

    // получить список транзакций по выводу денег
    virtual void get_payments_out_list(gbp_i64 admin_id, const ns_api::ns_admin::ns_payments::user_payments_out_filter& filter) = 0;

    // установка статуса для транзакции по выводу денег
    virtual void set_payment_out_status(gbp_i64 admin_id, gbp_i64 payment_out_id, ns_api::e_payment_out_status status_) = 0;

    // выдача списка перевода денег от юзера к юзеру (first - всего записей по фильтру)
    virtual void get_money_transfers(gbp_i64 admin_id, const ns_api::ns_admin::transfers_filter_t& filter) = 0;

    /******* special offers ********/

    // взять все времена по акции
    virtual void get_special_offer_times(gbp_i64 admin_id, ns_api::ns_special_offers::e_type type_) = 0;
    
    /******* admins ********/

    virtual void add_new_admin(gbp_i64 admin_id, const gbp::ns_api::ns_admin::new_admin_t& new_admin) = 0;
    virtual void remove_admin(gbp_i64 admin_id, gbp_i64 rem_admin_id) = 0;
    virtual void get_admins(gbp_i64 admin_id) = 0;

   }; // adminka
  } // ns_request
 } // ns_db_server
} // namespace gbp