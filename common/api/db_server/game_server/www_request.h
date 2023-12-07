#pragma once

#include "../../api_common.h"
#include "../../adminko/types.h"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_request
  {
   struct www
   {
    virtual ~www() { }

    virtual void add_payment_in(const gbp::ns_api::ns_common::payment_in& payment_in) = 0;

    // common api
    virtual void set_new_password(gbp_i64 user_id, const std::string& new_password) = 0;

   }; // www
  } // ns_request
 } // ns_db_server
} // namespace gbp