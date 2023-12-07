#pragma once

#include <string>
#include <vector>

#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_table
  {
   // high/low победители
   GBP_DECLARE_TYPE(
    winner_info_t
    ,
    (_user_id, (gbp_i64), (-1))
    (_new_cash, (money)) // деньги юзера после выигрыша
    (_winner_cash, (money)) // деньги выигрыша
    /**
     * true - деньги начислять из главного банка,
     * false - из побочного */
    (_main_pot, (bool), (true))
    // номер банка (если главный, то всегда 0)
    (_pot_number, (gbp_i8))
    (_combination, (ns_api::ns_table::cards_combination_t))
   )

   typedef std::pair<std::vector<ns_api::ns_table::winner_info_t>
                   , std::vector<ns_api::ns_table::winner_info_t>> hi_low_winners;

   typedef std::vector<std::pair<gbp_i64, cards>> users_and_cards;

   // состояние игрока
   GBP_DECLARE_TYPE(
    user_state_t
    ,
    (_left_table, (bool)) // покинул стол (нажал в лобби "выйти")
    (_sit_out, (bool)) // покинуть стол в конце раздачи
    (_frozen, (bool)) // заморожен
    (_anticipant, (bool)) // ожидающий
    (_folded, (bool)) // выбывший в текущей раздаче
    (_all_ined, (bool)) // игрок ушёл в all-in
    (_all_ined_in_cur_round, (bool)) // игрок ушёл в all-in на текущем круге
   )

   // данные игрока для отсыла в лобби
   GBP_DECLARE_TYPE(
    table_player_info_t
    ,
    (_id, (gbp_i64))
    (_GBP_DECLARE_TYPE_DECLARE_VAR_AS_UNUSED(_is_user, 0), (bool)) // true, если не бот
    (_pos, (gbp_i8))
    (_nickname, (std::string))
    (_country, (gbp_u16)) // страна юзера
    (_money, (money)) // все деньги игрока
    (_start_stack, (money)) // деньги игрока на начало раздачи
    (_bounty_progressive_money, (money)) // деньги "прогрессивного" баунти
    (_hash_avatar, (std::string))
    (_is_dealer, (bool))
    (_cards, (cards)) // карты игрока
    // сумма, которую поставил игрок в текущем круге
    (_round_bet, (money))
    // является ли текущим игроком
    (_is_current, (bool))
    (_vip_status, (ns_vip_system::e_status))
    (_state, (user_state_t))
   )

   // состояние стола
   GBP_DECLARE_TYPE(
    table_state_t
    ,
    /**
     * ID турнира. Если -1, то это кеш-игра.
     * Если > 0, то лобби дополнительно требует состояние турнира */
    (_tournament_id, (gbp_i64))
    // game id
    (_game_id, (gbp_i64))
    // большой блайнд
    (_big_blind, (money))
    // кол-во мест стола
    (_seats_count, (gbp_i8))
    // все игроки в данный момент
    (_players, (std::vector<ns_api::ns_table::table_player_info_t>))
    // текущий игрок, который делает ход
    (_current_user_pos, (gbp_i8))
    (_table_cards, (cards)) // карты стола
    (_all_pots, (std::vector<money>)) // все банки стола ([0] - главный банк)
    (_waiters_count, (gbp_i32)) // все ожидающие по данному столу
    (_time_short, (ns_api::time_s)) // короткое время
    (_time_main, (ns_api::time_s)) // основное время
    (_time_action_passed, (ns_api::time_s)) // прошло времени на ожидании хода от игрока
    (_time_action_left, (ns_api::time_s)) // осталось времени на ход игрока
    (_distribution_number, (gbp_i64)) // текущий номер раздачи
   )

   GBP_DECLARE_TYPE(
    tournament_state_t
    ,
    (_start_time, (time_s)) // дата/время старта турнира
    (_current_level, (gbp_u32)) // текущий раунд турнира
    (_rebuys, (rebuys_t))
    (_round_conditions, (std::vector<ns_api::round_conditions_t>))
   )

   // для установки денег игрокам
   GBP_DECLARE_TYPE(
    user_cashes_item_t
    ,
    (_user_id, (gbp_i64), (-1))
    (_cash, (money))
    (_round_bet, (money))
   )
  
   typedef std::vector<ns_api::ns_table::user_cashes_item_t> users_cashes_t;

   GBP_DECLARE_TYPE(
    recharge_money_t
    ,
    // вилка доступных денег для пополнения игрового стека у игрока
    (_money_from, (money))
    (_money_to, (money))
    // деньги на аккаунте
    (_account_money, (money))
    // деньги за столом
    (_in_game_money, (money))
    // минимальный и максимальный стеки игры
    (_min_stack, (money))
    (_max_stack, (money))
   )

   /**
    * при посадке юзера за стол ему передаются min/max денег, с которыми
    * он может сесть, позиция и т.п. */
   GBP_DECLARE_TYPE(
    sit_on_table_info_t
    ,
    (_money, (ns_api::ns_table::recharge_money_t))
    // время включения режима крысы
    (_rat_time, (ns_api::time_s))
    (_pos, (gbp_i8))
   )

   GBP_DECLARE_TYPE(
    money_info_t
    ,
    // сумма в банке текущего круга (не раздачи)
    (_current_pot, (money))
    /*
     * по-умолчанию размер ББ.
     * Для преддействий, если это фикс. лимит и больше чем второй круг, 
     * то размер будет в 2 раза больше */
    (_default_bet, (money))
    // текущая ставка стола
    (_current_bet, (money))
    // ставка юзера на текущей раздаче
    (_user_distr_bet, (money))
    // ставка юзера на текущем круге торгов
    (_user_bet, (money))
    // текущий стек (игровые деньги) юзера
    (_user_cash, (money))
   )

   GBP_DECLARE_TYPE(
    user_info_t
    ,
    (_id, (gbp_i64))
    // ник
    (_nickname, (std::string))
    // позиция за столом
    (_pos, (gbp_i8))
    // текущая ставка стола
    (_bet, (ns_api::money))
    // текущий стек игрока
    (_money, (ns_api::money))
    // дилер
    (_is_dealer, (bool))
    // состояние
    (_state, (ns_api::ns_table::user_state_t))
   )

   // инфа при передаче доступных ходов
   GBP_DECLARE_TYPE(
    action_info_t
    ,
    // текущий раунд раздачи (от 0 и выше)
    (_current_round, (gbp_u8))
    // текущий раунд турнира
    (_current_tourn_level, (gbp_i32))
    // короткое врмя на ход
    (_time_short, (ns_api::time_s))
    // основное время 
    (_time_main, (ns_api::time_s))
    // дополнительное
    (_time_additional, (ns_api::time_s))
    // данные по деньгам
    (_money_info, (ns_api::ns_table::money_info_t))
    // данные по юзерам
    (_users_info, (std::vector<ns_api::ns_table::user_info_t>))
   )

   GBP_DECLARE_TYPE(
    tournament_dialog_info
    ,
    (_type, (ns_api::ns_table::tournament_dialog_info::e_dialog_type))
    // позиция игрока в турнире
    (_winner_pos, (gbp_i32))
    // является ли победителем (1 место)
    (_is_winner, (bool))
    (_money, (money)) // выигранные деньги
    // ID главного события
    (_main_event_id, (gbp_i64))
    ,
    GBP_DECLARE_ENUM(
     e_dialog_type
     , gbp_u8
     , friend
     ,
      /**
       * "Поздравляем! Вы вошли в призовую зону и заняли {_winner_pos} место.
       * Деньги будут начисленны вам после аддона" */
      (_type1)
      // "Вы выбыли из турнира. Свою позицию вы узнаете после аддона."
      (_type2)
      // "Вы выбыли из турнира заняв {_winner_pos} место"
      (_type3)
      // "Турнир завершён"
      (_type4)
      /**
       * Поздравляем! Вы вошли в призовую зону и заняли {_winner_pos}
       * место. Ваш выиграш составил {сумма} */
      (_type5)
      /**
       * "Поздравляем, Вы стали победителем турнира! Вы выиграли билет на 
       * турнир {_main_event_id} и {_money}" */
      (_type6)
      /**
       * "Поздравляем, Вы стали победителем турнира! Вы выиграли билет на 
       * турнир {_main_event_id}" */
      (_type7)
      // "Поздравляем! Вы стали победителем турнира (1-е место) {_money}, {кубок}"
      (_type8)
      // "Вы выбыли из турнира. Свою позицию вы узнаете после периода поздней регистрации"
      (_type9)
     )
    )

   // для регистрации юзера в турнире
   GBP_DECLARE_TYPE(
    tourn_register_info
    ,
    // доступные деньги юзера
    (_user_available_money, (ns_api::money))
    /**
     * стоимость турнира для игрока, по-умолчанию 
     * это "комиссия + бай_ин + баунти" */
    (_tournament_price, (ns_api::money))
    // кол-во билетов юзера, которые также указаны для турнира
    (_user_tickets_count, (gbp_i32))
   )

  } // namespace ns_table

 } // namespace ns_api
} // namespace gbp