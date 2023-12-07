#pragma once

#include <QObject>
#include <interface/identifiable.h>
#include <api_models/algorithm.hpp>
#include "TimelineHandler.h"
#include <tools/types_fwd.hpp>
#include "gamehandlerinterface.hpp"

class GameTable;
class TableOver;
class Player;
class FramedEntity;

namespace ns_model
{
    class table_state_t;
}

namespace
{
    using money_info_t            = gbp::ns_api::ns_table::money_info_t;
    using action_info_t           = gbp::ns_api::ns_table::action_info_t;
    using user_action_info_t      = gbp::ns_api::ns_table::user_action_info_t;
    using e_user_action           = gbp::ns_api::ns_table::e_user_action;
    using available_user_course_t = gbp::ns_api::ns_table::available_user_course_t;
    using table_player_info_t     = gbp::ns_api::ns_table::table_player_info_t;
    using user_state_t            = gbp::ns_api::ns_table::user_state_t;
    using winner_info_t           = gbp::ns_api::ns_table::winner_info_t;
    using table_state_t           = gbp::ns_api::ns_table::table_state_t;
    using recharge_money_t        = gbp::ns_api::ns_table::recharge_money_t;
    using e_game_type             = gbp::ns_api::ns_game::e_game_type;
    using e_game_limit            = gbp::ns_api::ns_game::e_game_limit;
}

struct StandardTime {
    const int _short;
    const int _main;
    StandardTime(int shortTime, int mainTime)
        : _short(shortTime)
        , _main(mainTime)
    {}
    inline bool operator==(const StandardTime& tm) const { return tm._short == _short && tm._main == _main; }
    inline bool operator!=(const StandardTime& tm) const { return !operator==(tm); }

    QString toString() const {
        return QString("{short time: %0 sec, main time: %1 sec}").arg(_short).arg(_main);
    }
};

class GameViewHandler : public QObject, public GameViewHandlerInterface
{
    Q_OBJECT
protected:
    ns_model::table_state_t* m_model;
    QHash<QByteArray, QWidget*> m_mappings;
public:
    GameViewHandler(const GameId& id, QObject* parent = nullptr);
    virtual ~GameViewHandler();

    virtual void initialize() override;
    virtual bool indexAffectsView(const QModelIndex& index) const;
public slots:
    virtual void onModelAboutToBeReset() override = 0;
    virtual void onModelReset() override = 0;
    virtual void setIndexWidget(QWidget* widget, const QModelIndex& index = QModelIndex()) override;
    virtual void setIndexWidget(QWidget* widget, const QByteArray& key);
public:
    virtual QWidget* indexWidget(const QModelIndex& index = QModelIndex()) const override;
    virtual QWidget* indexWidget(const QByteArray& key) const;
protected:
    virtual void connectToModel() override;
    virtual void disconnectFromModel() override;
private slots:
    void _onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    void _onModelRowsAboutToBeInserted(const QModelIndex& parent, int first, int last);
    void _onModelRowsInserted(const QModelIndex& parent, int first, int last);
    void _onModelRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
    void _onModelRowsRemoved(const QModelIndex& parent, int first, int last);
    // ибо эти невиртуальны, и не будут вызываться, когда не нужно
    void _impl_connectToModel();
    void _impl_disconnectFromModel();
};


//class GameHandler : public QObject, public GameDataHandlerInterface
//{
//    Q_OBJECT
//public:
//    enum class ETime {
//        Short,
//        Main,
//        Common
//    };
//private:
//    const StandardTime* m_time;
//protected:
//    QScopedPointer<GameTable> m_table;
//    QScopedPointer<QActionGroup> m_tablePlayers;
//    QScopedPointer<TimelineHandler> m_timeline;
//    Player* m_player;        // Current player controls
//    FramedEntity* m_glow;
//public:
//    explicit GameHandler(const GameId& gameId, QObject *parent = nullptr);
//    virtual ~GameHandler();

////    GameTable* getTable() const;
//    TableOver* getLayers() const;
////    virtual Player* getPlayer() const override;

//protected:
//    GameTable* createTable(QWidget *parent = nullptr) const;
//    int standardTime(ETime timeType = ETime::Common) const;
//    void setStandardTime(int shortTime, int mainTime);
//    Q_SLOT virtual void timelineEvent(const TimeLineEvent &ev) = 0;
//signals:
//    void tableOpened(const GameId& id);
//    void tableClosed(const GameId& id);
//public slots:
//    void openGame(bool useDirtyHack = false);
//public:
//    bool isOpened() const;
//    virtual gbp_i64         pot(int index = 0) const = 0;
//    virtual gbp_i64         bigBlind() const = 0;
//    virtual QString         tableName() const = 0;
//    virtual e_game_type     gameType() const = 0;
//    virtual e_game_limit    gameLimit() const = 0;
//public:
//    virtual void initialize() override;
//    virtual void makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info) override;
//    virtual void clearTableCards() override;
//    virtual void addPlayer(gbp_i64 playerID) override;
//    virtual void removePlayer(gbp_i64 playerID) override;
//    virtual void updatePlayer(const table_player_info_t& info) override;

//    TablePlayer *findPlayer(gbp_i64 playerID) const;
//    TablePlayer *playerAt(gbp_i8 pos) const;
//    gbp_i8 playerPos(gbp_i64 playerID) const;
//    gbp_i64 playerIdAt(gbp_i8 pos) const;
//    QString playerNickname(gbp_i64 playerID) const;

//    QActionGroup* playersGroup() const;
//    TablePlayerList tablePlayers() const;
//    TimelineHandler* timeline() const { return m_timeline.data(); }

//    Q_SLOT virtual void openTable() override {
//        openGame(true);
//    }
//    Q_SLOT virtual void closeTable() override {}
//};

