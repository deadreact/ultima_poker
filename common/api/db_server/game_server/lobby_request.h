#pragma once

#include "../../api_common.h"
#include "../../lobby/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_request
  {
   struct lobby
   {
    virtual ~lobby() { }

    // успешность выдачи секретного ключа по железу юзера
    virtual void secret_key(gbp_i64 user_id, const std::string& hardware_key) = 0;

    // железо юзера (внесение/удаление т.п.)
    virtual void user_hardware(gbp_i64 user_id, const std::string& nickname, const std::string& hardware_key) = 0;

    // установка новой локали юзера
    virtual void set_new_locale_info(gbp_i64 user_id, const std::string& locale) = 0;

    // смена пароля юзером
    virtual void set_new_password(gbp_i64 user_id, const std::string& new_password) = 0;
    // смена аватара
    virtual void change_avatar(gbp_i64 user_id, const std::string& hash_avatar, const std::string& new_avatar) = 0;
    // смена почты
    virtual void change_email(gbp_i64 user_id, const std::string& new_email) = 0;

    // обновление данных юзера по адресу
    virtual void update_user_address_info(
     gbp_i64 user_id
     , gbp_u16 country
     , const std::string& city
     , const std::string& mail_index
     , const std::string& address) = 0;

    virtual void update_phone_number(
     gbp_i64 user_id
     , const std::string& phone_number) = 0;

    // внесение заявки на выплату денег юзеру
    virtual void add_user_payment_out(
     gbp_i64 user_id
     , const std::string& user_nickname
     , ns_api::e_payment_system ps
     , const std::string& description
     , ns_api::money amount) = 0;

   }; // lobby
  } // ns_request
 } // ns_db_server
} // namespace gbp