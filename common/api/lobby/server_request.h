#pragma once

#include "types.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_lobby
  {

   /*************************************************************************************************/
   /*************************************** RPC LEVEL 1 *********************************************/
   /*************************************************************************************************/

   // запросы от контроллера к клиенту

   namespace ns_server_request
   {
    struct special_offers
    {
     // сервер сообщает юзеру о том, что он выиграл приз
     virtual void user_won_prize(const ns_api::user_prize& prize) = 0;

     // отображение диалога в лобби по акции "случайный приз на депозит"
     virtual void show_soffer_random_prize_dialog() = 0;
    };

    struct lobby // server -> user
    {
     virtual ~lobby() { }

     // сервер перешёл в стадию останова, передаётся время останова
     virtual void server_stop(ns_api::time_s date_time) = 0;

    };

   } // namespace ns_server_request
  } // namespace ns_lobby
 } // namespace ns_api
} // namespace gbp
