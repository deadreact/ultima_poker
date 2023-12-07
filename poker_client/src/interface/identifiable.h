#pragma once

#include <basewidgets/types.h>

template <typename id_type>
struct Identifiable_tmp
{
public:
    static const id_type& invalidId() {
        static const id_type id(-1);
        return id;
    }

protected:
    id_type m_id;
    Identifiable_tmp(id_type id = invalidId()) : m_id(id) {}
    Identifiable_tmp(const Identifiable_tmp<id_type>& obj) : m_id(obj.id()) {}
public:
    inline const id_type& id() const { return m_id; }
    inline bool isValid() const { return m_id != invalidId();}

    inline bool operator<(const Identifiable_tmp<id_type>& other) const { return m_id < other.m_id; }
    inline bool operator==(const Identifiable_tmp<id_type>& other) const { return m_id == other.m_id; }
    inline bool operator!=(const Identifiable_tmp<id_type>& other) const { return m_id != other.m_id; }
};

struct SingleIdObject : public Identifiable_tmp<gbp_i64>
{
    SingleIdObject(gbp_i64 id = -1)
        : Identifiable_tmp<gbp_i64>(id)
    {}
};

struct GameIdObject : public Identifiable_tmp<GameId>
{
    GameIdObject(const GameId& gameId = GameId::invalidID())
        : Identifiable_tmp<GameId>(gameId)
    {}
    gbp_i64 tableId() const { return id().m_tableID; }
};

struct GamePlayerIdObject : public Identifiable_tmp<GamePlayerId>
{
    GamePlayerIdObject(const GamePlayerId& gamePlayerId = GamePlayerId::invalidID());
    gbp_i64 tableId() const;
    gbp_i64 playerId() const;
};

struct GameIdInterface
{
    virtual ~GameIdInterface() {}
    virtual const GameId& gameId() const = 0;
    inline gbp_i64 tableId() const { return gameId().m_tableID; }
};

struct GameIdRefObject : GameIdInterface
{
    const GameId& m_gameID;
    GameIdRefObject(const GameId& gameID);
    virtual const GameId& gameId() const override;
    inline const GameId& id() const;
};
const GameId &GameIdRefObject::id() const { return gameId(); }

struct GamePlayerIdInterface : GameIdInterface
{
    virtual gbp_i64 playerId() const = 0;
    inline GamePlayerId id() const;
};
inline GamePlayerId GamePlayerIdInterface::id() const { return GamePlayerId(gameId(), playerId()); }

struct CurrentGamePlayerIdObject : public GamePlayerIdInterface
{
    const GameId& m_gameID;
    CurrentGamePlayerIdObject(const GameId& gameID);

    virtual const GameId& gameId() const override;
    virtual gbp_i64 playerId() const override;
};


