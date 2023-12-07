#pragma once

#ifndef Q_MOC_RUN
    #include <api/table/server_reply.h>
#endif // Q_MOC_RUN
#include "../../common/api/gbp_int.hpp"

class NetworkClient;

namespace ns_server_reply
{
    struct table_cash : gbp::ns_api::ns_table::ns_server_reply::cash
    {
    private:
        NetworkClient& m_client;
        static const std::string& parentClassName();
    public:
        explicit table_cash(NetworkClient * const client);

        virtual void user_blind_done(gbp_i64 game_id, bool accepted) override;
    };
} //namespace ns_server_reply
