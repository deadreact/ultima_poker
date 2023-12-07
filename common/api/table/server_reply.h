#pragma once

#include "../api_common.h"

namespace gbp
{
 namespace ns_api
 {
  namespace ns_table
  {

   /*************************************************************************************************/
   /*************************************** RPC LEVEL 2 *********************************************/
   /*************************************************************************************************/

   namespace ns_server_reply
   {

    /**
     * ответы от клиента к серверу по запросам контроллера (запросы от 
     * контроллера к клиенту лежат в файле server_request.h) */
    struct cash // user -> server
    {
     virtual ~cash() { }

     // ответ от юзера по блайнду
     virtual void user_blind_done(
      gbp_i64 game_id
      , bool accepted) = 0;

    };

   } // namespace ns_server_reply
  } // namespace ns_table
 } // namespace ns_api
} // namespace gbp