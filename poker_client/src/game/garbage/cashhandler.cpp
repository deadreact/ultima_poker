#include <precompiled.h>
#include "cashhandler.h"

#include <QAction>
#include <dialogs/buyin/dialog_CashBuyIn.h>
#include <dialogs/dialog_gametable.h>
#include <PokerApplication.h>
#include <db/Database.h>
#include <db/SharedDataService.h>
#include <utils/logger.h>
#include "TimelineHandler.h"
#include <db/modeldatabase.h>
#include <api_models/model_table_state_t.hpp>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/table/request.h>
    #include <rpc/table/server_reply.h>
    #include <rpc/request_lobby.h>
    #include <rpc/reply_statistic.h>
#endif //!Q_MOC_RUN
#include <widgets/widget_ProfilePicture.h>

#include <iostream>
#include <helpers/helper_statistic.h>
#include <descriptors/descriptor_GameTable.h>
#include <algorithm>
#include <qtextbrowser.h>
#include "userbox.h"
#include "tableItems/potitem.h"
#include <utils/helper_debug.h>
#include <helpers/helpers_convert.h>
#include <basewidgets/util.h>
#include <basewidgets/framedentity.h>

#include <api_models/api_models.hpp>
#include <api_models/algorithm.hpp>
#include <boost/type_traits.hpp>
#include <widgets/CardWidget.h>

template <typename Type, bool is_defined = !std::is_abstract<Type>::value>
struct FnClass
{
    template <typename Cls>
    static void call_if_defined(const Cls* /*obj*/) {
//        obj->
    }
};
template <typename Type>
struct FnClass<Type, false>
{
    template <typename Cls, typename ...Args>
    static void call_if_defined(std::function<void(Cls::*)(const Args&...)> /*foo*/, const Args&... /*args*/) {}
};


struct CashHandlerData
{
private:
    bool m_gameIsStarted      = false;
    bool m_gameIsFinished     = false;
public:
    void setStarted() {
        m_gameIsStarted = true;
        m_distribution = 0;
        m_tradeRound = 0;
    }
    void setFinished() {
        m_gameIsFinished = true;
    }
    bool isStarted() const { return m_gameIsStarted; }
    bool isFinished() const { return m_gameIsFinished; }

    gbp_i64 m_distribution    = -1;
    gbp_i8 m_currentPlayerPos = -1;
    gbp_i8 m_tradeRound       = 0;

    bool m_chatEnabledForActions = true;
};


// -------------- CASH HANDLER ------------------
CashHandler::CashHandler(gbp_i64 gameId, QObject *parent)
    : SuperClass(gameId, parent)
    , m_dPtr(new CashHandlerData)
{}

CashHandler::~CashHandler()
{

}

void CashHandler::updatePlayers()
{
    

//    getLayers()->updateUserboxes();
}

void CashHandler::update(const table_state_t &tableState)
{
////    call_if_defined<table_state_t, FnClass<decltype(&SuperClass::updatePlayers)>::&FnClass(this, &SuperClass, &FnClass, decltype(&SuperClass::update)>::call_if_defined(this, &SuperClass::update, (&FnClass<decltype(&SuperClass::update)>::call_if_defined(this, &SuperClass::update,  tableState);
////   FnClass<decltype(&SuperClass::update)>::call_if_defined(this, &SuperClass::update, tableState);


//    static const char* errTmp = "Wrong game id, got %0, expected %1";
//    GameId gameID(tableState._game_id, tableState._tournament_id);

//    DEBUG.assertX(id() == gameID, __FILELINE__) << QString(errTmp).arg(gameID.toString()).arg(id().toString());
////    tableState._big_blind
//    games_stat_t::first_type::value_type* item = ns_helpers::findCashItem(tableState._game_id);


//    setStandardTime(tableState._time_short, tableState._time_main);
//    clearPlayersCards();
//    clearTableCards();
//    for (gbp_i8 card: tableState._table_cards) {
//        addTableCard(card);
//    }
//    clearPots();
//    setPots(tableState._all_pots);
////    tableState._waiters_count

//    if (tableState._players.size() > 1 && tableState._distribution_number >= 0)
//    {
//        if (!m_dPtr->isStarted()) {
//            startGame();
//            startDistribution(tableState._distribution_number);
//            startTradeRound(std::max(gbp_i8(0), gbp_i8(tableState._table_cards.size() - 2)));
//        }
//    }
//    if (!tableState._players.empty())
//    {
//        for (const table_player_info_t& pl: tableState._players) {
//            updatePlayer(pl);
//            if (pl._is_current) {
//                setCurrentPlayer(pl._id, tableState._time_action_left, tableState._time_action_passed);
//            }
//        }
//        m_dPtr->m_currentPlayerPos = tableState._current_user_pos;
//    }
//    else
//    {
//        m_dPtr->m_currentPlayerPos = -1;
//    }
//    // TODO: исправить, данные устанавливаются минуя модель, -> никто не в курсе что данные изменились
//    item->_players_count.first = tableState._players.size();
//    if (item->_players.size() != tableState._players.size()) {
//        item->_players.resize(0);

//        for (const table_player_info_t& pl: tableState._players) {
//            item->_players.push_back(gbp_cast<gbp::ns_api::ns_statistic::player_t>(pl));
//        }
//    }
//    item->_wait_players = tableState._waiters_count;

//    if (TablePlayer* pl = findPlayer(getPlayer()->playerId()))
//    {
//        pl->setWaitersCount(item->_wait_players);
//    }
//    getPlayer()->setWaitersInfo(tableState._waiters_count, getPlayer()->waiterPos());
////    for (QAction* a: m_tablePlayers->actions()) {
////        TablePlayer* pl = qobject_cast<TablePlayer*>(a);
////        pl->setWaitersCount(tableState._waiters_count);
////    }
//    updatePlayers();
//    getPlayer()->updateGameControls();
}

void CashHandler::clearPots()
{
    // FnClass<decltype(&SuperClass::clearPots)>::call_if_defined(this, &SuperClass::clearPots);
//    getLayers()->setPotsCount(0);
}

void CashHandler::setPots(const std::vector<gbp_i64> &pots)
{
    // FnClass<decltype(&SuperClass::setPots)>::call_if_defined(this, &SuperClass::setPots, pots);
//    getLayers()->setPotsCount(pots.size());
//    for (size_t i = 0; i < pots.size(); i++) {
//        setPot(pots[i], i);
//    }
}

void CashHandler::setPot(gbp_i64 money, gbp_i8 idx)
{
    // FnClass<decltype(&SuperClass::setPot)>::call_if_defined(this, &SuperClass::setPot, money, idx);
//    getLayers()->tablePot(idx)->setMoney(money);
}

void CashHandler::updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet)
{
    // FnClass<decltype(&SuperClass::updateMoneyInfo)>::call_if_defined(this, &SuperClass::updateMoneyInfo, defaultBet, currentBet);
//    if (getPlayer()->seatIndex() != m_dPtr->m_currentPlayerPos) {
//        getPlayer()->setPredefinedInfo(defaultBet, currentBet);
//    }
}

void CashHandler::addTableCard(gbp_i8 card)
{
    // FnClass<decltype(&SuperClass::addTableCard)>::call_if_defined(this, &SuperClass::addTableCard, card);
//    int idx = 0;
//    for (; getLayers()->tableCard(idx)->isVisible(); idx++);
//    setTableCard(card, idx);
}

void CashHandler::setTableCard(gbp_i8 card, gbp_i8 idx)
{
    // FnClass<decltype(&SuperClass::setTableCard)>::call_if_defined(this, &SuperClass::setTableCard, card, idx);
//    Q_ASSERT_X(card >= 0 && card < 52, "setTableCard", QString("Card %0 out of range").arg(card).toLatin1());
//    Q_ASSERT_X(idx < 5, "setTableCard", QString("Card %0 index(%1) out of range").arg(card).arg(idx).toLatin1());

//    getLayers()->tableCard(idx)->setVisible(true);
//    getLayers()->tableCard(idx)->setCardIndex(card);
}

void CashHandler::clearPlayersCards()
{
    // FnClass<decltype(&SuperClass::clearPlayersCards)>::call_if_defined(this, &SuperClass::clearPlayersCards);
//    for (Userbox* usrbox: getLayers()->userboxes()) {
//        usrbox->setCardsCount(0);
//    }
//    updatePlayers();
}

void CashHandler::addPlayerCard(gbp_i64 playerID, gbp_i8 card)
{
    // FnClass<decltype(&SuperClass::addPlayerCard)>::call_if_defined(this, &SuperClass::addPlayerCard, playerID, card);
//    if (Userbox* usrbox = getLayers()->userboxByPlayerId(playerID)) {
//        int idx = usrbox->cardsCount();
//        usrbox->setCardsCount(idx + 1);
//        usrbox->setCard(idx, card);
//    }
//    else
//    {
//        Logger::error(QString("Can not set players cards, player(id:%0) doesn't sit on table(id:%1)").arg(playerID).arg(id().toString()), __FILE__, __LINE__);
//    }
//    updatePlayers();
}

void CashHandler::setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards)
{
    // FnClass<decltype(&SuperClass::setPlayerCards)>::call_if_defined(this, &SuperClass::setPlayerCards, playerID, cards);
//    if (Userbox* usrbox = getLayers()->userboxByPlayerId(playerID)) {
//        usrbox->setCardsCount(cards.size());
//        for (size_t i = 0; i < cards.size(); i++) {
//            usrbox->setCard(i, cards[i]);
//        }
//    }
//    else
//    {
//        Logger::error(QString("Can not set players cards, player(id:%0) doesn't sit on table(id:%1)").arg(playerID).arg(id().toString()), __FILE__, __LINE__);
//    }
//    updatePlayers();
}

void CashHandler::setDealerCard(gbp_i64 playerID, gbp_i8 card)
{
    // FnClass<decltype(&SuperClass::setDealerCard)>::call_if_defined(this, &SuperClass::setDealerCard, playerID, card);
//    if (Userbox* usrbox = getLayers()->userboxByPlayerId(playerID))
//    {
//        usrbox->setCardsCount(1);
//        usrbox->setCard(0, card);
//    }
//    else
//    {
//        Logger::error(QString("Can not set players dealer card, player(id:%0) doesn't sit on table(id:%1)").arg(playerID).arg(id().toString()), __FILE__, __LINE__);
//    }
}

void CashHandler::setPlayerPos(gbp_i64 playerID, gbp_i8 pos)
{
    // FnClass<decltype(&SuperClass::setPlayerPos)>::call_if_defined(this, &SuperClass::setPlayerPos, playerID, pos);
//    static const char* errTmp = "Can not sit player(id:%0, nickname:%1) on invalid sit(%2)";
//    Q_ASSERT_X(pos >= 0 && pos < 9, "setPlayerPos", QString(errTmp).arg(playerID).arg(playerNickname(playerID)).arg(pos).toLatin1());
//    Userbox* usrbox = getLayers()->userbox(pos);
//    TablePlayer* pl = findPlayer(playerID);
//    gbp_i8 prevPos = pl->seatIndex();
//    pl->setSeatIndex(usrbox->seatIndex());
//    usrbox->setPlayer(pl);
//    updatePlayers();

//    if ((prevPos == -1 && pos != -1) || (prevPos != -1 && pos == -1)) {
//        onFreeSeatsCountChanged();
//    }
}

void CashHandler::setPlayerAction(gbp_i64 playerID, const GameAction &action)
{
//    // FnClass<decltype(&SuperClass::setPlayerAction)>::call_if_defined(this, &SuperClass::setPlayerAction, playerID, action);
//    TablePlayer* pl = findPlayer(playerID);
//    std::ostringstream os;
//    if (!pl->isCurrent() && (action.first != e_user_action::guest_bet))
//    {
//        os << action << ", current user pos is " << m_dPtr->m_currentPlayerPos << std::endl;
//        Logger::info(os.str().c_str(), __FILELINE__);
//    }
////    Q_ASSERT_X(pl->isCurrent() || (action.first == e_user_action::guest_bet), "", os.str().c_str());

//    pl->course(action);
//    if (Userbox* usrbox = getLayers()->userboxByPlayerId(playerID))
//    {
//        usrbox->displayAction(action);
//        chatPlayerAction(playerID, ns_util::toString(action.first), action.second);
//    }
//    else
//    {
//        QString errMsg = QString("Player(id: %0, nickname: %1) not found, but server requested user action(%2 %3)")
//                        .arg(playerID)
//                        .arg(playerNickname(playerID))
//                        .arg(ns_util::toString(action.first))
//                        .arg(action.second > 0 ? QString::number(action.second) : "");
//        Logger::error(errMsg, __FILE__, __LINE__);
//    }
}

void CashHandler::setPlayerCash(gbp_i64 playerID, gbp_i64 money)
{
//    // FnClass<decltype(&SuperClass::setPlayerCash)>::call_if_defined(this, &SuperClass::setPlayerCash, playerID, money);
//    TablePlayer* pl = findPlayer(playerID);
//    Q_ASSERT(pl);
//    pl->setGameMoney(money);
}

void CashHandler::setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money)
{
//    // FnClass<decltype(&SuperClass::setPlayerRoundBet)>::call_if_defined(this, &SuperClass::setPlayerRoundBet, playerID, money);
//    TablePlayer* pl = findPlayer(playerID);
//    Q_ASSERT(pl);
//    pl->setRoundBet(money);
////    gbp_i8 pos = getLayers()->playerPos(playerID);
////    if (BetItem* betItem = getLayers()->betItem(pos)) {
////        betItem->setMoney(money);
////    }
////    else
////    {
////        Logger::error(QString("Can not set players round bet, player(id:%0) doesn't sit on table(id:%1)").arg(playerID).arg(id().toString()), __FILE__, __LINE__);
////    }
}

void CashHandler::setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money)
{
    // FnClass<decltype(&SuperClass::setPlayerDistributionBet)>::call_if_defined(this, &SuperClass::setPlayerDistributionBet, playerID, money);
//    TablePlayer* pl = findPlayer(playerID);
//    DEBUG.assertX(pl, __FILELINE__) << playerID << Money(money).toString();
//    pl->setDistributionBet(money);
}

void CashHandler::askBlind(gbp_i64 blind, bool isBB) const
{
    // FnClass<decltype(&SuperClass::askBlind)>::call_if_defined(this, &SuperClass::askBlind, blind, isBB);
//    bool res = getPlayer()->askBlind(blind, isBB);

//    ns_server_reply::table_cash* serv_reply = pApp->client().server_reply<ns_server_reply::table_cash>();
//    serv_reply->user_blind_done(tableId(), res);

//    findPlayer(getPlayer()->playerId())->setInGame(res);
}

void CashHandler::setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed)
{
    // FnClass<decltype(&SuperClass::setCurrentPlayer)>::call_if_defined(this, &SuperClass::setCurrentPlayer, playerID, timeLeft, timePassed);
//    gbp_i8 pos = getLayers()->playerPos(playerID);
//    if (m_dPtr->m_currentPlayerPos == pos) {
//        return;
//    }
//    if (standardTime() == -1) {
//        return;
//    }

//    gbp_i64 tmShort = std::max<gbp_i64>(0, standardTime(ETime::Short) - timePassed);
//    gbp_i64 tmMain = std::max<gbp_i64>(0, standardTime(ETime::Main) - timePassed);
//    gbp_i64 tmAdditional = timeLeft - tmShort - tmMain;

//    m_timeline->reset(playerID, tmShort, tmMain, tmAdditional);

//    if (m_dPtr->m_currentPlayerPos != -1) {
//        if (TablePlayer* pl = findPlayer(getLayers()->userbox(m_dPtr->m_currentPlayerPos)->playerId())) {
//            pl->makeCurrent(false);
//        }
//    }

//    m_dPtr->m_currentPlayerPos = pos;

//    if (playerID != -1) {
//        findPlayer(playerID)->makeCurrent(true);
//        m_timeline->restart(30, 100, 140);
//    }

//    updatePlayers();
}

void CashHandler::setDealer(gbp_i64 playerID)
{
//    // FnClass<decltype(&SuperClass::setDealer)>::call_if_defined(this, &SuperClass::setDealer, playerID);
//    gbp_i8 seat = getLayers()->playerPos(playerID);
//    getLayers()->setDealerPos(seat);
//    getPlayer()->setDealerIndex(seat);
}

void CashHandler::startGame()
{
    // FnClass<decltype(&SuperClass::startGame)>::call_if_defined(this, &SuperClass::startGame);
//    m_dPtr->setStarted();

//    for (QAction* a: m_tablePlayers->actions()) {
//        if (TablePlayer* pl = qobject_cast<TablePlayer*>(a)) {
//            pl->setRoundBet(0);
//        }
//    }

//    updatePlayers();
}

void CashHandler::endGame()
{
    // FnClass<decltype(&SuperClass::endGame)>::call_if_defined(this, &SuperClass::endGame);
//    m_dPtr->setFinished();
//    m_table->displayEndOfGame();
//    updatePlayers();
}

void CashHandler::startDistribution(gbp_i64 n)
{
    // FnClass<decltype(&SuperClass::startDistribution)>::call_if_defined(this, &SuperClass::startDistribution, n);
//    if (m_dPtr->m_distribution != n)
//    {
//        m_dPtr->m_distribution = n;
//        m_player->distributionNumber()->setText(tr("@distribution_%0").arg(n));

//        for (QAction* a: m_tablePlayers->actions()) {
//            a->setCheckable(true);
//        }

//        updatePlayers();
//    }
}

void CashHandler::endDistribution()
{
    // FnClass<decltype(&SuperClass::endDistribution)>::call_if_defined(this, &SuperClass::endDistribution);
//    m_table->displayEndOfDistribution();
//    getLayers()->clearBetItems();
//    getLayers()->clearPots();
//    updatePlayers();
}

void CashHandler::startTradeRound(gbp_i8 n)
{
    // FnClass<decltype(&SuperClass::startTradeRound)>::call_if_defined(this, &SuperClass::startTradeRound, n);
//    if (m_dPtr->m_tradeRound != n) {
//        m_dPtr->m_tradeRound = n;
//        for (QAction* a: m_tablePlayers->actions()) {
//            if (TablePlayer* pl = qobject_cast<TablePlayer*>(a)) {
//                pl->setRoundIndex(n);
//            }
//        }

//        updatePlayers();
//    }
}

void CashHandler::endTradeRound(gbp_i8 n)
{
    // FnClass<decltype(&SuperClass::endTradeRound)>::call_if_defined(this, &SuperClass::endTradeRound, n);
//    if (n == -1) {
//        n = m_dPtr->m_tradeRound;
//    } else if (n != m_dPtr->m_tradeRound) {
//        Logger::warning(QString("End of trade round #%0, but current trade round is #%1").arg(n).arg(m_dPtr->m_tradeRound), __FILE__, __LINE__);
//    }

//    m_table->displayEndOfRound(n);
//    for (QAction* a: m_tablePlayers->actions()) {
//        qobject_cast<TablePlayer*>(a)->setRoundBet(0);
//    }
//    getLayers()->clearBetItems();
//    updatePlayers();
}

void CashHandler::setWinners( const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners
                            , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards, bool by_combination)
{
    // FnClass<decltype(&SuperClass::setWinners)>::call_if_defined(this, &SuperClass::setWinners, winners, users_cards, by_combination);
//    const std::vector<winner_info_t>& hi_winners = winners.first;
//    const std::vector<winner_info_t>& low_winners = winners.second;
//    if (by_combination) {
//        for (const std::pair<gbp_i64, std::vector<gbp_i8>> cards: users_cards) {
//            setPlayerCards(cards.first, cards.second);
//        }
//    }
//    for (const winner_info_t& hi_winner : hi_winners) {
//        setWinner(hi_winner, by_combination);
////        currentPlayerIsWinner = currentPlayerIsWinner || (hi_winner._user_id == m_player->playerId());
//    }
//    for (const winner_info_t& low_winner : low_winners) {
//        setWinner(low_winner, by_combination);
////        currentPlayerIsWinner = currentPlayerIsWinner || (low_winner._user_id == m_player->playerId());
//    }
//    updatePlayers();
}

void CashHandler::setWinner(const winner_info_t& winner, bool showCombination)
{
//    gbp_i8 playerPos = getLayers()->playerPos(winner._user_id);
//    static const char* errTmp = "No player(id:%0, nickname:%1) sits on table(id:%2)";
//    Q_ASSERT_X(playerPos != -1, "setWinner", QString(errTmp).arg(winner._user_id).arg(playerNickname(winner._user_id).arg(id().toString())).toLatin1());
//    if (playerPos != -1)
//    {
//        m_table->displayWinningAt(playerPos
//                                , winner._new_cash
//                                , winner._winner_cash
//                                , winner._main_pot ? 0 : winner._pot_number
//                                , winner._combination._combination
//                                , winner._combination._cards
//                                , showCombination);
//    }
//    GBP_DECLARE_ENUM(
//     e_cards_combination
//     , std::gbp_u8
//     , inline
//     , (hight_card)
//       (pair)
//       (doper)
//       (set)
//       // когда в стрите туз - наименьшая карта
//       (wheel)
//       (straight)
//       // флеш-стрит с наименьшим тузом
//       (steel_wheel)
//       (flush)
//       (full_house)
//       (four)
//       (straight_flush)
//       (royal_flush)
    //    )
}

void CashHandler::chatPlayerAction(gbp_i64 playerID, const QString &actionName, Money actionMoney)
{
//    if (m_dPtr->m_chatEnabledForActions)
//    {
//        QTextBrowser* chatBrowser = getLayers()->chatBrowser();
//        QString msg = QString("Player %0 action: %1 (%2)%3").arg(playerNickname(playerID)).arg(actionName).arg(actionMoney.toString()).arg("</body></html>");
//        QString html = chatBrowser->toHtml().replace("</body></html>", msg);
//        chatBrowser->setHtml(html);
//        chatBrowser->moveCursor(QTextCursor::End);
//    }
}

void CashHandler::setChatMessage(gbp_i64 playerID, const QString &msg)
{
    // FnClass<decltype(&SuperClass::setChatMessage)>::call_if_defined(this, &SuperClass::setChatMessage, playerID, msg);
//    getTable()->chatPlayersMsg(playerNickname(playerID), msg);
}

void CashHandler::initialize()
{
//    GameHandler::initialize();
//    // FnClass<decltype(&SuperClass::initialize)>::call_if_defined(this, &SuperClass::initialize);

//    connect(timeline(), &TimelineHandler::started, this, [this](gbp_i64 playerID){
//        getLayers()->userboxByPlayerId(playerID)->setGlowWidget(m_glow);
//        m_glow = nullptr;
//    });
//    connect(timeline(), &TimelineHandler::finished, this, [this](gbp_i64 playerID){
//        m_glow = qobject_cast<FramedEntity*>(getLayers()->userboxByPlayerId(playerID)->takeGlowWidget());
//    });
}

void CashHandler::timelineEvent(const TimeLineEvent& ev)
{
//   FnClass<decltype(&SuperClass::timelineEvent)>::call_if_defined(this, &SuperClass::timelineEvent, ev);
//    QLabel* label_pl = getLayers()->findChild<QLabel*>(QString("label_player"));
//    QLabel* label_s  = getLayers()->findChild<QLabel*>(QString("label_s"));
//    QLabel* label_m  = getLayers()->findChild<QLabel*>(QString("label_m"));
//    QLabel* label_a  = getLayers()->findChild<QLabel*>(QString("label_a"));

//    label_pl->setText(playerNickname(ev.playerID));

//    label_s->setText(QString::number(ev.handler->durationShort()/1000));
//    label_m->setText(QString::number(ev.handler->durationMain() /1000));
//    label_a->setText(QString::number(ev.handler->durationFull() /1000));

//    label_s->setEnabled(ev.type == TimeLineEvent::s);
//    label_m->setEnabled(ev.type == TimeLineEvent::m);
//    label_a->setEnabled(ev.type == TimeLineEvent::a);


//    if (Userbox* usrbox = getLayers()->userboxByPlayerId(ev.playerID)) {
//        usrbox->timelineEvent(ev);
//    }
//    if (ev.playerID == getPlayer()->playerId()) {
//        getPlayer()->timelineEvent(ev);
//    }
}

void CashHandler::makeBuyIn(const recharge_money_t &money, bool beforeSit, gbp_i64 ratTime)
{
//    ns_descriptor::CashBuyIn desc( tableName()
//                             , QString("%0/%1 %2 %3").arg(Money(bigBlind()/2).toString())
//                                                     .arg(Money(bigBlind()).toString())
//                                                     .arg(ns_util::toString(gameLimit()))
//                                                     .arg(ns_util::toString(gameType()))
//                             , money._money_from
//                             , money._money_to
//                             , money._account_money
//                             , ratTime
//                             , !beforeSit);

//    ns_dialog::CashBuyIn* buyin = new ns_dialog::CashBuyIn(tableId(), getLayers());
//    buyin->setWindowModality(Qt::WindowModal);
//    buyin->initWithDescriptor(&desc);

//    connect(buyin, &ns_dialog::CashBuyIn::accepted, this, [this, buyin] {
////        netClient().userInfoModel()->moveMoneyToIngameMoney(buyin->money());
//    });
//    connect(buyin, &ns_dialog::CashBuyIn::rejected, this, [this]
//    {
//        TablePlayer* pl = findPlayer(m_player->playerId());
//        getLayers()->userbox(m_player->seatIndex())->removeAction(pl);
//        pl->setSeatIndex(-1);

//        updatePlayers();
//    });

//    buyin->open();
}

gbp_i64 CashHandler::pot(int index) const
{
//    return FnClass<decltype(&SuperClass::pot)>::call_if_defined(this, &SuperClass::pot, index);
//    return getLayers()->tablePot(index)->money();
}

gbp_i64 CashHandler::bigBlind() const
{
//    return FnClass<decltype(&SuperClass::bigBlind)>::call_if_defined(this, &SuperClass::bigBlind, );
    return ns_helpers::bigBlind(id());
}

QString CashHandler::tableName() const
{
//    return FnClass<decltype(&SuperClass::tableName)>::call_if_defined(this, &SuperClass::tableName, );
    return ns_helpers::tableName(id().m_tableID);
}

e_game_type CashHandler::gameType() const
{
//    return FnClass<decltype(&SuperClass::gameType)>::call_if_defined(this, &SuperClass::gameType, );
    return ns_helpers::gameType(id());
}

e_game_limit CashHandler::gameLimit() const
{
//    return FnClass<decltype(&SuperClass::gameLimit)>::call_if_defined(this, &SuperClass::gameLimit, );
    return ns_helpers::gameLimit(id().m_tableID);
}

QVariant CashHandler::userState(gbp_i64 playerID) const
{
    if (auto* pl = findPlayer(playerID)) {
        gbp::ns_api::ns_table::user_state_t state;
        state._folded = pl->isFolded();
        state._frozen = pl->isFrozen();
        state._anticipant = pl->isSitOut();
        state._sit_out = state._anticipant && (pl->gameMoney().isZero());
        state._all_ined = pl->isOnline() && pl->isInGame() && !pl->isFolded() && (pl->gameMoney().isZero()) && (pl->distributionBet().isPositive());
        state._all_ined_in_cur_round = state._all_ined && (pl->roundBet().isZero());
        return QVariant::fromValue(state);
    }
    return QVariant();
}

void CashHandler::openTable()
{
    GameHandler::openTable();
}

void CashHandler::closeTable()
{
    GameHandler::closeTable();
}


void CashHandler::onFreeSeatsCountChanged()
{
//    // FnClass<decltype(&SuperClass::onFreeSeatsCountChanged)>::call_if_defined(this, &SuperClass::onFreeSeatsCountChanged);
//    games_stat_t::first_type::value_type* item = ns_helpers::findCashItem(tableId());
//    auto seats = ns_util::range(gbp_i8(0), item->_players_count.second);
//    for (QAction* a: m_tablePlayers->actions()) {
//        if (TablePlayer* pl = qobject_cast<TablePlayer*>(a)) {
////            qDebug() << pl->text() << pl->seatIndex();
//            seats.erase(pl->seatIndex());
//        }
//    }
//    item->_players_count.first = item->_players_count.second - seats.size();
//    getPlayer()->updateGameControls();
}

void CashHandler::onModelReset()
{
    qDebug() << "RESET!!!";
}

void CashHandler::onModelDataChanged(const QModelIndexRange& range)
{
    for (const QModelIndex& index: range) {
        qDebug() << "index changes" << ns_util::traceModelIndexWIthData(index);
    }
}


void CashHandler::onModelAboutToInsertItems(const QModelIndex &parent)
{
    qDebug() << "about to insert items to" << ns_util::traceModelIndexWIthData(parent);
}

void CashHandler::onModelInsertItems(const QModelIndexRangeList &items)
{
    qDebug() << "inserted new items:";
    for (const QModelIndexRange& range: items) {
        qDebug() << "range:" << ns_util::traceModelIndexWIthData(range.first) << "-" << ns_util::traceModelIndexWIthData(range.second);
    }
}

void CashHandler::onModelAboutToRemoveItems(const QModelIndexRangeList &items)
{
    qDebug() << "about to remove items:";
    for (const QModelIndexRange& range: items) {
        qDebug() << "range:" << ns_util::traceModelIndexWIthData(range.first) << "-" << ns_util::traceModelIndexWIthData(range.second);
    }
}

void CashHandler::onModelRemoveItems(const QModelIndex &parent)
{
    qDebug() << "items from" << ns_util::traceModelIndexWIthData(parent) << "have been removed";
}


void CashHandler::setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state)
{
}
