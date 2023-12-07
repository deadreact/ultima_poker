#include <precompiled.h>
#include <QObject>
#ifndef Q_MOC_RUN
    #include "reply_statistic.h"
    #include "network_client.h"
#endif // Q_MOC_RUN

#include <iostream>
#include <PokerApplication.h>
#include <db/SharedDataService.h>

int ns_util::toInt(gbp::ns_api::ns_game::e_game_type gameType)
{
    return (int)(gameType);
}


static const char classname_common[] = "gbp::ns_api::ns_statistic::ns_reply::stat_common";
static const char classname_desktop[] = "gbp::ns_api::ns_statistic::ns_reply::stat_desktop";

reply_statistic::reply_statistic(NetworkClient * const client)
    : m_reg_desktop(client->create_registry(classname_desktop))
    , m_isInitialized(false)
{
    init();
}

const reply_statistic::emitter_t *reply_statistic::emitter() const {
    static const emitter_t e;
    return &e;
}

void reply_statistic::init()
{
    if (!m_isInitialized)
    {
        m_reg_desktop->on("games_statistic"          , this, &reply_statistic::games_statistic);
        m_reg_desktop->on("get_additional_tourn_stat", this, &reply_statistic::get_additional_tourn_stat);
        m_reg_desktop->on("get_tourn_users_stat"     , this, &reply_statistic::get_tourn_users_stat);

        m_isInitialized = true;
    }
}

void reply_statistic::games_statistic(const games_stat_t &stat)
{
//    std::cout << "reply.game_statistic" << std::endl;
    emit emitter()->games_statistic(stat);
}

void reply_statistic::get_additional_tourn_stat(e_stat result, const gbp::ns_api::ns_statistic::tourn_additional_stat_t &stat)
{
    emit emitter()->get_additional_tourn_stat(result, stat);
}

void reply_statistic::get_tourn_users_stat(e_stat result, gbp_i64 tourn_id, const std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info> &users_stat)
{
    emit emitter()->get_tourn_users_stat(result, tourn_id, users_stat);
}
