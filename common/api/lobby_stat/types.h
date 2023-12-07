#pragma once

#include "../api_common.h"
#include "../api_special_offers.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_statistic
  {
   namespace ns_mobile
   {
    // статистика по кешу
    GBP_DECLARE_TYPE(
     item_cash_game
     ,
     (_id, (gbp_i64))
     // ББ
     (_big_blind, (ns_api::money))
     // название стола
     (_title, (std::string))
     (_game_type, (ns_api::ns_game::e_game_type))
     (_game_limit, (ns_api::ns_game::e_game_limit))
     // средний банк
     (_average_pot, (ns_api::money))
     // кол-во мест за столом
     (_seats_count, (gbp_i8))
     // кол-во играющих
     (_players_count, (gbp_i8))
    )

    // статистика по SNG
    GBP_DECLARE_TYPE(
     item_sng
     ,
     (_id, (gbp_i64))
     // название
     (_title, (std::string))
     // бай-ин (0 если фриролл)
     (_buy_in, (ns_api::money))
     // нокаут-турнир
     (_is_knockout, (bool), (false))
     (_game_type, (ns_api::ns_game::e_game_type))
     (_game_limit, (ns_api::ns_game::e_game_limit))
     (_game_speed, (ns_api::ns_game::e_game_speed))
     // мин. кол-во игроков для старта
     (_min_players_count, (gbp_i32))
     // кол-во мест за столом
     (_seats_count, (gbp_i8))
     // общее количество зареганых игроков в турнире
     (_reg_users_count, (gbp_i32))
     // кол-во играющих игроков
     (_players_count, (gbp_i32))
    )

    // статистика 
    GBP_DECLARE_TYPE(
     item_tourn
     ,
     (_id, (gbp_i64))
     // название
     (_title, (std::string))
     // бай-ин (0 если фриролл)
     (_buy_in, (ns_api::money))
     (_game_type, (ns_api::ns_game::e_game_type))
     (_game_limit, (ns_api::ns_game::e_game_limit))
     (_game_speed, (ns_api::ns_game::e_game_speed))
     (_type, (ns_api::ns_statistic::ns_mobile::item_tourn::e_type))
     // общее количество зареганых игроков в турнире
     (_reg_users_count, (gbp_i32))
     // кол-во играющих игроков
     (_players_count, (gbp_i32))
     // партнёрский
     (_is_partner_tourn, (bool))
     // время завершения регистрации, utc
     (_reg_finish_time, (ns_api::time_s))
     // время завершения поздней регистрации (-1 если нет поздн. рег-ции)
     (_late_reg_finish_time, (ns_api::time_s))
     // кол-во мест за столом
     (_seats_count, (gbp_i8))
     ,
     GBP_DECLARE_ENUM(
      e_type
      , gbp_u8
      , friend
      , (freezout)
        (rebuy)
        (knockout)
        (qualifier)
     )
    )
   }

   // вкладки для spin-sng
   GBP_DECLARE_TYPE(
    spin_sng_info_t
    ,
    // доступные для игры турниры
    (_available_tournaments, (std::vector<spin_sng_tourn_t>))
    // текущий юзер уже зареган в турнирах
    (_is_registered_in, (std::vector<spin_sng_registered_stat_t>))
    ,
    GBP_DECLARE_TYPE(
     spin_sng_tourn_t
     ,
     (_tourn_template_id, (gbp_i64))
     (_buy_in, (ns_api::money))
     (_prize, (ns_api::money))
    )
    GBP_DECLARE_TYPE(
     spin_sng_registered_stat_t
     ,
     (_tourn_template_id, (gbp_i64))
     (_buy_in, (ns_api::money))
     (_waiting_for_count, (gbp_i32))
    )
   )

   /** общая статистика по лобби */
   // игроки столов с лучшими выигрышами (для кешей)
   typedef std::vector<std::pair<std::string, ns_api::money>> top_winners_t;

   // разная статистика
   GBP_DECLARE_TYPE(
     common_statistic_t
     ,
     (_players_online, (gbp_u64)) // кол-во играющих
     (_tables_online, (gbp_u64)) // кол-во существующих столов
     (_user_prize, (user_prize)) // приз игрока
     ,
     GBP_DECLARE_TYPE(
     user_prize
     ,
     (_user_id, (gbp_i64))
     (_nickname, (std::string))
     (_money, (ns_api::money)) // 0, если получил не деньги
     (_subject, (std::string)) // товар, пустая если получил деньги
     (_hash_avatar, (std::string)) // хеш аватара
     )
    )

   // баннер в шапке, высылает НЕ контроллер
   GBP_DECLARE_TYPE(
    banner_data_t
     ,
     (_data, (std::string))
    )

   // promo banner внизу
   GBP_DECLARE_TYPE(
    banner2_data_t
     ,
     (_data, (std::string))
    )

   GBP_DECLARE_TYPE(
    invited_t
    ,
    (_id, (gbp_i64)) // ID юзера
    (_nickname, (std::string)) // логин
    (_money, (ns_api::money)) // сколько денег принёс приглашённому данный пользователь
    (_registration_datetime, (time_s)) // дата-время регистрация юзверя
   )

   /**************** игровая статистика ****************/

   // структура игрока для отображения в маленьком столе
   GBP_DECLARE_TYPE(
    player_t
    ,
    (_nick, (std::string))
    (_pos, (gbp_i8), (-1))
    (_money, (money))
    (_country, (gbp_u16))
   )

   // все кеши
   GBP_DECLARE_TYPE(
    cash_stat_t
    ,
    (_id, (gbp_i64)) // ID игры
    (_title, (std::string)) // название
    (_is_freeroll, (bool)) // фриролл, да/нет
    (_big_blind, (ns_api::money)) // большой блайнд
    (_type, (ns_game::e_game_type)) // тип игры (табл.). фильтр.
    (_limit, (ns_game::e_game_limit)) // лимит
    (_players_count, (std::pair<gbp_i8, gbp_i8>)) // играющие/максимум_игроков за столом
    (_pot, (ns_api::money)) // средний банк: сумма/валюта (табл.)
    // игр / Flp. Процент человек, которые играют на флопе (первые 3 карты)
    (_flp, (gbp_u8))
    (_hands_per_hour, (gbp_i32)) // рук / час (табл.) кол-во
    (_wait_players, (gbp_i32)) // кол-во ожидающих в очереди
    (_players, (std::vector<ns_api::ns_statistic::player_t>)) // игроки в игре
    (_is_flash, (bool)) // true, если флеш-стол
    (_flash_id, (gbp_i64)) // -1 если не флеш-стол
   )

   /************** статистика по турнирам **************/

   // тип пола юзера
   GBP_DECLARE_ENUM(
    e_tourn_button_type
    , gbp_u8
    , inline
    , (hidden) // скрыта
      (register_in_tourn) // "регистрация в турнире"
      (unregister) // "отменить регистрацию"
      (back_to_game) // "назад в игру"
      (late_register_in_tourn) // "поздняя регистрация"
   )

//   GBP_DECLARE_TYPE(
//     user_finished_info_t
//     ,
//     (_id, (gbp_i64)) // ID игрока
//     (_nickname, (std::string)) // ник
//     (_country, (gbp_u16)) // страна юзера
//     // позиция вылетевшего игрока в списке вылетевших
//     (_tournament_pos, (gbp_i32))
//     // тип приза игрока
//     (_prize_type, (e_prize_type))
//     // сколько выиграл
//     (_prize_money, (ns_api::money))
//     // выбитые игроки и деньги с них
//     (_bounty_info, (std::pair<gbp_u32, money>))
//     // цена игрока на момент его вылета
//     (_player_bounty_value, (money))     
//     ,
//     // приз юзера в завершённом турнире
//     GBP_DECLARE_ENUM(
//      e_prize_type
//      , gbp_u8
//      , friend
//      , (money) // деньги
//        (ticket) // билет
//        (vip_points) // vip-очки
//        (lost) // проиграл без приза
//     )
//    )

   // диалог списка турниров
   GBP_DECLARE_TYPE(
    tournament_stat_t
    ,
    (_id, (gbp_i64)) // ID турнира
    (_seats_count, (gbp_i8)) // кол-во мест за столом
    // максимально доступное для регистрации кол-во игроков
    (_max_players_count, (gbp_i32))
    // зареган ли получающий статистику игрок в этом турнире
    (_is_current_user_registered, (bool))
    /**
     * для обычных турниров передавать только сколько зерагано игроков (_players_registered_count);
     * Если это sit_n_go проходящий или завершённый, то передавать так же, как и для обычного турнира. */
    (_players_registered_count, (gbp_i32))
    // нужное для старта sit_n_go кол-во игроков
    (_sit_n_go_users_count, (gbp_i32))
    // ID главного турнира (только для квалификаторов)
    (_main_tournament_id, (gbp_i64))
    (_buyin, (money)) // байин + баунти + комиссия
    // ББ (для идущего турнира и поздней регистрации)
    (_big_blind, (money))
    // длительность раунда (через это время также увеличится блайнд)
    (_round_duration, (ns_api::time_s))
    // стартовый стек (фишек за buy-in)
    (_start_stack, (ns_api::money))
    // кол-во призовых мест
    (_prize_places_count, (gbp_i32))
    // общий призовой фонд
    (_common_prize, (ns_api::money))
    // типы турнира
    (_types, (ns_api::ns_tournament_types::e_tournament_type))
    (_game_type, (ns_game::e_game_type)) // тип игры
    (_game_speed, (ns_game::e_game_speed)) // скорость игры
    (_game_limit, (ns_game::e_game_limit)) // лимит игры
    (_title, (std::string)) // название турнира
    // текущий статус турнира
    (_status, (ns_api::ns_tournament_types::e_tournament_status))
    // турнир - партнёрский
    (_is_partner_tourn, (bool), (false))
    // время начала регистрации
    (_registration_start_time, (time_s))
    // время начала турнира (только для турниров на стадии регистрации и не sit_n_go)
    (_start_time, (time_s))
    // время завершения турнира (для завершённых)
    (_finish_time, (ns_api::time_s))
    // состояние кнопки
    (_button_state, (e_tourn_button_type))
    // длительность поздней регистрации (секунды)
    (_late_registration_duration, (ns_api::time_s))
    // ID'ы квалификаторов турнира (только для главного события)
    (_qualifiers_ids, (std::vector<gbp_i64>))
    // ребаи (кроме завершённых турниров)
    (_rebuys, (rebuys_t))
    // аддон (кроме завершённых турниров)
    (_addon, (addon_t))
    // размер баунти (кроме завершённых турниров)
    (_bounty, (money))
    // первоначальная стоимость игрока в прогр. баунти (кроме завершённых турниров);
    (_player_bounty_value, (money))
    // текущий уровень турнира
    (_current_level, (gbp_i16))
    // игроки, только первые 9 штук
    (_users, (std::vector<tournament_stat_t::user_info>))
    ,
    GBP_DECLARE_TYPE(
     user_info 
     ,
     (_id, (gbp_i64))
     (_nickname, (std::string))
     (_country, (gbp_i32))
     // выигрыл билет
     (_is_ticket, (bool))
     // деньги выигрыша
     (_money, (ns_api::money))
     // если обычный bounty, то кол-во выбитых человек
     (_bounty_knocked_out_count, (gbp_i32))
     // если progressive-bounty, то цена по данному юзеру
     (_bounty_progr_money, (ns_api::money))
    )
   )

   // дополнительная турнирная статистика
   GBP_DECLARE_TYPE(
    tourn_additional_stat_t
    ,
    // ID турнира
    (_id, (gbp_i64))
    // cписок столов (только на этапах "поздняя регистрация" и "идёт")
    (_games, (std::vector<tourn_additional_stat_t::game>))
    // блайнды
    (_blinds, (std::vector<tourn_additional_stat_t::blind>))
    // призовые места 
    (_prizes, (std::vector<tourn_additional_stat_t::prize>))
    // завершение периода поздней регистрации через, сек
    (_late_register_ends_in, (ns_api::time_s))
    // всего сделано ребаев
    (_rebuys_count, (gbp_i32))
    // всего сделано аддонов
    (_addons_count, (gbp_i32))
    // "моя позиция в турнире", -1 если не участвую
    (_current_pos, (gbp_i32))
    ,
    GBP_DECLARE_TYPE(
     prize
     ,
      // призовое место
      (_winner_pos, (gbp_i32))
      // приз
      (_prize, (ns_api::money))
     )
    GBP_DECLARE_TYPE(
     blind
     ,
      // уровень в котором будет этот блайнд
      (_level, (gbp_i64))
      // размер ББ
      (_big_blind, (ns_api::money))
      // анте
      (_ante, (ns_api::money))
      // длительность раунда, сек
      (_duration, (ns_api::time_s))
     )
    GBP_DECLARE_TYPE(
     user_info
     ,
      (_id, (gbp_i64))
      (_nickname, (std::string))
      // размер стека игрока
      (_stack, (ns_api::money))
      // позиция юзера за столом
      (_pos, (gbp_i8))
     )
    GBP_DECLARE_TYPE(
     game
     ,
     (_id, (gbp_i64))
     (_title, (std::string))
     (_players, (std::vector<tourn_additional_stat_t::user_info>))
    )
   )

  } // namespace ns_statistic
 } // namespace ns_api
} // namespace gbp