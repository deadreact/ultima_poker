#include <precompiled.h>
#include "helper_cash.h"
#include <QDebug>

namespace ns_helpers
{
    gbp_i64 potBet(gbp_i64 commonPot, gbp_i64 currentBet) {
        return (commonPot + currentBet) + currentBet;
    }

    gbp_i64 halfPotBet(gbp_i64 commonPot, gbp_i64 currentBet) {
        return (commonPot + currentBet)/2 + currentBet;
    }
} //namespace ns_helpers
