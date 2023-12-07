#pragma once

#include <QObject>
#include <api/lobby_stat/reply.h>
#include "defs.h"

//! @namespace ns_statistic
namespace ns_statistic
{
    class emitter_t : public QObject
    {
        Q_OBJECT
    public:
        explicit emitter_t(QObject *parent = nullptr);
    signals:
        void games_statistic(const games_stat_t&) const;
        void get_additional_tourn_stat(e_status, const tourn_additional_stat_t&) const;
        void get_tourn_users_stat(e_status, long long id, const std::vector<tournament_stat_t::user_info>&) const;
    };
} // namespace ns_statistic
