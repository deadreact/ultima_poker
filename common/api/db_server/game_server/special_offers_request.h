#pragma once

#include "../../api_special_offers.h"
#include "../../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_db_server
  {
   namespace ns_request
   {
    namespace ns_game_server
    {
     struct special_offers
     {
      virtual ~special_offers() { }

      // список билетов
      virtual void get_tickets() = 0;

      // создание нового билета
      virtual void insert_ticket(const ns_api::ns_special_offers::ticket& ticket_) = 0;

      // удаление билета
      virtual void remove_ticket(gbp_i64 ticket_id) = 0;

      // обновление билетов юзера
      virtual void update_user_tickets(gbp_i64 user_id, const std::string& tickets_json) = 0;

      // обновить данные по акциям
      virtual void update_user_special_offers_info(
       gbp_i64 user_id
       , const std::string& tickets_json
       , ns_api::money first_deposit
       , bool first_deposit_was_used) = 0;

      // обновить акции
      virtual void update_special_offers_info(const ns_api::ns_special_offers::special_offer& soffer) = 0;

     }; // campaigns
    } // ns_game_server
   } // ns_request
  } // ns_db_server
 } // ns_api
} // namespace gbp