#pragma once

#include "PlayersManager.h"
#include "utils/logger.h"
#include <QMetaEnum>
#include "ErrorHandler.h"
#include <dialogs/dialog_gametable.h>
#include "gamehandlerinterface.hpp"

class SingleIdObject;
class GameDataHandlerInterface;
//class GameHandler;
//class CashHandler;
class GameTable;
//class Player;
class ProxyDataHandler;
class ProxyViewHandler;
class ControlsHandler;

class GameId;
namespace ns_server_request {
    class emitter_table;
} //namespace ns_server_request


class HandlerGroup : public GameHandlerInterface
{
    struct Data;
    QSharedDataPointer<Data> m_data;
public:
    HandlerGroup(const GameId &id, QObject* hParent = nullptr);
    virtual ~HandlerGroup();

    ProxyDataHandler& dataHandler();
    ProxyViewHandler& viewHandler();
    ControlsHandler& ctrlHandler();

    // GameHandlerInterface interface
public:
    virtual void initialize() override;
    virtual void timelineEvent(const TimeLineEvent &ev) override;
    virtual void openTable() override;
    virtual void closeTable() override;
};

class GameManager : public QObject
{
    Q_OBJECT
public:
    enum PreferedPos : gbp_i8
    {
        Any = -1
    };
signals:
    void tableClosed(const GameId& id);
    void tableOpened(const GameId& id);
//    void waitersInfoChanged(int changesType/*WaiterInfoChangeType*/);
private:
    NetworkClient& m_client;
    GameErrorHandler m_errHandler;
    PlayersManager m_playersMgr;
    QMap<GameId, HandlerGroup> m_handlers;
    QMap<GameId, GameTable*> m_gameTables;

//    QMap<GameId, ProxyViewHandler*> m_viewHandlers;
//    TablePlayerMap m_allPlayers;
    const ns_server_request::emitter_table* m_serverRequest;
public:
    GameManager(QObject* parent = nullptr);
    virtual ~GameManager();
    virtual void registerServerEvents();
    virtual void registerServerRequestHandlers();
//    virtual void registerReplyHandlers();

    // юзер за разными столами
//    TablePlayerList findPlayersByPlayerId(gbp_i64 playerID);
    // юзеры за конкретным столом
//    TablePlayerList findPlayersByGameId(const GameId& gameID);
//    TablePlayer* findTablePlayer(const GamePlayerId& id);

    bool initGame(const GameId &gameId, QWidget* viewParent = nullptr);
    Q_SLOT void removeGame(const GameId &gameId);

    HandlerGroup& handlers(const GameId& id);
    ProxyDataHandler &dataHandler(const GameId& id);
    ProxyViewHandler &viewHandler(const GameId& id);
    ControlsHandler &ctrlHandler(const GameId& id);

//    GameHandler *gameHandler(const GameId& id) const;
//    CashHandler* cashHandler(gbp_i64 id) const;

//    bool requestSitOnTable(gbp_i64 gameId, gbp_i8 pos = PreferedPos::Any);
//    bool requestWatchTheGame(gbp_i64 gameId);
//    bool requestJoinQueue(gbp_i64 gameId);
//    bool requestLeaveQueue(gbp_i64 gameId);

protected:
//    void onSitOnTableConfirmed(const GameId& gameId, const gbp::ns_api::ns_table::sit_on_table_info_t& info);
//    void enshurePlayerCreated(const GameId& gameID, gbp_i64 playerID, const QString &nickname = QString(), const sha1_t &hashAvatar = sha1_t());
//    void enshureCurrentPlayerCreated(const GameId& gameID);
private:
//    CashHandler *createCashHandler(gbp_i64 id);
    GameTable *createGameTable(const GameId& gameID, QWidget* parent = nullptr);
    void removeGameTable(const GameId& gameID);
public:
    GameTable *getGameTable(const GameId& gameID) const;
    bool isTableCreated(const GameId& gameID) const;
public slots:
    void onLogin();
    void onLogout(long long playerID);
    void onDisconnected();
private slots:
    void handleFreezeUser(const GameId& gameID, gbp_i64 user_id);
    void handlePutUserInAnticipant(const GameId& gameID, gbp_i64 user_id, bool return_to_game_btn);
    void handleSetUserOnline(const GameId& gameID, gbp_i64 user_id);
};
