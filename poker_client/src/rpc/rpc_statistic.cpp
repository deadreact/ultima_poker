#include <precompiled.h>
#include "rpc_statistic.h"
#include <basewidgets/util.h>
#include <qtimer.h>

#ifndef Q_MOC_RUN
    #include "request_statistic.h"
    #include "network_client.h"
#endif // Q_MOC_RUN

namespace
{
//    void killConnection(QMetaObject::Connection* conn) {
//        if (conn != nullptr) {
//            QObject::disconnect(*conn);
//            delete conn;
//        }
//    }
}

rpc_statistic::rpc_statistic(NetworkClient * const client)
    : ns_statistic::rpc_object()
    , m_request(client->request<request_statistic>())
    , m_emitter(client->emitter<reply_statistic>())
{

}

gbp::ns_api::ns_statistic::ns_request::stat_common *rpc_statistic::request_common() const {
    return m_request;
}

gbp::ns_api::ns_statistic::ns_request::stat_desktop *rpc_statistic::request_desktop() const {
    return m_request;
}

const ns_statistic::emitter_t *rpc_statistic::emitter() const {
    return m_emitter;
}
