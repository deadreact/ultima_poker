#include <precompiled.h>
#include <api/table/types.h>
#include <db/modeldatabase.h>
#include "debuginfoprintergamehandler.h"
#include <api_models/model_log.h>
#include <api_models/api_util.hpp>
#include <sstream>
#include <api_models/itemtype_concept.hpp>
#include <QItemDelegate>
#include <qstringbuilder.h>
#include <core/money.h>

#define __FILELINE__ __FILE__, __LINE__
#define ARG_STR(arg) (QStringLiteral(#arg) % QStringLiteral(": ") % makeStr(arg))

#define TODEBUG0() debug(__FUNCTION__, __FILELINE__)
#define TODEBUG1(args) debug(__FUNCTION__, __FILE__, __LINE__, args)


/** @class LogMsgItemViewFunction ***********************/
class LogMsgItemViewFunction : public ns_model::LogMsgItem
{
public:
    LogMsgItemViewFunction(const QString& msg = QString())
        : ns_model::LogMsgItem(msg, ns_model::LogMsgItem::MsgUser)
    {}
public:
    virtual QVariant data(int role) const override;
};

QVariant LogMsgItemViewFunction::data(int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::DecorationRole:
    case UserType::FilterRole:
    case UserType::RowItemIdRole:
    case UserType::DateTimeRole:
        return ns_model::LogMsgItem::data(role);
    case Qt::ToolTipRole:
        return "View Function: " + m_msg;
    case Qt::ForegroundRole:
        return QColor(0x55aeef);
    case Qt::BackgroundRole:
        return QColor(0xfafaea);
    case Qt::FontRole:
        return QFont("Open Sans", 10, 450);
    default:
        return QVariant();
    }
}




class Delegate : public QItemDelegate
{
public:
    Delegate(QObject* parent = nullptr)
        : QItemDelegate(parent)
    {}

    // QItemDelegate interface
protected:
    virtual void drawDisplay(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QString &text) const override
    {
        static const QRegularExpression re("(♥|♦|♣|♠)(2|3|4|5|6|7|8|9|10|J|Q|K|A)");
    }
};

Debug::Debug(const bool &enabled): m_enabled(enabled) {}

Debug &Debug::contexual(const char *file, int line, const char *msg) {
    if (m_enabled) {
        //qDebug() << q_check_ptr(msg);
        qDebug("   Loc: [%s:%i]", q_check_ptr(file), line);
    }
    return *this;
}


template <typename T>
QString makeStr(const T& value) {
    std::ostringstream os;
    os << value;
    return QString::fromStdString(os.str());
}

template <>
QString makeStr(const QString& value) {
    return value;
}

template <>
QString makeStr(const TimeLineEvent& value) {
    return value.toString();
}
template <>
QString makeStr(const GameAction& value) {
    return ns_util::toString(value.first) % " " % Money(value.second).toString();
}
template <>
QString makeStr(void* const& value) {
    return QString::number(quintptr(value));
}
template <>
QString makeStr(const bool &value) {
    return (value ? "true" : "false");
}
template <>
QString makeStr(const Money &value) {
    return value.toString();
}
template <>
QString makeStr(const std::vector<gbp_i64>& pots) {
    QStringList lst;
    for (gbp_i64 m: pots) {
        lst << Money(m).toString();
    }
    return lst.join(" ");
}
template <>
QString makeStr(const gbp_i8& card) {
    return ns_meta::card_type::display(card);
}
template <>
QString makeStr(const std::vector<gbp_i8>& cards) {
    return ns_meta::cards_type::display(cards);
}
template <>
QString makeStr(const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>>& users_cards) {
    QStringList lst;
    for (const std::pair<gbp_i64, std::vector<gbp_i8>>& user_cards: users_cards) {
        lst << QString("%0: %1").arg(ns_meta::id_type::display(user_cards.first)).arg(ns_meta::cards_type::display(user_cards.second));
    }
    return lst.join("; ");
}


template <>
QString makeStr(const QModelIndex& value) {
    return ns_util::traceModelIndexWIthData(value);
}
template <>
QString makeStr(const QModelIndexRange& value) {
    if (value.indexCount() <= 1) {
        return makeStr(value.first);
    }
    return QStringLiteral("Area: ") % makeStr(value.first) % QStringLiteral(" - ") % makeStr(value.second);
}
template <>
QString makeStr(const QModelIndexRangeList& value) {
    QStringList lst;
    for (const QModelIndexRange& range: value) {
        lst << makeStr(range);
    }
    return QString("QModelIndexRangeList[%0]").arg(lst.join(", "));
}


DebugInfoPrinter::DebugInfoPrinter()
    : m_enableLog(true)
    , m_enableDebug(true)
{}

ns_model::LogModel &DebugInfoPrinter::log(const char* func, const char *file, int line, const QStringList& args) const
{
    ns_model::LogModel& logModel = *modelsDB().logModel();
    if (!m_enableLog) {
        return logModel;
    }
    if (!args.isEmpty()) {
        QString msg = scope() % q_check_ptr(func) % QStringLiteral("(") % args.join(", ") % QStringLiteral(")");
        logModel.newMsg(msg, ns_model::LogMsgItem::MsgFunction);
    } else {
        logModel.newMsg(scope() % q_check_ptr(func) % QStringLiteral("()"), ns_model::LogMsgItem::MsgFunction);
    }
    if (file) {
        logModel << (file % QStringLiteral(":") % QString::number(line));
    }
    return logModel;
}

ns_model::LogModel &DebugInfoPrinter::log(const char *func, const QStringList &args) const
{
    return log(func, nullptr, 0, args);
}

Debug& getDebug(const bool& e) {
    static Debug _debug(e);
    return _debug;
}

Debug &DebugInfoPrinter::debug(const char *func, const char *file, int line, const QStringList& args) const
{
    if (!m_enableDebug) {
        return getDebug(m_enableDebug);
    }
    if (args.isEmpty()) {
        QString msg = scope() % func % QStringLiteral("()");
        return getDebug(m_enableDebug).contexual(file, line, qUtf8Printable(msg));
    }
    QString msg = scope() % func % QStringLiteral("(") % args.join(", ") % QStringLiteral(")");
    return getDebug(m_enableDebug).contexual(file, line, qUtf8Printable(msg));
}

Debug &DebugInfoPrinter::debug(const char *func, const QStringList &args) const
{
    if (args.isEmpty()) {
        return getDebug(m_enableDebug) << scope() % func;
    }
    QString msg = scope() % func % args.join(", ");
    return getDebug(m_enableDebug) << msg;
}



//! @class DebugInfoPrinterGameDataHandler

QString DebugInfoPrinterGameDataHandler::scope() const
{
    return "DebugInfoPrinterGameDataHandler::";
}

DebugInfoPrinterGameDataHandler::DebugInfoPrinterGameDataHandler(const GameId &id)
    : DebugInfoPrinter()
    , GameDataHandlerInterface(id)
{}

void DebugInfoPrinterGameDataHandler::initialize()
{

}


void DebugInfoPrinterGameDataHandler::updatePlayers()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::update(const table_state_t &tableState)
{
    QStringList args = QStringList() << ARG_STR(table_state_t(tableState));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::clearPots()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::setPots(const std::vector<gbp_i64> &pots)
{
     QStringList args = QStringList() << ARG_STR(pots);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPot(gbp_i64 money, gbp_i8 idx)
{
    QStringList args = QStringList() << ARG_STR(Money(money)) << ARG_STR(int(idx));//QString("(%0|***|%1)").arg(Money(money).toString()).arg(idx);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::updateMoneyInfo(gbp_i64 defaultBet, gbp_i64 currentBet)
{
    QStringList args = QStringList() << ARG_STR(Money(currentBet)) << ARG_STR(Money(currentBet));//QString("(%0|***|%1)").arg(Money(defaultBet).toString()).arg(Money(currentBet).toString());
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::clearTableCards()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::addTableCard(gbp_i8 card)
{
    QStringList args = QStringList() << ARG_STR(card);//QString("(%0)").arg(ARG_STR(card));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setTableCard(gbp_i8 card, gbp_i8 idx)
{
    QStringList args = QStringList() << ARG_STR(card) << ARG_STR(int(idx));//QString("(%0|***|%1)").arg(ARG_STR(card)).arg(idx);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::clearPlayersCards()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::addPlayerCard(gbp_i64 playerID, gbp_i8 card)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(card);//QString("(%0|***|%1)").arg(playerID).arg(ARG_STR(card));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPlayerCards(gbp_i64 playerID, const std::vector<gbp_i8> &cards)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(cards);//QString("(%0|***|%1)").arg(playerID).arg(ARG_STR(cards));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setDealerCard(gbp_i64 playerID, gbp_i8 card)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(card);//QString("(%0|***|%1)").arg(playerID).arg(ARG_STR(card));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPlayerPos(gbp_i64 playerID, gbp_i8 pos)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(int(pos));//QString("(%0|***|%1)").arg(playerID).arg(pos);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPlayerAction(gbp_i64 playerID, const GameAction &action)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(GameAction(action));//QString("(%0|***|%1)").arg(playerID).arg(ARG_STR(action));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPlayerCash(gbp_i64 playerID, gbp_i64 money)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(money);//QString("(%0|***|%1)").arg(playerID).arg(Money(money).toString());
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPlayerRoundBet(gbp_i64 playerID, gbp_i64 money)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(Money(money));//QString("(%0|***|%1)").arg(playerID).arg(Money(money).toString());
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setPlayerDistributionBet(gbp_i64 playerID, gbp_i64 money)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(Money(money));//QString("(%0|***|%1)").arg(playerID).arg(Money(money).toString());
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setDealer(gbp_i64 playerID)
{
    QStringList args = QStringList() << ARG_STR(playerID);//QString("(%0)").arg(playerID);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setCurrentPlayer(gbp_i64 playerID, gbp_i64 timeLeft, gbp_i64 timePassed)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(timeLeft) << ARG_STR(timePassed);//QString("(%0|***|%1|***|%2)").arg(playerID).arg(timeLeft).arg(timePassed);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::askBlind(gbp_i64 blind, bool isBB) const
{
    QStringList args = QStringList() << ARG_STR(Money(blind)) << ARG_STR(isBB);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::startGame()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::endGame()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::startDistribution(gbp_i64 n)
{
    QStringList args = QStringList() << ARG_STR(n);//QString("(%0)").arg(n);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::endDistribution()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::startTradeRound(gbp_i8 n)
{
    QStringList args = QStringList() << ARG_STR(n);//QString("(%0)").arg(n);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::endTradeRound(gbp_i8 n)
{
    QStringList args = QStringList() << ARG_STR(n);//QString("(%0)").arg(n);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setWinners(const std::pair<std::vector<winner_info_t>, std::vector<winner_info_t> > &winners
                                           , const std::vector<std::pair<gbp_i64, std::vector<gbp_i8>>> &users_cards
                                           , bool by_combination)
{
    QStringList args = QStringList() << ARG_STR(winners) << ARG_STR(users_cards) << ARG_STR(by_combination);//QString("(%0|***|%1|***|%2)").arg(ARG_STR(winners)).arg(ARG_STR(users_cards)).arg(by_combination ? "true" : "false");
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setChatMessage(gbp_i64 playerID, const QString &msg)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(msg);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::addPlayer(gbp_i64 playerID)
{
    QStringList args = QStringList() << ARG_STR(playerID);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::removePlayer(gbp_i64 playerID)
{
    QStringList args = QStringList() << ARG_STR(playerID);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::updatePlayer(const table_player_info_t &info)
{
    QStringList args = QStringList() << ARG_STR(table_player_info_t(info));
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::onFreeSeatsCountChanged()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::timelineEvent(const TimeLineEvent &ev)
{
    QStringList args = QStringList() << ARG_STR(ev);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameDataHandler::setUserState(gbp_i64 playerID, const gbp::ns_api::ns_table::user_state_t& state)
{
    QStringList args = QStringList() << ARG_STR(playerID) << ARG_STR(state);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

QVariant DebugInfoPrinterGameDataHandler::userState(gbp_i64 playerID) const
{
    QStringList args = QStringList() << ARG_STR(playerID);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
    return QVariant();
}


void DebugInfoPrinterGameDataHandler::makeUserCurrentPlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info)
{
    QStringList args = QStringList() << ARG_STR(actions) << ARG_STR(action_info);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}


void DebugInfoPrinterGameDataHandler::openTable()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameDataHandler::closeTable()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

/** ********************************************************************/
//! @class DebugInfoPrinterGameViewHandler
/** ********************************************************************/

QString DebugInfoPrinterGameViewHandler::scope() const
{
    return "DebugInfoPrinterGameViewHandler::";
}

DebugInfoPrinterGameViewHandler::DebugInfoPrinterGameViewHandler(const GameId &gameID, QObject *parent)
    : GameViewHandler(gameID, parent)
    , DebugInfoPrinter()
{

}

ns_model::LogModel &DebugInfoPrinterGameViewHandler::log(const char *func, const char *file, int line, const QStringList &args) const
{
    ns_model::LogModel& logModel = *modelsDB().logModel();
    if (!m_enableLog) {
        return logModel;
    }
    if (!args.isEmpty()) {
        QString msg = scope() % q_check_ptr(func) % QStringLiteral("(") % args.join(", ") % QStringLiteral(")");
        logModel.newMsg(new LogMsgItemViewFunction(msg));
    } else {
        logModel.newMsg(new LogMsgItemViewFunction(scope() % q_check_ptr(func) % QStringLiteral("()")));
    }
    if (file) {
        logModel << (file % QStringLiteral(":") % QString::number(line));
    }
    return logModel;
}


void DebugInfoPrinterGameViewHandler::initialize()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameViewHandler::timelineEvent(const TimeLineEvent &ev)
{
    QStringList args = QStringList() << ARG_STR(ev);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameViewHandler::onModelAboutToBeReset()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameViewHandler::onModelReset()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameViewHandler::onModelDataChanged(const QModelIndexRange &range)
{
    QStringList args = QStringList() << ARG_STR(range);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameViewHandler::onModelAboutToInsertItems(const QModelIndex &parent)
{
    QStringList args = QStringList() << ARG_STR(parent);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameViewHandler::onModelInsertItems(const QModelIndexRangeList &items)
{
    QStringList args = QStringList() << ARG_STR(items);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameViewHandler::onModelAboutToRemoveItems(const QModelIndexRangeList &items)
{
    QStringList args = QStringList() << ARG_STR(items);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

void DebugInfoPrinterGameViewHandler::onModelRemoveItems(const QModelIndex &parent)
{
    QStringList args = QStringList() << ARG_STR(parent);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
}

bool DebugInfoPrinterGameViewHandler::indexAffectsView(const QModelIndex &index) const
{
    QStringList args = QStringList() << ARG_STR(index);
    log(__FUNCTION__, __FILELINE__, args);
    TODEBUG1(args);
    return true;
}

void DebugInfoPrinterGameViewHandler::connectToModel()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
    GameViewHandler::connectToModel();
}

void DebugInfoPrinterGameViewHandler::disconnectFromModel()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
    GameViewHandler::disconnectFromModel();
}

void DebugInfoPrinterGameViewHandler::openTable()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}

void DebugInfoPrinterGameViewHandler::closeTable()
{
    log(__FUNCTION__, __FILELINE__);
    TODEBUG0();
}
