#pragma once

#include "../api_common.h"
#include "../mail/types.hpp"

namespace gbp
{
 namespace ns_db_server
 {
  namespace ns_reply
  {
   // db_server -> mail_server
   struct db_server_mail_server
   {
    virtual ~db_server_mail_server() { }

    virtual void get_mail_items(const std::vector<gbp::ns_api::ns_mail::db_mail_user>& items) = 0;
   };
  } // ns_request
 } // ns_db_server

} // ns gbp