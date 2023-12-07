#pragma once

#include "../../api_common.h"
#include "../../lobby/reply.h"
#include "../../lobby/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_reply
  {
   struct lobby
   {
    virtual ~lobby() { }

    GBP_DECLARE_ENUM(
     e_db_lobby
     , gbp_u8
     , friend
     , (ok)
       (unknown_error)
    )

    virtual void hardware_checking(
     gbp::ns_db_server::ns_reply::lobby::e_db_lobby result     
     , gbp_i64 user_id
     , gbp::ns_api::ns_lobby::ns_reply::e_lobby lobby_result) = 0;

   }; // lobby
  } // ns_reply
 } // ns_db_server
} // namespace gbp