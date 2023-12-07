#include "gameid.h"
#include <qregularexpression.h>

//const GameId &GameId::invalidID()
//{
//    static const GameId invalidID;
//    return invalidID;
//}


GameId &GameId::operator=(const GameId &other) {
    m_tableID = other.m_tableID;
    m_tournamentID = other.m_tournamentID;
    return *this;
}

GameId &GameId::operator=(gbp_i64 tableID) {
    m_tableID = tableID;
    return *this;
}

QString GameId::toString() const {
    return QString("ID(table:%0%1)").arg(m_tableID).arg(m_tournamentID == -1 ? "" : QString(", tournament:%0").arg(m_tournamentID));
}
QString GameId::repr() const {
    return toString().replace(QRegularExpression("[\\(\\)\\:]"), "_");
}

const GamePlayerId &GamePlayerId::invalidID()
{
    static const GamePlayerId invalidID;
    return invalidID;
}

GamePlayerId::GamePlayerId(const GameId &gameID, gbp_i64 playerID)
    : m_gameID(gameID), m_playerID(playerID)
{}

GamePlayerId::GamePlayerId(gbp_i64 playerID, const GameId &gameID)
    : m_gameID(gameID), m_playerID(playerID)
{}

QString GamePlayerId::toString() const {
    return QString("ID(player:%0, table:%1%2)").arg(m_playerID).arg(m_gameID.m_tableID).arg(m_gameID.m_tournamentID == -1 ? "" : QString(", tournament:%0").arg(m_gameID.m_tournamentID));
}
QString GamePlayerId::repr() const {
    return QString("ID_player_%0__table_%1%2_").arg(m_playerID).arg(m_gameID.m_tableID).arg(m_gameID.m_tournamentID == -1 ? "" : QString("__tournament_%0").arg(m_gameID.m_tournamentID));
}

bool GamePlayerId::isCurrentPlayer() const
{
#ifdef NOT_A_PLUGIN
    return netClient().userId() == m_playerID;
#endif //NOT_A_PLUGIN
    return false;
}
