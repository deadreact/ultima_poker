#pragma once

#include <string>
#include <vector>

#include "../api_common.h"
#include "../api_special_offers.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_admin
  {
   GBP_DECLARE_ENUM(
    e_admin_authorization
    , gbp_u8
    , inline
    , (ok)
      (not_registered)
   )

   GBP_DECLARE_ENUM(
    e_admin_type
    , gbp_u8
    , inline
    , (superadmin)
      (support)
      (marketing)
   )

   // для авторизации админа
   GBP_DECLARE_TYPE(
    admin_auth_info_t
    ,
    (_id, (gbp_i64))
    (_auth, (gbp::ns_api::ns_admin::e_admin_authorization))
    (_type, (gbp::ns_api::ns_admin::e_admin_type))
   )

   // для списка админов
   GBP_DECLARE_TYPE(
    admin_item_t
    ,
    (_id, (gbp_i64))
    (_nickname, (std::string))
    (_type, (gbp::ns_api::ns_admin::e_admin_type))
    (_email, (std::string))
    (_register_datetime, (ns_api::time_s))
   )

   // для создания нового админа
   GBP_DECLARE_TYPE(
    new_admin_t
    ,
    (_id, (gbp_i64))
    (_nickname, (std::string))
    (_password, (std::string))
    (_email, (std::string))
    (_type, (gbp::ns_api::ns_admin::e_admin_type))
   )

   // для создания нового пула
   GBP_DECLARE_TYPE(
    new_spin_sng_pool_t
    ,
    (_title, (std::string))
    (_tourn_count, (gbp_i64))
    (_buy_in, (gbp_i64))
    (_list, (std::vector<spin_sng_pool_item_t>))
    ,
    GBP_DECLARE_TYPE(
     spin_sng_pool_item_t
     ,
     // главный приз (деньги или кеш-код)
     (_is_main, (bool))
     (_is_ticket, (bool))
     (_ticket_id, (gbp_i64))
     (_money, (ns_api::money))
     (_tourn_count, (gbp_i32))
    )
   )

   // инфа по отдельному пулу
   GBP_DECLARE_TYPE(
    spin_sng_pool_info_t
    ,
    (_id, (gbp_i64))
    (_title, (std::string))
    (_tourn_count, (gbp_i64))
    (_buy_in, (gbp_i64))
    (_list, (std::vector<spin_sng_pool_item_t>))
    ,
    GBP_DECLARE_TYPE(
     spin_sng_pool_item_t
     ,
     (_is_main, (bool))
     (_is_ticket, (bool))
     (_ticket_title, (std::string))
     (_money, (ns_api::money))
     (_tourn_count, (gbp_i32))
    )
   )

   // инфа для списка пулов
   GBP_DECLARE_TYPE(
    spin_sng_pool_t
    ,
    (_id, (gbp_i64))
    (_title, (std::string))
    (_tourn_count, (gbp_i64))
    // используется сейчас каким-нибудь spin-sng-шаблоном или нет
    (_in_using, (bool))
    (_buy_in, (gbp_i64))
   )

   // статистика по юзеру
   GBP_DECLARE_TYPE(
    user_statistic_t
    ,
    (_nickname, (std::string))
    (_email, (std::string))
    (_register_datetime, (ns_api::time_s))
    // сумма вводов денег
    (_deposits_money, (ns_api::money))
    // сумма выводов
    (_payments_out_money, (ns_api::money))
    // доход с юзера по кешам
    (_proceeds_from_cash, (ns_api::money))
    // доход с юзера по турнирам
    (_proceeds_from_tourn, (ns_api::money))
   )

   // для верификации
   GBP_DECLARE_ENUM(
    e_verification_action
    , gbp_u8
    , inline
    , (verification_docs_request)
    (verification_confirmation)
   )

   // операция ввода/вывода денег юзером
   GBP_DECLARE_TYPE(
    user_payment_t,
    // порядковый номер
    (_number, (gbp_i64))
    (_datetime, (ns_api::time_s))
    (_money, (ns_api::money))
    (_payment_system, (ns_api::e_payment_system))
    (_action, (e_user_payment_type))
    ,
    // тип платежа
    GBP_DECLARE_ENUM(
     e_user_payment_type
     , gbp_u8
     , friend
     , (payment_in)
     (payment_out)
    )
   )

   // статистика для админа
   GBP_DECLARE_TYPE(
    get_tickets_filter_t,
    (_title, (std::string))
   )

   // статистика для админа
   GBP_DECLARE_TYPE(
    admin_statistic_t
    ,
    (_daily_start_time, (ns_api::time_s))
    (_registrations_count, (gbp_i32))
    (_deposits_count, (gbp_i32))
    (_deposits_all_money, (ns_api::money))
    (_avarage_deposit, (ns_api::money))
    (_payments_out_count, (gbp_i32))
    (_payments_out_all_money, (ns_api::money))
    (_avarage_payment_out, (ns_api::money))
    (_rake_cash, (ns_api::money))
    (_rake_tournament, (ns_api::money))
    (_rake_sit_n_go, (ns_api::money))
    // сумма рейков рума
    (_rake_room_sum, (ns_api::money))
    // сумма рейков игроков
    (_rake_players_sum, (ns_api::money))
    // среднее кол-во игроков онлайн
    (_players_online_count, (gbp_i32))
    // среднее кол-во игроков за столами (фиксируются каждый час)
    (_players_in_games_count, (gbp_i32))
   )

   // фильтр статистики
   GBP_DECLARE_TYPE(
    admin_statistic_filter_t
    ,
    (_datetime_from, (ns_api::time_s))
    (_datetime_to, (ns_api::time_s))
   )

   // фильтр для взятия транзакций переводов денег от одного юзера к другому
   GBP_DECLARE_TYPE(
    transfers_filter_t
    ,
    (_limit_from, (gbp_u32))
    (_limit_to, (gbp_u32))
    // ник того, кто переводил
    (_nickname_from, (std::string))
    // ник того, кому перевели
    (_nickname_to, (std::string))
    // время от
    (_datetime_from, (ns_api::time_s))
    // время до
    (_datetime_to, (ns_api::time_s))
   )

   GBP_DECLARE_TYPE(
    money_transfer_item_t
    ,
    // время перевода
    (_datetime, (ns_api::time_s))
    // ник того, кто переводил
    (_nickname_from, (std::string))
    // ник того, кому перевели
    (_nickname_to, (std::string))
    // сумма перевода
    (_amount, (ns_api::money))
   )

   // фильтр поиска юзеров
   GBP_DECLARE_TYPE(
    user_info_filter_t
    ,
    (_nickname, (std::string)) // по нику (если пустая - не используется)
    (_id, (gbp_i64)) // ID юзера
    (_email, (std::string)) // по email
    (_status, (ns_api::e_user_status)) // по текущему статусу (если all - все статусы)
    (_affiliate, (std::string)) // по афилейту
    (_reg_date, (std::pair<time_s, time_s>)) // по времени регистрации от/до
    (_money, (std::pair<money, money>)) // по кол-ву денег на счету, от/до
    (_vip_status, (ns_vip_system::e_status)) // по vip-статусу игрока
    (_user_type, (ns_api::ns_admin::user_info_filter_t::e_user_type))
    ,
    // тип игрока
    GBP_DECLARE_ENUM(
     e_user_type
     , gbp_u8
     , friend
     , (all)
     (is_user)
     (is_bot)
    )
   )

   // для списка юзеров в админке
   GBP_DECLARE_TYPE(
    user_info_t
    ,
    (_id, (gbp_i64)) // id юзера
    (_nickname, (std::string)) // ник юзера (UTF-8)
    (_vip_status, (ns_vip_system::e_status))
    (_freeroll_money, (money)) // фрирольные деньги
    (_affiliate, (std::string))
    (_reg_date, (time_s)) // дата/время регистрации игрока
    (_status, (ns_api::e_user_status)) // текущий статус юзера
    (_money, (money_t)) // деньги юзера
    (_is_user, (bool)) // юзер (true) или нет
    (_is_chat_blocked, (bool)) // забанен ли для сообщений
    (_verifying, (ns_api::e_verifying)) // статус верификации юзера
    (_locale, (std::string)) // локаль юзера
    ,
    GBP_DECLARE_TYPE(
     money_t
     ,
     (_current, (money)) // на балансе
     (_active, (money)) // в игре
     (_all_in, (money)) // всего введено
     (_all_out, (money)) // всего выведено
     (_wait_out, (money)) // на выплату
    )
   )

   // профиль юзера
   GBP_DECLARE_TYPE(
    user_profile_t
    ,
    (_id, (gbp_i64)) // user_id
    (_nickname, (std::string)) // никнейм
    (_money, (ns_api::money)) // деньги
    (_email, (std::string)) // мыло
    (_email_verified, (bool)) // верификация почты
    (_locale, (std::string)) // локаль/страна
    (_vip_status, (ns_vip_system::e_status)) // рейтинг
    (_reg_date, (ns_api::time_s)) // время регистрации
    // статус верификации юзера
    (_verifying, (ns_api::e_verifying))
    // билеты юзера
    (_tickets, (std::unordered_map<gbp_i64, ns_special_offers::user_ticket>))
    // первый депозит
    (_first_deposit, (ns_api::money))
    // прибыль с игрока по кешам
    (_gain_cashes, (ns_api::money))
    // прибыль с игрока по турнирам
    (_gain_tournaments, (ns_api::money))
    // последний вход
    (_last_enter, (ns_api::time_s))
    // статистика по рейкам
    (_stat_rakes, (ns_api::ns_admin::user_profile_t::stat_rakes_t))   
    // доп. инфа
    (_country, (gbp_u16)) // страна юзера
    (_username, (std::string)) // имя
    (_last_name, (std::string)) // фамилия
    (_city, (std::string))
    (_phone, (std::string))
    (_mail_index, (std::string))
    (_birth_date, (ns_api::birth_date))
    (_mail_address, (std::string))
    (_gender, (ns_details::e_gender))
    (_btag, (std::string))
    ,
    GBP_DECLARE_TYPE(
     stat_rakes_t
     ,
     // рейк за всё время
     (_rake_all_time, (ns_api::money))
     // рейк за прошлый месяц
     (_rake_last_month, (ns_api::money))
     // рейк за текущий месяц
     (_rake_current_month, (ns_api::money))
     // рейкбек за всё время
     (_rakeback_all_time, (ns_api::money))
     // рейкбек за прошлый месяц
     (_rakeback_last_month, (ns_api::money))
     // рейкбек за текущий месяц
     (_rakeback_current_month, (ns_api::money))
     // рейкбек за текущую неделю
     (_rakeback_current_week, (ns_api::money))
    )
   )

   //------------------- hardware dialog -------------------//

   // hardware info
   GBP_DECLARE_TYPE(
    hardware_filter_t
    ,
    (_user_id, (gbp_i64)) // ID юзера
    (_hardware_id, (std::string)) // ID железа
   )

   // hardware info
   GBP_DECLARE_TYPE(
    hardware_t
    ,
    (_id, (gbp_i64)) // ID записи
    (_status, (e_status))
    (_nickname, (std::string))
    (_hardware_id, (std::string))
    (_reg_date, (time_s)) // дата/время регистрации железа
    (_last_enter, (time_s)) // дата/время последнего посещения
    ,
    GBP_DECLARE_ENUM(
    e_status
    , gbp_u8
    , friend
    , (blocked)
    (unblocked)
    )
   )

   //----------------- lobby theme dialog ------------------//

   // тема лобби + стол
   GBP_DECLARE_TYPE(
   theme_t
    ,
    (_id, (gbp_u16)) // id (-1 при отправке темы)
    (_title, (std::string)) // название темы
    (_theme, (std::string)) // тема - отправляется только по запросу get_theme(id) (base64)
    (_table_preview, (std::string)) // картинка-превьюшка стола (base64)
    (_lobby_preview, (std::string)) // картинка-превьюшка лобби (base64)
   )

   //-------------------- banner dialog --------------------//

   // баннеры (TODO: ссылка одна на все баннеры?)
   GBP_DECLARE_TYPE(
   lobby_banner_t
    ,
    (_url, (std::string)) // ссылка, на которую будет переход при клике на баннер
    (_banners_img, (std::vector<std::string>)) // массив бинарей-картинок баннеров
   )

   //--------------------- cash dialog ---------------------//

   GBP_DECLARE_ENUM(
    e_game_activity
    , gbp_u8
    , inline
    , (all)
    (empty)
    (partially)
    (full)
   )

   GBP_DECLARE_TYPE(
    new_cash_t
    ,
    (_title, (std::string)) // название
    (_type, (ns_game::e_game_type)) // техас, омаха, стад...
    (_limit, (ns_game::e_game_limit)) // безлимит, лимитный,...
    // основное время для ожидания хода, сек
    (_time_action_main, (time_s))
    // дополнительное время для ожидания хода, сек
    (_time_action_additional, (time_s))
    (_users_count, (gbp_u8)) // количество мест за столом
    (_rake, (double)) // процент по выигрышу в пользу рума
    (_rake_cap, (ns_api::money)) // макс. сумма для рейка
    (_big_blind, (ns_api::money)) // большой блайнд
    (_register_money, (std::pair<money, money>)) // минимум-максимум для регистрации в кеше
    (_is_freeroll, (bool)) // бесплатный (true) или нет
   )

   // кеш-столы
   GBP_DECLARE_TYPE(
    cash_t
    ,
    (_id, (gbp_i64)) // id стола
    (_title, (std::string)) // название
    (_type, (ns_game::e_game_type)) // техас, омаха, стад...
    (_limit, (ns_game::e_game_limit)) // безлимит, лимитный,...
    (_big_blind, (ns_api::money)) // большой блайнд
    // кол-во игроков активных/всего
    (_users, (std::pair<gbp_u8, gbp_u8>)) // кол-во игроков активных/всего
    (_wait_users, (gbp_u8)) // кол-во ожидающих игроков
    // короткое время для ожидания хода, сек
    (_time_action_short, (time_s))
    // основное время для ожидания хода, сек
    (_time_action_main, (time_s))
    // дополнительное время для ожидания хода, сек
    (_time_action_additional, (time_s))
    (_avarage_pot, (money)) // средний банк
    (_flop, (double)) // игр/Flp% - процентность игр закончившихся на(до) флопа
    (_hands_per_hour, (gbp_u16)) // рук в час
    (_rake, (double)) // процент по выигрышу в пользу рума
    (_rake_cap, (ns_api::money)) // макс. сумма для рейка
    (_money, (money)) // прибыль со стола
   )

   // фильтр кеш-столов
   GBP_DECLARE_TYPE(
    cash_filter_t
    ,
    (_id, (gbp_i64)) // по id (-1 - не используется)
    (_title, (std::string)) // по названию
    (_users_count, (gbp_i8)) // по кол-ву игроков (максимальных) (-1 - не используется)
    (_type, (ns_game::e_game_type)) // по типу (Техас, Омаха, Стад...)
    (_limit, (ns_game::e_game_limit)) // по лимиту (Безлимит, лимитный,...)
    (_big_blind, (money)) // по большому блайнду
    (_avarage_pot_from, (money)) // значение среднего банка, от
    (_avarage_pot_to, (money)) // значение среднего банка, до
    (_activity, (e_game_activity)) // по активности стола (пустой / заполненый ?)
   )

   // для фильтров в диалогах
   namespace ns_filter
   {
    GBP_DECLARE_ENUM(
    e_game_type
     , gbp_u8
     , inline
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
     , inline
     , (all)
       (fixed_limit)
       (pot_limit)
       (no_limit)
    )
    GBP_DECLARE_ENUM(
    e_game_speed
     , gbp_u8
     , inline
     , (all)
       (normal)
       (turbo)
       (super_turbo)
    )
    // статусы турниров
    GBP_DECLARE_ENUM(
     e_tourn_status
     , gbp_u8
     , inline
     , (all) // все статусы 
       (not_activated) // турнир не активирован
       (wait) // турнир в ожидании
       (registering) // начало процесса регистрации
       // в процессе и регистрация также доступна (для sit_n_go)
       (late_registration)
       (started) // в процессе игры
       (finished) // завершён
    )
    // стек турнира
    GBP_DECLARE_ENUM(
     e_tourn_stack
     , gbp_u8
     , inline
     , (all)
       (normal_stack)
       (double_stack)
       (super_stack)
       (manual_stack)
    )
    // типы категории 1
    GBP_DECLARE_ENUM(
     e_tourn_type1
     , gbp_u8
     , inline
     , (all)
       (normal)
       (main_tourn)
       (qualifier)
       (sit_n_go)
       (warranty)
       (no_warranty)
       (freeroll)
    )
    // типы категории 2
    GBP_DECLARE_ENUM(
     e_tourn_type2
     , gbp_u8
     , inline
     , (all)
       (freezout)
       (rebuy)
       (knockout)
       (knockout_progr)
    )
   }   

   // турниры
   GBP_DECLARE_TYPE(
    tournament_t
    ,
    (_id, (gbp_i64)) // id
    (_title, (std::string)) // название турнира
    (_start_datetime, (time_s)) // время старта
    (_game_type, (ns_api::ns_game::e_game_type)) // техас, омаха, стад...
    (_game_limit, (ns_api::ns_game::e_game_limit)) // безлимит, лимитный,...
    (_game_speed, (ns_api::ns_game::e_game_speed)) // скорость турнирного стола
    (_tournament_type, (ns_tournament_types::e_tournament_type)) // тип турнира
    (_status, (ns_tournament_types::e_tournament_status))
    (_stack_type, (ns_tournament_types::e_tournament_stack)) // стек
    (_stack, (money)) // стек
    (_buy_in, (money)) // бай-ин
    (_fee, (double)) // комиссия, %
    (_bounty, (money)) // баунти
    (_is_progressive_bounty, (bool)) // прогр. баунти    
    (_qualifiers, (std::vector<qualifier_t>)) // квалификаторы
    // зарегано
    (_users_reg_count, (gbp_i32))
    // играет
    (_users_play_count, (gbp_i32))
    // минимум-максимум
    (_users_min_max_counts, (std::pair<gbp_i32, gbp_i32>))
    (_gain, (money)) // прибыль с турнира
    (_warranty_money, (money)) // гарантийная сумма денег
    (_table_size, (gbp_u8)) // размер стола, человек
    (_addon, (ns_api::addon_t)) // характеристики аддона (если доступен)
    (_rebuys, (ns_api::rebuys_t))
    // время от начала турнира, в течение которого доступны ребаи (до аддона)
    (_time_for_rebuys, (ns_api::time_s))
    // минимальное кол-во игроков для старта
    (_min_players_count, (gbp_i64))
    // турнир с цепочкой (только для sit_n_go и на регистрации)
    (_chain_is_available, (bool))
    // турнир видимый или нет
    (_is_visible, (bool))
   )

   // фильтр турниров
   GBP_DECLARE_TYPE(
    tournament_filter_t
    ,
    (_id, (gbp_i64)) // по id
    (_start_datetime, (std::pair<time_s, time_s>)) // время старта турнира, от/до
    (_buy_in, (money)) // по бай-ину
    (_game_type, (ns_api::ns_admin::ns_filter::e_game_type)) // по типу (техас, омаха, стад...)
    (_game_limit, (ns_api::ns_admin::ns_filter::e_game_limit)) // по лимиту (Безлимит, лимитный,...)
    (_game_speed, (ns_api::ns_admin::ns_filter::e_game_speed)) // скорость турнирного стола   
    (_tournament_type, (ns_api::ns_tournament_types::e_tournament_type)) // по типу турнира
    (_stack, (ns_api::ns_admin::ns_filter::e_tourn_stack)) // по стеку
    (_status, (ns_api::ns_admin::ns_filter::e_tourn_status))
    (_is_bounty, (bool)) // баунти
    (_is_progressive_bounty, (bool)) // прогр. баунти
    /**
     * если турнир на регистрации, то сколько зарегистрировано игроков.
     * Если турнир в процессе - то сколько сейчас играет */
    (_gamers_count, (std::pair<gbp_i64, gbp_i64>))
    (_gain, (std::pair<money, money>)) // прибыль с турнира
   )

   GBP_DECLARE_TYPE(
     template_t
     ,
     (_id, (gbp_i64))
     (_sort_index, (gbp_i64))
     (_title, (std::string))
     (_data, (std::string))
    )

   namespace ns_payments
   {
    // транзакция по вводу денег
    GBP_DECLARE_TYPE(
     payment_in
     ,
     (_id, (gbp_i64))
     (_user_login, (std::string))
     // верифицирован
     (_is_verifyed, (bool))
     // сумма пополнения
     (_amount, (ns_api::money))
     // доп. сумма (например по акции)
     (_additional_money, (ns_api::money))
     // плат. система
     (_payment_system, (ns_api::e_payment_system))
     // бонус-код
     (_bonuscode, (std::string))
     // номер счёта (если есть)
     (_purse, (std::string))
     // дата пополнения
     (_create_date, (time_s))
     // призы (например, выданные по бонус-коду)
     (_prizes, (std::string))
    )

    // фильтр по заявкам на выплаты (вывод средств)
    GBP_DECLARE_TYPE(
     user_payments_out_filter
     ,
     // ID записи в БД
     (_id, (gbp_i64))
     // время подачи заявки, от/до
     (_datetime, (std::pair<time_s, time_s>))
     // сумма на вывод, от/до
     (_money, (std::pair<money, money>))
     // номер кошелька, номер счёта, etc.
     (_description, (std::string))
     // текущий статус заявки
     (_current_status, (ns_api::e_payment_out_status))
     // плат. система
     (_payment_system, (ns_api::e_payment_system))
     // логин
     (_nickname, (std::string))
     // пагинация, offset/limit
     (_pagination, (std::pair<gbp_u32, gbp_u32>))
    )

    // фильтр по успешным пополнениям игровых стеков (вводам средств)
    GBP_DECLARE_TYPE(
    user_payments_in_filter
     ,
     // ID записи в БД
     (_id, (gbp_i64))
     // логин
     (_nickname, (std::string))
     // сумма транзакции на ввод, от/до
     (_money, (std::pair<money, money>))
     // номер кошелька, номер счёта, etc.
     (_purse, (std::string))
     // время выполнения, от/до
     (_datetime, (std::pair<time_s, time_s>))
     // плат. система
     (_payment_system, (ns_api::e_payment_system))
     // пагинация, offset/limit
     (_pagination, (std::pair<gbp_u32, gbp_u32>))
    )

   } // ns_payments

   // полная статистика за переданный админом период
   namespace ns_autocontrol
   {
    GBP_DECLARE_TYPE(
     fin_plan_stat_t
     ,
     (_plan, (ns_api::money))
     (_plan_fact, (ns_api::money))
     (_fact, (ns_api::money))
     (_diff, (ns_api::money))
     (_residue, (ns_api::money))
    )
   } // ns_autocontrol

   // для списка завершённых турниров
   GBP_DECLARE_TYPE(
    tourn_finished_item
    ,
    (_id, (gbp_i64)) // id
    (_tourn_id, (gbp_i64)) // турнирный id
    (_title, (std::string)) // название турнира
    (_start_datetime, (ns_api::time_s)) // время старта
    (_finish_datetime, (ns_api::time_s))
    (_game_type, (ns_game::e_game_type))
    (_game_limit, (ns_game::e_game_limit))
    (_game_speed, (ns_game::e_game_speed))
    (_types, (ns_tournament_types::e_tournament_type))
    (_stack, (ns_api::money)) // стартовый стек
    (_buy_in, (ns_api::money))
    (_fee, (double)) // комиссия, %
    (_bounty, (ns_api::money)) // баунти
    (_is_progressive_bounty, (bool)) // прогр. баунти
    // было зарегано всего
    (_players_reg_count, (gbp_i32))
    // мин. кол-во игроков для старта
    (_players_min, (gbp_i32))
    // макс. кол-во игроков для старта
    (_players_max, (gbp_i32))
    (_warranty_prize, (ns_api::money)) // гарантийная сумма
    (_gain, (ns_api::money)) // прибыль с турнира
    (_table_size, (gbp_u8)) // размер стола, человек
   )

   // статистика по отдельному завершённому турниру
   GBP_DECLARE_TYPE(
    tourn_finished_info_t
    ,
    (_tourn_id, (gbp_i64)) // id турнира
    (_title, (std::string))
    (_start_datetime, (ns_api::time_s)) // время старта
    (_finish_datetime, (ns_api::time_s))
    // было зарегано всего
    (_players_reg_count, (gbp_i32))
    // было зарегано на поздней регистрации
    (_players_late_reg_count, (gbp_i32))
    (_warranty_prize, (ns_api::money)) // гарантийная сумма денег
    (_buy_in, (ns_api::money)) // бай-ин
    (_bounty, (ns_api::money)) // баунти
    (_fee_money, (ns_api::money)) // комиссия
    (_win_places_count, (gbp_i32))
    (_rebuy_info, (ns_api::rebuys_t)) // характеристики ребая
    (_rebuys_count, (gbp_i32)) // кол-во ребаев
    (_addon_info, (ns_api::addon_t)) // характеристики аддона
    (_addons_count, (gbp_i32)) // кол-во аддонов
    (_prize, (ns_api::money)) // призовой фонд
    (_users, (std::vector<ns_api::tournament_finished_t::user_info>))
   )

   GBP_DECLARE_TYPE(
    tourn_finished_filter_t
    ,
    (_tourn_id, (gbp_i64))
    // время старта турнира, от/до
    (_start_datetime, (std::pair<ns_api::time_s, ns_api::time_s>))
    (_buy_in, (ns_api::money))
    (_game_type, (ns_api::ns_admin::ns_filter::e_game_type))
    (_game_limit, (ns_api::ns_admin::ns_filter::e_game_limit))
    (_game_speed, (ns_api::ns_admin::ns_filter::e_game_speed))
    (_tourn_type1, (ns_api::ns_admin::ns_filter::e_tourn_type1))
    (_tourn_type2, (ns_api::ns_admin::ns_filter::e_tourn_type2))
    // тип стека игрока
    (_stack_type, (ns_api::ns_admin::ns_filter::e_tourn_stack))
    (_is_bounty, (bool)) // баунти
    // прогр. баунти
    (_is_progressive_bounty, (bool))
    // кол-во зареганых юзеров, от
    (_reg_users_count_from, (gbp_i32))
    (_reg_users_count_to, (gbp_i32))
    // прибыль с турнира, от
    (_gain_from, (ns_api::money))
    (_gain_to, (ns_api::money))
   )

  } // namespace ns_admin
 } // namespace ns_api
} // namespace gbp