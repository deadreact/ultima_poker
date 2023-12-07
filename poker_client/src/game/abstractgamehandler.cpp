#include <precompiled.h>
#include <qtimer.h>
#include "abstractgamehandler.h"
//#include "player.h"
#include <utils/logger.h>
#include <descriptors/descriptor_GameTable.h>
#include <helpers/helper_statistic.h>
#include <widgets/MainWindow.h>
#include <QActionGroup>
#include <PokerApplication.h>
#include <basewidgets/framedentity.h>
#include <dialogs/dialog_gametable.h>
#include <widgets/widget_TableOver.h>

#ifndef Q_MOC_RUN
#include <db/modeldatabase.h>
    #include <rpc/rpc_common.h>
    #include <rpc/table/rpc_table.h>
#endif // !Q_MOC_RUN

#include <api_models/model_table_state_t.hpp>

namespace
{
//    rpc_table* getRpc() { return rpc_object<rpc_table>(); }

    using user_action_info_t    = gbp::ns_api::ns_table::user_action_info_t;
    using e_user_action         = gbp::ns_api::ns_table::e_user_action;
    using table_player_info_t   = gbp::ns_api::ns_table::table_player_info_t;
    using user_state_t          = gbp::ns_api::ns_table::user_state_t;
    //_left_table, _sit_out, _frozen, _anticipant, _folded, _all_ined, _all_ined_in_cur_round
    using winner_info_t         = gbp::ns_api::ns_table::winner_info_t;
    using table_state_t         = gbp::ns_api::ns_table::table_state_t;
    using recharge_money_t      = gbp::ns_api::ns_table::recharge_money_t;
    using e_game_type           = gbp::ns_api::ns_game::e_game_type;
    using e_game_limit          = gbp::ns_api::ns_game::e_game_limit;
}

//GameHandler::GameHandler(const GameId &gameId, QObject *parent)
//    : QObject(parent)
//    , GameDataHandlerInterface(gameId)
//    , m_time(nullptr)
//    , m_table(nullptr)
//    , m_tablePlayers(new QActionGroup(this))
//    , m_timeline(new TimelineHandler(this))
//    , m_player(nullptr)
//    , m_glow(new FramedEntity)
//{}

//GameHandler::~GameHandler()
//{

//}

//void GameHandler::initialize()
//{
//    m_glow->setPixmap(QPixmap(":/textures/profiler/glow.png"));
//    m_glow->setSizeOfFrame(88, 88);
//    m_glow->setFixedSize(88, 88);

//    connect(timeline(), &TimelineHandler::timeout, this, &GameHandler::timelineEvent);
//    connect(timeline(), &TimelineHandler::started, this, [this](gbp_i64 playerID){
//        setCurrentPlayer(playerID, timeline()->durationFull(), 0);
//    });
//    connect(timeline(), &TimelineHandler::finished, this, [this](gbp_i64 playerID){
//        if (TablePlayer* pl = findPlayer(playerID)) {
//            pl->makeCurrent(false);
//        }
//    });
//}

////GameTable *GameHandler::getTable() const {
////    return m_table.data();
////}

//TableOver *GameHandler::getLayers() const
//{
//    return m_table.isNull() ? nullptr : m_table->layers();
//}

////Player *GameHandler::getPlayer() const
////{
////    return m_player;
////}

//GameTable* GameHandler::createTable(QWidget* parent) const
//{
////    GameTable* table = new GameTable(id(), nullptr);
////    table->moveToThread(parent->thread());

////    table->setEditorVisible(false);

////    gbp::ns_api::ns_statistic::cash_stat_t* stat = ns_helpers::findCashItem(tableId());
////    std::set<gbp_i8> freeSeats = ns_helpers::cashFreeSeats(tableId());
////    std::vector<bool> seats;
////    seats.assign(stat->_players_count.second, false);
////    for (auto it = freeSeats.begin(); it != freeSeats.end(); it++) {
////        seats[*it] = true;
////    }
////    ns_descriptor::GameTable desc(id(), stat->_title.c_str(), ns_helpers::tableTitle(tableId()), seats, stat->_limit, stat->_type);
////    table->initWithDescriptor(&desc);

////    connect(table, &GameTable::finished, this, &GameHandler::closeTable);

////    Logger::info(QString("Just created table %0 %1, addresses %2 %3").arg(table->id().toString()).arg(id().toString()).arg(quintptr(&table->id())).arg(quintptr(&id())), __FILELINE__);

////    return table;
//}

//int GameHandler::standardTime(ETime timeType) const
//{
//    if (m_time == nullptr) {
//        return -1;
//    }
//    switch (timeType) {
//    case ETime::Short:  return m_time->_short;
//    case ETime::Main:   return m_time->_main;
//    case ETime::Common: return m_time->_short + m_time->_main;
//    default:
//        return -1;
//    }
//}

//void GameHandler::setStandardTime(int shortTime, int mainTime)
//{
//    if (m_time != nullptr) {
//        if (m_time->_short != shortTime || m_time->_main != mainTime) {
//            QString warningMsg = QString("Standard time has been overriden from %0 to %1").arg(m_time->toString());
//            delete m_time;
//            m_time = new StandardTime(shortTime, mainTime);
//            Logger::warning(warningMsg.arg(m_time->toString()), __FILELINE__);
//        }
//    } else {
//        m_time = new StandardTime(shortTime, mainTime);
//    }
//}

//void GameHandler::openGame(bool useDirtyHack)
//{
////    if (m_table.isNull()) {
////        m_table.reset(createTable(pApp->mainWindow()));
////    }
////    if (m_player == nullptr) {
//////        createPlayer(pApp->mainWindow());
////        m_player = m_table->findChild<Player*>();
////        Q_ASSERT(m_player);
////        m_player->init();
////        m_table->open();
////        // Грязный хак
////        if (useDirtyHack) { QDialog(getTable()).open(); }
////    }
////    else
////    {
////        m_table->open();
////    }

////    emit tableOpened(id());
//}

//bool GameHandler::isOpened() const
//{
//    return !m_table.isNull();
//}

//void GameHandler::clearTableCards() {
////    getLayers()->clearTableCards();
//}

//void GameHandler::makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info) {
//////    setCurrentPlayer(getPlayer()->playerId(), action_info._time_action_short, action_info._time_action_main, action_info._time_action_additional);
////    if (nullptr == getPlayer()) {
////        qCritical() << QString("player is NULL, %0 %1:%2").arg(__FUNCTION__).arg(__FILE__).arg(__LINE__);
////        Logger::error(QString("player is NULL, %0").arg(__FUNCTION__), __FILELINE__);
////        return;
////    }
////    if (auto pl = findPlayer(getPlayer()->playerId())) {
////        pl->makeCurrent(true);
////    } else {
////        qCritical() << QString("player is not found, %0 %1:%2").arg(__FUNCTION__).arg(__FILE__).arg(__LINE__);
////        Logger::error(QString("player is not found, %0").arg(__FUNCTION__), __FILELINE__);
////        return;
////    }
////    getPlayer()->setCurrentActivePlayer(actions, action_info, pot(0));
//}

//TablePlayer* GameHandler::findPlayer(gbp_i64 playerID) const
//{
//    for (QAction* player: m_tablePlayers->actions()) {
//        TablePlayer* tPlayer = qobject_cast<TablePlayer*>(player);
//        Q_ASSERT(tPlayer);
//        if (tPlayer->playerID() == playerID) {
//            return tPlayer;
//        }
//    }
//    return nullptr;
//}

//gbp_i8 GameHandler::playerPos(gbp_i64 playerID) const
//{
//    if (TablePlayer* pl = findPlayer(playerID)) {
//        return pl->seatIndex();
//    }
//    return -1;
//}

//TablePlayer* GameHandler::playerAt(gbp_i8 pos) const
//{
//    for (QAction* player: m_tablePlayers->actions()) {
//        TablePlayer* tPlayer = qobject_cast<TablePlayer*>(player);
//        Q_ASSERT(tPlayer);
//        if (tPlayer->seatIndex() == pos) {
//            return tPlayer;
//        }
//    }
//    return nullptr;
//}

//gbp_i64 GameHandler::playerIdAt(gbp_i8 pos) const
//{
//    if (TablePlayer* pl = playerAt(pos)) {
//        return pl->playerID();
//    }
//    return -1;
//}

//void GameHandler::addPlayer(gbp_i64 playerID)
//{
////    if (TablePlayer* pl = findPlayer(playerID))
////    {
////        if (m_tablePlayers->actions().contains(pl)) {
////            Logger::error(QString("GameHandler::addPlayer(id:%0, nickname:%1), already exists").arg(pl->id().toString()).arg(pl->text()), __FILE__, __LINE__);
////            return;
////        }
////        m_tablePlayers->addAction(pl);
////        if (getPlayer()->playerId() == pl->playerID()) {
////            getPlayer()->setTablePlayer(pl);
////        }
////        if (pl->seatIndex() != -1) {
////            onFreeSeatsCountChanged();
////        }
////    }
//}

//void GameHandler::removePlayer(gbp_i64 playerID)
//{
////    TablePlayer* pl = findPlayer(playerID);
//////    qDebug() << "pl->seatIndex()" << pl->seatIndex();
////    m_tablePlayers->removeAction(pl);
////    if (playerID != getPlayer()->playerId()) {
////        for (QWidget* w: pl->associatedWidgets()) {
////            w->removeAction(pl);
////        }
////    }
//////    qDebug() << "pl->seatIndex()" << pl->seatIndex();

////    if (pl == nullptr || pl->seatIndex() != -1) {
////        onFreeSeatsCountChanged();
////    }
//}

//void GameHandler::updatePlayer(const table_player_info_t &info)
//{
////    TablePlayer* pl = findPlayer(info._id);
////    if (pl == nullptr)
////    {

//////        Q_ASSERT_X(pl, "updatePlayer", os.str().c_str());
////    }

////    setPlayerCash(info._id, info._money);
////    setPlayerPos(info._id, info._pos);
////    // info._start_stack
////    if (info._is_dealer) {
////        getLayers()->setDealerPos(info._pos);
//////        setDealer(info._id);
////    }
////    setPlayerCards(info._id, info._cards);
////    setPlayerRoundBet(info._id, info._round_bet);
////    if (info._state._sit_out || info._state._anticipant || info._state._left_table) {
////        pl->sitOut();
////    } else if (info._state._frozen) {
////        pl->freeze();
////    } else if (info._state._folded) {
////    } else {

////    }
////    pl->fold(info._state._folded);
//////    if (info._is_current) {
//////        setCurrentPlayer(info._id);
//////    }
//}

//QString GameHandler::playerNickname(gbp_i64 playerID) const
//{
////    QAction* pl = findPlayer(playerID);
////    if (pl) {
////        return pl->text();
////    }
////    return "NICKNAME IS NOT FOUND";
//}

//QActionGroup *GameHandler::playersGroup() const
//{
//    return m_tablePlayers.data();
//}

//TablePlayerList GameHandler::tablePlayers() const
//{
//    TablePlayerList players;
//    for (QAction* player: m_tablePlayers->actions()) {
//        if (TablePlayer* tablePlayer = qobject_cast<TablePlayer*>(player)) {
//            players.push_back(tablePlayer);
//        }
//    }
//    return players;
//}



GameViewHandler::GameViewHandler(const GameId &id, QObject *parent)
    : QObject(parent)
    , GameViewHandlerInterface(id)
    , m_model(nullptr)
{}

GameViewHandler::~GameViewHandler()
{
    disconnectFromModel();
}

void GameViewHandler::initialize()
{
    _impl_connectToModel();

    if (property("indexWidgetConnections").isValid()) {
        QMap<QByteArray, QWidget*> indexWidgets = qvariant_cast<QMap<QByteArray, QWidget*>>(property("indexWidgetConnections"));

        for (const QByteArray& indexKey: indexWidgets.keys()) {
            setIndexWidget(indexWidgets.value(indexKey), indexKey);
        }
        setProperty("indexWidgetConnections", QVariant());
    }
}

bool GameViewHandler::indexAffectsView(const QModelIndex &index) const
{
    return index.isValid() && index.model() == modelsDB().getTableState(id());
}

void GameViewHandler::setIndexWidget(QWidget *widget, const QModelIndex &index) {
    if (m_model == nullptr) {
        QVariant idxWgtConn = property("indexWidgetConnections");
        QMap<QByteArray, QWidget*> indexWidgets;
        if (idxWgtConn.isValid()) {
            indexWidgets = qvariant_cast<QMap<QByteArray, QWidget*>>(idxWgtConn);
        }
        indexWidgets.insert(ns_model::ApiBase::anyIndexToKey(index), widget);
        setProperty("indexWidgetConnections", QVariant::fromValue(indexWidgets));
        return;
    }
    setIndexWidget(widget, m_model->indexToKey(index));
}

void GameViewHandler::setIndexWidget(QWidget *widget, const QByteArray &key)
{
    if (QWidget* oldWidget = indexWidget(key)) {
        disconnect(oldWidget);
        m_mappings.remove(key);
    }

    m_mappings.insert(key, widget);
    if (widget != nullptr)
    {
        connect(widget, &QWidget::destroyed, this, [this, key] {
            disconnect(sender());
            m_mappings.remove(key);
        });
    }
}

QWidget* GameViewHandler::indexWidget(const QModelIndex &index) const {
    return indexWidget(m_model->indexToKey(index));
}

QWidget *GameViewHandler::indexWidget(const QByteArray &key) const {
    return m_mappings.value(key);
}

void GameViewHandler::_onModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles)
{
    if (roles.isEmpty() || roles.contains(UserType::OriginDataRole) || roles.contains(UserType::RowDataRole)) {
        QModelIndexRange range(topLeft, bottomRight);
        onModelDataChanged(range);
    }
}
void GameViewHandler::_onModelRowsAboutToBeInserted(const QModelIndex& parent, int first, int last)
{
    setProperty("insert_row_first", first);
    setProperty("insert_row_last", last);
    onModelAboutToInsertItems(parent);
}
void GameViewHandler::_onModelRowsInserted(const QModelIndex& parent, int first, int last)
{
    Q_ASSERT(property("insert_row_first").toInt() == first && property("insert_row_last").toInt() == last);
    onModelInsertItems(QModelIndexRangeList() << QModelIndexRange(m_model->index(first, 0, parent), m_model->index(last, m_model->columnCount(parent)-1, parent)));
}
void GameViewHandler::_onModelRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last)
{
    setProperty("removed_row_first", first);
    setProperty("removed_row_last", last);
    onModelAboutToRemoveItems(QModelIndexRangeList() << QModelIndexRange(m_model->index(first, 0, parent), m_model->index(last, m_model->columnCount(parent)-1, parent)));
}
void GameViewHandler::_onModelRowsRemoved(const QModelIndex& parent, int first, int last)
{
    Q_ASSERT(property("removed_row_first").toInt() == first && property("removed_row_last").toInt() == last);
    onModelRemoveItems(parent);
}

void GameViewHandler::connectToModel() {
    _impl_connectToModel();
}
void GameViewHandler::disconnectFromModel() {
    _impl_disconnectFromModel();
}

void GameViewHandler::_impl_connectToModel()
{
    if (!m_model)
    {
        m_model = modelsDB().getTableState(id());
        connect(m_model, &ns_model::table_state_t::modelAboutToBeReset, this, &GameViewHandler::onModelAboutToBeReset);
        connect(m_model, &ns_model::table_state_t::modelReset, this, &GameViewHandler::onModelReset);
        connect(m_model, &ns_model::table_state_t::dataChanged, this, &GameViewHandler::_onModelDataChanged);
        connect(m_model, &ns_model::table_state_t::rowsAboutToBeInserted, this, &GameViewHandler::_onModelRowsAboutToBeInserted);
        connect(m_model, &ns_model::table_state_t::rowsInserted, this, &GameViewHandler::_onModelRowsInserted);
        connect(m_model, &ns_model::table_state_t::rowsAboutToBeRemoved, this, &GameViewHandler::_onModelRowsAboutToBeRemoved);
        connect(m_model, &ns_model::table_state_t::rowsRemoved, this, &GameViewHandler::_onModelRowsRemoved);
    }
}
void GameViewHandler::_impl_disconnectFromModel()
{
    if (m_model)
    {
        disconnect(m_model, &ns_model::table_state_t::modelAboutToBeReset, this, &GameViewHandler::onModelAboutToBeReset);
        disconnect(m_model, &ns_model::table_state_t::modelReset, this, &GameViewHandler::onModelReset);
        disconnect(m_model, &ns_model::table_state_t::dataChanged, this, &GameViewHandler::_onModelDataChanged);
        disconnect(m_model, &ns_model::table_state_t::rowsAboutToBeInserted, this, &GameViewHandler::_onModelRowsAboutToBeInserted);
        disconnect(m_model, &ns_model::table_state_t::rowsInserted, this, &GameViewHandler::_onModelRowsInserted);
        disconnect(m_model, &ns_model::table_state_t::rowsAboutToBeRemoved, this, &GameViewHandler::_onModelRowsAboutToBeRemoved);
        disconnect(m_model, &ns_model::table_state_t::rowsRemoved, this, &GameViewHandler::_onModelRowsRemoved);
        m_model = nullptr;
    }
}


//GameHandlerInterface::GameHandlerInterface(const GameId &id): GameIdObject(id) {}
//GameHandlerInterface::~GameHandlerInterface() {}
//GameDataHandlerInterface::GameDataHandlerInterface(const GameId &id): GameHandlerInterface(id) {}
//GameDataHandlerInterface::~GameDataHandlerInterface() {}
//GameViewHandlerInterface::GameViewHandlerInterface(const GameId &id): GameHandlerInterface(id) {}
//GameViewHandlerInterface::~GameViewHandlerInterface() {}
