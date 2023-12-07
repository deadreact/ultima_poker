#pragma once

namespace gbp
{
 namespace ns_api
 {
  namespace ns_table
  {
   namespace ns_server_reply
   {

    /**
     * ответы от бота к серверу по запросам контроллера (запросы от
     * контроллера к боту лежат в файле gameserver_bot_request.h) */
    struct cash
    { // bot -> server
     virtual ~cash() { }

     // ответ от бота по блайнду
     virtual void user_blind_done(gbp_i64 game_id, bool accepted) = 0;
    };

   } // namespace ns_server_reply
  } // namespace ns_table
 } // namespace ns_api
} // namespace gbp