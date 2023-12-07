#include <precompiled.h>
#include "helper_statistic.h"

#ifndef Q_MOC_RUN
    #include <api/lobby_stat/types.h>
#endif //!Q_MOC_RUN

#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <tools/item_reference.hpp>
#include <core/money.h>

template <bool> struct Item {
    typedef gbp::ns_api::ns_statistic::cash_stat_t type;
};

template <> struct Item<false> {
    typedef gbp::ns_api::ns_statistic::tournament_stat_t type;
};


namespace ns_helpers
{
    gbp::ns_api::ns_statistic::cash_stat_t *findCashItem(gbp_i64 gameId) {
        return modelsDB().getCashStatistic()->item(gameId).ptr();
    }

    gbp::ns_api::ns_statistic::tournament_stat_t *findTournamentItem(gbp_i64 tournamentId) {
        return modelsDB().getTournamentStatistic()->item(tournamentId).ptr();
    }

    gbp::ns_api::ns_statistic::cash_stat_t copyCashItem(gbp_i64 gameId, bool *ok)
    {
        if (ok) *ok = false;
        if (gbp::ns_api::ns_statistic::cash_stat_t *cashItem = findCashItem(gameId)) {
            if (ok) *ok = true;
            return *cashItem;
        }
        return gbp::ns_api::ns_statistic::cash_stat_t();
    }

    gbp::ns_api::ns_statistic::tournament_stat_t copyTournamentItem(gbp_i64 tournamentId, bool *ok)
    {
        if (ok) *ok = false;
        if (gbp::ns_api::ns_statistic::tournament_stat_t *tournamentItem = findTournamentItem(tournamentId)) {
            if (ok) *ok = true;
            return *tournamentItem;
        }
        return gbp::ns_api::ns_statistic::tournament_stat_t();
    }


    int tableSeatCount(const GameId &id)
    {
        if (id.m_tournamentID == -1) {
            if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(id.m_tableID)) {
                return item->_players_count.second;
            }
            return -1;
        }
        if (gbp::ns_api::ns_statistic::tournament_stat_t* item = findTournamentItem(id.m_tournamentID)) {
            return item->_seats_count;
        }
        return -1;
    }

    int freeSeatsCount(const GameId &id)
    {
        if (id.m_tournamentID == -1) {
            return cashFreeSeatsCount(id.m_tableID);
        }
        return tournamentFreeSeatsCount(id);
    }
    std::set<gbp_i8> freeSeats(const GameId& id)
    {
        if (id.m_tournamentID == -1) {
            return cashFreeSeats(id.m_tableID);
        }
        return tournamentFreeSeats(id);
    }

    int cashFreeSeatsCount(gbp_i64 id)
    {
        if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(id)) {
            return item->_players_count.second - item->_players_count.first;
        }
        return -1;
    }

    std::set<gbp_i8> cashFreeSeats(gbp_i64 id)
    {
        std::set<gbp_i8> seats;

        if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(id)) {
            seats = ns_util::range(gbp_i8(0), item->_players_count.second);
            for (const gbp::ns_api::ns_statistic::player_t& player: item->_players) {
                seats.erase(player._pos);
            }
        }
        return seats;
    }

    QString tableName(gbp_i64 id)
    {
        if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(id)) {
            return QString::fromStdString(item->_title);
        }
        return QString();
    }

    QString tableTitle(gbp_i64 id, const QString& pattern)
    {
        if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(id)) {
            gbp_i64 bigBlind = item->_big_blind;
            return pattern.arg(item->_title.c_str())
                          .arg(Money(bigBlind/2).toString())
                          .arg(Money(bigBlind).toString())
                          .arg(ns_util::toString(item->_limit))
                          .arg(ns_util::toString(item->_type));
        }
        return "";
    }

    gbp::ns_api::ns_game::e_game_limit gameLimit(gbp_i64 gameId, bool isCash)
    {
        if (isCash) {
            if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(gameId)) {
                return item->_limit;
            }
        } else {
            if (gbp::ns_api::ns_statistic::tournament_stat_t* item = findTournamentItem(gameId)) {
                return item->_game_limit;
            }
        }
        return gbp::ns_api::ns_game::e_game_limit();
    }

    gbp::ns_api::ns_game::e_game_type gameType(const GameId& gameId)
    {
        if (gameId.isCash()) {
            if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(gameId.m_tableID)) {
                return item->_type;
            }
        } else {
            if (gbp::ns_api::ns_statistic::tournament_stat_t* item = findTournamentItem(gameId.m_tournamentID)) {
                return item->_game_type;
            }
        }

        return gbp::ns_api::ns_game::e_game_type();
    }

    gbp_i64 bigBlind(gbp_i64 gameId, bool isCash)
    {
        if (isCash) {
            if (Item<true>::type* item = findCashItem(gameId)) {
                return item->_big_blind;
            }
        } else if (Item<false>::type* item = findTournamentItem(gameId)) {
            return item->_big_blind;
        }

        return -1;
    }

    gbp_i64 bigBlind(const GameId &gameId)
    {
        if (gameId.isCash()) {
            if (Item<true>::type* item = findCashItem(gameId.m_tableID)) {
                return item->_big_blind;
            }
        } else if (Item<false>::type* item = findTournamentItem(gameId.m_tableID)) {
            return item->_big_blind;
        }

        return -1;
    }

    int waitersCount(gbp_i64 tableId)
    {
        if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(tableId)) {
            return item->_wait_players;
        }
        return -1;
    }

    int tournamentFreeSeatsCount(const GameId &) { return 0; }

    std::set<gbp_i8> tournamentFreeSeats(const GameId &) { return std::set<gbp_i8>(); }

    std::vector<bool> tableSeats(const GameId &id)
    {
        std::vector<bool> seats;

        if (id.isCash())
        {
            if (gbp::ns_api::ns_statistic::cash_stat_t* item = findCashItem(id.m_tableID)) {
                seats.assign(true, item->_players_count.second);
                for (const gbp::ns_api::ns_statistic::player_t& player: item->_players) {
                    seats[player._pos] = false;
                }
            }
        }
        else if (id.isTournament())
        {
        }
        std::cout << seats << std::endl;
        return seats;
    }



} //namespace ns_helpers
