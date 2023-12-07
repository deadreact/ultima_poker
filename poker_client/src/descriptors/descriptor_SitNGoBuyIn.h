
#include "descriptor_TournamentBuyIn.h"
#pragma once

#include <QString>

namespace ns_descriptor
{
    struct SitNGoBuyIn : TournamentBuyIn
    {
        SitNGoBuyIn(const QString& tableName, const QString& title, gbp_i64 money, gbp_i64 price, gbp_i16 tickets)
            : TournamentBuyIn(tableName, title, money, price, tickets, -1, false)
        {}
    };
} //namespace ns_descriptor
