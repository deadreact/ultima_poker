#pragma once

#include <string>
#include <vector>

#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_lobby
  {
   // билет юзера
   GBP_DECLARE_TYPE(
    user_ticket
    ,
    // ID
    (_id, (gbp_i64))
    // стоимость билета
    (_price, (ns_api::money))
    // кол-во
    (_count, (gbp_i32))
   )

   GBP_DECLARE_TYPE(
    vip_system_info
    ,
    // текущий vip-статус
    (_current_vip_status, (ns_vip_system::e_status))
    // текущий рейк
    (_current_rake, (ns_api::money))
    /**
     * деньги, оставшиеся для достижения текущего 
     * статуса (сумма, необходимая для подтверждения статуса) */
    (_current_status_money_left, (ns_api::money))
    // текущий процент рейбека
    (_rakeback_current_percent, (gbp_u8))
    // текущий рейк-бек
    (_rakeback_current, (ns_api::money))
    // рейкбек за этот месяц
    (_rakeback_current_month, (ns_api::money))
    // рейкбек за прошлый месяц
    (_rakeback_last_month, (ns_api::money))
    // рейкбек за всё время
    (_rakeback_all_time, (ns_api::money))
    // история бонусов за 90 дней
    (_bonuses, (std::vector<vip_system_info::bonus_item>))
    ,
    GBP_DECLARE_TYPE(
    bonus_item
    ,
    // вид бонуса
    (_type, (ns_vip_system::e_bonus))
    // дата/время начисления бонуса
    (_datetime, (ns_api::time_s))
    // полученный статус
    (_vip_status, (ns_vip_system::e_status))
    // деньги по рейк-беку
    (_rakeback_money, (ns_api::money))
    // кол-во билетов при повышении статуса
    (_tickets_count, (gbp_i16))
    // период за который начислен рейк-бек, от-до
    (_rakeback_period, (std::pair<ns_api::time_s, ns_api::time_s>))
    // номер месяца начисления статусного бонуса
    (_status_bonus_month, (gbp_u8))
    )
   )

   // структура юзера, высылается лоббе при авторизации
   GBP_DECLARE_TYPE(
    user_info_t
    ,
    (_user_id, (gbp_i64))
    (_nickname, (std::string))
    (_hash_avatar, (std::string))
    (_email, (std::string)) // email
    (_username, (std::string)) // имя
    (_last_name, (std::string)) // фамилия
    (_birth_date, (ns_api::birth_date))
    (_city, (std::string)) // город проживания
    (_mail_index, (std::string)) // почтовый индекс
    (_mail_address, (std::string)) // почтовый адрес
    (_country, (gbp_u16)) // страна юзера
    (_lang, (std::string)) // текущий язык юзера
    (_phone, (std::string)) // телефон
    (_money, (money)) // все деньги аккаунта
    (_in_game_money, (ns_api::money)) // в игре
    (_wait_out_money, (ns_api::money)) // ожидают выплаты
    (_gender, (ns_details::e_gender)) // пол
    (_verifying, (ns_api::e_verifying)) // верификация юзера
    (_vip_status, (ns_vip_system::e_status)) // vip-статус
   )

   GBP_DECLARE_TYPE(
    mobile_user_info_t
   ,
    (_user_id, (gbp_i64))
    (_nickname, (std::string))
    (_hash_avatar, (std::string))
    (_email, (std::string)) // email
    (_username, (std::string)) // имя
    (_last_name, (std::string)) // фамилия
    (_birth_date, (ns_api::birth_date))
    (_city, (std::string)) // город проживания
    (_mail_index, (std::string)) // почтовый индекс
    (_mail_address, (std::string)) // почтовый адрес
    (_country, (gbp_u16)) // страна юзера
    (_lang, (std::string)) // текущий язык юзера
    (_phone, (std::string)) // телефон
    (_money, (money))
    (_gender, (ns_details::e_gender)) // пол
    (_token, (std::string))
    (_verifying, (ns_api::e_verifying)) // верификация юзера
    (_vip_status, (ns_vip_system::e_status)) // текущий vip-статус
   )

  } // namespace ns_lobby  

 } // namespace ns_api
} // namespace gbp