#pragma once


#ifndef Q_MOC_RUN
    #include <rpcbase/rpcbase_statistic.h>
    #include "reply_statistic.h"
#endif // Q_MOC_RUN

#include <core/money.h>

class request_statistic;
class NetworkClient;


struct rpc_statistic : ns_statistic::rpc_object
{
private:
    request_statistic* m_request;
    const reply_statistic::emitter_t* m_emitter;
public:
    explicit rpc_statistic(NetworkClient * const client);

public:
    virtual gbp::ns_api::ns_statistic::ns_request::stat_common *request_common() const override;
    virtual gbp::ns_api::ns_statistic::ns_request::stat_desktop *request_desktop() const override;
    virtual const ns_statistic::emitter_t *emitter() const override;
};

