#pragma once
#include <QVariant>

#include <rpcbase/emitter_statistic.h>
#ifndef Q_MOC_RUN
    #include <api/lobby_stat/reply.h>
    #include <netio/object_registry.hpp>
#endif // Q_MOC_RUN

#include <tools/types_fwd.hpp>

class NetworkClient;
class QString;

namespace ns_util
{
    int toInt(gbp::ns_api::ns_game::e_game_type gameType);
}

struct reply_statistic : gbp::ns_api::ns_statistic::ns_reply::stat_common, gbp::ns_api::ns_statistic::ns_reply::stat_desktop
{
public:
    using e_stat = gbp::ns_api::ns_statistic::ns_reply::e_stat;
    typedef ns_statistic::emitter_t emitter_t;
    const emitter_t* emitter() const;
private:
//    gbp::net::object_registry_ptr m_reg_common;
    gbp::net::object_registry_ptr m_reg_desktop;
    bool m_isInitialized;
    // prev && new
    quintptr m_sharedDataId[2];

    void init();
public:
    reply_statistic(NetworkClient* const client);
    virtual ~reply_statistic() {}
    // stat_desktop interface
public:
    virtual void games_statistic(const games_stat_t &stat) override;
    virtual void get_additional_tourn_stat(gbp::ns_api::ns_statistic::ns_reply::e_stat result, const gbp::ns_api::ns_statistic::tourn_additional_stat_t &stat) override;
    virtual void get_tourn_users_stat(gbp::ns_api::ns_statistic::ns_reply::e_stat result, gbp_i64 tourn_id, const std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info> &users_stat) override;
};
