#pragma once

#include "../../api_common.h"
#include "../../api_special_offers.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_reply
  {
   struct special_offers
   {
    virtual ~special_offers() { }

    GBP_DECLARE_ENUM(
     e_special_offers
     , unsigned char
     , friend
     , (ok)
       (unknown_error)
    )

    virtual void get_tickets(
     gbp::ns_db_server::ns_reply::special_offers::e_special_offers result
     , const std::unordered_map<gbp_i64, gbp::ns_api::ns_special_offers::ticket>& tickets) = 0;

    virtual void insert_ticket(
     gbp::ns_db_server::ns_reply::special_offers::e_special_offers result
     , const gbp::ns_api::ns_special_offers::ticket& ticket_) = 0;

   };
  } // ns_reply
 } // ns_db_server
} // namespace gbp