#include <precompiled.h>
#include "server_reply.h"
#include <rpc/network_client.h>

namespace ns_server_reply
{
/*static*/ const std::string &table_cash::parentClassName()
{
    static const std::string className = "gbp::ns_api::ns_table::ns_server_reply::cash";
    return className;
}

table_cash::table_cash(NetworkClient * const client)
    : gbp::ns_api::ns_table::ns_server_reply::cash()
    , m_client(*client)
{}

void table_cash::user_blind_done(gbp_i64 game_id, bool accepted)
{
    m_client.call_server(parentClassName() + ".user_blind_done", game_id, accepted);
}

} //namespace ns_server_reply
