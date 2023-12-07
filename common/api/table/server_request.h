#pragma once

#include "../lobby_stat/types.h"
#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_table
  {

   /*************************************************************************************************/
   /*************************************** RPC LEVEL 1 *********************************************/
   /*************************************************************************************************/

   /**
    * запросы от контроллера к клиенту (ответы клиента
    * серверу лежат в файле server_reply.h) */

   namespace ns_server_request
   {
    // внутриигровые api
    struct cash // server -> user
    {
     virtual ~cash() { }

     /**
      * выдача состояния стола. Выдаётся юзеру в момент открытия
      * диалога игры или в начале новой раздачи */
     virtual void set_table_state(
      gbp_i64 game_id
      , const table_state_t& table_state) = 0;

     // показать в лобби кнопки "сделать гостевую ставку" и "дождаться ББ"
     virtual void show_guest_buttons(gbp_i64 game_id) = 0;

     // скрыть в лобби кнопки "сделать гостевую ставку" и "дождаться ББ"
     virtual void hide_guest_buttons(gbp_i64 game_id) = 0;

     /**
      * показать в лобби кнопки "скрыть карты" и "показать карты" (и соотв. галки)
      * Они должны скрываться при начале след. раздачи */
     virtual void show_hand_buttons(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // отображение карт юзера
     virtual void show_user_hand(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , const ns_api::ns_table::cards& cards) = 0;

     // открытие стола юзера
     virtual void open_table(
      gbp_i64 game_id
      , const ns_api::ns_table::sit_on_table_info_t& info
      , const table_state_t& table_state) = 0;

     // открытие всех кешей юзера, в которых он играет
     virtual void open_cashes(const std::vector<ns_api::ns_table::table_state_t>& cashes) = 0;

     // сервер закрывает стол юзера
     virtual void close_table(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     /**
      * передача юзеру списка доступных ходов когда
      * наступает очередь хода */
     virtual void set_user_actions(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<available_user_course_t>& actions
      , const ns_api::ns_table::action_info_t& action_info) = 0;

     // отображение у юзера хода кого-либо из юзеров.
     virtual void set_user_action(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const ns_table::user_action_info_t& info) = 0;

     // "белый" банк
     virtual void set_common_pot(gbp_i64 tournament_id, gbp_i64 game_id, ns_api::money pot) = 0;
     virtual void clear_common_pot(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // предварительные действия игрока
     virtual void set_predefined_actions(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const ns_api::ns_table::money_info_t& money_info) = 0;

     /**
      * раздача карт игрокам за столом.
      * Юзер получает весь набор карт по раздаче. Если карта -1, то отображать
      * рубашкой вверх, иначе оригинальную */
     virtual void add_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const distrib_users_cards& users_cards) = 0;

     // раздача карт для определения дилера
     virtual void add_dealer_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const distrib_users_cards& users_cards) = 0;

     // передать текущий номер раздачи
     virtual void set_distribution_number(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 distribution_number) = 0;

     // передать текущий тип раунда
     virtual void set_current_round_type(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::ns_table::e_game_round_type round_type) = 0;

     // раздача карт на стол
     virtual void add_table_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i8>& table_cards) = 0;

     // юзер принял сообщение стола
     virtual void set_chat_message(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , const std::string& msg) = 0;

     // запрос у юзера блайнда
     virtual void ask_blind(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , bool is_big_blind
      , ns_api::money blind) = 0;

     // заморозить игрока
     virtual void freeze_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     // поместить игрока в ожидающие
     virtual void put_user_in_anticipant(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      // отобразить кнопку "вернуться в игру"
      , bool return_to_game_btn) = 0;

     // добавление игрока в стол (когда внёс бай-ин), всегда добавляется как "замороженный"
     virtual void add_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const table_player_info_t& user_info) = 0;

     /**
      * удаление игроков.
      * Если у удаляемого в лобби была кнопка "продолжить игру" (перед удалением игрой он
      * был в ожидании), то убрать её */
     virtual void remove_users(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<gbp_i64>& removed_users) = 0;

     /**
      * переместить юзера из игроков в наблюдающие.
      *
      * Причины перемещения из ожидающих в просматривающие:
      * distributions_reason: true - показать сообщение "вы просидели
      * много раздач и удалены" (если у игрока на момент его перемещения из
      * ожидающих в просматривающие стек был больше 0);
      *
      * recharge_reason: true - показать сообщение "вы долго не пополнялись,
      * поэтому удалены" (если у игрока на момент его перемещения стек был равен 0)
      * Если причин нет, то оба флага - false (напр. при выходе со стола в конце раздачи) */
     virtual void from_player_to_viewer(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , bool distributions_reason
      , bool recharge_reason) = 0;

     // установить победителей, возможно не по комбинации
     virtual void set_winners(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const hi_low_winners& winners
      , const users_and_cards& users_cards
      , bool by_combination) = 0;

     // отображение новых cash и round_bet игроков
     virtual void set_users_cashes(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const users_cashes_t& users_cashes) = 0;

     /**
      * устанавливаем текущего игрока (которому передаётся право хода).
      * Подсветить это место, показать тайм-бар, ...
      * Лобби рисует таймбар, значение которого уменьшается каждую секунду в соответствии со значениями скоростей.
      * time_left - осталось времени на ход */
     virtual void set_current_user(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 current_user_id
      , ns_api::time_s time_left) = 0;

     // устанавливаем банки стола
     virtual void set_pots(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<ns_api::money>& pots) = 0;

     // устанавливаем дилера
     virtual void set_dealer(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     // очистить стол от всех карт
     virtual void clear_cards(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // конец круга торгов
     virtual void end_of_trade_round(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_u8 round_number) = 0;

     // конец раздачи
     virtual void end_of_distribution(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // конец игры
     virtual void end_of_game(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

     // какой-либо из юзеров за столом стал онлайн
     virtual void set_user_online(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id) = 0;

     // установка новой позиции ожидающего за столом
     virtual void set_new_waiting_position(
      gbp_i64 game_id
      , gbp_u32 waiters_count
      , gbp_u16 w_pos) = 0;

     // отобразить у юзера диалог пополнения денег
     virtual void show_recharge_message(
      gbp_i64 game_id,
      const ns_api::ns_table::recharge_money_t& money) = 0;

     // установка юзеру денег за столом
     virtual void set_user_money(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , gbp_i64 user_id
      , ns_api::money money_) = 0;

    };

    // турнирные api
    struct tournament // server -> user
    {
     virtual ~tournament() { }

     /**
      * показать сообщение "В игре объявлен перерыв. Игра продолжится через {break_time} минут."
      * break_time - длительность перерыва в сек. */
     virtual void tournament_break(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::time_s break_time) = 0;

     /**
      * показать сообщение "В игре объявлен перерыв. Игра продолжится через {break_time} минут. Игроки 
      * могут докупить {addon_chips} фишек за {addon_money}"
      * break_time - длительность перерыва в сек. */
     virtual void tournament_addon_break(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::time_s break_time
      , ns_api::money addon_money
      , gbp_i64 addon_chips) = 0;

     /**
      * показать сообщение "перерыв начнётся 
      * тогда, когда закончатся раздачи на всех столах".
      * delay_time - через сколько секунд показать сообщение */
     virtual void show_autobreak_massage(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::time_s delay_time) = 0;

     /**
      * показать сообщение "перерыв и период аддонов начнётся тогда, когда 
      * закончатся раздачи на всех столах".
      * delay_time - через сколько секунд показать сообщение */
     virtual void show_autobreak_addon_massage(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::time_s delay_time) = 0;

     /**
      * начался автоперерыв и аддон доступен. Изменить кнопку ребая на кнопку 
      * аддона. Аддон не может сделать юзер, у которого денег на аккаунте 0. */
     virtual void addon_available(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const ns_api::addon_t& info
      , ns_api::money account_money
      , ns_api::time_s time_left) = 0;

     // выплата bounty игроку
     virtual void bounty(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const std::vector<std::string>& nicknames
      , ns_api::money money_) = 0;

     // открытие диалога турнирной игры
     virtual void open_tournament(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , const ns_api::ns_table::tournament_state_t& tournament_state
      , const ns_api::ns_table::table_state_t& table_state
      , gbp_i64 players_count) = 0;

     // игрок пересаживается за другой стол
     virtual void table_change(
      gbp_i64 tournament_id
      , gbp_i64 old_game_id
      , gbp_i64 new_game_id
      , gbp_i8 new_pos
      , const ns_api::ns_table::table_state_t& table_state
      /*, const ns_api::ns_statistic::tournament_description_t& tourn_desrc*/) = 0;

     /**
      * отображение кнопок "ребай", "двойной ребай" и "выйти из турнира". Вызывается, 
      * например, когда у игрока становится 0 денег и доступен ребай. 
      * Если double_available == false, то двойной ребай недоступен */
     virtual void show_rebuy_buttons(gbp_i64 tournament_id, gbp_i64 game_id, bool double_available) = 0;

     // скрытие диалога аддона
     virtual void hide_addon(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // скрытие кнопки ребая (например перед аддоном)
     virtual void hide_rebuy_btn(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // отображение турнирной инфы в отдельном диалоге
     virtual void show_tournament_dialog_info(
      gbp_i64 tournament_id
      , const gbp::ns_api::ns_table::tournament_dialog_info& info) = 0;

     // увеличение раунда турнира
     virtual void level_up(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , ns_api::money big_blind
      , ns_api::money ante
      , gbp_i16 current_round) = 0;

     // турнир отменён (например из-за недостаточного кол-ва зареганых в нём)
     virtual void tournament_canceled(gbp_i64 tournament_id, ns_api::money buy_in) = 0;

     // ожидание возможных ребаев (в конце раздачи)
     virtual void rebuys_waiting(
      gbp_i64 tournament_id
      , gbp_i64 game_id
      , time_s waiting_time) = 0;

     // сообщение о том, что турнирный стол удалён
     virtual void game_deleted(
      gbp_i64 tournament_id
      , gbp_i64 game_id) = 0;

    };    

   } // namespace ns_server_request
  } // namespace ns_table
 } // namespace ns_api
} // namespace gbp
