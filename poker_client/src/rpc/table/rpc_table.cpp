#include <precompiled.h>
#include "rpc_table.h"

#ifndef Q_MOC_RUN
    #include "../network_client.h"
    #include "request.h"
#endif // !Q_MOC_RUN

namespace {

}

rpc_table::rpc_table(NetworkClient * const client)
    : m_request_cash(client->request<request_table_cash>())
    , m_request_tournament(client->request<request_table_tournament>())
    , m_emitter(client->emitter<reply_table_cash>())
{}


gbp::ns_api::ns_table::ns_request::cash *rpc_table::request_cash() const {
    return m_request_cash;
}
gbp::ns_api::ns_table::ns_request::tournament *rpc_table::request_tournament() const {
    return m_request_tournament;
}
const ns_table::emitter_t *rpc_table::emitter() const {
    return m_emitter;
}
