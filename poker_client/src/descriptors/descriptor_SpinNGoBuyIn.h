
#include "descriptor_TournamentBuyIn.h"
#pragma once

#include <QString>

namespace ns_descriptor
{
    struct SpinNGoBuyIn : TournamentBuyIn
    {
        SpinNGoBuyIn(const QString& tableName, const QString& title, gbp_i64 money, gbp_i64 price, gbp_i16 tickets, gbp_i16 gamesQuantity)
            : TournamentBuyIn(tableName, title, money, price, tickets, gamesQuantity, false)
        {}
    };
} //namespace ns_descriptor
