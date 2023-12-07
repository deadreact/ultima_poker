#include <precompiled.h>
#include "tabledataupdater.h"
#include <db/modeldatabase.h>
#include <api_models/model_table_state_t.hpp>
#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/api_table_types.hpp>
#include <api_models/model_log.h>

#include <rpc/rpc_common.h>
#include <table/rpc_table.h>
#include <helpers/helper_enum.h>
#include <api_models/algorithm.hpp>
#include "TimelineHandler.h"
#include <models/proxy/model_commongametable.h>

Q_DECLARE_METATYPE(e_game_type)
Q_DECLARE_METATYPE(e_game_limit)

namespace {
    rpc_table* getRpc() { return rpc_object<rpc_table>(); }
}


struct TableDataUpdater::TableDataUpdaterPrivate
{
    GameId id;
    ns_model::table_state_t* model;
    QScopedPointer<ns_model::CommonGameTable> proxyModel;

    TableDataUpdaterPrivate(const GameId& id)
        : id(id)
        , model(modelsDB().getTableState(id))
        , proxyModel(new ns_model::CommonGameTable)
    {
        proxyModel->setGameID(id);
    }

    void timelineEvent(const TimeLineEvent &ev)
    {
        QModelIndex timeLeftIndex = model->index_time_action_left();// model->indexOf(ns_model::table_state_t::element::_time_action_left);
        QModelIndex timePassedIndex = model->index_time_action_passed();//->indexOf(ns_model::table_state_t::element::_time_action_passed);
        gbp_i64 lastValueLeft = timeLeftIndex.data(UserType::OriginDataRole).toLongLong();
        gbp_i64 lastValuePassed = timePassedIndex.data(UserType::OriginDataRole).toLongLong();
        gbp_i64 diff = lastValueLeft - ev.handler->durationFull()/1000;
        if (diff != 0) {
            model->setOriginData(timeLeftIndex, ev.handler->durationFull()/1000 );
            model->setOriginData(timePassedIndex, lastValuePassed + diff );
        }
    }

    void updatePlayers() {

    }
    void update(const table_state_t &tableState) {
        model->setOriginData(tableState);
    }
    void clearPots() {
        setPots(std::vector<gbp_i64>({0}));
    }
    void setPots(const std::vector<gbp_i64> &pots) {
        model->setOriginData(model->index_all_pots(), pots);
    }
    void setPot(gbp_i64 money, gbp_i8 idx) {
        if (model->potsCount() > idx) {
            model->setOriginData(model->index_pot(idx), money);
        } else if (model->potsCount() == idx) {
            std::vector<gbp_i64> pots = qvariant_cast<std::vector<gbp_i64>>(model->index_all_pots().data(UserType::OriginDataRole));
            pots.push_back(money);
            setPots(pots);
        }
    }
    void updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet) {
        Q_UNUSED(defaultBet)
        Q_UNUSED(currentBet)
    }
    void clearTableCards() {
        model->setOriginData(model->index_table_cards(), std::vector<gbp_i8>());
    }
    void addTableCard(gbp_i8 card) {
        std::vector<gbp_i8> cards = qvariant_cast<std::vector<gbp_i8>>(model->index_table_cards().data(UserType::OriginDataRole));
        cards.push_back(card);
        model->setOriginData(model->index_table_cards(), cards);
    }
    void setTableCard(gbp_i8 card, gbp_i8 idx) {
        QModelIndex cards = model->index_table_cards();
        model->setOriginData(model->index(idx, 0, cards), card);
    }
    void clearPlayersCards() {
        for (int row = 0; row < model->playersCount(); row++) {
            QModelIndex cardsIndex = model->index_player_cards(row);
            model->setOriginData(cardsIndex, std::vector<gbp_i8>());
        }
    }
    void addPlayerCard(gbp_i64 playerID, gbp_i8 card)
    {
        static const ns_model::table_state_t::element_of_players element = ns_model::table_state_t::element_of_players::_cards;
        QModelIndex cardsIndex = model->findElementOfPlayer(playerID, element);

        std::vector<gbp_i8> cards = qvariant_cast<std::vector<gbp_i8>>(cardsIndex.data(UserType::OriginDataRole));
        cards.push_back(card);
        setPlayerCards(playerID, cards);
    }

    void setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards)
    {
        static const ns_model::table_state_t::element_of_players element = ns_model::table_state_t::element_of_players::_cards;
        QModelIndex cardsIndex = model->findElementOfPlayer(playerID, element);
        model->setOriginData(cardsIndex, cards);
    }

    void setDealerCard(gbp_i64 playerID, gbp_i8 card)
    {
        setPlayerCards(playerID, std::vector<gbp_i8>({card}));
    }

    void setPlayerPos(gbp_i64 playerID, gbp_i8 pos)
    {
        static const ns_model::table_state_t::element_of_players element = ns_model::table_state_t::element_of_players::_pos;
        model->setOriginData(model->findElementOfPlayer(playerID, element), pos);
    }

//    (check)
//    (bet)
//    (call)
//    (raise)
//    (all_in)
//    (blind)
//    (guest_bet)

    void setPlayerAction(gbp_i64 playerID, const GameAction &action)
    {
        int playerRow = model->rowById(playerID, &model->origin()._players, model->index_players());
        using e_user_action = gbp::ns_api::ns_table::e_user_action;
        switch (action.first) {
        case e_user_action::fold:
        {
            model->setOriginData(model->index_player_state_folded(playerRow), true);
            setPlayerCards(playerID, std::vector<gbp_i8>());
        } break;
        case e_user_action::bet:
        case e_user_action::call:
        case e_user_action::raise:
        case e_user_action::blind:
        case e_user_action::guest_bet:
        {
            gbp_i64 playerCash     = qvariant_cast<gbp_i64>(model->index_player_money(playerRow).data(UserType::OriginDataRole));
            gbp_i64 playerRoundBet = qvariant_cast<gbp_i64>(model->index_player_round_bet(playerRow).data(UserType::OriginDataRole));
            setPlayerCash(playerID, playerCash - action.second);
            setPlayerRoundBet(playerID, playerRoundBet + action.second);
            Q_FALLTHROUGH();
        }
        case e_user_action::check:
        {
            model->setOriginData(model->index_player_state_anticipant(playerRow), false);
            model->setOriginData(model->index_player_state_frozen(playerRow), false);
        } break;
        case e_user_action::all_in:
        {
            gbp_i64 playerRoundBet = qvariant_cast<gbp_i64>(model->index_player_round_bet(playerRow).data(UserType::OriginDataRole));
            setPlayerCash(playerID, 0);
            setPlayerRoundBet(playerID, playerRoundBet + action.second);
            model->setOriginData(model->index_player_state_anticipant(playerRow), false);
            model->setOriginData(model->index_player_state_frozen(playerRow), false);
            model->setOriginData(model->index_player_state_all_ined(playerRow), false);
            model->setOriginData(model->index_player_state_all_ined_in_cur_round(playerRow), false);
        } break;
        default:
            break;
        }
    }

    void setPlayerCash(gbp_i64 playerID, gbp_i64 money)
    {
        static const ns_model::table_state_t::element_of_players element = ns_model::table_state_t::element_of_players::_money;
        QModelIndex index = model->findElementOfPlayer(playerID, element);
        model->setOriginData(index, money);
    }

    void setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money)
    {
        static const ns_model::table_state_t::element_of_players element = ns_model::table_state_t::element_of_players::_round_bet;
        QModelIndex index = model->findElementOfPlayer(playerID, element);
        model->setOriginData(index, money);
    }

    void setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money)
    {
        Q_UNUSED(playerID)
        Q_UNUSED(money)
    }

    void setDealer(gbp_i64 playerID)
    {
        QModelIndex players = model->index_players();
        int newRow = model->rowById(playerID, &model->origin()._players, players);

        for (int row = 0; row < model->rowCount(players); row++) {
            model->setOriginData(model->index_player_is_dealer(row), row == newRow);
        }
    }

    void setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed)
    {
        QModelIndex players = model->index_players();
        int newRow = model->rowById(playerID, &model->origin()._players, players);

        for (int row = 0; row < model->rowCount(players); row++)
        {
            model->setOriginData(model->index_player_is_current(row), newRow == row);
            if (newRow == row) {
                model->setData(model->index_current_user_pos(), model->index_player_pos(row).data(UserType::OriginDataRole), UserType::OriginDataRole);
            }
        }

        model->setOriginData(model->index_time_action_left(), timeLeft);
        model->setOriginData(model->index_time_action_passed(), timePassed);
    }
    void makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info)
    {
        Q_UNUSED(actions)
        setCurrentPlayer(model->userId(), action_info._time_short + action_info._time_main + action_info._time_additional, 0);
        setPlayerRoundBet(model->userId(), action_info._money_info._user_bet);
        setPlayerCash(model->userId(), action_info._money_info._user_cash);

        for (const gbp::ns_api::ns_table::user_info_t& user_info: action_info._users_info) {
            int row = model->rowById(user_info._id, &model->origin()._players, model->index_players());
            setPlayerPos(user_info._id, user_info._pos);
            //TODO: уточнить user_info::_bet это round_bet или distribution_bet
            setPlayerDistributionBet(user_info._id, user_info._bet);
            setPlayerCash(user_info._id, user_info._money);
            if (user_info._is_dealer) {
                setDealer(user_info._id);
            }
            model->setOriginData(model->index_player_state(row), user_info._state);
        }
    }

    void askBlind(gbp_i64 blind, bool isBB) const
    {
        Q_UNUSED(blind)
        Q_UNUSED(isBB)
    //    GameHandler::askBlind(blind, isBB);
    }

    void addPlayer(gbp_i64 playerID)
    {
        model->insertItem(playerID, model->index_players());
    }

    void removePlayer(gbp_i64 playerID)
    {
        model->removeItem(playerID, model->index_players());
    }

    void updatePlayer(const table_player_info_t &info)
    {
        model->setOriginData(info);
    }

    void startGame()
    {
    }

    void endGame()
    {
    }

    void startDistribution(gbp_i64 n)
    {
        model->setOriginData(model->index_distribution_number(), n);
    }

    void endDistribution()
    {
    }

    void startTradeRound(gbp_i8 n)
    {
        Q_UNUSED(n)
        for (int row = 0; row < model->rowCount(model->indexOf(ns_model::table_state_t::element::_players)); row++) {
            model->setOriginData(model->index_player_round_bet(row), 0ll);
        }
    }

    void endTradeRound(gbp_i8 n)
    {
        Q_UNUSED(n)
    }

    void setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners
                                     , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>> &users_cards
                                     , bool by_combination)
    {
        Q_UNUSED(by_combination)

        for (const winner_info_t& winner: winners.first) {
            setPlayerCash(winner._user_id, winner._new_cash);
        }
        for (const winner_info_t& winner: winners.second) {
            setPlayerCash(winner._user_id, winner._new_cash);
        }
        for (const std::pair<gbp_i64, std::vector<gbp_i8>>& pair: users_cards) {
            if (pair.first != -1) {
                setPlayerCards(pair.first, pair.second);
            }
        }
    }

    void setChatMessage(gbp_i64 playerID, const QString &msg)
    {
        Q_UNUSED(playerID)
        Q_UNUSED(msg)
        //    GameHandler::setChatMessage(playerID, msg);
    }

    void setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state) {
        QModelIndex stateIndex = model->findElementOfPlayer(playerID, ns_model::table_state_t::element_of_players::_state);
        model->setOriginData(stateIndex, state);
    }
    QVariant userState(gbp_i64 playerID) const {
        return model->findElementOfPlayer(playerID, ns_model::table_state_t::element_of_players::_state).data(UserType::OriginDataRole);
    }

    void onFreeSeatsCountChanged()
    {}

    void openTable() {}
    void closeTable() {}
};


/***************************************************************************/

TableDataUpdater::TableDataUpdater(const GameId &gameId, QObject *parent)
    : QObject(parent)
    , GameDataHandlerInterface(gameId)
    , m_impl(new TableDataUpdaterPrivate(gameId))
{
}
TableDataUpdater::~TableDataUpdater()
{
    delete m_impl;
}

ns_model::table_state_t *TableDataUpdater::model() const
{
    return modelsDB().getTableState(tableId());
}

void TableDataUpdater::timelineEvent(const TimeLineEvent &ev)
{
    m_impl->timelineEvent(ev);
}

void TableDataUpdater::updatePlayers()
{
    m_impl->updatePlayers();
}

void TableDataUpdater::initialize()
{

}

gbp_i64 TableDataUpdater::pot(int index) const
{
    return qvariant_cast<gbp_i64>(model()->index(index, 0, model()->indexOf(ns_model::table_state_t::element::_all_pots)).data(UserType::OriginDataRole));
}

gbp_i64 TableDataUpdater::bigBlind() const
{
    return qvariant_cast<gbp_i64>(model()->indexOf(ns_model::table_state_t::element::_big_blind).data(UserType::OriginDataRole));
}

QString TableDataUpdater::tableName() const
{
    if (id().isCash()) {
        return modelsDB().getCashStatistic()->indexOf(tableId(), ns_model::cash_stat_t::column::_title).data(Qt::DisplayRole).toString();
    }
    return modelsDB().getTournamentStatistic()->indexOf(tableId(), ns_model::tournament_stat_t::column::_title).data(Qt::DisplayRole).toString();
}

e_game_type TableDataUpdater::gameType() const
{
    if (id().isCash()) {
        return qvariant_cast<e_game_type>(modelsDB().getCashStatistic()->indexOf(tableId(), ns_model::cash_stat_t::column::_type).data(UserType::OriginDataRole));
    }
    return qvariant_cast<e_game_type>(modelsDB().getTournamentStatistic()->indexOf(tableId(), ns_model::tournament_stat_t::column::_game_type).data(UserType::OriginDataRole));
}

e_game_limit TableDataUpdater::gameLimit() const
{
    if (id().isCash()) {
        return qvariant_cast<e_game_limit>(modelsDB().getCashStatistic()->indexOf(tableId(), ns_model::cash_stat_t::column::_limit).data(UserType::OriginDataRole));
    }
    return qvariant_cast<e_game_limit>(modelsDB().getTournamentStatistic()->indexOf(tableId(), ns_model::tournament_stat_t::column::_game_limit).data(UserType::OriginDataRole));
}

void TableDataUpdater::update(const table_state_t &tableState)
{
    m_impl->update(tableState);
}

void TableDataUpdater::clearPots()
{
    m_impl->clearPots();
}

void TableDataUpdater::setPots(const std::vector<gbp_i64> &pots)
{
    m_impl->setPots(pots);
}

void TableDataUpdater::setPot(gbp_i64 money, gbp_i8 idx)
{
    m_impl->setPot(money, idx);
}

void TableDataUpdater::updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet)
{
    m_impl->updateMoneyInfo(defaultBet, currentBet);
}

void TableDataUpdater::clearTableCards()
{
    m_impl->clearTableCards();
}

void TableDataUpdater::addTableCard(gbp_i8 card)
{
    m_impl->addTableCard(card);
}

void TableDataUpdater::setTableCard(gbp_i8 card, gbp_i8 idx)
{
    m_impl->setTableCard(card, idx);
}

void TableDataUpdater::clearPlayersCards()
{
    m_impl->clearPlayersCards();
}

void TableDataUpdater::addPlayerCard(gbp_i64 playerID, gbp_i8 card)
{
    m_impl->addPlayerCard(playerID, card);
}

void TableDataUpdater::setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards)
{
    m_impl->setPlayerCards(playerID, cards);
}

void TableDataUpdater::setDealerCard(gbp_i64 playerID, gbp_i8 card)
{
    m_impl->setDealerCard(playerID, card);
}

void TableDataUpdater::setPlayerPos(gbp_i64 playerID, gbp_i8 pos)
{
    m_impl->setPlayerPos(playerID, pos);
}

void TableDataUpdater::setPlayerAction(gbp_i64 playerID, const GameAction &action)
{
    m_impl->setPlayerAction(playerID, action);
}

void TableDataUpdater::setPlayerCash(gbp_i64 playerID, gbp_i64 money)
{
    m_impl->setPlayerCash(playerID, money);
}

void TableDataUpdater::setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money)
{
    m_impl->setPlayerRoundBet(playerID, money);
}

void TableDataUpdater::setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money)
{
    m_impl->setPlayerDistributionBet(playerID, money);
}

void TableDataUpdater::setDealer(gbp_i64 playerID)
{
    m_impl->setDealer(playerID);
}

void TableDataUpdater::setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed)
{
    m_impl->setCurrentPlayer(playerID, timeLeft, timePassed);
}

void TableDataUpdater::makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info)
{
    m_impl->makeUserCurrentPlayer(actions, action_info);
}

void TableDataUpdater::askBlind(gbp_i64 blind, bool isBB) const
{
    m_impl->askBlind(blind, isBB);
}

void TableDataUpdater::addPlayer(gbp_i64 playerID)
{
    m_impl->addPlayer(playerID);
}

void TableDataUpdater::removePlayer(gbp_i64 playerID)
{
    m_impl->removePlayer(playerID);
}

void TableDataUpdater::updatePlayer(const table_player_info_t &info)
{
    m_impl->updatePlayer(info);
}

void TableDataUpdater::startGame()
{
    m_impl->startGame();
}

void TableDataUpdater::endGame()
{
    m_impl->endGame();
}

void TableDataUpdater::startDistribution(gbp_i64 n)
{
    m_impl->startDistribution(n);
}

void TableDataUpdater::endDistribution()
{
    m_impl->endDistribution();
}

void TableDataUpdater::startTradeRound(gbp_i8 n)
{
    m_impl->startTradeRound(n);
}

void TableDataUpdater::endTradeRound(gbp_i8 n)
{
    m_impl->endTradeRound(n);
}

void TableDataUpdater::setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners
                                 , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>> &users_cards
                                 , bool by_combination)
{
    m_impl->setWinners(winners, users_cards, by_combination);
}

void TableDataUpdater::setChatMessage(gbp_i64 playerID, const QString &msg)
{
    m_impl->setChatMessage(playerID, msg);
}

void TableDataUpdater::setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state) {
    m_impl->setUserState(playerID, state);
}

QVariant TableDataUpdater::userState(gbp_i64 playerID) const
{
    return m_impl->userState(playerID);
}

void TableDataUpdater::onFreeSeatsCountChanged()
{
    m_impl->onFreeSeatsCountChanged();
}

void TableDataUpdater::openTable()
{
    m_impl->openTable();
}

void TableDataUpdater::closeTable()
{
    m_impl->closeTable();//[this]{
//        emit tableClosed(tableId());
//        modelsDB().removeTableState(tableId());
//    });
}

