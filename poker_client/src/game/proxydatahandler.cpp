#include <precompiled.h>
#include "proxydatahandler.h"

ProxyDataHandler::ProxyDataHandler(const GameId &id)
    : ProxyHandler<GameDataHandlerInterface>(id)
{

}

ProxyDataHandler::~ProxyDataHandler()
{

}

void ProxyDataHandler::initialize()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->initialize();
    }
}

void ProxyDataHandler::updatePlayers()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->updatePlayers();
	}
}

void ProxyDataHandler::update(const table_state_t &tableState)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->update(tableState);
	}
}

void ProxyDataHandler::clearPots()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->clearPots();
	}
}

void ProxyDataHandler::setPots(const std::vector<gbp_i64> &pots)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setPots(pots);
	}
}

void ProxyDataHandler::setPot(gbp_i64 money, gbp_i8 idx)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->setPot(money, idx);
	}
}

void ProxyDataHandler::updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->updateMoneyInfo(defaultBet, currentBet);
	}
}

void ProxyDataHandler::clearTableCards()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->clearTableCards();
    }
}

void ProxyDataHandler::addTableCard(gbp_i8 card)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->addTableCard(card);
	}
}

void ProxyDataHandler::setTableCard(gbp_i8 card, gbp_i8 idx)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setTableCard(card, idx);
	}
}

void ProxyDataHandler::clearPlayersCards()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->clearPlayersCards();
	}
}

void ProxyDataHandler::addPlayerCard(gbp_i64 playerID, gbp_i8 card)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->addPlayerCard(playerID, card);
	}
}

void ProxyDataHandler::setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setPlayerCards(playerID, cards);
	}
}

void ProxyDataHandler::setDealerCard(gbp_i64 playerID, gbp_i8 card)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setDealerCard(playerID, card);
	}
}

void ProxyDataHandler::setPlayerPos(gbp_i64 playerID, gbp_i8 pos)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setPlayerPos(playerID, pos);
	}
}

void ProxyDataHandler::setPlayerAction(gbp_i64 playerID, const GameAction &action)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setPlayerAction(playerID, action);
	}
}

void ProxyDataHandler::setPlayerCash(gbp_i64 playerID, gbp_i64 money)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setPlayerCash(playerID, money);
	}
}

void ProxyDataHandler::setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setPlayerRoundBet(playerID, money);
	}
}

void ProxyDataHandler::setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->setPlayerDistributionBet(playerID, money);
	}
}

void ProxyDataHandler::setDealer(gbp_i64 playerID)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setDealer(playerID);
	}
}

void ProxyDataHandler::setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setCurrentPlayer(playerID, timeLeft, timePassed);
    }
}

void ProxyDataHandler::makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->makeUserCurrentPlayer(actions, action_info);
    }
}

void ProxyDataHandler::askBlind(gbp_i64 blind, bool isBB) const
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->askBlind(blind, isBB);
	}
}

void ProxyDataHandler::startGame()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->startGame();
	}
}

void ProxyDataHandler::endGame()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->endGame();
	}
}

void ProxyDataHandler::startDistribution(gbp_i64 n)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->startDistribution(n);
	}
}

void ProxyDataHandler::endDistribution()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->endDistribution();
	}
}

void ProxyDataHandler::startTradeRound(gbp_i8 n)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->startTradeRound(n);
	}
}

void ProxyDataHandler::endTradeRound(gbp_i8 n)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->endTradeRound(n);
	}
}

void ProxyDataHandler::setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners, const std::vector<std::pair<gbp_i64, std::vector<gbp_i8> > > &users_cards, bool by_combination)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->setWinners(winners, users_cards, by_combination);
	}
}

void ProxyDataHandler::setChatMessage(gbp_i64 playerID, const QString &msg)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->setChatMessage(playerID, msg);
	}
}

void ProxyDataHandler::addPlayer(gbp_i64 playerID)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->addPlayer(playerID);
	}
}

void ProxyDataHandler::removePlayer(gbp_i64 playerID)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->removePlayer(playerID);
	}
}

void ProxyDataHandler::updatePlayer(const table_player_info_t &info)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->updatePlayer(info);
	}
}

void ProxyDataHandler::onFreeSeatsCountChanged()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->onFreeSeatsCountChanged();
	}
}

void ProxyDataHandler::timelineEvent(const TimeLineEvent &ev)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
		handler->timelineEvent(ev);
	}
}

void ProxyDataHandler::setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state)
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->setUserState(playerID, state);
    }
}

QVariant ProxyDataHandler::userState(gbp_i64 playerID) const
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        QVariant value = handler->userState(playerID);
        if (value.isValid()) {
            return value;
        }
    }
    return QVariant();
}


void ProxyDataHandler::openTable()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->openTable();
    }
}

void ProxyDataHandler::closeTable()
{
    for (GameDataHandlerInterface* handler: m_handlers) {
        handler->closeTable();
    }
}
