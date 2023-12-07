#include "rpcbase_statistic.h"

#include <api/lobby_stat/request.h>
#include <api/lobby_stat/reply.h>
#include <api/lobby_stat/types.h>

namespace
{
    void killConnection(QMetaObject::Connection* conn) {
        if (conn != nullptr) {
            QObject::disconnect(*conn);
            delete conn;
        }
    }
}

namespace ns_statistic
{
    QString rpc_object::classname_common()  { return QLatin1String("gbp::ns_api::ns_statistic::ns_request::stat_common"); }
    QString rpc_object::classname_desktop() { return QLatin1String("gbp::ns_api::ns_statistic::ns_request::stat_desktop"); }

    void rpc_object::games_statistic(on_games_statistic callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, conn](const games_stat_t& statistic) {
                killConnection(conn);
                callback(statistic);
            };
            *conn = QObject::connect(emitter(), &emitter_t::games_statistic, callback2);
        }
        request_desktop()->games_statistic();
    }
    void rpc_object::get_additional_tourn_stat(gbp_i64 tourn_id, on_get_additional_tourn_stat callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tourn_id, conn](e_status status, const tourn_additional_stat_t& statistic) {
                if (tourn_id  == statistic._id) {
                    killConnection(conn);
                    callback(status, statistic);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_additional_tourn_stat, callback2);
        }
        request_desktop()->get_additional_tourn_stat(tourn_id);
    }
    void rpc_object::get_tourn_users_stat(gbp_i64 tourn_id, on_get_tourn_users_stat<tournament_stat_t::user_info> callback)
    {
        if (callback)
        {
            QMetaObject::Connection* conn = new QMetaObject::Connection;
            auto callback2 = [callback, tourn_id, conn](e_status status, gbp_i64 id, const std::vector<tournament_stat_t::user_info>& users_stat) {
                if (tourn_id == id) {
                    killConnection(conn);
                    callback(status, users_stat);
                }
            };
            *conn = QObject::connect(emitter(), &emitter_t::get_tourn_users_stat, callback2);
        }
        request_desktop()->get_tourn_users_stat(tourn_id);
    }


} //ns_statistic

