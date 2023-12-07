#pragma once

#include <string>
#include <vector>
#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_common
  {

   /*************************************************************************************************/
   /*************************************** RPC LEVEL 1 *********************************************/
   /*************************************************************************************************/

   // апихи, не отсящиеся к каким-либо другим видам

   namespace ns_request // user -> server
   {

    struct common // user -> server
    {
     virtual ~common() { }
     // установка статуса юзера
     virtual void set_usertype(ns_details::e_user_type u_type) = 0;

    };

   } // namespace ns_request
  } // namespace ns_common
 } // namespace ns_api
} // namespace gbp