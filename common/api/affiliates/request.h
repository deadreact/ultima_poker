#pragma once

#include "types.h"
#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_affiliates
  {
   namespace ns_request
   {
    struct misc
    {
     virtual ~misc() { }

     virtual void users_register(const std::vector<gbp::ns_api::ns_affiliates::user_register>& users_info) = 0;
     virtual void users_activity(const std::vector<gbp::ns_api::ns_affiliates::user_activity>& users_activity) = 0;

    };

   } // namespace ns_request
  } // namespace ns_affiliates
 } // namespace ns_api
} // namespace gbp
