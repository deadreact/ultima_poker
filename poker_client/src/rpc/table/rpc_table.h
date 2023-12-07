#pragma once

#ifndef Q_MOC_RUN
    #include <rpcbase/rpcbase_table.h>
    #include "reply.h"
#endif // Q_MOC_RUN

#include <core/money.h>

class NetworkClient;

struct request_table_cash;
struct request_table_tournament;

struct rpc_table : ns_table::rpc_object
{
private:
    request_table_cash* m_request_cash;
    request_table_tournament* m_request_tournament;
    const reply_table_cash::emitter_t* m_emitter;
public:
    explicit rpc_table(NetworkClient * const client);

    virtual gbp::ns_api::ns_table::ns_request::cash *request_cash() const override;
    virtual gbp::ns_api::ns_table::ns_request::tournament *request_tournament() const override;
    virtual const ns_table::emitter_t *emitter() const override;

};
