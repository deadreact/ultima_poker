#pragma once

#include <QString>
#include <tools/types_fwd.hpp>
#include <set>

namespace ns_helpers
{
    gbp::ns_api::ns_statistic::cash_stat_t*       findCashItem(gbp_i64 gameId);
    gbp::ns_api::ns_statistic::tournament_stat_t* findTournamentItem(gbp_i64 tournamentId);

    gbp::ns_api::ns_statistic::cash_stat_t       copyCashItem(gbp_i64 gameId, bool* ok = nullptr);
    gbp::ns_api::ns_statistic::tournament_stat_t copyTournamentItem(gbp_i64 tournamentId, bool* ok = nullptr);

// -------------------------------------------------------------------------
    int tableSeatCount(const GameId& id);
    int freeSeatsCount(const GameId& id);
    std::set<gbp_i8> freeSeats(const GameId& id);

    int tournamentFreeSeatsCount(const GameId& /*id*/);
    std::set<gbp_i8> tournamentFreeSeats(const GameId& /*id*/);

    int cashFreeSeatsCount(gbp_i64 id);
    std::set<gbp_i8> cashFreeSeats(gbp_i64 id);

    std::vector<bool> tableSeats(const GameId& id);

    QString tableName(gbp_i64 id);
    QString tableTitle(gbp_i64 id, const QString& pattern = "%0 $%1/$%2 %3 %4");

    gbp::ns_api::ns_game::e_game_limit gameLimit(gbp_i64 gameId, bool isCash = true);
    gbp::ns_api::ns_game::e_game_type  gameType(const GameId &gameId);
    gbp_i64 bigBlind(const GameId& gameId);
    gbp_i64 bigBlind(gbp_i64 gameId, bool isCash = true);

    int waitersCount(gbp_i64 tableId);

} //namespace ns_helpers
