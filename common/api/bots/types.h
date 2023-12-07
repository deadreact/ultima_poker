#pragma once

#include <string>
#include <vector>
#include <map>

#include "../api_common.h"
#include "../table/types.h"
#include "../declare_type.h"

namespace gbp
{
 namespace ns_api
 {

  namespace ns_game
  {
   GBP_DECLARE_ENUM(
    e_game_status
    , gbp_u8
    , inline
    , (indefinite)
    (wait)
    (reg)
    (started)
    (finished)
   )
   GBP_DECLARE_ENUM(
    e_game_activity
    , gbp_u8
    , inline
    , (full)// полностью заполненные
    (active)// активные
    (passive) // пустые
   )
  } // namespace ns_game

  namespace ns_bots
  {
   // информация по боту для его авторизации на игровом сервере
   GBP_DECLARE_TYPE(
    bot_info
    ,
    (_id, (gbp_i64))
    (_nickname, (std::string))
    (_nickname_suffix, (std::string))
    (_account_money, (ns_api::money))
    (_freeroll_money, (ns_api::money))
    (_avatar, (std::string))
    (_hash_avatar, (std::string))
   )

   GBP_DECLARE_TYPE(
    country_info_t
    ,
    // ID страны
    (_country_id, (gbp_i64))
    // название страны
    (_country_title, (std::string))
    // кол-во свободных ников по стране
    (_fee_nicknames_count, (gbp_i64))
   )

   GBP_DECLARE_TYPE(
    character_info_test
    ,
    // кол-во ботов онлайн по данному характеру
    (_bots_online_count, (gbp_i64))
    /**
     * кол-во ботов с доступными счётчиками кешей.
     * Например, один бот имеет 2 доступных кеша и другой - 3. Значение будет 2. */
    (_bots_online_cashes_count, (gbp_i64))
    // кол-во ботов с доступными счётчиками турниров
    (_bots_online_tournaments_count, (gbp_i64))
    // кол-во ботов с доступными счётчиками sit_n_go
    (_bots_online_sit_n_go_count, (gbp_i64))
   )

   namespace ns_distr_templates
   {
    GBP_DECLARE_ENUM(
     e_game_item_type
     , gbp_u8
     , inline
     , (cash)
     (tournament)
     (sit_n_go)
    )

    GBP_DECLARE_TYPE(
     filter
     ,
     (_game_type, (e_filter_game_type))
     (_type, (e_filter_type))
     ,
     GBP_DECLARE_ENUM(
      e_filter_type
      , gbp_u8
      , friend
      , (all)
      (light)
      (middle)
      (high)
      (good)
     )
     GBP_DECLARE_ENUM(
      e_filter_game_type
      , gbp_u8
      , friend
      , (all)
      (holdem)
      (omaha)
     )
    )

    GBP_DECLARE_ENUM(
     e_type
     , gbp_u8
     , inline
     , (light)
     (middle)
     (high)
     (good)
    )

    GBP_DECLARE_ENUM(
     e_game_type
     , gbp_u8
     , inline
     , (holdem)
     (omaha)
    )

    // стр-ра шаблона раздачи
    GBP_DECLARE_TYPE(
     d_template
     ,
     (_id, (gbp_i64))
     (_type, (e_type))
     (_game_type, (e_game_type))
     (_table_cards, (ns_api::ns_table::cards))
     (_user_cards, (ns_api::ns_table::cards))
     (_bot_cards, (ns_api::ns_table::cards))
    )

    GBP_DECLARE_TYPE(
     set_template_req_type
     ,
     (_tournament_id, (gbp_i64))
     (_game_id, (gbp_i64))
     (_user_id, (gbp_i64))
     (_user_nickname, (std::string))
     (_user_stack, (ns_api::money))
     // случайно выбранный бот из допустимых за столом
     (_bot_id, (gbp_i64))
     (_bot_nickname, (std::string))
     (_bot_stack, (ns_api::money))
     // текущий номер раздачи
     (_current_distribution, (gbp_i64))
     (_game_item_type, (e_game_item_type))
     (_template_game_type, (e_game_type))
     (_template_type, (e_type))
    )
   } // ns_distr_templates

   /**
    * лимиты по ББ или бай-инам.
    * По размерам ББ:
    * микро -  $0.02 - $0.10 
    * низкие -  0.2 - 0.5
    * средние - 1 - 2
    * высокие - от 4
    * 
    * По бай-инам
    * микро - до $9
    * низкие - 10 - 49
    * средние - 50 - 199
    * высокие - от 200 */
   GBP_DECLARE_ENUM(
    e_money_limit
    , gbp_u8
    , inline
    , (micro)
    (low)
    (middle)
    (high)
   )

   // для добавления контекстов ботов
   GBP_DECLARE_TYPE(
    bot_runtime_info
    ,
    (_id, (gbp_i64))
    (_bot_character, (gbp_i64))
    (_country, (gbp_i32))
    (_locale, (std::string))
    (_login, (std::string))
    (_login_suffix, (std::string))
    (_cash, (ns_api::money))
    (_freeroll_cash, (ns_api::money))
    (_avatar, (std::string))
    (_hash_avatar, (std::string))
    (_vip_status, (ns_vip_system::e_status))
   )

   /***************************************************************************/

   // соответствия для таблиц БД на бот-сервере для их блокировок
   namespace ns_database
   {
    GBP_DECLARE_ENUM(
     e_table
     , gbp_u8
     , inline
     , (bot_chars)// характеры
     (account_hardware)// железки админов
     (planned_distribution)// запланированные раздачи
     (characters_balance)// соотношения характеров и их количеств ботов
     (users)// админы
     (tables) // правила для столов
    )
   } // namespace nsDatabase

   /***************************************************************************/

   // блокировки таблиц целиком
   namespace ns_locks
   {
    GBP_DECLARE_ENUM(
     e_table
     , gbp_u8
     , inline
     , (accounts)// аккаунты
     (characters)// характеры ботов
     (cash_rules)// диалог настроек общих правил кешей
     (tournament_rules)// диалог настроек общих правил турниров
     (sit_n_go_rules) // диалог настроек общих правил sit_n_go
    )
   } // namespace ns_locks

   /***************************************************************************/

   // аккаунт админки. Также нужно для инфы, отображаемой в списке аккаунтов
   GBP_DECLARE_TYPE(
    account_info
    ,
    (_id, (gbp_i64))
    (_login, (std::string))
    (_pass, (std::string))
    (_status, (e_status))
    (_usertype, (e_usertype))
    ,
    GBP_DECLARE_ENUM(
     e_status
     , gbp_u8
     , friend
     , (online)
     (offline)
     (blocked)
    )
    GBP_DECLARE_ENUM(
     e_usertype
     , gbp_u8
     , friend
     , (admin)
     (user)
     (chat_admin)
    )
   )

   GBP_DECLARE_TYPE(
    accounts_filter
    ,
    (_usertypes, (e_usertypes))
    ,
    GBP_DECLARE_ENUM(
     e_usertypes
     , gbp_u8
     , friend
     , (all)
     (only_admins)
     (only_users)
     (only_chat_admins)
    )
   )

   /***************************************************************************/

   // материнка и hdd аккаунта
   GBP_DECLARE_TYPE(
    account_hardware
    ,
    (_id, (gbp_i64))
    (_user_id, (gbp_i64))
    (_mb_id, (std::string))
    (_hdd_id, (std::string))
    (_place, (std::string))
    (_status, (e_status))
    ,
    GBP_DECLARE_ENUM(
     e_status
     , gbp_u8
     , friend
     , (activated)
     (blocked)
    )
   )

   /****************************** CHARACTERS *********************************/

   // для "умных" ходов ботов
   GBP_DECLARE_ENUM(
    e_bot_ai_character
    , gbp_u8
    , inline
    , (rock)
    (tight)
    (lose)
    (fish)
    (whale)
   )

   GBP_DECLARE_TYPE(
    bet_raise_percents
    ,
    /**
     * для префлопа учитываются только рейзы (без бетов):
     * - _raise_2_bet
     * - _raise_3_bet
     * - _raise_pot,
     * Процент на выпадение варианта ставки по кол-ву миним. рейзов 
     * (т.е. если > 0, то учитывать _raise_min_count)
     * - _raise_min_percents ("По мин. рейзам")
     * - _raise_min_count ("Кол-во мин. рейзов") */
    (_preflop, (percents))
    /**
     * для остальных типов кругов, для рейзов:
     * - _raise_2_bet
     * - _raise_pot_half
     * - _raise_pot
     * - _raise_min_percents ("По мин. рейзам")
     * - _raise_min_count ("Кол-во мин. рейзов")
     * для остальных типов кругов, для бетов:
     * - _bet
     * - _bet_pot_half
     * - _bet_pot
     * - _bet_manual
     * Процент на выпадение варианта ставки по проценту от банка 
     * (т.е. если > 0, то учитывать _bet_pot_percents_to)
     * - _bet_percents_for_blinds ("Процент для блайндов")
     * - _bet_pot_percents_to ("Процент от банка, до") */
    (_flop, (percents))
    (_tern, (percents))
    (_river, (percents))
    ,
    GBP_DECLARE_TYPE(
     percents
     ,
     (_bet, (gbp_u8))
     (_bet_pot, (gbp_u8))
     (_bet_pot_half, (gbp_u8))
     (_bet_manual, (gbp_u8))
     (_bet_percents_for_blinds, (gbp_u8))
     (_bet_pot_percents_to, (gbp_i32))
     (_raise_2_bet, (gbp_u8))
     (_raise_3_bet, (gbp_u8))
     (_raise_pot, (gbp_u8))
     (_raise_pot_half, (gbp_u8))
     (_raise_min_percents, (gbp_u8))
     (_raise_min_count, (gbp_i32))
    )
   )

   // для ожидания перед ходом
   /**
    * Пример по типам:
    * - будет делать на первой секунде (преддействие);
    * - будет делать действие на 2-3 секунде;
    * - будет делать действие в первые 50% времени обычного хода;
    * - будет делать действие во вторые 50% времени обычного хода;
    * - будет делать действие в дополнительное время;
    * - второй процент на доп. время */
   GBP_DECLARE_TYPE(
    action_times
    ,
    /*** для столов более чем на 2 места ***/
    // preflop, fold
    (_type1_preflop_fold_type1, (gbp_u8))
    (_type1_preflop_fold_type2, (gbp_u8))
    (_type1_preflop_fold_type3, (gbp_u8))
    (_type1_preflop_fold_type4, (gbp_u8))
    (_type1_preflop_fold_type5, (gbp_u8))
    (_type1_preflop_fold_additional_percent, (gbp_u8))
    // check
    (_type1_preflop_check_type1, (gbp_u8))
    (_type1_preflop_check_type2, (gbp_u8))
    (_type1_preflop_check_type3, (gbp_u8))
    (_type1_preflop_check_type4, (gbp_u8))
    (_type1_preflop_check_type5, (gbp_u8))
    (_type1_preflop_check_additional_percent, (gbp_u8))
    // call
    (_type1_preflop_call_type1, (gbp_u8))
    (_type1_preflop_call_type2, (gbp_u8))
    (_type1_preflop_call_type3, (gbp_u8))
    (_type1_preflop_call_type4, (gbp_u8))
    (_type1_preflop_call_type5, (gbp_u8))
    (_type1_preflop_call_additional_percent, (gbp_u8))
    // raise
    (_type1_preflop_raise_type2, (gbp_u8))
    (_type1_preflop_raise_type3, (gbp_u8))
    (_type1_preflop_raise_type4, (gbp_u8))
    (_type1_preflop_raise_type5, (gbp_u8))
    (_type1_preflop_raise_additional_percent, (gbp_u8))
    // all-in
    (_type1_preflop_all_in_type2, (gbp_u8))
    (_type1_preflop_all_in_type3, (gbp_u8))
    (_type1_preflop_all_in_type4, (gbp_u8))
    (_type1_preflop_all_in_type5, (gbp_u8))
    (_type1_preflop_all_in_additional_percent, (gbp_u8))
    // остальные круги раздачи, fold
    (_type1_others_fold_type1, (gbp_u8))
    (_type1_others_fold_type2, (gbp_u8))
    (_type1_others_fold_type3, (gbp_u8))
    (_type1_others_fold_type4, (gbp_u8))
    (_type1_others_fold_type5, (gbp_u8))
    (_type1_others_fold_additional_percent, (gbp_u8))
    // check
    (_type1_others_check_type1, (gbp_u8))
    (_type1_others_check_type2, (gbp_u8))
    (_type1_others_check_type3, (gbp_u8))
    (_type1_others_check_type4, (gbp_u8))
    (_type1_others_check_type5, (gbp_u8))
    (_type1_others_check_additional_percent, (gbp_u8))
    // bet
    (_type1_others_bet_type2, (gbp_u8))
    (_type1_others_bet_type3, (gbp_u8))
    (_type1_others_bet_type4, (gbp_u8))
    (_type1_others_bet_type5, (gbp_u8))
    (_type1_others_bet_additional_percent, (gbp_u8))
    // call
    (_type1_others_call_type1, (gbp_u8))
    (_type1_others_call_type2, (gbp_u8))
    (_type1_others_call_type3, (gbp_u8))
    (_type1_others_call_type4, (gbp_u8))
    (_type1_others_call_type5, (gbp_u8))
    (_type1_others_call_additional_percent, (gbp_u8))
    // raise
    (_type1_others_raise_type2, (gbp_u8))
    (_type1_others_raise_type3, (gbp_u8))
    (_type1_others_raise_type4, (gbp_u8))
    (_type1_others_raise_type5, (gbp_u8))
    (_type1_others_raise_additional_percent, (gbp_u8))
    // all_in
    (_type1_others_all_in_type2, (gbp_u8))
    (_type1_others_all_in_type3, (gbp_u8))
    (_type1_others_all_in_type4, (gbp_u8))
    (_type1_others_all_in_type5, (gbp_u8))
    (_type1_others_all_in_additional_percent, (gbp_u8))

    /*** для столов на 2 места ***/
    // preflop, fold
    (_type2_preflop_fold_type2, (gbp_u8))
    (_type2_preflop_fold_type3, (gbp_u8))
    (_type2_preflop_fold_type4, (gbp_u8))
    (_type2_preflop_fold_type5, (gbp_u8))
    (_type2_preflop_fold_additional_percent, (gbp_u8))
    // check
    (_type2_preflop_check_type2, (gbp_u8))
    (_type2_preflop_check_type3, (gbp_u8))
    (_type2_preflop_check_type4, (gbp_u8))
    (_type2_preflop_check_type5, (gbp_u8))
    (_type2_preflop_check_additional_percent, (gbp_u8))
    // call
    (_type2_preflop_call_type2, (gbp_u8))
    (_type2_preflop_call_type3, (gbp_u8))
    (_type2_preflop_call_type4, (gbp_u8))
    (_type2_preflop_call_type5, (gbp_u8))
    (_type2_preflop_call_additional_percent, (gbp_u8))
    // raise
    (_type2_preflop_raise_type2, (gbp_u8))
    (_type2_preflop_raise_type3, (gbp_u8))
    (_type2_preflop_raise_type4, (gbp_u8))
    (_type2_preflop_raise_type5, (gbp_u8))
    (_type2_preflop_raise_additional_percent, (gbp_u8))
    // all-in
    (_type2_preflop_all_in_type3, (gbp_u8))
    (_type2_preflop_all_in_type4, (gbp_u8))
    (_type2_preflop_all_in_type5, (gbp_u8))
    (_type2_preflop_all_in_additional_percent, (gbp_u8))
    // остальные круги раздачи, fold
    (_type2_others_fold_type2, (gbp_u8))
    (_type2_others_fold_type3, (gbp_u8))
    (_type2_others_fold_type4, (gbp_u8))
    (_type2_others_fold_type5, (gbp_u8))
    (_type2_others_fold_additional_percent, (gbp_u8))
    // check
    (_type2_others_check_type2, (gbp_u8))
    (_type2_others_check_type3, (gbp_u8))
    (_type2_others_check_type4, (gbp_u8))
    (_type2_others_check_type5, (gbp_u8))
    (_type2_others_check_additional_percent, (gbp_u8))
    // bet
    (_type2_others_bet_type2, (gbp_u8))
    (_type2_others_bet_type3, (gbp_u8))
    (_type2_others_bet_type4, (gbp_u8))
    (_type2_others_bet_type5, (gbp_u8))
    (_type2_others_bet_additional_percent, (gbp_u8))
    // call
    (_type2_others_call_type2, (gbp_u8))
    (_type2_others_call_type3, (gbp_u8))
    (_type2_others_call_type4, (gbp_u8))
    (_type2_others_call_type5, (gbp_u8))
    (_type2_others_call_additional_percent, (gbp_u8))
    // raise
    (_type2_others_raise_type2, (gbp_u8))
    (_type2_others_raise_type3, (gbp_u8))
    (_type2_others_raise_type4, (gbp_u8))
    (_type2_others_raise_type5, (gbp_u8))
    (_type2_others_raise_additional_percent, (gbp_u8))
    // all_in
    (_type2_others_all_in_type2, (gbp_u8))
    (_type2_others_all_in_type3, (gbp_u8))
    (_type2_others_all_in_type4, (gbp_u8))
    (_type2_others_all_in_type5, (gbp_u8))
    (_type2_others_all_in_additional_percent, (gbp_u8))
   )

   // для обновления каких-либо данных у характера
   GBP_DECLARE_TYPE(
    character_update_info_t
    ,
    (_character_id, (gbp_i64))
    (_freeze_percents, (std::pair<gbp_i8, gbp_i8>))
    // вероятность автодокупки, от-до
    (_autorecharge, (std::pair<gbp_i8, gbp_i8>))
    // докупка если есть фишки, от-до
    (_recharge_money_exists, (std::pair<gbp_i8, gbp_i8>))
    // докупка если нет фишек, от-до
    (_recharge_money_not_exists, (std::pair<gbp_i8, gbp_i8>))
    // срок жизни в раздачах - от, -1 если не учитывать
    (_distribution_count_from, (gbp_i32))
    // срок жизни в раздачах - до, -1 если не учитывать
    (_distribution_count_to, (gbp_i32))
    // время ожидания хода
    (_action_times, (action_times))
   )

   // характер бота
   GBP_DECLARE_TYPE(
    character
    ,
    (_id, (gbp_i64))
    // название характера
    (_title, (std::string))
    // характер для AI
    (_ai_character, (e_bot_ai_character))
    // суточное поведение
    (_daily_behavior, (e_daily_behavior))
    // подтип (только для регуляров и обычных)
    (_behavior_subtype, (e_behavior_subtype))
    // проценты выпаданий по бетам и рейзам
    (_bet_raise_percents, (bet_raise_percents))
    // минимальная сумма на аккаунте бота (необходима для регистрации ботов в БД)
    (_money_min, (money))
    // максимальная сумма на аккаунте бота
    (_money_max, (money))
    // количество создаваемых в БД ботов (страна - кол-во)
    (_bot_counts, (std::unordered_map<gbp_i64, gbp_i64>))
    /**
     * процент заморозки бота (второе значение - дополнительный процент, которое 
     * используется, если выпал первый). Напр., если 5%, то в 5% случаев бот не будет 
     * ходить и, соответственно будет заморожен игровым сервером */
    (_freeze_percents, (std::pair<gbp_i8, gbp_i8>))
    // вероятность автодокупки, от-до
    (_autorecharge, (std::pair<gbp_i8, gbp_i8>))
    // докупка если есть фишки, от-до
    (_recharge_money_exists, (std::pair<gbp_i8, gbp_i8>))
    // докупка если нет фишек, от-до
    (_recharge_money_not_exists, (std::pair<gbp_i8, gbp_i8>))
    // срок жизни в раздачах - от, -1 если не учитывать
    (_distribution_count_from, (gbp_i32))
    // срок жизни в раздачах - до, -1 если не учитывать
    (_distribution_count_to, (gbp_i32))
    // срок жизни в выигранных деньгах, -1 если не учитывать (пока не используется)
    (_win_money, (money))
    // срок жизни в проигранных деньгах, -1 если не учитывать (пока не используется)
    (_lost_money, (money))
    // вероятности действий при посадке за стол (гост. ставка и ББ)
    (_sit_on_table_action, (std::pair<gbp_u8, gbp_u8>))
    // для турниров с бай-ином < $9
    (_tourn_settings_type1, (tournament_settings))
    // для турниров с бай-ином $10 - $49
    (_tourn_settings_type2, (tournament_settings))
    // для турниров с бай-ином $150-199
    (_tourn_settings_type3, (tournament_settings))
    // для турниров с бай-ином > $200
    (_tourn_settings_type4, (tournament_settings))
    // время ожидания хода
    (_action_times, (action_times))
    ,
    // у характера может быть только один тип
    GBP_DECLARE_ENUM(
     e_daily_behavior
     , gbp_u8
     , friend
     , (regular)
     (usual)
     (insurance)
    )
     // учитывать, если бот либо regular, либо usual
    GBP_DECLARE_ENUM(
     e_behavior_subtype
     , gbp_u8
     , friend
     // турнирный игрок
     , (mtt)
     // кешовый игрок
     (cash_player)
     // и турниры, и кеша
     (mix)
     // играет больше в sit_n_go
     (sit_n_go)
    )
    GBP_DECLARE_TYPE(
     tournament_settings
     ,
     // вероятность ребая, от-до
     (_rebuys_percent, (std::pair<gbp_u8, gbp_u8>))
     // вероятность двойного ребая, от-до
     (_rebuys_double_percent, (std::pair<gbp_u8, gbp_u8>))
     // вероятность ребая, если есть фишки, от-до
     (_rebuys_with_money_percent, (std::pair<gbp_u8, gbp_u8>))
     // вероятность аддона, от-до
     (_addon_percent, (std::pair<gbp_u8, gbp_u8>))
    )
   )

   /**
    * для того, чтобы отобразить админу актуальную информацию
    * о существующем характере используются два типа:
    * - gbp::ns_api::ns_bots::character
    * - gbp::ns_api::ns_bots::character_info
    * Первый тип запрашивается админкой в момент открытия диалога
    * характеров (сразу по всем характерам), а ниже описана дополнительная
    * инфа по характеру, которую также надо отобразить в диалоге */
   GBP_DECLARE_TYPE(
    character_info
    ,
    // ID характера
    (_id, (gbp_i64))
    // всего ботов
    (_bots_count, (gbp_u32))
    // всего задействовано ботов
    (_bots_used, (gbp_u32))
    // всего бездействующих (т.е. ожидающих своих игр) ботов
    (_bots_unused, (gbp_u32))
    // список доступных стран (название - количество ботов)
    (_countries, (std::unordered_map<std::string, gbp_i32>))
   )

   /************************** CASHES & CASH_RULES ***************************/

   GBP_DECLARE_TYPE(
    cash_rules_character_info
    ,
    // ID характера
    (_id, (gbp_i64))
    // название характера
    (_title, (std::string))
   )

   // правила ожидающих стола
   GBP_DECLARE_TYPE(
    cash_rules_waiters
    ,
    // если 0, то не учитывать
    (_percent, (gbp_u8))
    // кол-во ожидающих, от
    (_count_from, (gbp_u8))
    // кол-во ожидающих, до
    (_count_to, (gbp_u8))
   )

   // общие правила для кешей
   GBP_DECLARE_TYPE(
    cash_rules
    ,
    // один из 24 типов кеша
    (_game_type, (e_game_type))
    (_cash_rules_type1, (cash_rules_type_1))
    (_cash_rules_type2, (cash_rules_type_2))
    (_cash_rules_type3, (cash_rules_type_3))
    (_cash_rules_type4, (cash_rules_type_4))
    (_cash_rules_type5, (cash_rules_type_5))

    /* для хедз-апа (2 игрока) */
    (_heads_up_change_rules_from, (gbp_i32))
    // внешний интервал изменения правил (мин.), до
    (_heads_up_change_rules_to, (gbp_i32))
    // интервал захода ботов в кеш (сек.), от
    (_heads_up_enter_cash_from, (gbp_i32))
    // интервал захода ботов в кеш (сек.), до
    (_heads_up_enter_cash_to, (gbp_i32))
    // интервал выхода бота по бездействию (сек.), от
    (_heads_up_inactivity_timeout_from, (gbp_i32))
    // интервал выхода бота по бездействию (сек.), до
    (_heads_up_inactivity_timeout_to, (gbp_i32))
    // суточный интервал (сек.), от
    (_heads_up_occupancy_timeout_from, (gbp_i32))
    // суточный интервал (сек.), до
    (_heads_up_occupancy_timeout_to, (gbp_i32))

    /* для остальных количеств игроков за столом */
    // внешний интервал изменения правил (мин.), от
    (_change_rules_from, (gbp_i32))
    // внешний интервал изменения правил (мин.), до
    (_change_rules_to, (gbp_i32))
    // интервал захода ботов в кеш (сек.), от
    (_enter_cash_from, (gbp_i32))
    // интервал захода ботов в кеш (сек.), до
    (_enter_cash_to, (gbp_i32))
    // интервал выхода бота по бездействию (сек.), от
    (_inactivity_timeout_from, (gbp_i32))
    // интервал выхода бота по бездействию (сек.), до
    (_inactivity_timeout_to, (gbp_i32))
    // суточный интервал (сек.), от
    (_occupancy_timeout_from, (gbp_i32))
    // суточный интервал (сек.), до
    (_occupancy_timeout_to, (gbp_i32))
    ,
    // тип кеша 1 (1-2 игрока)
    GBP_DECLARE_TYPE(
     cash_rules_type_1
     ,
     (_seat1, (gbp_i8))
     (_seat2, (gbp_i8))
     // характеры типа
     (_characters, (std::vector<cash_rules_character_info>))
     // правила по ожидающим
     (_waiters, (cash_rules_waiters))
    )
    // тип кеша 2 (1-6 игроков)
    GBP_DECLARE_TYPE(
     cash_rules_type_2
     ,
     (_seat1, (gbp_i8))
     (_seat2, (gbp_i8))
     (_seat3, (gbp_i8))
     (_seat4, (gbp_i8))
     (_seat5, (gbp_i8))
     (_seat6, (gbp_i8))
     (_characters, (std::vector<cash_rules_character_info>))
     (_waiters, (cash_rules_waiters))
    )
    // тип кеша 3 (1-8 игроков)
    GBP_DECLARE_TYPE(
     cash_rules_type_3
     ,
     (_seat1, (gbp_i8))
     (_seat2, (gbp_i8))
     (_seat3, (gbp_i8))
     (_seat4, (gbp_i8))
     (_seat5, (gbp_i8))
     (_seat6, (gbp_i8))
     (_seat7, (gbp_i8))
     (_seat8, (gbp_i8))
     (_characters, (std::vector<cash_rules_character_info>))
     (_waiters, (cash_rules_waiters))
    )
    // тип кеша 4 (1-9 игроков)
    GBP_DECLARE_TYPE(
     cash_rules_type_4
     ,
     (_seat1, (gbp_i8))
     (_seat2, (gbp_i8))
     (_seat3, (gbp_i8))
     (_seat4, (gbp_i8))
     (_seat5, (gbp_i8))
     (_seat6, (gbp_i8))
     (_seat7, (gbp_i8))
     (_seat8, (gbp_i8))
     (_seat9, (gbp_i8))
     (_characters, (std::vector<cash_rules_character_info>))
     (_waiters, (cash_rules_waiters))
    )
    // тип кеша 5 (1-10 игроков)
    GBP_DECLARE_TYPE(
     cash_rules_type_5
     ,
     (_seat1, (gbp_i8))
     (_seat2, (gbp_i8))
     (_seat3, (gbp_i8))
     (_seat4, (gbp_i8))
     (_seat5, (gbp_i8))
     (_seat6, (gbp_i8))
     (_seat7, (gbp_i8))
     (_seat8, (gbp_i8))
     (_seat9, (gbp_i8))
     (_seat10, (gbp_i8))
     (_characters, (std::vector<cash_rules_character_info>))
     (_waiters, (cash_rules_waiters))
    )
    /**
     * По размерам ББ:
     * микро -  $0.02 - $0.1
     * низкие -  0.2 - 0.5
     * средние - 1 - 2
     * высокие  от 4 */
    GBP_DECLARE_ENUM(
     e_game_type
     , gbp_u8
     , friend
     , (holdem_fixed_micro)
     (holdem_fixed_low)
     (holdem_fixed_middle)
     (holdem_fixed_high)
     (holdem_pot_micro)
     (holdem_pot_low)
     (holdem_pot_middle)
     (holdem_pot_high)
     (holdem_no_micro)
     (holdem_no_low)
     (holdem_no_middle)
     (holdem_no_high)
     (omaha_fixed_micro)
     (omaha_fixed_low)
     (omaha_fixed_middle)
     (omaha_fixed_high)
     (omaha_pot_micro)
     (omaha_pot_low)
     (omaha_pot_middle)
     (omaha_pot_high)
     (omaha_no_micro)
     (omaha_no_low)
     (omaha_no_middle)
     (omaha_no_high)
    )
   )

   // заполняемость кешей в зависимости от времени суток
   GBP_DECLARE_TYPE(
    cashes_occupancy
    ,
    // тип игры
    (_game_type, (ns_api::ns_bots::cash_rules::e_game_type))
    /**
     * процент заполняемости на каждый час суток в
     * зависимости от размера блайндов.
     * тип_по_блайндам: час (0-23) и процент */
    (_percents, (std::map<gbp_u8, gbp_u8>))
    // заметки
    (_notes, (std::string))
   )

   // от игрового к бот-серверу
   GBP_DECLARE_TYPE(
    users_stat,
    // ID кеша
    (_game_id, (gbp_i64))
    // фрирольный кеш или нет
    (_is_freeroll, (bool))
    // ID турнира
    (_tournament_id, (gbp_i64))
    // иды ботов за столом
    (_bot_ids, (std::unordered_set<gbp_i64>))
    // иды юзеров за столом
    (_user_ids, (std::unordered_set<gbp_i64>))
    // иды наблюдающих за столом
    (_viewer_ids, (std::unordered_set<gbp_i64>))
   )

   // от бот-сервера к бот-админке
   GBP_DECLARE_TYPE(
    users_stat_admin,
    // ID кеша
    (_game_id, (gbp_i64))
    // фрирольный кеш или нет
    (_is_freeroll, (bool))
    // ID турнира
    (_tournament_id, (gbp_i64))
    // кол-во всех юзеров
    (_all_users_count, (gbp_i64))
    // кол-во новых юзеров
    (_new_users_count, (gbp_i64))
    // кол-во всех наблюдающих
    (_all_viewers_count, (gbp_i64))
    // кол-во новых наблюдающих
    (_new_viewers_count, (gbp_i64))
   )

   // игры online для кеш и турниров
   GBP_DECLARE_TYPE(
    cash
    ,
    // ID игры
    (_id, (gbp_i64))
    // название
    (_title, (std::string))
    // мин/макс. стеки для входа
    (_min_stack, (ns_api::money))
    (_max_stack, (ns_api::money))
    // тип игры
    (_type, (ns_api::ns_game::e_game_type))
    // лимит игры
    (_limit, (ns_api::ns_game::e_game_limit))
    // текущая раздача
    (_current_distribution, (gbp_i64))
    // кол-во мест
    (_places_count, (gbp_i8))
    // кол-во игроков
    (_players_count, (gbp_i8))
    // ID'ы ботов
    (_bot_ids, (std::unordered_set<gbp_i64>))
    // кол-во людей
    (_people_count, (gbp_i8))
    // viewers count
    (_viewers_count, (gbp_i32))
    // состояние юзеров
    (_users_stat, (users_stat))
    // список свободных позиций
    (_free_places, (std::vector<gbp_u8>))
    // ББ
    (_big_blind, (ns_api::money))
    // ожидающие игры
    (_waiting_players, (gbp_i32))
    // заблокирован ли кеш в данный момент
    (_locked, (bool))
    // фрирольный
    (_is_freeroll, (bool))
    // ID заблокировавшего кеш, -1 если кеш не заблокирован
    (_owner_id, (gbp_i64))
    // никнейм заблокировавшего кеш. Пустой, если кеш не заблокирован
    (_owner_nickname, (std::string))
   )

   GBP_DECLARE_TYPE(
    cashes_filter
    ,
    // для всех численных полей: если стоит -1, то поле не учитывать
    // ID игры
    (_id, (gbp_i64))
    // название игры
    (_title, (std::string))
    // людей в кеше, от
    (_people_from, (gbp_i8))
    // людей в кеше, до
    (_people_to, (gbp_i8))
    // большой блайнд, от
    (_big_blind_from, (ns_api::money))
    // большой блайнд, до
    (_big_blind_to, (ns_api::money))
    // ожидающих, от
    (_waiting_players_from, (gbp_i32))
    // ожидающих, до
    (_waiting_players_to, (gbp_i32))
    // наблюдающих, от
    (_viewers_from, (gbp_i32))
    // наблюдающих, до
    (_viewers_to, (gbp_i32))
    /**
     * только те, где идёт игра вместо ботов (теряет приоритет, если 
     * активна кнопка доп. фильтра "столы с людьми без админ-контроля") */
    (_with_admins, (bool))
    // столы с людьми без админ-контроля
    (_with_people_without_admins, (bool))
    // столы с людьми
    (_with_people, (bool))
    // логин админа, пустой если нет управления админом
    (_nickname, (std::string))
    // тип игры
    (_game_type, (e_game_type))
    // лимит игры
    (_game_limit, (e_game_limit))
    // только на реальные деньги
    (_no_freerolls, (bool))
    ,
    GBP_DECLARE_ENUM(
    e_game_type
    , gbp_u8
    , friend
    , (all)
    (texas_holdem)
    (omaha)
    (omaha_hi_low)
    (stud)
    (stud_hi_low)
    )
    GBP_DECLARE_ENUM(
    e_game_limit
    , gbp_u8
    , friend
    , (all)
    (fixed_limit)
    (pot_limit)
    (no_limit)
    )
   )

   /************************** TOURNAMENTS ****************************/

   GBP_DECLARE_ENUM(
    e_registration_schedule_type
    , gbp_u8
    , inline
    , (less_hour)
    (less_day)
    (less_week)
    (less_month)
    (less_year)
   )

   // турнир
   GBP_DECLARE_TYPE(
    tournament
    ,
    // id
    (_id, (gbp_i64))
    // дата начала регистрации
    (_registration_start_time, (time_s))
    // партнёрский турнир
    (_is_partner_tourn, (bool), (false))
    // дата старта
    (_start_time, (time_s))
    // время до конца периода поздней регистрации (-1 если его нет)
    (_end_late_reg_time, (time_s))
    // кол-во ребаев на одного игрока
    (_rebuys_count, (gbp_i32))
    // тип турнира
    (_tournament_type, (ns_tournament_types::e_tournament_type))
    // техас, омаха, стад...
    (_game_type, (ns_game::e_game_type))
    // безлимит, лимитный,...
    (_game_limit, (ns_game::e_game_limit))
    // статус турнира
    (_status, (ns_tournament_types::e_tournament_status))
    // стек
    (_stack, (money))
    // бай-ин
    (_buy_in, (money))
    // ББ
    (_big_blind, (money))
    // доступный максимум игроков в турнире
    (_max_users_count, (gbp_i32))
    // кол-во игроков для sit_n_go
    (_sit_n_go_users_count, (gbp_i32))
    // кол-во игроков в турнире
    (_players_count, (gbp_u32))
    // иды ботов
    (_bot_ids, (std::unordered_set<gbp_i64>))
    // кол-во людей
    (_people_count, (gbp_u32))
    // кол-во наблюдающих за всеми столами
    (_viewers_count, (gbp_i32))
    // кол-во мест за столом
    (_seats_count, (gbp_i8))
    /**
     * турнир считается под контролем, только если 
     * все его столы под контролем */
    (_is_under_admin_control, (bool))
    // состояние юзеров по столам
    (_users_cashes_stat, (std::vector<users_stat>))
    // гарантийная сумма денег
    (_warranty_money, (money))
    // тип турнирной регистрации
    (_schedule_type, (e_registration_schedule_type))
   )

   // фильтр турниров
   GBP_DECLARE_TYPE(
    tournaments_filter
    ,
    // по id
    (_id, (gbp_i64))
    // по времени старта турнира, от/до
    (_start_datetime, (std::pair<time_s, time_s>))
    // по типу игры (техас, омаха, стад...)
    (_game_type, (e_filter_game_type))
    // по лимиту (безлимит, лимитный,...)
    (_game_limit, (e_filter_game_limit))
    // тип по докупке
    (_rebuy, (gbp_i32))
    // по статусу
    (_status, (e_filter_tournament_status))
    // по бай-ину
    (_buy_in, (money))
    // макс. кол-во юзеров, до
    (_max_users_count, (gbp_i32))
    // кол-во игроков, от/до
    (_players_count, (std::pair<gbp_i32, gbp_i32>))
    // кол-во ботов, от/до
    (_bots_count, (std::pair<gbp_i32, gbp_i32>))
    // кол-во людей, от/до
    (_people_count, (std::pair<gbp_i32, gbp_i32>))
    // кол-во наблюдающих, от/до
    (_viewers_count, (std::pair<gbp_i32, gbp_i32>))
    // с людьми
    (_with_people, (bool))
    // с людьми без админ контроля
    (_with_people_without_admins, (bool))
    // гарантия, от/до
    (_warranty_money, (std::pair<money, money>))
    ,
    GBP_DECLARE_ENUM(
     e_filter_game_type
     , gbp_u8
     , friend
     , (all)
     (texas_holdem)
     (omaha)
     (omaha_hi_low)
    )
    GBP_DECLARE_ENUM(
     e_filter_game_limit
     , gbp_u8
     , friend
     , (all)
     (fixed_limit)
     (pot_limit)
     (no_limit)
    )
    GBP_DECLARE_ENUM(
     e_filter_tournament_status
     , gbp_u8
     , friend
     , (all)
     (not_activated)
     (wait)
     (registering)
     (started)
     (finished)
    )
   )

   GBP_DECLARE_TYPE(
    tournament_rules_character_info
    ,
    // ID характера
    (_id, (gbp_i64))
    // название характера
    (_title, (std::string))
   )

   // условия заморозки ботов для типа турнира
   GBP_DECLARE_TYPE(
    tournament_freezing
    ,
    // максимум замороженных, %
    (_freezing_max_from, (gbp_i8))
    (_freezing_max_to, (gbp_i8))
    // минимум замороженных, %
    (_freezing_min_from, (gbp_i8))
    (_freezing_min_to, (gbp_i8))
    // кол-во раздач (от/до), после которых бот уходит в заморозку
    (_distributions_from, (gbp_i32))
    (_distributions_to, (gbp_i32))
    // кол-во раздач (от/до), после которых бот выходит из ожидания
    (_distributions_out_from, (gbp_i32))
    (_distributions_out_to, (gbp_i32))
   )

   // расписание регистраций для типа турнира
   GBP_DECLARE_TYPE(
    tournament_registering_schedule
    ,
    (_less_hour, (std::vector<less_hour>))
    (_less_day, (std::vector<less_day>))
    (_less_week, (std::vector<less_week>))
    (_less_month, (std::vector<less_month>))
    (_less_year, (std::vector<less_year>))
    ,
    // если от начала регистрации до старта турнира - менее часа (включительно)
    GBP_DECLARE_TYPE(
     less_hour
     ,
     // должно быть зарегано в течение 30 мин после начала регистрации, %
     (_min_30, (gbp_i8))
     (_min_20, (gbp_i8))
     (_min_10, (gbp_i8))
    )
    // если от начала регистрации до старта турнира - менее суток
    GBP_DECLARE_TYPE(
     less_day
     ,
     (_hours_15, (gbp_i8))
     (_hours_6, (gbp_i8))
     (_hours_2, (gbp_i8))
     (_min_30, (gbp_i8))
     (_min_20, (gbp_i8))
     (_min_10, (gbp_i8))
    )
    // если от начала регистрации до старта турнира - менее недели
    GBP_DECLARE_TYPE(
     less_week
     ,
     (_days_6, (gbp_i8))
     (_hours_15, (gbp_i8))
     (_hours_6, (gbp_i8))
     (_hours_2, (gbp_i8))
     (_min_30, (gbp_i8))
     (_min_20, (gbp_i8))
     (_min_10, (gbp_i8))
    )
    // если от начала регистрации до старта турнира - менее месяца
    GBP_DECLARE_TYPE(
     less_month
     ,
     (_weeks_3, (gbp_i8))
     (_days_6, (gbp_i8))
     (_hours_15, (gbp_i8))
     (_hours_6, (gbp_i8))
     (_hours_2, (gbp_i8))
     (_min_30, (gbp_i8))
     (_min_20, (gbp_i8))
     (_min_10, (gbp_i8))
    )
    // если от начала регистрации до старта турнира - менее года
    GBP_DECLARE_TYPE(
     less_year
     ,
     (_months_11, (gbp_i8))
     (_weeks_3, (gbp_i8))
     (_days_6, (gbp_i8))
     (_hours_15, (gbp_i8))
     (_hours_6, (gbp_i8))
     (_hours_2, (gbp_i8))
     (_min_30, (gbp_i8))
     (_min_20, (gbp_i8))
     (_min_10, (gbp_i8))
    )
   )

   // коэффициенты регистраций
   GBP_DECLARE_TYPE(
    tournament_coefficients
    ,
    (_coefficients, (std::vector<coefficient>))
    // отдельно коэффициент для фриролла, от/до
    (_freeroll_coefficient_from, (double))
    (_freeroll_coefficient_to, (double))
    ,
    GBP_DECLARE_TYPE(
     coefficient
     ,
     (_buy_in_from, (ns_api::money))
     (_buy_in_to, (ns_api::money))
     (_value, (double))
    )
   )

   // по типам игры
   GBP_DECLARE_TYPE(
    buy_in_coefficients
    ,
    (_holdem_fixed, (tournament_coefficients))
    (_holdem_pot, (tournament_coefficients))
    (_holdem_no, (tournament_coefficients))
    (_omaha_fixed, (tournament_coefficients))
    (_omaha_pot, (tournament_coefficients))
    (_omaha_no, (tournament_coefficients))
   )

   // общие правила для турниров
   GBP_DECLARE_TYPE(
    tournament_rules
    ,
    // тип по лимитам бай-ина
    (_buy_in_limit, (e_buy_in_limit))
    // тип турнира 1 (0-100 игроков)
    (_rules_type1, (rules_type))
    // тип турнира 2 (101-500 игроков)
    (_rules_type2, (rules_type))
    // тип турнира 3 (501-1000 игроков)
    (_rules_type3, (rules_type))
    // тип турнира 4 (1001-5000 игроков)
    (_rules_type4, (rules_type))
    (_type1_freezing, (tournament_freezing))
    (_type2_freezing, (tournament_freezing))
    (_type3_freezing, (tournament_freezing))
    (_type4_freezing, (tournament_freezing))
    (_type1_reg_schedule, (tournament_registering_schedule))
    (_type2_reg_schedule, (tournament_registering_schedule))
    (_type3_reg_schedule, (tournament_registering_schedule))
    (_type4_reg_schedule, (tournament_registering_schedule))
    (_type1_coefficients, (buy_in_coefficients))
    (_type2_coefficients, (buy_in_coefficients))
    (_type3_coefficients, (buy_in_coefficients))
    (_type4_coefficients, (buy_in_coefficients))
    ,
    GBP_DECLARE_TYPE(
     rules_type
     ,
     // характеры типа
     (_characters, (std::vector<tournament_rules_character_info>))
     // кол-во ботов, от/до (%)
     (_bots_count_from, (gbp_i8))
     (_bots_count_to, (gbp_i8))
     // процент ботов, которые не будут участвовать в турнире
     (_bots_not_participate_from, (gbp_i8))
     (_bots_not_participate_to, (gbp_i8))
     // кол-во ботов, которые вошли уже после начала турнира, от/до (%)
     (_bots_late_from, (gbp_i8))
     (_bots_late_to, (gbp_i8))
     // процент тех ботов, которые выполнят отмену регистрации, от/до
     (_cancel_registration_from, (gbp_i8))
     (_cancel_registration_to, (gbp_i8))
     // поздняя регистрация (%)
     (_late_registration_from, (gbp_i8))
     (_late_registration_to, (gbp_i8))
    )
     /**
      * бай-ины:
      * микро до 9
      * низкие  10 - 49
      * средние 50 - 199
      * высокие от 200 */
     GBP_DECLARE_ENUM(
     e_buy_in_limit
     , gbp_u8
     , friend
     , (micro)
     (low)
     (middle)
     (high)
    )
   )

   // коэффициенты sit_n_go для времён суток
   GBP_DECLARE_TYPE(
    sit_n_go_day_coefficients
    ,
    // утро - часы (от 6 до 12 утра) и коэффициент
    (_6_12, (double))
    // день
    (_12_18, (double))
    // вечер
    (_18_23, (double))
    // ночь
    (_23_6, (double))
   )

   // коэффициенты для sit_n_go по buy-in'ам
   GBP_DECLARE_TYPE(
    sit_n_go_buy_in_coefficients
    ,
    (_coefficients, (std::vector<coefficient>))
    // отдельно коэффициент для фриролла, от/до
    (_freeroll_coefficient_from, (double))
    (_freeroll_coefficient_to, (double))
    ,
    GBP_DECLARE_TYPE(
     coefficient
     ,
     (_buy_in_from, (ns_api::money))
     (_buy_in_to, (ns_api::money))
     (_value, (double))
    )
   )

   // по типам игры
   GBP_DECLARE_TYPE(
    sit_n_go_coefficients
    ,
    (_holdem_fixed, (sit_n_go_buy_in_coefficients))
    (_holdem_pot, (sit_n_go_buy_in_coefficients))
    (_holdem_no, (sit_n_go_buy_in_coefficients))
    (_omaha_fixed, (sit_n_go_buy_in_coefficients))
    (_omaha_pot, (sit_n_go_buy_in_coefficients))
    (_omaha_no, (sit_n_go_buy_in_coefficients))
   )

   // времена регистраций для sit_n_go
   GBP_DECLARE_TYPE(
    sit_n_go_registration_times
    ,
    /**
     * в первые 10% времени посадить указанное кол-во ботов (first);
     * second - кол-во ботов, которые должны "отменить регистрацию" */
    (_0_10, (std::pair<gbp_i32, gbp_i32>))
    (_11_20, (std::pair<gbp_i32, gbp_i32>))
    (_21_30, (std::pair<gbp_i32, gbp_i32>))
    (_31_40, (std::pair<gbp_i32, gbp_i32>))
    (_41_50, (std::pair<gbp_i32, gbp_i32>))
    (_51_60, (std::pair<gbp_i32, gbp_i32>))
    (_61_70, (std::pair<gbp_i32, gbp_i32>))
    (_71_80, (std::pair<gbp_i32, gbp_i32>))
    (_81_90, (std::pair<gbp_i32, gbp_i32>))
    (_91_94, (std::pair<gbp_i32, gbp_i32>))
    (_95_97, (std::pair<gbp_i32, gbp_i32>))
    (_98_99, (std::pair<gbp_i32, gbp_i32>))
    (_100, (std::pair<gbp_i32, gbp_i32>))
   )

   GBP_DECLARE_TYPE(
    sit_n_go_rules
    ,
    // тип по лимитам бай-ина
    (_buy_in_limit, (e_sit_n_go_buy_in_limit))

    (_rules_type1, (rules_type))
    (_rules_type2, (rules_type))
    (_rules_type3, (rules_type))
    (_rules_type4, (rules_type))
    (_rules_type5, (rules_type))
    (_rules_type6, (rules_type))
    (_rules_type7, (rules_type))
    ,
    GBP_DECLARE_TYPE(
     rules_type
     ,
     /**
      * интервал времени "старта" турнира, от/до.
      * То есть до окончания этого времени должны быть посажены все боты */
     (_start_time_from, (ns_api::time_s))
     (_start_time_to, (ns_api::time_s))
     // кол-во ботов, которые вошли уже после начала турнира, от/до (%)
     (_bots_late_from, (gbp_i8))
     (_bots_late_to, (gbp_i8))
     // процент ботов, которые не будут участвовать в турнире
     (_bots_not_participate_from, (gbp_i8))
     (_bots_not_participate_to, (gbp_i8))
     // времена регистраций ботов
     (_registration_times, (std::vector<sit_n_go_registration_times>))
     // коэффициенты по времени суток
     (_day_coefficients, (sit_n_go_day_coefficients))
     // коэффициенты по бай-инам
     (_buy_in_coefficients, (sit_n_go_coefficients))
     // характеры типа
     (_characters, (std::vector<tournament_rules_character_info>))
     // заморозка
     (_freezing, (tournament_freezing))
    )
    /**
     * бай-ины:
     * микро до 9
     * низкие  10 - 49
     * средние 50 - 199
     * высокие от 200 */
    GBP_DECLARE_ENUM(
     e_sit_n_go_buy_in_limit
     , gbp_u8
     , friend
     , (micro)
     (low)
     (middle)
     (high)
     )
    )

   GBP_DECLARE_TYPE(
    additional_tournament_stat
    ,
    // id
    (_id, (gbp_i64))
    // столы турнира
    (_cashes, (std::vector<ns_api::ns_bots::cash>))
    // комбинация типов турнира
    (_tournament_type, (ns_tournament_types::e_tournament_type))
    // текущий номер раунда турнира
    (_current_level, (gbp_i16))
    // время до следующего раунда
    (_next_round_time, (ns_api::time_s))
    // время до следующего перерыва
    (_next_break_time, (ns_api::time_s))
    // список раундов турнира
    (_rounds, (std::vector<round_conditions_t>))
    // кол-во сделаных ребаев (всего по турниру)
    (_rebuy_count, (gbp_u32))
    // кол-во сделаных аддонов (всего по турниру)
    (_addon_count, (gbp_u32))
   )

   // фильтр турнирных игр
   GBP_DECLARE_TYPE(
    tourn_stat_cashes_filter
    ,
    // ID турнирной игры
    (_id, (gbp_i64))
    // кол-во игроков, от/до
    (_players_count, (std::pair<gbp_i32, gbp_i32>))
    // кол-во ботов, от/до
    (_bots_count, (std::pair<gbp_i32, gbp_i32>))
    // ко-во людей, от/до
    (_people_count, (std::pair<gbp_i32, gbp_i32>))
   )

   // игрок со своими картами
   GBP_DECLARE_TYPE(
    player
    ,
    (_id, (gbp_i64))
    (_nickname, (std::string))
    // стек игрока на момент текущей раздачи
    (_stack, (ns_api::money))
    (_player_cards, (ns_table::cards))
    (_is_bot, (bool))
   )

   // для истории шаблонных запл. раздач
   GBP_DECLARE_TYPE(
    planned_distr_templ_history_item
    ,
    (_distr_number, (gbp_i32))
    (_create_datetime, (ns_api::time_s))
    (_user_id, (gbp_i64))
    (_user_login, (std::string))
    (_bot_id, (gbp_i64))
    (_bot_login, (std::string))
    (_type, (ns_api::ns_bots::ns_distr_templates::e_type))
    (_table_cards, (ns_api::ns_table::cards))
    (_user_cards, (ns_api::ns_table::cards))
    (_bot_cards, (ns_api::ns_table::cards))
    (_result_money, (ns_api::money))
   )

   // запланированная колода карт
   GBP_DECLARE_TYPE(
    planned_distribution
    ,
    // номер раздачи
    (_distribution_number, (gbp_i64))
    // карты, оставшиеся в колоде (от розданных на стол и игрокам)
    (_deck_cards, (ns_table::cards))
    // карты стола
    (_table_cards, (ns_table::cards))
    // игроки со своими картами
    (_players, (std::vector<ns_api::ns_bots::player>))
    // является шаблонной (бот выбран системой автоматически)
    (_is_template, (bool), (false))
    (_templ_type, (ns_api::ns_bots::ns_distr_templates::e_type))
    (_templ_user_id, (gbp_i64))
    (_templ_bot_id, (gbp_i64))
   )

   GBP_DECLARE_TYPE(
    planned_distributions_filter
    ,
    (_distribution_number, (gbp_i64))
   )

   GBP_DECLARE_TYPE(
    table_deck
    ,
    // карты стола
    (_table_cards, (ns_table::cards))
    // игроки (их логины) с картами
    (_players, (std::vector<std::pair<std::string, ns_table::cards >>))
   )

   // данные игрока для отсыла на бот-сервер
   GBP_DECLARE_TYPE(
    table_player_info_t
    ,
    (_id, (gbp_i64))
    (_is_user, (bool)) // true, если не бот
    (_pos, (gbp_u8))
    (_nickname, (std::string))
    (_nickname_suffix, (std::string))
    (_money, (money)) // все деньги игрока
    (_start_stack, (money)) // деньги на начало раздачи
    (_hash_avatar, (std::string))
    (_is_dealer, (bool))
    (_cards, (ns_api::ns_table::cards)) // карты игрока
    // сумма, которую поставил игрок в текущем круге
    (_round_bet, (ns_api::money))
    // является ли текущим игроком
    (_is_current, (bool))
    // выйдет по кол-ву раздач (со след. раздачи)
    (_close_by_distribution_count, (bool))
    (_state, (ns_api::ns_table::user_state_t))
    // страна игрока
    (_country_title, (std::string))
    // бот жив (подключён)
    (_bot_is_alive, (bool))
   )

   // для получения инфы по отдельному кешу
   GBP_DECLARE_TYPE(
    single_cash_info
    ,
    // ID турнира
    (_tournament_id, (gbp_i64))
    // ID кеша
    (_game_id, (gbp_i64))
    // все игроки в данный момент
    (_players, (std::vector<ns_api::ns_bots::table_player_info_t>))
    (_current_distribution, (gbp_i64)) // текущий номер раздачи
   )

   // состояние стола для игры вместо бота
   GBP_DECLARE_TYPE(
    table_state_instead_bots_t
    ,
    // ID турнира
    (_tournament_id, (gbp_i64))
    // большой блайнд
    (_big_blind, (money))
    // ID кеша
    (_game_id, (gbp_i64))
    // все игроки в данный момент
    (_players, (std::vector<ns_api::ns_bots::table_player_info_t>))
    // текущий игрок, который делает ход
    (_current_user_pos, (gbp_u8))
    (_table_cards, (ns_api::ns_table::cards)) // карты стола
    (_table_cards_count, (gbp_u8)) // кол-во уже розданных на стол карт
    (_all_pots, (std::vector<money>)) // все банки стола ([0] - главный банк)
    (_time_left, (ns_api::time_s)) // осталось времени на ход игрока
    (_distribution_number, (gbp_i64)) // текущий номер раздачи
   )

   GBP_DECLARE_ENUM(
    e_play_instead_bots_status
    , gbp_u8
    , inline
    , (with_manual_control)// управление игрой ботов
    (without_manual_control)
    (close)
   )

   GBP_DECLARE_TYPE(
    quialifier_finish_result
    ,
    // ID текущего квалификатора
    (_qualifier_id, (gbp_i64))
    // ID главного события
    (_main_tournament_id, (gbp_i64))
    // боты, получившие билеты в гл. событие
    (_bot_ids, (std::vector<gbp_i64>))
   )

   // история игр вместо ботов
   GBP_DECLARE_TYPE(
    history
    ,
    // ID записи
    (_id, (gbp_i64))
    // ID стола
    (_game_id, (gbp_i64))
    // ID турнира
    (_tournament_id, (gbp_i64))
    // логин
    (_nickname, (std::string))
    // номер раздачи "от"
    (_distribution_number_from, (gbp_i64))
    // номер раздачи "до"
    (_distribution_number_to, (gbp_i64))
    // время начала игры вместо бота
    (_start_time, (ns_api::time_s))
    // время окончания игры вместо бота
    (_stop_time, (ns_api::time_s))
   )

   GBP_DECLARE_TYPE(
    history_filter
    ,
    // для всех численных полей: если стоит -1, то поле не учитывать
    // ID игры
    (_game_id, (gbp_i64))
    // логин
    (_nickname, (std::string))
    // время игры "от"
    (_time_from, (ns_api::time_s))
    // время игры "до"
    (_time_to, (ns_api::time_s))
    // номер раздачи "от"
    (_distribution_number_from, (gbp_i64))
    // номер раздачи "до"
    (_distribution_number_to, (gbp_i64))
   )

   GBP_DECLARE_TYPE(
    all_cards
    ,
    (_table_cards, (ns_table::cards))
    (_players_cards, (std::unordered_map<gbp_i64, player_cards>))
    ,
    GBP_DECLARE_TYPE(
     player_cards
     ,
     (_user_id, (gbp_i64))
     (_frozen, (bool))
     (_anticipant, (bool))
     (_is_bot, (bool))
     (_cards, (ns_table::cards))
    )
   )

   GBP_DECLARE_TYPE(
    end_of_distribution_user_info
    ,
    (_user_id, (gbp_i64))
    (_is_bot, (bool))
    // внутриигровой стек игрока
    (_cash, (ns_api::time_s))
   )

   GBP_DECLARE_TYPE(
    chat_message_info
    ,
    // ID того, кто написал сообщение
    (_user_id, (gbp_i64))
    // логин того, кто написал сообщение
    (_user_nickname, (std::string))
    (_is_bot, (bool))
    // само сообщение
    (_message, (std::string))
   )

   /**
    * только для чатов.
    * Общий тип для кеша и турнирного кеша */
   GBP_DECLARE_TYPE(
    chat_cash
    ,
    // ID турнира
    (_tournament_id, (gbp_i64))
    // ID стола
    (_id, (gbp_i64))
    // тип (texas_holdem, etc.)
    (_type, (ns_api::ns_game::e_game_type))
    // турнирный тип (Rebuy, etc.)
    (_tournament_type, (ns_tournament_types::e_tournament_type))
    // лимит
    (_limit, (ns_api::ns_game::e_game_limit))
    // ББ
    (_big_blind, (ns_api::money))
    // кол-во мест
    (_places_count, (gbp_i8))
    // кол-во игроков
    (_players_count, (gbp_i8))
    // кол-во ботов
    (_bots_count, (gbp_i8))
    // кол-во людей
    (_people_count, (gbp_i8))
    // кол-во наблюдающих
    (_viewers_count, (gbp_i32))
    // заблокирован (id / nickname)
    (_locked_by, (std::pair<gbp_i64, std::string>))
    // содержит новые сообщения
    (_contains_new_messages, (bool))
   )

   // для передачи бот-серверу при взятии списка чат-кешей
   GBP_DECLARE_ENUM(
    e_chat_cashes_filter
    , gbp_u8
    , inline
    // все столы
    , (all_cashes)
     // наблюдающие
     (viewers)
     // с новыми сообщениями
     (new_messages)
     // наблюдающие и играющие
     (viewers_and_players)
     // играющие
     (players)
   )

   /************************ ТАБЛИЦЫ НАБЛЮДЕНИЙ ***************************/

   // игрок-человек, для всех таблиц
   GBP_DECLARE_TYPE(
    chase_user_player
    ,
    (_user_id, (gbp_i64))
    (_nickname, (std::string))
    (_user_notes, (std::string))
    // номер задачи (турниры)
    (_task_number, (gbp_i32))
    // текст задачи (турниры)
    (_task_text, (std::string))
    // страна
    (_country, (gbp_i32))
    // на счету
    (_account_money, (ns_api::money))
    // в игре
    (_in_game_money, (ns_api::money))
    // итог общий
    (_total_all, (ns_api::money))
    // итог месячный
    (_total_month, (ns_api::money))
   )

   /**
    * тип для таблицы наблюдений по кешам. Только 
    * те столы, в которых есть реальные люди */
   GBP_DECLARE_TYPE(
    chase_cash
    ,
    // ID стола
    (_game_id, (gbp_i64))
    // тип игры
    (_type, (ns_api::ns_game::e_game_type))
    // лимит
    (_limit, (ns_api::ns_game::e_game_limit))
    // ББ
    (_big_blind, (ns_api::money))
    // номер раздачи (для игры вместо ботов)
    (_distribution_number, (gbp_i64))
    // кол-во мест
    (_places_count, (gbp_i8))
    // кол-во игроков
    (_players_count, (gbp_i8))
    // кол-во ботов
    (_bots_count, (gbp_i8))
    // кол-во людей
    (_people_count, (gbp_i8))
    // игроки-люди
    (_users, (std::vector<ns_api::ns_bots::chase_user_player>))
    // кеш заблокироан
    (_locked, (bool))
    // ID заблокировавшего кеш, -1 если кеш не заблокирован
    (_owner_id, (gbp_i64))
    // никнейм заблокировавшего кеш. Пустой, если кеш не заблокирован
    (_owner_nickname, (std::string))
   )

   /**
    * тип для таблицы наблюдений по турнирам. Только 
    * те столы, в которых есть реальные люди */
   GBP_DECLARE_TYPE(
    chase_tournament_cash
    ,
    // ID турнира
    (_tournament_id, (gbp_i64))
    // sin_n_go
    (_is_sit_n_go, (bool))
    // rebuy
    (_is_rebuy, (bool))
    // bounty
    (_is_bounty, (bool))
    // сумма баунти
    (_bounty_money, (ns_api::money))
    // ID стола
    (_game_id, (gbp_i64))
    // тип игры
    (_type, (ns_api::ns_game::e_game_type))
    // лимит
    (_limit, (ns_api::ns_game::e_game_limit))
    // ББ
    (_big_blind, (ns_api::money))
    // Бай-ин
    (_buy_in, (ns_api::money))
    // гарантия
    (_warranty, (ns_api::money))
    // средний стек турнира
    (_avarage_stack, (ns_api::money))
    // кол-во призов
    (_prizes_count, (gbp_i32))
    // кол-во активных игроков
    (_active_players_count, (gbp_i32))
    // номер раздачи (для игры вместо ботов)
    (_distribution_number, (gbp_i64))
    // кол-во мест
    (_places_count, (gbp_i8))
    // кол-во игроков
    (_players_count, (gbp_i8))
    // кол-во ботов
    (_bots_count, (gbp_i8))
    // кол-во людей
    (_people_count, (gbp_i8))
    // игроки-люди за столом
    (_users, (std::vector<ns_api::ns_bots::chase_user_player>))
    // кеш заблокироан
    (_locked, (bool))
    // ID заблокировавшего кеш, -1 если кеш не заблокирован
    (_owner_id, (gbp_i64))
    // никнейм заблокировавшего кеш. Пустой, если кеш не заблокирован
    (_owner_nickname, (std::string))
   )

   // для статистики по характерам AI
   GBP_DECLARE_TYPE(
    ai_characters_stat
    ,
    (_items, (std::vector<ns_api::ns_bots::ai_characters_stat::ai_stat_info>))
    (_available_nicknames_count, (gbp_i32))
    ,
    GBP_DECLARE_TYPE(
    ai_stat_info
    ,
    // если true, то не смотреть _ai_type
    (_is_insurance, (bool))
    (_ai_type, (e_bot_ai_character))
    (_created_count, (gbp_i32))
    (_online_count, (gbp_i32))
    (_in_game_count, (gbp_i32))
    )
   )

  } // ns_bots

 } // ns_api
} // ns gbp