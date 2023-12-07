#include <precompiled.h>
#include "identifiable.h"
#include <rpc/network_client.h>
#include <PokerApplication.h>

// GamePlayerIdObject
GamePlayerIdObject::GamePlayerIdObject(const GamePlayerId &gamePlayerId)
    : Identifiable_tmp<GamePlayerId>(gamePlayerId)
{}
gbp_i64 GamePlayerIdObject::tableId() const { return id().m_gameID.m_tableID;  }
gbp_i64 GamePlayerIdObject::playerId() const { return id().m_playerID; }


// GameIdRefObject
GameIdRefObject::GameIdRefObject(const GameId &gameID)
    : m_gameID(gameID)
{}
const GameId &GameIdRefObject::gameId() const { return m_gameID; }


// CurrentGamePlayerIdObject
CurrentGamePlayerIdObject::CurrentGamePlayerIdObject(const GameId &gameID)
    : GamePlayerIdInterface()
    , m_gameID(gameID)
{}
const GameId &CurrentGamePlayerIdObject::gameId() const { return m_gameID; }
gbp_i64 CurrentGamePlayerIdObject::playerId() const { return pApp->client().userId(); }
