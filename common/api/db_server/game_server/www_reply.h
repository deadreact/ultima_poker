#pragma once

#include "../../api_common.h"
#include "../../adminko/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_reply
  {
   struct www
   {
    virtual ~www() { }

    GBP_DECLARE_ENUM(
     e_www
     , gbp_u8
     , friend
     , (ok)
       (unknown_error)
    )

//    virtual void add_new_user(
//     gbp::ns_api::ns_common::e_common_result result
//     , const gbp::ns_api::ns_admin::new_user_profile_t& ui
//     , const std::string& hash_avatar
//     , const gbp::ns_api::ns_common::reg_new_user_db_result& u_result) = 0;

   }; // www
  } // ns_replies
 } // ns_db_server
} // namespace gbp