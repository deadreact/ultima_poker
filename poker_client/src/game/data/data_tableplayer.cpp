#include <precompiled.h>
#include "data_tableplayer.h"
#include <api_models/model_table_state_t.hpp>

//#include <utils/logger.h>
//#include <basewidgets/util.h>


//namespace
//{
//    using e_user_action = gbp::ns_api::ns_table::e_user_action;
//}


//TablePlayer::TablePlayer(GamePlayerId id, const QString &nickname, const sha1_t &hashAvatar, QLocale::Country country, QObject *parent)
//    : QAction(QIcon(pApp->db().getAvatarPixmap(hashAvatar)), nickname, parent)
//    , m_id(id)
//    , m_country(country)
//    , m_roundBet{0,0,0,0,0}
//    , m_distributionBet(0)
//    , m_gameMoney(0)
//    , m_seatIndex(-1)
//    , m_roundIndex(0)
//    , m_waiterIndex(-1)
//    , m_online(true)
//    , m_frozen(false)
//    , m_ingame(false)
//    , m_folded(false)
//    , m_current(false)
//{
//    setCheckable(true);
//    setEnabled(true);
//    setVisible(true);

//    Logger::event(QString("player created, %0").arg(m_id.toString()), __FILELINE__);
//}

//const QMetaProperty &TablePlayer::lastChanges() const {
//    return m_changes.back();
//}
//const QMetaProperty &TablePlayer::changes(int idx) const {
//    return m_changes.at(idx);
//}

//int TablePlayer::changesCountFrom(int rev) const {
//    return m_changes.size() - rev;
//}

//std::vector<QMetaProperty> TablePlayer::changesFrom(int rev) const
//{
//    std::vector<QMetaProperty> vec;
//    vec.assign(m_changes.begin() + rev, m_changes.end());
//    return vec;
//}

//const std::vector<QMetaProperty> &TablePlayer::allChanges() const
//{
//    return m_changes;
//}

//TablePlayer *TablePlayer::create(const GamePlayerId &id, const QString &nickname, const sha1_t &hashAvatar, QLocale::Country country, QObject *parent)
//{
//    return new TablePlayer(id, nickname, hashAvatar, country, parent);
//}


//TablePlayer::~TablePlayer()
//{
//}

//QByteArray TablePlayer::info() const
//{
//    QString templ("id: %0\n"
//                  "nickname: %1\n"
//                  "country: %2\n"
//                  "seatIndex: %3\n"
//                  "roundBet: %4\n"
//                  "distrBet: %5\n"
//                  "cash: %6\n"
//                  "online: %7\n"
//                  "sitout: %8\n"
//                  "frozen: %9\n"
//                  "ingame: %10\n"
//                  "folded: %11\n");
//    return templ.arg(id().toString()).arg(text()).arg(ns_util::enumToString(m_country)).arg(m_seatIndex)
//                .arg(roundBet().toString()).arg(distributionBet().toString()).arg(gameMoney().toString())
//                .arg(isOnline()).arg(isSitOut()).arg(isFrozen()).arg(isInGame()).arg(isFolded()).toLatin1();
//}

//QPixmap TablePlayer::avatar(const QSize &size) const
//{
//    if (size.isValid()) {
//        return icon().pixmap(size);
//    }
//    return icon().pixmap(QSize(100, 100));
//}

//void TablePlayer::setAvatar(const QPixmap &avatar, bool override)
//{
//    if (override) {
//        setIcon(QIcon(avatar));
//    } else {
//        QIcon ico = icon();
//        ico.addPixmap(avatar);
//        setIcon(ico);
//    }
//    emit avatarUpdated(avatar);
//}

//const GameAction& TablePlayer::lastCourse() const {
//    return m_lastCourse;
//}

//gbp_i16 TablePlayer::seatIndex() const {
//    return m_seatIndex;
//}

//void TablePlayer::setSeatIndex(gbp_i16 seatIndex)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("seatIndex"));
//    if (m_seatIndex != seatIndex) {
//        m_seatIndex = seatIndex;
//        m_changes.push_back(context);

//        emit indexChanged(context);
//    }
//}

//gbp_i16 TablePlayer::roundIndex() const {
//    return m_roundIndex;
//}

//void TablePlayer::setRoundIndex(gbp_i16 round)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("roundIndex"));
//    if (m_roundIndex != round) {
//        m_roundIndex = round;
//        if (round == 0) {
//            m_roundBet.fill(0);
//        }
//        m_changes.push_back(context);

//        emit indexChanged(context);
//    }
//}

//gbp_i16 TablePlayer::waiterIndex() const {
//    return m_waiterIndex;
//}

//void TablePlayer::setWaiterIndex(gbp_i16 waiter)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("waiterIndex"));
//    if (m_waiterIndex != waiter) {
//        m_waiterIndex = waiter;
//        m_changes.push_back(context);

//        emit indexChanged(context);
//    }
//}

//gbp_i16 TablePlayer::waitersCount() const {
//    waiter_t* sharedData = pApp->db().sharedDataService().getSingleDataForType<waiter_t>();
//    return sharedData->second[tableID()];
//}

//void TablePlayer::setWaitersCount(gbp_i16 count) {
//    waiter_t* sharedData = pApp->db().sharedDataService().getSingleDataForType<waiter_t>();
//    sharedData->second[tableID()] = count;
//}

//Money TablePlayer::roundBet(gbp_i16 round) const
//{
//    round = (round == -1) ? m_roundIndex : round;
//    return m_roundBet.at(round);
//}

//Money TablePlayer::distributionBet() const {
//    return m_distributionBet;
//}

//Money TablePlayer::gameMoney() const {
//    return m_gameMoney;
//}

//Money TablePlayer::totalIngameMoney() const
//{
//    return gameMoney() + distributionBet();
//}

//void TablePlayer::setRoundBet(Money m) {
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("roundBet"));
//    if (m_roundBet[m_roundIndex] != m) {
//        m_roundBet[m_roundIndex] = m;
//        m_changes.push_back(context);

//        emit moneyChanged(context);
//    }
//}

//void TablePlayer::setDistributionBet(Money m) {
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("distributionBet"));
//    if (m_distributionBet != m) {
//        m_distributionBet = m;
//        m_changes.push_back(context);

//        emit moneyChanged(context);
//    }
//}

//void TablePlayer::setGameMoney(Money m) {
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("gameMoney"));
//    if (m_gameMoney != m) {
//        m_gameMoney = m;
//        m_changes.push_back(context);

//        emit moneyChanged(context);
//    }
//}

//void TablePlayer::setOnline(bool v)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("online"));
//    if (isOnline() != v)
//    {
//        m_changes.push_back(context);
//        m_online = v;
//        emit statusChanged(context);
//    }
//}
//void TablePlayer::sitOut(bool v)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("sitOut"));
//    if (isOnline() == v)
//    {
//        m_changes.push_back(context);
//        m_online = !v;
//        emit statusChanged(context);
//    }
//}

//void TablePlayer::freeze(bool v)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("frozen"));
//    if (isFrozen() != v)
//    {
//        m_changes.push_back(context);
//        m_frozen = v;
//        if (m_frozen)
//        {
//            setChecked(false);
//            m_current = false;
//            m_online = true;
//            m_ingame = false;
//        }
//        emit statusChanged(context);
//    }
//}

//void TablePlayer::setInGame(bool v)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("ingame"));
//    if (isInGame() != v)
//    {
//        m_changes.push_back(context);
//        m_ingame = v;
//        if (m_ingame)
//        {
//            m_online = true;
//            m_frozen = false;
//        }
//        emit statusChanged(context);
//    }
//}


//void TablePlayer::fold(bool v)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("folded"));
//    if (isFolded() != v)
//    {
//        m_changes.push_back(context);
//        m_folded = v;

//        if (m_folded)
//        {
//            setChecked(false);
//            m_current = false;
//            m_ingame = true;
//            m_online = true;
//        }

//        emit statusChanged(context);
//    }
//}

//void TablePlayer::course(const GameAction& course)
//{
//    if (!isCurrent() && course.first != e_user_action::guest_bet) {
//        Logger::warning(QString("Player %0 can not course, current player is %1").arg(playerID()).arg((actionGroup()->checkedAction() ? qobject_cast<TablePlayer*>(actionGroup()->checkedAction())->playerID() : -1ll)), __FILELINE__);
//    }

//    m_lastCourse = course;
//    setInGame(course.first != e_user_action::fold);
//    makeCurrent(false);
//    if (course.first == e_user_action::fold) {
//        fold(true);
//    } else if (course.first == e_user_action::blind || course.first == e_user_action::guest_bet) {
//        setRoundBet(course.second);
//    }
//}

//void TablePlayer::makeCurrent(bool v)
//{
//    static const QMetaProperty context = staticMetaObject.property(staticMetaObject.indexOfProperty("current"));
//    if (v != isCurrent())
//    {
//        m_changes.push_back(context);

//        setChecked(v);
//        m_current = v;
//        m_online = true;
//        if (m_current)
//        {
//            m_frozen = false;
//            m_ingame = true;
//            m_folded = false;
//        }

//        emit statusChanged(context);
//    }
//}

namespace ns_data
{
    ns_model::table_state_t *TablePlayer::tableModel() const
    {
        return (m_gameID.isValid()) ? modelsDB().getTableState(m_gameID) : nullptr;
    }

//    Accessor<gbp::ns_api::ns_table::table_player_info_t> TablePlayer::getTableData() const
//    {
//        if (auto* m = tableModel())
//        {
//            m->originData(m->index_players())
//        }
//    }

    TablePlayerMapping *TablePlayerMapping::create(gbp_i64 table_id, const PlayerBase &baseData)
    {
        if (auto* m = modelsDB().getTableState(table_id)) {
            const auto& tableState = m->origin();

        }
    }

} // namespace ns_data
