#pragma once

#include <QString>

namespace ns_descriptor
{
    struct CashBuyIn
    {
        const QString tableName;
        const QString title;
        const std::pair<gbp_i64, gbp_i64> range;
        const gbp_i64 playersMoney;
        const gbp_i64 ratTime;
        const bool playerSitOnTable;

        CashBuyIn(const QString& tableName, const QString& title, gbp_i64 min, gbp_i64 max, gbp_i64 playersMoney, gbp_i64 ratTime = -1, bool playerSitOnTable = false)
            : tableName(tableName)
            , title(title)
            , range(std::make_pair(min, max))
            , playersMoney(playersMoney)
            , ratTime(ratTime)
            , playerSitOnTable(playerSitOnTable)
        {}
    };
} //namespace ns_descriptor
