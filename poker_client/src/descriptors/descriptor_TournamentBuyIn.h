#pragma once

#include <QString>

namespace ns_descriptor
{
    struct TournamentBuyIn
    {
        const QString tableName;
        const QString title;
        const gbp_i64 money;
        const gbp_i64 price;
        const gbp_i16 tickets;
        const gbp_i16 gamesQuantity;
        const bool rebuy;

        TournamentBuyIn(const QString& tableName, const QString& title, gbp_i64 money, gbp_i64 price, gbp_i16 tickets, gbp_i16 gamesQuantity, bool rebuy)
            : tableName(tableName)
            , title(title)
            , money(money)
            , price(price)
            , tickets(tickets)
            , gamesQuantity(gamesQuantity)
            , rebuy(rebuy)
        {}

    };

    struct FreerollBuyIn : TournamentBuyIn
    {
        FreerollBuyIn(const QString& tableName, const QString& title, gbp_i16 tickets, bool rebuy)
            : TournamentBuyIn(tableName, title, -1, -1, tickets, -1, rebuy)
        {}
    };

} //namespace ns_descriptor
