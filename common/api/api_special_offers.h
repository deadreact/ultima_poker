#pragma once

#include "api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_special_offers
  {
   // тип операции для акции
   GBP_DECLARE_ENUM(
    e_operation_type
    , gbp_u8
    , inline
    , (start)
      (stop)
   )

   // статусы игроков 
   GBP_DECLARE_ENUM(
    e_player_status
    , gbp_u8
    , inline
    , (empty)
      (beginner)
      (semi_pro)
      (professional)
      (elite)
      (master)
      (legend)
   )

   // билет
   GBP_DECLARE_TYPE(
    ticket
    ,
    (_id, (gbp_i64))
    // название
    (_title, (std::string))
    // стоимость
    (_price, (ns_api::money))
    // рейтинг юзера
    (_user_rating, (gbp_i32))
   )  

   // напр. для подписи кнопок в админке
   GBP_DECLARE_TYPE(
    special_offers_state
    ,
    (_type, (ns_api::ns_special_offers::e_type))
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_is_enabled, (bool))
   )

   // акция (напр., для БД)
   GBP_DECLARE_TYPE(
    special_offer
    ,
    (_type, (ns_api::ns_special_offers::e_type))
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_settings_json, (std::string))
    (_is_enabled, (bool))
   )

   // для акции бонуса за первый депозит
   GBP_DECLARE_TYPE(
    first_deposit_item
    ,
    (_dep_money_from, (ns_api::money))
    (_dep_money_to, (ns_api::money))
    (_percent, (gbp_i32))
    // макс. сумма
    (_cap, (ns_api::money))
   )

   // для акции бездепозитного бонуса
   GBP_DECLARE_TYPE(
    nodeposit_item
    ,
    (_money, (ns_api::money))
    (_tickets_count, (gbp_i32))
    (_ticket_id, (gbp_i64))
   )

   /**
    * данные по конкретным акциям (напр. для 
    * админки для отображения в настройках акций) */
   GBP_DECLARE_TYPE(
    soffer_rake_race_cash
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_is_enabled, (bool))
   )

   GBP_DECLARE_TYPE(
    soffer_rake_race_tournament
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_is_enabled, (bool))
   )

   GBP_DECLARE_TYPE(
    soffer_nodeposit_bonus
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_is_enabled, (bool))
    // доступные для акции билеты
    (_tickets, (std::vector<ns_special_offers::ticket>))
    (_items, (std::vector<ns_special_offers::nodeposit_item>))
   )

   GBP_DECLARE_TYPE(
    soffer_first_deposit_bonus
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_bonuscode, (std::string))
    (_is_enabled, (bool))
    (_items, (std::vector<ns_special_offers::first_deposit_item>))
   )

   // для акции "случайный приз за депозит"
   GBP_DECLARE_TYPE(
    random_prize_item
    ,
    (_money_from, (ns_api::money))
    (_money_to, (ns_api::money))
    // "случайный выигрыш"
    (_win_item, (e_win_type))
    // если билет, то его ID
    (_type1_ticket_id, (gbp_i64))
    // вероятность выпадения
    (_probability, (gbp_i32))
    ,
    GBP_DECLARE_ENUM(
     e_win_type
     , gbp_u8
     , friend
     , // билет
       (type1)
       // $5
       (type2)
       // $10
       (type3)
       // $100
       (type4)
       // 5% на след. депозит
       (type5)
       // 10% на след. депозит
       (type6)
       // 25% на след. депозит
       (type7)
       // 50% на след. депозит
       (type8)
       // 100% на след. депозит
       (type9)
       // VIP-уровень +1 от текущего
       (type10)
       // VIP-уровень +2 от текущего
       (type11)
       // максимальный VIP-уровень
       (type12)
    )
   )

   GBP_DECLARE_TYPE(
    soffer_random_prize
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_bonuscode, (std::string))
    (_tickets, (std::vector<ns_special_offers::ticket>))
    // кол-во использований одним игроком
    (_usage_count, (gbp_i32))
    (_items, (std::vector<ns_special_offers::random_prize_item>))
    (_is_enabled, (bool))
   )

   // для акции "статус за депозит"
   GBP_DECLARE_TYPE(
    status_for_deposit_item
    ,
    (_money_from, (ns_api::money))
    (_money_to, (ns_api::money))
    (_player_status, (ns_special_offers::e_player_status))
   )

   GBP_DECLARE_TYPE(
    soffer_status_for_deposit
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_bonuscode, (std::string))
    (_usage_count, (gbp_i32))
    (_items, (std::vector<ns_special_offers::status_for_deposit_item>))
    (_is_enabled, (bool))
   )

   GBP_DECLARE_TYPE(
    soffer_status_rake_back
    ,
    (_datetime_from, (gbp_i64))
    (_datetime_to, (gbp_i64))
    (_description, (std::string))
    (_usage_count, (gbp_i32))
    (_confirmations_count, (gbp_i32))
    (_player_status, (ns_special_offers::e_player_status))
    (_prize, (soffer_status_rake_back::e_prize))
    (_is_enabled, (bool))
    ,
    // приз
    GBP_DECLARE_ENUM(
     e_prize
     , gbp_u8
     , friend
     , (empty)
       // рейк-бек 100%
       (rake_back_100)
    )
   )
  } // ns_special_offers
 } // ns_api
} // namespace gbp