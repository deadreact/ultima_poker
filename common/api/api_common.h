#pragma once

// std
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <sstream>

#ifndef GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED
#define _GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED(var, n) var
#else
#define _GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED(var, n) __unused_##n##__
#endif // GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED

#include "declare_type.h"
#include "gbp_int.hpp"

namespace gbp
{

 namespace ns_details
 {
  // тип пола юзера
  GBP_DECLARE_ENUM(
   e_gender
   , gbp_u8
   , inline
   , (male)
     (female)
  )
 }

 namespace ns_api
 {
  // деньги, центы
  typedef gbp_i64 money;

  // время, сек
  typedef gbp_i64 time_s;

  // для дат рождений
  GBP_DECLARE_TYPE(
   birth_date
   ,
   (_year, (gbp_i16), (1970))
   (_month, (gbp_i16), (1)) // 1 - 12
   (_day, (gbp_i16), (1)) // 1 - 31
  )

  GBP_DECLARE_TYPE(
   affiliate_item_t
   ,
   (_id, (gbp_i64))
   (_netrefer_affiliate_id, (gbp_i64))
   (_nickname, (std::string))
  )

  // структура юзера для регистрации
  GBP_DECLARE_TYPE(
   user_register_info_t
   ,
   (_nickname, (std::string))
   (_email, (std::string)) // почта
   (_password_sha1, (std::string))
   (_first_name, (std::string)) // имя
   (_surename, (std::string)) // фамилия
   (_gender, (ns_details::e_gender), (ns_details::e_gender::male))
   (_birth_date, (ns_api::birth_date))
   (_phone, (std::string))
   (_country, (gbp_i16))
   (_city, (std::string))
   (_address, (std::string))
   // локаль юзера
   (_lang, (std::string))
   (_zip, (std::string))
   (_btag, (std::string))
   (_email_verifying_code, (std::string))
  )

  GBP_DECLARE_TYPE(
   bounty_item
   ,
   (_knocked_out_count, (gbp_i32)) // кол-во выбитых игроков
   (_money, (ns_api::money)) // деньги за выбитых
  )

  GBP_DECLARE_TYPE(
   country_item
   ,
   (_id, (gbp_i32)) // идентификатор страны
   (_title, (std::string)) // название страны
   (_phone_code, (std::string)) // тел. код страны
   (_iso_code, (std::string)) // iso-код страны
  )

  namespace ns_special_offers
  {
   // билет юзера
   GBP_DECLARE_TYPE(
    user_ticket
    ,
    // ID
    (_id, (gbp_i64))
    // название
    (_title, (std::string))
    // кол-во билетов
    (_count, (gbp_i32))
   )

   // билет для vip-системы
   GBP_DECLARE_TYPE(
    vip_system_ticket
    ,
    // ID
    (_id, (gbp_i64))
    // название
    (_title, (std::string))
    // кол-во билетов
    (_count, (gbp_i32))
   )

   // тип акции
   GBP_DECLARE_ENUM(
    e_type
    , gbp_u8
    , inline
    , (rake_race_cash)
      (rake_race_tournament)
      (nodeposit_bonus)
      (first_deposit_bonus)
      (random_prize)
      (status_for_deposit)
      (status_rake_back)
   )

   GBP_DECLARE_ENUM(
    e_user_bonuses_status
    , gbp_u8
    , inline
      /*
       * игрок выбрал для отыгрыша данную акцию,
       * активна может быть только одна */
    , (active)
    // приостановлена игроком
    (paused_by_user)
    /**
     * приостановлена админом,
     * игрок не может снять паузу */
    (paused_by_admin)
    // завершённая (акция отыграна или по дате)
    (finished)
    /**
     * за нарушение или если был совершён вывод средств до
     * полного отыгрыша бонуса */
    (emptied)
   )

   // отыгрываемые бонусы игрока
   GBP_DECLARE_TYPE(
    user_bonuses
    ,
    (_type, (ns_api::ns_special_offers::e_type))
    (_status, (ns_api::ns_special_offers::e_user_bonuses_status))
    // дата/время получения акции
    (_datetime_created, (ns_api::time_s))
    // дата/время завершения акции для данного юзера
    (_datetime_finished, (ns_api::time_s))
    // начислено по акции (надо отыграть)
    (_amount, (ns_api::money))
    // переведено на счёт
    (_amount_transferred, (ns_api::money))
    // осталось отыграть
    (_amount_left, (ns_api::money))
    /**
     * подсчёт рейка по акции (обнуляется при 
     * переводе денег на основной счёт) */
    (_rake_counter, (ns_api::money))
   )
  }
 } // ns_api

 namespace ns_vip_system
 {
  // тип vip-статуса
  GBP_DECLARE_ENUM(
   e_status
   , gbp_u8
   , inline
   , (all) // для фильтров
     (beginner)
     (semi_pro)
     (professional)
     (elite)
     (master)
     (legend)
  )

  // тип бонуса
  GBP_DECLARE_ENUM(
   e_bonus
   , gbp_u8
   , inline
   , (all) // для фильтров
     (rakeback_bonus)
     (status_bonus)
  )

  GBP_DECLARE_TYPE(
   bonus_item
   ,
   (_id, (gbp_i64))
   (_user_id, (gbp_i64))
   (_type, (ns_vip_system::e_bonus))
   (_create_datetime, (ns_api::time_s))
   // накопленный рейк с юзера за неделю
   (_rake_week_money, (ns_api::money))
   (_rakeback_money, (ns_api::money))
   (_rakeback_datetime_from, (ns_api::time_s))
   (_rakeback_datetime_to, (ns_api::time_s))
   (_status_tickets_count, (gbp_i32))
   (_status, (ns_vip_system::e_status))
  )

  // для рейка юзеров
  GBP_DECLARE_TYPE(
   user_rake_info
   ,
   // за всё время, увеличивается в users_manager.cpp::calculate_rake()
   (_rake_all_time, (ns_api::money))
   // за прошлый месяц (обновляется при перерасчёте vip-статуса - vip_system.cpp)
   (_rake_last_month, (ns_api::money))
   /**
    * накапливаемый рейк, который обнуляется раз в неделю при
    * vip_system.cpp::handler_applying_rakebacks() */
   (_rake_week, (ns_api::money))
   /**
    * текущий рейк (обнуляется при перерасчёте vip-статуса).
    * Увеличивается в users_manager.cpp::calculate_rake() */
   (_rake_current, (ns_api::money))
   /**
    * текущий rakeback.
    * Увеличивается в users_manager.cpp::calculate_rake().
    * Центы * 100 */
   (_rakeback_current, (ns_api::money))
   /**
    * номер месяца обработки vip-статуса.
    * Если во время обработки текущий месяц и этот -
    * совпадают, то vip-статус не обрабатывать */
   (_vip_status_month, (gbp_i16))
  )

  GBP_DECLARE_TYPE(
   status_info
   ,
   (_type, (ns_vip_system::e_status))
   (_rake_money, (ns_api::money))
   (_rake_back_percent, (gbp_i8))
   // билеты
   (_tickets, (std::vector<ns_api::ns_special_offers::vip_system_ticket>))
  )

  // для разгрузки из БД
  GBP_DECLARE_TYPE(
   load_info
   ,
   (_statuses, (std::vector<ns_vip_system::status_info>))
   (_bonuses, (std::vector<ns_vip_system::bonus_item>))
  )
 } // ns_vip_system

 namespace ns_details
 {
  // зареганые в турнире юзеры
  GBP_DECLARE_TYPE(
   tourn_table_user_info
   ,
   (_id, (gbp_i64))
   (_nickname, (std::string))
   // позиция юзера за игровым столом
   (_table_pos, (gbp_i8))
   // позиция в турнире на момент формирования
   (_tournament_pos, (gbp_i32))
   (_is_user, (bool))
   // после завершения турнира юзера получил билет
   (_finished_by_ticket, (bool))
   // деньги выигрыша
   (_winner_money, (ns_api::money))
   // стартовый стек игрока
   (_start_stack, (ns_api::money))
   (_vip_status, (ns_vip_system::e_status))
   (_bounty_info, (tourn_table_user_info::bounty_info))
   (_game_id, (gbp_i64))
   (_country, (gbp_i32))
   ,
   GBP_DECLARE_TYPE(
    bounty_info
    ,
    // кол-во выбитых игроков
    (_knocked_out_count, (gbp_i32))
    // деньги за выбитых
    (_money, (ns_api::money))
    // деньги игрока по прогрессивному баунти
    (_progressive_money, (ns_api::money))
   )
  )

  // тип юзера
  GBP_DECLARE_ENUM(
   e_user_type
   , gbp_u8
   , inline
   ,
   (_GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED(is_user, 0))
   (_GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED(is_bot, 1))
   (is_admin)
  )

 } // ns_details

 namespace ns_api
 {
  /**
   * раздаваемые юзерам карты
   * pair.first - ID юзера,
   * pair.second - раздаваемая карта */
  typedef std::vector<std::pair<gbp_i64, gbp_i8>> distrib_users_cards;

  // статус юзера
  GBP_DECLARE_ENUM(
   e_user_status
   , gbp_u8
   , inline
   , (all)
     (online)
     (offline)
     (blocked)
  )

  /***************************************************************************/

  namespace ns_tourn
  {
   // высылается при отмене регистрации в турнире
   GBP_DECLARE_TYPE(
    cancel_tourn_register_t
    ,
    // отмена регистрации - во фрирольном турнире
    (_is_freeroll, (bool))
    // если 0, то нет денег для возврата
    (_money, (gbp_i64))
    // название возвращённого билета
    (_ticket_title, (std::string))
   )
  }

  namespace ns_tournament_types
  {
   typedef gbp_i32 e_tournament_type;

   /**
    * условие начала турнира:
    * запуск по времени, либо как только заполняются все места */
   GBP_DECLARE_ENUM_SIMPLE(
    e_start_time
    , inline
    , (st_normal, 0x2) // в десятичной 2
      (st_sit_n_go, 0x4) // 4
   )

   /**
    * докупка фишек:
    * 1) без докупки;
    * 2) с докупкой;
    * 3) баунти-фишки добавляются в конце раздачи
    * 4) прогрессивный баунти (knockout) */
   GBP_DECLARE_ENUM_SIMPLE(
    e_chips_rebuy
    , inline
    , (cr_normal, 0x8) // 8
      (cr_rebuy, 0x10) // 16
      (cr_bounty, 0x20) // 32
      (cr_progressive, 0x40) // 64
   )

   /**
    * тип турнира:
    * - обычный
    * - главное событие
    * - квалификатор */
   GBP_DECLARE_ENUM_SIMPLE(
    e_win_result
    , inline
    , (wr_normal, 0x80) // 128
      (wr_main, 0x100) // 256
      (wr_qualifier, 0x200) // 512
    )

   // турнирная выплата победителям, обычная/гарантийная_сумма
   GBP_DECLARE_ENUM_SIMPLE(
    e_win_prize
    , inline
    , (wp_normal, 0x400) // 1024
      (wp_warranty, 0x800) // 2048
    )

   /**
    * тип игры по деньгам:
    * обычный (на реальные деньги) и фриролл - деньги виртуальные */
   GBP_DECLARE_ENUM_SIMPLE(
    e_money_in
    , inline
    , (mi_normal, 0x1000) // 4096
      (mi_free_roll, 0x2000) // 8192
    )
   // регистрация (обычная - до старта турнира, и поздняя)
   GBP_DECLARE_ENUM_SIMPLE(
    e_registration
    , inline
    , (reg_normal, 0x4000) // 16384
      (reg_late, 0x8000) // 32768
    )

   // статусы турниров
   GBP_DECLARE_ENUM(
    e_tournament_status
    , gbp_u8
    , inline
    , (not_activated)// турнир не активирован
      (wait)// турнир в ожидании
      (registering)// начало процесса регистрации
      // в процессе и регистрация также доступна (для sit_n_go)
      (late_registration)
      (started)// в процессе игры
      (finished) // завершён
    )

   GBP_DECLARE_ENUM(
    e_tournament_stack
    , gbp_u8
    , inline
    , (normal_stack)
      (double_stack)
      (super_stack)
      (manual_stack)
    )

  } // namespace ns_tournament_types

  // SAVES INTO DB
  // структура, учитывающая ОДИН из интервалов участников турнира
  GBP_DECLARE_TYPE(
   win_payments_t
   ,
   /**
    * players_range - сам интервал игроков, участвующих в турнире
    * percentages - проценты выплат */
   (_players_range, (std::pair<gbp_u32, gbp_u32>))
   (_percentages, (std::vector<double>))
  )

  /***************************************************************************/

  namespace ns_table
  {
   typedef gbp_i8 card;
   typedef std::vector<card> cards;
   typedef cards deck;
   typedef std::map<gbp_i8, cards> grouped_deck;
   GBP_DECLARE_ENUM(
    e_user_action
    , gbp_u8
    , inline
    , (fold)
      (check)
      (bet)
      (call)
      (raise)
      (all_in)
      (blind)
      (guest_bet)
   )

   GBP_DECLARE_ENUM(
    e_cards_combination
    , gbp_u8
    , inline
    , (hight_card)
      (pair)
      (doper)
      (set)
      // когда в стрите туз - наименьшая карта
      (wheel)
      (straight)
      // флеш-стрит с наименьшим тузом
      (steel_wheel)
      (flush)
      (full_house)
      (four)
      (straight_flush)
      (royal_flush)
   )

   /**
    * структура хранения комбинации карт. В случае сравнения двух структур,
    * сначала они сравниваются по старшинству комбинаций, и если они
    * совпадают, то по след. картам */
   GBP_DECLARE_TYPE(
    cards_combination_t
    ,
    (_combination, (e_cards_combination))
    (_cards, (cards), (5, -1))
   )

   // структура для low-комбинации
   GBP_DECLARE_TYPE(
    cards_low_combination
    ,
    (_cards, (cards), (5, -1))
   )

   GBP_DECLARE_ENUM(
    e_game_round_type
    , gbp_u8
    , inline
    , (preflop)
      (flop)
      (tern)
      (river)
    )

   GBP_DECLARE_TYPE(
    available_user_course_t
    ,
    (_ua, (e_user_action))
    (_min, (ns_api::money))
    (_max, (ns_api::money))
   )

   // информация о ходе игрока
   GBP_DECLARE_TYPE(
    user_action_info_t
    ,
    // игрок, который ходит
    (_user_id, (gbp_i64))
    (_action, (e_user_action))
    // деньги действия
    (_action_cash, (ns_api::money))
    /**
     * всего осталось денег у игрока
     * в наличии за столом после его хода */
    (_user_cash, (ns_api::money))
    // деньги юзера на текущем круге торгов (не раздачи)
    (_user_round_bet, (ns_api::money))
   )

  } // namespace ns_table

  /***************************************************************************/

  namespace ns_game
  {

   GBP_DECLARE_ENUM(
    e_game_limit
    , gbp_u8
    , inline
    , (fixed_limit)
    (pot_limit)
    (no_limit)
   )

   GBP_DECLARE_ENUM(
    e_game_type
    , gbp_u8
    , inline
    , (texas_holdem)
    (omaha)
    (omaha_hi_low)
    (stud)
    (stud_hi_low)
   )

   GBP_DECLARE_ENUM(
    e_game_speed
    , gbp_u8
    , inline
    , (normal)
    (turbo)
    (super_turbo)
   )

  } // namespace ns_game

  /***************************************************************************/

  // условия ребаев
  GBP_DECLARE_TYPE(
   rebuys_t
   ,
   // -1 если неограниченное
   (_count, (gbp_i16))
   // цена ребая
   (_money, (ns_api::money))
   // размер ребая в фишках
   (_chips, (ns_api::money))
  )

  // условия аддона
  GBP_DECLARE_TYPE(
   addon_t
   ,
   // аддон доступен
   (_is_available, (bool))
   // цена аддона
   (_money, (ns_api::money))
   // размер аддона в фишках
   (_chips, (ns_api::money))
  )

  // турнир может иметь квалификатор
  GBP_DECLARE_TYPE(
   qualifier_t
   ,
   // ID турнира
   (_id, (gbp_i64))
   // дата/время начала
   (_datetime, (time_s))
   // название
   (_title, (std::string))
   // игра турнира
   (_limit, (ns_game::e_game_limit))
   (_game_type, (ns_game::e_game_type))
   (_game_speed, (ns_game::e_game_speed))
   // типы турнира
   (_types, (ns_tournament_types::e_tournament_type))
   // байин
   (_buyin, (ns_api::money))

   /**
    * если турнир на регистрации либо закончен - зарегано/максимум,
    * для идущего - осталось_активных_игроков/сколько_всего_зарегано */
   (_players_count, (std::pair<gbp_i32, gbp_i32>))

   /**
    * статус. Меняется самим турниром-квалификатором
    * в процессе игры */
   (_status, (ns_tournament_types::e_tournament_status), (ns_tournament_types::e_tournament_status::wait))
  )

  // действия раундов
  GBP_DECLARE_TYPE(
   round_conditions_t
   ,
   // номер раунда для отображения в лобби
   (_level, (gbp_i16))
   // большой блайнд в игре
   (_big_blind, (ns_api::money))
   // длительность раунда (может быть также длительностью перерыва)
   (_round_duration, (time_s))
   // анте. Если -1, то игнорируется
   (_ante, (ns_api::money))
   // основное время ожидания хода от игрока
   (_time_user_action_main, (time_s))
   /**
    * доп. время ожидания хода от игрока. Добавляется к дополнительному
    * времени каждого игрока а начале раунда */
   (_time_user_action_additional, (time_s))
  )

  // для выдачи приза юзеру
  GBP_DECLARE_TYPE(
   user_prize
   ,
   (_user_id, (gbp_i64))
   (_tournament_id, (gbp_i64)) // -1, если не билет
   (_prize, (e_user_prize))
   ,
   // тип приза
   GBP_DECLARE_ENUM(
    e_user_prize
    , gbp_u8
    , friend
    , (_5)// $5
      (_10)// $10
      (_25)// $25
      (_50)// $50
      (_100)// $100
      (_iphone)
      (_ticket)
   )
  )

  // верификация юзера
  GBP_DECLARE_ENUM(
   e_verifying
   , gbp_u8
   , inline
   , (in_process)
     (verified)
     (not_verified)
  )

  // статус заявки юзера на подачу выплаты
  GBP_DECLARE_ENUM(
   e_payment_out_status
   , gbp_u8
   , inline
   , (all)// для фильтра
     (wait)// "заявка в ожидании"
     (success)// "оплачено"
     (fail) // "отклонено"
  )

  // доступные плат. системы
  GBP_DECLARE_ENUM(
   e_payment_system
   , gbp_u8
   , inline
   , (all)
     // если ручное добавление денег
     (manual)
     (skrill)
     (neteller)
     (paysafecard)
     (visa)
     (mastercard)
     (banktransfer)
     (entropay)
     (bitcoin)
     (ecopayz)
  )

  GBP_DECLARE_TYPE(
   payment_system
   ,
   (_type, (ns_api::e_payment_system))
   // процент за депозит
   (_deposit_percent, (gbp_i8))
   // процент за вывод
   (_withdraw_percent, (gbp_i8))
  )

  // заявка на выплату юзеру (вывод денег юзером)
  GBP_DECLARE_TYPE(
   payment_out
   ,
   (_id, (gbp_i64))
   (_user_id, (gbp_i64))
   (_user_login, (std::string))
   // верифицирован
   (_is_verifyed, (bool))
   // выбранная юзером плат. система
   (_payment_system, (ns_api::e_payment_system))
   // описание (номер кошелька, телефон и т.п.)
   (_description, (std::string))
   // сумма выплаты
   (_amount, (ns_api::money))
   // текущий статус заявки
   (_current_status, (ns_api::e_payment_out_status))
   // дата создания заявки
   (_create_date, (time_s))
  )

  // для турниров
  GBP_DECLARE_TYPE(
    tourn_user
    ,
    (_id, (gbp_i64)) // ID юзера
    (_nickname, (std::string)) // логин
    (_is_ticket, (bool)) // true, если юзер выиграл билет
    (_money, (ns_api::money)) // деньги. Отображать, если их > 0
   )

   // полная статистика по завершённому турниру
   GBP_DECLARE_TYPE(
    tournament_finished_t
    ,
    (_id, (gbp_i64)) // id
    (_title, (std::string)) // название турнира
    (_start_datetime, (ns_api::time_s)) // время старта
    (_finish_datetime, (ns_api::time_s)) // время завершения
    (_game_type, (ns_game::e_game_type)) // техас, омаха, стад...
    (_game_limit, (ns_game::e_game_limit)) // безлимит, лимитный,...
    (_game_speed, (ns_game::e_game_speed)) // скорость турнирного стола
    (_tournament_type, (ns_tournament_types::e_tournament_type)) // тип турнира
    (_stack_type, (ns_tournament_types::e_tournament_stack)) // стек
    (_stack, (ns_api::money)) // стек
    (_buy_in, (ns_api::money)) // бай-ин
    (_fee, (double)) // % комиссии
    (_fee_money, (ns_api::money)) // комиссия
    (_bounty, (ns_api::money)) // баунти
    (_is_progressive_bounty, (bool)) // прогр. баунти
    // было зарегано всего
    (_players_reg_count, (gbp_i32))
    // было зарегано на поздней регистрации
    (_players_late_reg_count, (gbp_i32))
    // мин. кол-во игроков для старта
    (_players_min, (gbp_i32))
    // макс. кол-во игроков для старта
    (_players_max, (gbp_i32))
    // кол-во призовых мест
    (_win_places_count, (gbp_i32))
    (_gain, (ns_api::money)) // прибыль с турнира
    (_warranty_prize, (ns_api::money)) // гарантийная сумма денег
    (_prize, (ns_api::money)) // призовой фонд
    (_table_size, (gbp_u8)) // размер стола, человек
    (_addon_info, (ns_api::addon_t)) // характеристики аддона
    (_addons_count, (gbp_i32)) // кол-во аддонов
    (_rebuy_info, (ns_api::rebuys_t)) // характеристики ребая
    (_rebuys_count, (gbp_i32)) // кол-во ребаев
    // игроки в завершённом турнире
    (_users, (std::vector<ns_api::tournament_finished_t::user_info>))
    ,
    GBP_DECLARE_TYPE(
     user_info
     ,
     (_pos, (gbp_i32)) // позиция в завершённом турнире
     (_nickname, (std::string))
     (_bounty_count, (gbp_i32)) // кол-во выбитых им игроков
     (_prize_ticket, (bool), (false)) // выиграл билет
     (_prize_money, (ns_api::money)) // выиграл деньги
    )
   )

  GBP_DECLARE_TYPE(
   fin_plan_day_item
   ,
   // номер дня по utc с начала эпохи
   (_day_number, (gbp_i32))
   // номер недели по utc с начала эпохи
   (_week_number, (gbp_i32))
   // запл. сумма на день
   (_amount, (ns_api::money))
   // скорректированный системой план
   (_amount_corrected, (ns_api::money))
   // часы дня, ключ - номер часа, начиная с 0
   (_hours, (std::unordered_map<gbp_i32, fin_plan_day_item::hour>))
   ,
   GBP_DECLARE_TYPE(
    hour
    ,
    // процент денег на каждый час от запланированной суммы
    (_percent, (gbp_i32))
    // сумма денег по проценту
    (_plan, (ns_api::money))
    /**
     * фактическая сумма денег по проценту,
     * т.е. когда остаток денег делится на все оставшиеся 
     * часы соответственно их проценту */
    (_plan_fact, (ns_api::money))
    // реально заработанная сумма
    (_fact, (ns_api::money))
    // разница
    (_diff, (ns_api::money))
    // остаток
    (_residue, (ns_api::money))
   )
  )

  namespace ns_common
  {
   GBP_DECLARE_ENUM(
     e_common_result
     , unsigned char
     , inline
     , (ok)
       (unknown_error)
    )

   // данные юзера после регистрации в БД
   GBP_DECLARE_TYPE(
    reg_new_user_db_result
    ,
    (_success, (bool))
    (_user_id, (gbp_i64))
   )

   GBP_DECLARE_TYPE(
   game_settings
    ,
     (_id, (gbp_i64))
     // -1 если игра не турнирная
     (_tournament_id, (gbp_i64))
     // номер текущий раздачи
     (_distr_number, (gbp_i64))
     // название
     (_title, (std::string))
     // большой блайнд
     (_big_blind, (ns_api::money))
     // минимум и максимум для регистрации в кеше
     (_register_money_from, (ns_api::money))
     (_register_money_to, (ns_api::money))
     (_game_type, (ns_api::ns_game::e_game_type))
     (_game_limit, (ns_api::ns_game::e_game_limit))
     // короткое время для ожидания хода, сек
     (_time_action_short, (ns_api::time_s))
     // основное время для ожидания хода, сек
     (_time_action_main, (ns_api::time_s))
     // дополнительное время для ожидания хода, сек
     (_time_action_additional, (ns_api::time_s))
     (_ante_mode, (bool))
     // мест за столом
     (_users_count, (gbp_u8))
     // рейк (процент)
     (_rake, (double))
     // больше этого значения рейк у стола быть не может
     (_rake_cap, (ns_api::money))
     // бесплатный кеш
     (_is_freeroll, (bool))
   )

   GBP_DECLARE_TYPE(
    tournament_settings
    ,
    (_id, (gbp_i64))
    // название турнира
    (_title, (std::string))
    // разновидности типов турнира
    (_types, (ns_api::ns_tournament_types::e_tournament_type))
    // максимальное количество игроков за столом
    (_max_seats_count, (gbp_i8))
    // минимальное количество игроков в турнире
    (_min_users_count, (gbp_i32))
    // максимальное количество игроков в турнире
    (_max_users_count, (gbp_i32))
    // типы турнирных игр
    (_game_type, (ns_api::ns_game::e_game_type))
    (_game_limit, (ns_api::ns_game::e_game_limit))
    (_game_speed, (ns_api::ns_game::e_game_speed))
    /**
     * ID главного события.
     * Если -1, то это не квалификатор */
    (_main_tournament_id, (gbp_i64))
    // картинка, слоган
    (_afisha, (std::pair<std::string, std::string>))
    // статус турнира
    (_status, (ns_api::ns_tournament_types::e_tournament_status))
    // время начала анонсирования
    (_announce_time, (ns_api::time_s))
    // время начала регистрации
    (_register_time, (ns_api::time_s))
    // время старта турнира
    (_start_time, (ns_api::time_s))
    // время завершения турнира
    (_finish_time, (ns_api::time_s))
    // время в сек., по окончании которого турнир запустится снова
    (_restart_interval, (ns_api::time_s))
    // максимальное количество повторов турнира
    (_replay_count_max, (gbp_i64))
    /**
     * стоимость buy_in'а.
     * при добавлении игрока вычитается стоимость buy-in'а,
     * bounty и комиссия, а в cash игрока турнира добавляется chips (см. ниже) */
    (_buyin, (ns_api::money))
    // тип стека
    (_stack, (ns_api::ns_tournament_types::e_tournament_stack))
    /**
     * стек, т.е. фишки, которые получит игрок за buy_in (или просто, если
     * это фриролл) и с которыми начнёт турнир */
    (_chips, (ns_api::money))
    // условия ребаев
    (_rebuys, (ns_api::rebuys_t))
    // условия аддона
    (_addon, (ns_api::addon_t))
    // номер автоперерыва для аддона
    (_addon_break_number, (gbp_i8))
    /**
     * длительность поздней регистрации (если это ребайник, то и начало 
     * времени для аддона - "ручного перерыва") */
    (_late_reg_minutes, (ns_api::time_s))
    // комиссия, %
    (_fee, (double))
    // размер bounty
    (_bounty, (ns_api::money))
    // гарантийная сумма денег (если турнир гарантийный)
    (_warranty_prize, (ns_api::money))
    // рейтинг пользователя, дающий право участвовать в турнире, от
    (_user_vip_status, (ns_vip_system::e_status))
    // билеты турнира
    (_tickets, (std::unordered_set<gbp_i64>))
    /**
     * квалификаторы (если это главное событие)
     * gbp_i64 - id турнира-квалификатора */
    (_qualifiers, (std::unordered_map<gbp_i64, ns_api::qualifier_t>))
    // действия по раундам
    (_round_conditions, (std::vector<ns_api::round_conditions_t>))
    // таблица выплат игрокам
    (_win_payments, (std::vector<ns_api::win_payments_t>))
    // прибыль с турнира
    (_gain, (ns_api::money), (0))
    // цепочка турнира
    (_with_chain, (bool))
    // для цепочечных - время начала следующего турнира
    (_time_interval, (ns_api::time_s))
    // партнёрский турнир
    (_is_partner_tourn, (bool), (false))
    // видимость турнира
    (_is_visible, (bool), (true))
    // сколько времени отображать турнир после его завершения
    (_time_after_finishing, (ns_api::time_s), (300))
   )

   GBP_DECLARE_TYPE(
    new_tournament_t
    ,
    (_with_chain, (bool)) // автостарт следующего турнира
    (_title, (std::string)) // название турнира
    (_game_type, (ns_game::e_game_type)) // техас, омаха, стад...
    (_game_limit, (ns_game::e_game_limit)) // безлимит, лимитный,...
    (_game_speed, (ns_game::e_game_speed)) // скорость турнирного стола
    (_tournament_type, (ns_tournament_types::e_tournament_type)) // тип турнира
    (_users_count, (gbp_u8)) // кол-во игроков за столом
    (_img_title, (std::pair<std::string, std::string>)) // слоган/изображение турнира
    // рейтинг юзера, чтобы он имел право участвовать в турнире, от
    (_user_vip_status, (ns_vip_system::e_status))
    (_min_max_gamers_count, (std::pair<gbp_u64, gbp_u64>)) // кол-во игроков, минимум/максимум
    (_fee, (double)) // комиссия, %
    (_buy_in, (money)) // бай-ин
    (_bounty, (money)) // баунти
    /**
     * номер автоперерыва для аддона (и конец 
     * поздней регистрации) */
    (_addon_break_number, (gbp_i8), (-1))
    /**
     * длительность поздней регистрации (если это ребайник, 
     * то и начало времени для аддона - "ручного перерыва") */
    (_late_reg_minutes, (ns_api::time_s), (-1))
    (_stack_type, (ns_tournament_types::e_tournament_stack)) // тип стека
    (_stack, (money)) // количество фишек за байин
    (_warranty_money, (money)) // гарантийная сумма денег (если турнир гарантийный)
    (_rebuys, (ns_api::rebuys_t)) // условия ребаев
    (_addon, (ns_api::addon_t)) // условия аддона   
    // партнёрский турнир
    (_is_partner_tourn, (bool), (false))
    (_announce_datetime, (ns_api::time_s)) // время начала анонсирования
    (_registration_datetime, (ns_api::time_s)) // время начала регистрации
    (_start_datetime, (ns_api::time_s)) // время старта турнира
    // сколько минут отображать турнир после его завершения
    (_time_after_finishing, (ns_api::time_s), (300))
    (_round_conditions, (std::vector<ns_api::round_conditions_t>)) // условия раундов
    (_win_payments, (std::vector<ns_api::win_payments_t>)) // таблица выплат игрокам
   )

   GBP_DECLARE_TYPE(
    tournament_template_t
    ,
    (_time_interval, (ns_api::time_s)) // интервал времени между однотипными турнирами
    (_count, (gbp_u16)) // кол-во турниров, которые должны быть созданы
    (_activate, (bool)) // активировать турнир при создании
    // билеты турнира
    (_tickets, (std::unordered_set<gbp_i64>))    
    /**
     * настройки, на основе которых будет создано _count турниров,
     * и у каждого турнира, начиная со второго, будут сдвинуты времена событий
     * на _time_interval по возрастающей относительно предыдущего */
    (_tournament, (new_tournament_t))
   )

   GBP_DECLARE_TYPE(
    user_info
    ,
    // table 'users'
    (_user_id, (gbp_i64))
    (_is_bot, (bool))
    (_bot_character, (gbp_i64))
    (_nickname, (std::string))
    (_nickname_lower_case, (std::string))
    (_nickname_suffix, (std::string))
    (_account_money, (ns_api::money))
    (_freeroll_money, (ns_api::money))
    (_avatar, (std::string))
    (_hash_avatar, (std::string))
    (_status, (ns_api::e_user_status))
    (_last_activity, (ns_api::time_s))
    (_register_time, (ns_api::time_s))     
    (_email, (std::string))
    (_email_verified, (bool))
    (_email_verifying_code, (std::string))
    (_password, (std::string))
    (_locale, (std::string))
    (_activation_code, (std::string))
    (_country, (gbp_i32))
    (_username, (std::string))
    (_last_name, (std::string))
    (_city, (std::string))
    (_phone, (std::string))
    (_mail_index, (std::string))
    (_birth_date, (ns_api::birth_date))
    (_mail_address, (std::string))
    (_gender, (ns_details::e_gender))    
    (_verifying, (ns_api::e_verifying))
    (_payments_all_in, (ns_api::money))
    (_payments_all_out, (ns_api::money))
    (_payments_all_wait_out, (ns_api::money))
    (_user_notes, (std::string))
    (_chat_banned, (bool))
    (_affiliate_btag, (std::string))
    (_bonuses, (std::vector<ns_api::ns_special_offers::user_bonuses>))

    // table 'user_bonus_actions_info'
    (_vip_status, (ns_vip_system::e_status))
    (_rake_info, (ns_vip_system::user_rake_info))
    (_proceeds_from_cash, (gbp::ns_api::money))
    (_proceeds_from_tourn, (gbp::ns_api::money))
    (_tickets_json, (std::string))
    (_first_deposit, (gbp::ns_api::money))
    (_first_deposit_was_used, (bool))
    (_registered_by_site, (bool))
    (_tournament_money, (gbp::ns_api::money))
    (_last_hour_money, (gbp::ns_api::money))
    // table 'user_payments_in'
    (_payments_in, (std::vector<ns_api::ns_common::user_info::user_payment_in>))
    // table 'user_payments_out'
    (_payments_out, (std::vector<user_info::user_payment_out>))
    ,
    GBP_DECLARE_TYPE(
     user_payment_in
     ,
     (_amount, (ns_api::money))
     (_bonuscode, (std::string))
     (_datetime, (ns_api::time_s))
     (_payment_system, (ns_api::e_payment_system))
    )
    GBP_DECLARE_TYPE(
     user_payment_out
     ,
     (_amount, (ns_api::money))
     (_datetime, (ns_api::time_s))
     (_payment_system, (ns_api::e_payment_system))
    )
   )

   GBP_DECLARE_ENUM(
     e_add_new_tourn_reply_type
     , gbp_i8
     , inline
     , (handler_create_sit_n_go_by_template)
       (try_create_next_sit_n_go)
       (create_chain_tournament_by_specified)
       (create_chain_qualifiers)
    )

   GBP_DECLARE_TYPE(
    tournament_template
    ,
    (_time_interval, (time_s))
    (_count, (gbp_u16))
    (_activate, (bool))
    (_tickets, (std::unordered_set<gbp_i64>))
    (_is_partner_tourn, (bool), (false))
   )

   GBP_DECLARE_TYPE(
    add_new_tournament_request
    ,
    (_reply_type, (e_add_new_tourn_reply_type))
    (_template, (ns_api::ns_common::tournament_template_t))
    // если для бд-возврата требует турнир
    (_tournament_id, (gbp_i64))
    // для tournament::create_chain_qualifiers()
    (_main_tourn_id, (gbp_i64))
   )

   // для ответа по созданию турнира
   GBP_DECLARE_TYPE(
    add_new_tournament_result
    ,
    (_reply_type, (e_add_new_tourn_reply_type))
    (_template, (ns_api::ns_common::tournament_template))
    (_list, (std::vector<gbp::ns_api::ns_common::tournament_settings>))
    (_tournament_id, (gbp_i64))
    (_main_tourn_id, (gbp_i64))
   )

   GBP_DECLARE_TYPE(
    sit_n_go_template_restore
    ,
    (_id, (gbp_i64))
    (_template_title, (std::string))
    (_count, (gbp_u16))
    (_activate, (bool))
    (_is_partner_tourn, (bool), (false))
    (_with_chain, (bool))
    (_title, (std::string))
    (_game_type, (ns_api::ns_game::e_game_type))
    (_game_limit, (ns_api::ns_game::e_game_limit))
    (_game_speed, (ns_api::ns_game::e_game_speed))
    (_tournament_type, (ns_api::ns_tournament_types::e_tournament_type))
    (_users_count, (gbp_u8))
    (_img_title, (std::pair<std::string, std::string>))
    (_user_vip_status, (ns_vip_system::e_status))
    (_min_max_gamers_count, (std::pair<gbp_u64, gbp_u64>))
    (_fee, (double))
    (_buy_in, (money))
    (_bounty, (money))
    (_time_after_finishing, (ns_api::time_s), (300))
    (_bounty_progressive, (bool))
    (_addon_break_number, (gbp_i8))
    (_late_reg_minutes, (ns_api::time_s))
    (_stack_type, (ns_api::ns_tournament_types::e_tournament_stack))
    (_stack, (ns_api::money))
    (_warranty_money, (ns_api::money))
    (_registration_datetime, (ns_api::time_s))
    (_start_datetime, (ns_api::time_s))
    (_tickets, (std::unordered_set<gbp_i64>))
    (_rebuys, (ns_api::rebuys_t))
    (_addon, (ns_api::addon_t))
    (_round_conditions, (std::vector<ns_api::round_conditions_t>))
    (_win_payments, (std::vector<ns_api::win_payments_t>))
   )

   GBP_DECLARE_TYPE(
    email_info
    ,
    (_locale, (std::string))
    (_subject, (std::string))
    (_text, (std::string))
    (_id, (gbp_i64))
    (_type, (e_mail_type))
    ,
    GBP_DECLARE_ENUM(
     e_mail_type
     , gbp_u8
     , friend
     , (email_changing)
       (new_password)
       (activation)
       (password_changing)
       (verification_docs_request)
       (verification_confirmation)
       (money_out_successful)
    )
   )

   GBP_DECLARE_TYPE(
    admin_template
    ,
    (_id, (gbp_i64))
    (_type, (e_tournament_template_type))
    (_title, (std::string))
    (_sort_index, (gbp_i64))
    (_data, (std::string))
    ,
    GBP_DECLARE_ENUM(
     e_tournament_template_type
     , gbp_i8
     , friend
     , (levels)
       (payments)
       (tournament)
    )
   )

   // статистика за сутки
   GBP_DECLARE_TYPE(
    stat_item
    ,
    (_daily_start_time, (ns_api::time_s))
    (_registrations_count, (gbp_i32))
    (_deposits_count, (gbp_i32))
    (_deposits_all_money, (ns_api::money))
    (_avarage_deposit, (ns_api::money))
    (_payments_out_count, (gbp_i32))
    (_payments_out_all_money, (ns_api::money))
    (_avarage_payment_out, (ns_api::money))

    // сумма рейков со всех типов игр (кеш, турниры и sit_n_go), центы * 100
    (_rake_cash, (ns_api::money))
    (_rake_tournament, (ns_api::money))
    (_rake_sit_n_go, (ns_api::money))
    // сумма рейков рума, центы * 100
    (_rake_room_sum, (ns_api::money))
    // сумма рейков игроков, центы * 100
    (_rake_players_sum, (ns_api::money))

    // среднее кол-во игроков онлайн (фиксируются каждый час)
    // час (0 - 23) и кол-во
    (_players_online, (std::unordered_map<gbp_i32, gbp_i32>), (init_()))
    // среднее кол-во игроков за столами (фиксируются каждый час)
    (_players_in_games, (std::unordered_map<gbp_i32, gbp_i32>), (init_()))
    ,
    private:
    static std::unordered_map<gbp_i32, gbp_i32> init_()
    {
     static std::unordered_map<gbp_i32, gbp_i32> res;
     for (gbp_i32 i = 0; i < 24; ++i) {
      res[i] = 0;
     }
     return res;
    }
    public:
   )

   GBP_DECLARE_TYPE(
    disconnected_user_info
    ,
    (_id, (gbp_i64))
    (_is_user, (bool))
    (_money, (ns_api::money))
    (_freeroll_money, (ns_api::money))
    (_last_activity, (ns_api::time_s))
    (_status, (ns_api::e_user_status))
    (_payments_all_in, (ns_api::money))
    (_payments_all_out, (ns_api::money))
    (_payments_all_wait_out, (ns_api::money))
    (_user_vip_status, (ns_vip_system::e_status))
    (_proceeds_from_cash, (ns_api::money))
    (_proceeds_from_tourn, (ns_api::money))
    (_bonus_money, (ns_api::money))
    (_tickets_json, (std::string))
    (_first_deposit_was_used, (bool))
   )

   // используется, например, при регистрации в турнире
   GBP_DECLARE_TYPE(
     tournament_user_info
     ,
     (_id, (gbp_i64))
     // логни
     (_nickname, (std::string))
     // тип бота
     (_nickname_suffix, (std::string))
     // страна
     (_country, (gbp_u16))
     // аватара игрока
     (_avatar, (std::string))
     // хэш аватара
     (_hash_avatar, (std::string))
     // все деньги игрока
     (_cash, (ns_api::money))
     // vip-статус игрока
     (_vip_status, (ns_vip_system::e_status))
     // зареган с помощью денег или квалификатора
     (_by_qualifier, (bool))
     // бот или нет
     (_is_user, (bool))
     // есть билет (т.е. скидка 100%)
     (_by_ticket, (bool))
     // ID билета
     (_ticket_id, (gbp_i64))
     // чистый buy-in турнира
     (_user_buy_in, (ns_api::money))
     // сколько потратил на регистрацию в турнире
     (_register_money, (ns_api::money))
     /**
      * выигрыш при завершении турнира: деньги или 
      * билет (в случае квалификатора) */
     (_finish_result, (e_finish_result))
     ,
     GBP_DECLARE_ENUM(
      e_finish_result
      , gbp_i8
      , friend
        // если ещё не вылетел с турнира
      , (e_empty)
        // выбыл с турнира с получением денег
        (e_money)
        // выбыл с турнира с билетом в гл. событие
        (e_ticket)
     )
    )

   GBP_DECLARE_ENUM(
     e_get_cash_next_ids_reply_type
     , gbp_i8
     , inline
     , (db_handler_add_user)
       (tournament_start)
    )

   GBP_DECLARE_TYPE(
     get_cash_next_ids_request_type
     ,
     (_type, (e_get_cash_next_ids_reply_type))
     // кол-во требуемых ID
     (_count, (gbp_i64))
     // если возврата требует турнир
     (_tournament_id, (gbp_i64))
     // если возврата требует турнир
     (_tourn_user_info, (ns_api::ns_common::tournament_user_info))
    )

   GBP_DECLARE_TYPE(
     get_cash_next_ids_reply_type
     ,
     (_type, (e_get_cash_next_ids_reply_type))
     (_tournament_id, (gbp_i64))
     (_tourn_user_info, (ns_api::ns_common::tournament_user_info))
     // список новых ID
     (_list, (std::vector<gbp_i64>))
    )

   GBP_DECLARE_TYPE(
    user_bonuses_info
    ,
    (_user_id, (gbp_i64))
    (_vip_status, (ns_vip_system::e_status))
    (_proceeds_from_cash, (ns_api::money))
    (_proceeds_from_tourn, (ns_api::money))
    (_tickets, (std::unordered_map<gbp_i64, ns_special_offers::user_ticket>))
    (_first_deposit, (ns_api::money))
    (_first_deposit_was_used, (bool))
    (_tournament_money, (ns_api::money))
   )

   GBP_DECLARE_TYPE(
    payment_in
    ,
    (_user_id, (gbp_i64))
    (_user_nickname, (std::string))
    (_amount, (ns_api::money))
    (_payment_system, (ns_api::e_payment_system))
    (_purse, (std::string))
    (_post_info, (std::string))
    (_bonuscode, (std::string))
    (_additional_money, (ns_api::money))
    (_prizes, (std::string))
   )
  } // ns_common

 } // namespace ns_api
} // namespace gbp