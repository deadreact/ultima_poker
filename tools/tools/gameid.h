#pragma once

#include "tools_global.h"
#include "types_fwd.hpp"
#include <qmetatype.h>
#include <qstring.h>

struct TOOLS_DLL GameId
{
    gbp_i64 m_tableID, m_tournamentID;

    constexpr inline GameId()
        : m_tableID(-1)
        , m_tournamentID(-1)
    {}
    constexpr static GameId invalidID() {
        return GameId();
    }

    constexpr inline GameId(gbp_i64 tableID, gbp_i64 tournamentID = -1)
     : m_tableID(tableID), m_tournamentID(tournamentID)
    {}
    constexpr inline GameId(const GameId &other)
        : m_tableID(other.m_tableID)
        , m_tournamentID(other.m_tournamentID)
    {}
    constexpr inline GameId(GameId&& moved)
        : m_tableID(std::move(moved.m_tableID))
        , m_tournamentID(std::move(moved.m_tournamentID))
    {}

    GameId& operator=(const GameId& other);
    GameId& operator=(gbp_i64 tableID);

    QString toString() const;
    QString repr() const;

    inline bool operator <(const GameId& gameID) const { return (m_tournamentID < gameID.m_tournamentID) || (m_tournamentID == gameID.m_tournamentID && m_tableID < gameID.m_tableID); }
    inline bool operator==(const GameId& gameID) const { return m_tournamentID == gameID.m_tournamentID && m_tableID == gameID.m_tableID; }
    inline bool operator!=(const GameId& gameID) const { return !operator==(gameID); }

    inline bool isTournament() const { return (m_tournamentID != -1); }
    inline bool isCash() const { return (m_tournamentID == -1); }
    inline bool isValid() const { return (m_tableID != -1); }
};
Q_DECLARE_METATYPE(GameId)

struct TOOLS_DLL GamePlayerId
{
    static const GamePlayerId& invalidID();

    GameId m_gameID;
    gbp_i64 m_playerID;

    GamePlayerId(const GameId& gameID, gbp_i64 playerID = -1);
    GamePlayerId(gbp_i64 playerID = -1, const GameId& gameID = GameId::invalidID());

    QString toString() const;
    QString repr() const;

    inline bool operator <(const GamePlayerId& id) const { return (m_gameID == id.m_gameID) ? (m_playerID < id.m_playerID) : m_gameID < id.m_gameID; }
    inline bool operator==(const GamePlayerId& id) const { return m_gameID == id.m_gameID && m_playerID == id.m_playerID; }
    inline bool operator!=(const GamePlayerId& id) const { return !operator==(id); }

    inline bool isValid() const { return m_gameID.isValid() && (m_playerID != -1); }

    bool isCurrentPlayer() const;
};
Q_DECLARE_METATYPE(GamePlayerId)
