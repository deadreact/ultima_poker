#pragma once

#include <string>
#include <vector>
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

   namespace ns_request
   {
    struct cash
    {
     virtual ~cash() { }

     // юзер запрашивает состояние стола
     virtual void get_table_state(gbp_i64 game_id) = 0;

     // юзер послал сообщение остальным за своим столом
     virtual void send_chat_message(gbp_i64 game_id, const std::string& msg) = 0;

     // юзер закрыл диалог игры
     virtual void close_table(gbp_i64 game_id) = 0;

     // юзер сделел ход
     virtual void user_action(
      gbp_i64 game_id
      , const std::pair<ns_table::e_user_action, ns_api::money>& action) = 0;

     // юзер нажал на кнопку "вернуться в игру"
     virtual void user_online(gbp_i64 game_id) = 0;

     /**
      * юзер регистрируется в кеше.
      * Передача ему нужных данных */
     virtual void sit_on_table(gbp_i64 game_id, gbp_i8 user_pos) = 0;

     // юзер передумал ставить стек при регистрации в кеше
     virtual void sit_on_table_canceled(gbp_i64 game_id) = 0;

     // юзер указывает деньги, с которыми хочет сесть за стол
     virtual void set_sit_money(gbp_i64 game_id, ns_api::money money_) = 0;

     // установка возможности автопополнения внутриигровых денег
     virtual void set_autorecharge(gbp_i64 game_id, bool set) = 0;

     // получение информации по деньгам для внутриигрового пополения
     virtual void get_recharge_money(gbp_i64 game_id) = 0;

     /**
      * пополнение внутриигровых денег игроку по его запросу (только для кеша).
      * Максимум игрок может пополнить до суммы равной входу в кеш. Не может
      * пополнять, если внутриигровых денег у него больше суммы равной входу в кеш.
      * В случае успешного пополнения игра рассылает это событие
      * по всем игрокам стола отдельным сигналом. */
     virtual void add_game_money(gbp_i64 game_id, ns_api::money money_) = 0;

     // юзер хочет смотреть игру
     virtual void add_viewer(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     // режим гостевой ставки
     virtual void set_guest_blind(gbp_i64 game_id, bool enabled) = 0;

     /**
      * если set == true, то юзер в конце раздачи хочет встать
      * со стола (перейти в просматривающие, viewers).
      * Если false, то отменить это решение */
     virtual void set_sit_out(gbp_i64 game_id, bool set) = 0;

     // юзер в конце раздачи хочет показать свои карты
     virtual void set_show_hand(gbp_i64 game_id) = 0;

     // игрок из просматривающих хочет сесть за игру
     virtual void from_viewer_to_player(gbp_i64 game_id, gbp_i8 pos) = 0;

     // юзер хочет встать в очередь ожидающих в игру
     virtual void add_waiter(gbp_i64 game_id) = 0;

     // юзер выходит из очереди ожидающих игру
     virtual void remove_from_waiters(gbp_i64 game_id) = 0;

    }; // struct request_cash

    // относящиеся только к турниру

    struct tournament // user -> server
    {
     virtual ~tournament() { }

     /**
      * запрос инфы для регистрации в турнире. Ответом также может быть то, 
      * что юзер только что зарегался в турнире по бонус-коду */
     virtual void get_tourn_register_info(gbp_i64 tournament_id) = 0;

     /**
      * регистрация в турнире.
      * by_ticket - юзер выбрал для регистрации билет */
     virtual void tournament_register(gbp_i64 tournament_id, bool by_ticket) = 0;

     // отмена регистрации в турнире
     virtual void cancel_tourn_register(gbp_i64 tournament_id) = 0;

     // запрос информации по ребаю (для возможной последующей апишки "make_rebuy") для отображения диалога его покупки,
     virtual void request_rebuy(gbp_i64 tournament_id, gbp_i64 game_id, bool double_rebuy) = 0;

     /**
      * юзер делает ребай. Фишки в игру добавляются (конвертируются с
      * учётом настроек ребаев турнира) за счёт денег user_context->cash,
      * повышая при этом счётчик ребаев турнира */
     virtual void make_rebuy(gbp_i64 tournament_id, gbp_i64 game_id, bool double_rebuy) = 0;

     /**
      * юзер делает аддон. Фишки в игру добавляются за
      * счёт user_context->cash, счётчик сделанных аддонов турнира повышается */
     virtual void make_addon(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     /**
      * юзер нажал на "отмена" в диалоге аддона.
      * После этого сервер может, например, выслать оповещение сколько времени
      *  осталось до конца перерыва */
     virtual void addon_dialog_canceled(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

     /**
      * юзер запросил состояние турнира (у него открылся диалог турнирной игры).
      * Это потребуется для определения возможности ребаев и аддона на
      * стороне лобби */
     virtual void tournament_get_state(gbp_i64 tournament_id) = 0;

     /**
      * юзер открывает диалог турнирной игры. Например, если он играл в турнире, потом закрыл диалог,
      * но позже решил продолжить игру */
     virtual void open_own_tournament(gbp_i64 tournament_id) = 0;

     /**
      * юзер нажал "встать со стола" среди кнопок "ребай", "двойной ребай"
      * и "встать со стола" (в ответ вызывается tournament_finished()) */
     virtual void tournament_sit_out(gbp_i64 tournament_id, gbp_i64 game_id) = 0;

    }; // struct request_tournament

    // flash-poker

    struct flash_poker // server -> user
    {
     virtual ~flash_poker() { }

     // открыть flash-стол
     virtual void open_flash_cash(gbp_i64 flash_cash_id, gbp_i8 tables_count, ns_api::money stack) = 0;

    }; // struct flash_poker

   } // ns_request
  } // namespace ns_table
 } // namespace ns_api
} // namespace gbp