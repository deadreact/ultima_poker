#include <precompiled.h>
#include "usercontrols.h"
#include "ui_usercontrols.h"
#include <tools/math_tools.hpp>
#include <dialogs/dialog_gametable.h>
#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_table_state_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>

#include <rpc/table/rpc_table.h>
#include <rpc/rpc_common.h>
#include <game/TimelineHandler.h>

std::vector<AutoAction> initAutoActions() {
    std::vector<AutoAction> actions;
    actions.push_back(AutoAction(e_user_action::fold, 0));                   //autoFold
    actions.push_back(AutoAction(e_user_action::check, 0, &actions.back())); //autoCheckFold
    actions.push_back(AutoAction(e_user_action::check, 0));                  //autoCheck
    actions.push_back(AutoAction(e_user_action::call, -1));                  //autoCallAny
    actions.push_back(AutoAction(e_user_action::check, 0, &actions.back())); //autoCheckCallAny

    actions.push_back(AutoAction(e_user_action::raise, -1));                  //autoRaiseAny
    actions.push_back(AutoAction(e_user_action::bet, 0, &actions.back()));    //autoBetRaiseAny

    return actions;
}


namespace
{
    rpc_table* getRpc() {
        return rpc_object<rpc_table>();
    }
    GameAction courseAction(ns_table::e_user_action id, QObject* obj) {
        return std::make_pair(id, qvariant_cast<gbp_i64>(obj->property("money")));
    }
//    GameAction courseAction(typename std::underlying_type<ns_table::e_user_action>::type id, QObject* obj) {
//        return courseAction(ns_table::e_user_action(id), obj);
//    }
} // namespace

template <>
struct IdGetter<GameTable>
{
    using T = GameTable;
    using type = IdGetter<GameTable>;
    using id_type = GameId;

    constexpr static id_type invalidID() {
        return GameId::invalidID();
    }

    static id_type get_id(const T& v) {
        return v.id();
    }
};

GameId findIdFromGameTable(QWidget* child) {
    return ns_util::findIdFromParent(&GameTable::id, child);
}

/** ******************* UserControls::Impl *******************************/
struct UserControls::Impl : Ui::UserControls
{
    QWidget* m_owner;
    GameId m_id;
    QString m_theme;
    QMap<QWidget*, int> m_mappingWgtToId;
    QMap<int, QWidget*> m_mappingIdToWidget;
    QHash<QByteArray, QMap<int, int>> m_enumMapping;

    Impl(QWidget* owner)
        : m_owner(owner)
        , m_id(findIdFromGameTable(owner))
        , m_theme("default")
    {}

    void setupUi(QWidget* w) {
        Q_ASSUME(w == m_owner);
        Ui::UserControls::setupUi(w);

    }

    int registerControl(QWidget* w) {
        Q_ASSERT(w);
        Q_ASSERT(!m_mappingWgtToId.contains(w));
        static int uniqueInc = 0;
        uniqueInc++;
        m_mappingWgtToId.insert(w, uniqueInc);
        m_mappingIdToWidget.insert(uniqueInc, w);

        return uniqueInc;
    }

    QWidget* unregisterControl(int uniqueID) {
        Q_ASSERT(m_mappingIdToWidget.contains(uniqueID));
        QWidget* w = m_mappingIdToWidget.take(uniqueID);

        m_mappingWgtToId.remove(w);

        return w;
    }

    template <typename Enum>
    void registerEnumId(Enum enumId, int uniqueID) {
        m_enumMapping[ns_meta::generic_enum_type<Enum>::type_name()].insert(underlying_cast(enumId), uniqueID);
    }
    template <typename Enum>
    void unregisterEnum() {
        m_enumMapping.remove(ns_meta::generic_enum_type<Enum>::type_name());
    }

    QWidget* getControl(int id) const {
        Q_ASSERT(m_mappingIdToWidget.contains(id));
        return m_mappingIdToWidget.value(id);
    }
    template <typename Enum>
    QWidget* getControl(Enum value) const {
        auto it = m_enumMapping.find(ns_meta::generic_enum_type<Enum>::type_name());
        Q_ASSERT(it != m_enumMapping.end());
        return getControl(it->value(underlying_cast(value)));
    }

    void applyTheme(QWidget* w) {
        QString currTheme = w->property("theme").toString();
        if (currTheme != m_theme) {
            w->style()->unpolish(w);
            w->setProperty("theme", "blue");
            w->style()->polish(w);
            for (QObject* child: w->children()) {
                if (child->isWidgetType()) {
                    applyTheme(qobject_cast<QWidget*>(child));
                }
            }
        }
    }
};

/** ******************* UserControls ***********************************/

UserControls::UserControls(QWidget *parent)
    : InitableWidget(parent)
    , m_impl(new Impl(this))
{
    m_impl->setupUi(this);
//    m_impl->m_id = ns_util::inheritedID<GameTable>(parent);
//    m_impl->setupUi(this);
}

UserControls::~UserControls()
{
    delete m_impl;
}

int UserControls::registerExternalControl(QWidget *w) {
    return m_impl->registerControl(w);
}
QWidget* UserControls::unregisterExternalControl(int uniqueID) {
    return m_impl->unregisterControl(uniqueID);
}

QWidget *UserControls::usrControl(int uniqueID) const {
    return m_impl->getControl(uniqueID);
}

void UserControls::on_bttnGrp_gameActions_buttonClicked(QAbstractButton* bttn)
{
    qDebug() << bttn->text();
}

void UserControls::on_bttn_joinQueue_clicked()
{

}

void UserControls::on_bttn_unjoinQueue_clicked()
{

}

void UserControls::registerCourseActionTrigger(QAbstractButton *bttn, e_user_action id, gbp_i64 moneyMin, gbp_i64 range)
{
    Q_ASSERT(bttn && underlying_cast(id) >= 0);

    int uniqueID = m_impl->registerControl(bttn);
    m_impl->registerEnumId(id, uniqueID);

    if (m_impl->bttnGrp_gameActions->buttons().contains(bttn)) {
        m_impl->bttnGrp_gameActions->setId(bttn, uniqueID);
    } else {
        m_impl->bttnGrp_gameActions->addButton(bttn, uniqueID);
    }
    bttn->setProperty("moneyMin", moneyMin);
    bttn->setProperty("moneyMax", moneyMin + range);
//    ns_table::CourseAction action = courseAction(id, bttn);
    gbp_i64 gameID = m_impl->m_id.m_tableID;
    auto on_click = [gameID, id, bttn] {
        getRpc()->user_action(gameID, courseAction(id, bttn), [](ns_table::e_status status) {
            std::cout << (status) << std::endl;
        });
    };
    connect(bttn, &QAbstractButton::clicked, this, on_click);

}

void UserControls::registerAutoActionTrigger(QAbstractButton *bttn, const AutoAction &autoAction)
{

}

template <>
typename UiElement::ChatBrowserType::Type UserControls::uiElement(UiElement::ChatBrowserType) const {
    return m_impl->chatList;
}

void UserControls::init()
{
    if (isInitialized()) {
        return;
    }

    gbp_i64 bb = 0;
    e_game_limit limit = e_game_limit::fixed_limit;

    if (m_impl->m_id.isCash()) {
        ns_model::cash_stat_t* model = modelsDB().getCashStatistic();
        bb = model->indexOf(m_impl->m_id.m_tableID, ns_model::cash_stat_t::column::_big_blind).data(UserType::OriginDataRole).value<gbp_i64>();
    } else {
        ns_model::tournament_stat_t* model = modelsDB().getTournamentStatistic();
        bb = model->indexOf(m_impl->m_id.m_tournamentID, ns_model::tournament_stat_t::column::_big_blind).data(UserType::OriginDataRole).value<gbp_i64>();
    }

    registerCourseActionTrigger(m_impl->bttn_fold,  e_user_action::fold);
    registerCourseActionTrigger(m_impl->bttn_check, e_user_action::check);
    registerCourseActionTrigger(m_impl->bttn_bet,   e_user_action::bet, bb);
    registerCourseActionTrigger(m_impl->bttn_call,  e_user_action::call, bb/2);
    registerCourseActionTrigger(m_impl->bttn_raise, e_user_action::raise, bb);
    registerCourseActionTrigger(m_impl->bttn_allin, e_user_action::all_in, 1);

    onInitialized();
}

void UserControls::onModelDataChange(const QModelIndex& index, int role)
{
    if (!index.isValid()) {
        return;
    }
    if (role != UserType::OriginDataRole) {
        qWarning("   Loc: [%s:%i], Unhandled model update in %s for role %s", __FILELINE__, Q_FUNC_INFO, roleToString(role).toLocal8Bit().data());
        return;
    }
    if (const ns_model::table_state_t* m = qobject_cast<const ns_model::table_state_t*>(index.model()))
    {
        Q_ASSERT(m_impl->m_id.m_tableID == m->origin()._game_id);

    }
}




//struct PlayerData : public Ui::Player
//{
//    const TablePlayer* m_loggedInPlayer;

//    GameActions m_predefinedActions;
//    GameActions m_courseActions;
//    GameAction  m_triggeredAction;

//    int m_dealerIndex;

//    gbp_i8 m_roundType;
//    bool m_viewer;
//    bool m_roundActionDone;
//    gbp_i64 m_totalGameMoney;
////---------------------------------------------------
//    PlayerData(::UserControls* owner/*, const GameId& gameID*/)
//        : Ui::Player()
//        , m_loggedInPlayer(nullptr)
//        , m_predefinedActions()
//        , m_courseActions()
//        , m_triggeredAction(::UserControls::noAction())
//        , m_dealerIndex(0)
//        , m_roundType(-1)
//        , m_viewer(false)
//        , m_roundActionDone(false)
//        , m_totalGameMoney(-1)
//    {
//        Ui::Player::setupUi(owner);
//        gameControls->setCurrentIndex(0);
//    }
//};

//UserControls::Player(QWidget *parent)
//    : InitableWidget(parent)
//    , CurrentGamePlayerIdObject(ns_util::inheritedID<TableOver>(parent))
//    , m_dPtr(nullptr)
//{
//    m_dPtr = new PlayerData(this);

//}

//UserControls::Player(const GameId &gameId, QWidget *parent)
//    : InitableWidget(parent)
//    , CurrentGamePlayerIdObject(gameId)
//    , m_dPtr(nullptr)
//    , m_rev(0)
//{
//    m_dPtr = new PlayerData(this);
//}

//UserControls::~UserControls()
//{
//    if (ns_model::user_info_t* model = modelsDB().getUserInfo()) {
//        model->moveMoneyFromIngameMoney(m_dPtr->m_totalGameMoney);
//    }

//    delete m_dPtr;
//    m_dPtr = nullptr;
//}

//void UserControls::setTablePlayer(TablePlayer *tPlayer)
//{
//    addAction(tPlayer);
//}

//void UserControls::onLogin(TablePlayer *loggedInPlayUserControlTypeser)
//{
////    addAction(loggedInPlayer);
//}

//void UserControls::onLogout()
//{
//    Q_ASSERT(m_dPtr->m_loggedInPlayer);
//    removeAction(actions().first());
//}

//void UserControls::setViewer(bool enabled)
//{
//    if (m_dPtr->m_viewer != enabled)
//    {
//        m_dPtr->m_viewer = enabled;
//        updateGameControls();
//    }
//}

//void UserControls::setDealerIndex(int dealerIndex)
//{
//    m_dPtr->m_dealerIndex = dealerIndex;
//}

//bool UserControls::isLoggedIn() const {
//    return playerId() != -1;
//}
//int UserControls::seatIndex() const {
//    if (isLoggedIn()) {
//        Q_ASSERT(m_dPtr->m_loggedInPlayer);
//        return m_dPtr->m_loggedInPlayer->seatIndex();
//    }
//    return -1;
//}
//bool UserControls::isSitsOnTable() const {
//    return seatIndex() != -1;
//}
//bool UserControls::isInGame() const {
//    return isLoggedIn() && m_dPtr->m_loggedInPlayer->isInGame();
//}
//bool UserControls::isCurrent() const {
//    return isLoggedIn() && m_dPtr->m_loggedInPlayer->isCurrent();
//}
//bool UserControls::isDealer() const {
//    return seatIndex() != -1 && seatIndex() == m_dPtr->m_dealerIndex;
//}


//bool UserControls::isViewer() const
//{
//    return (!m_dPtr->m_loggedInPlayer || !m_dPtr->m_loggedInPlayer->isSitOnTable());
//}

//bool UserControls::isWaiter() const
//{
//    return isLoggedIn() && waiterPos() > 0;
//}

//gbp_i16 UserControls::waitersCount() const
//{
//    waiter_t* sharedData = pApp->db().sharedDataService().getSingleDataForType<waiter_t>();
//    return sharedData->second[tableId()];
//}

//gbp_i16 UserControls::waiterPos() const
//{
//    return (m_dPtr->m_loggedInPlayer == nullptr) ? -1 : m_dPtr->m_loggedInPlayer->waiterIndex();
//}

//Money UserControls::roundBet() const {
//    Q_ASSERT(m_dPtr->m_loggedInPlayer);
//    return m_dPtr->m_loggedInPlayer->roundBet();
//}
//Money UserControls::distributionBet() const {
//    Q_ASSERT(m_dPtr->m_loggedInPlayer);
//    return m_dPtr->m_loggedInPlayer->distributionBet();
//}
//Money UserControls::gameMoney() const {
//    Q_ASSERT(m_dPtr->m_loggedInPlayer);
//    return m_dPtr->m_loggedInPlayer->gameMoney();
//}

//Money UserControls::totalGameMoney() const {
//    return m_dPtr->m_totalGameMoney;
//}

////gbp_i64 Player::takeMoney()
////{
////    gbp_i64 amount = m_dPtr->m_totalGameMoney;
////    m_dPtr->m_totalGameMoney -= amount;
////    return amount;
////}

//void UserControls::triggerCourseAction()
//{
//    Q_ASSERT(isCurrent());
//    m_dPtr->m_roundActionDone = true;

//    getRpc()->user_action(tableId(), m_dPtr->m_triggeredAction);
//}

//const GameAction &UserControls::triggeredAction() const
//{
//    return m_dPtr->m_triggeredAction;
//}

//void UserControls::updateGameControls(const QMetaProperty &hint)
//{
//    const TablePlayer* pl = m_dPtr->m_loggedInPlayer;

//    if (pl == nullptr) {
////        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_unloginned);
//        m_dPtr->gameControls->setCurrentWidget((ns_helpers::freeSeatsCount(gameId()) == 0)
//                                               ? m_dPtr->page_viewer_full_table
//                                               : m_dPtr->page_viewer_can_sit);
//    } else if (isWaiter()) {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_waiter);
//    } else if (!pl->isSitOnTable()) {
//        m_dPtr->gameControls->setCurrentWidget((ns_helpers::freeSeatsCount(gameId()) == 0)
//                                               ? m_dPtr->page_viewer_full_table
//                                               : m_dPtr->page_viewer_can_sit);
//    } else {
//        onStatusChanged(hint);
//    }
//}

//void UserControls::onStatusChanged(const QMetaProperty &hint)
//{
//    const TablePlayer* pl = m_dPtr->m_loggedInPlayer;

//    QString hName(hint.name());
//    bool value = hint.read(pl).toBool();

//    if ((hName == "online" || hName == "frozen") && value) {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_frozen);
//    } else if (hName == "sitOut" && value) {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_sitOut);
//    } else if (hName == "ingame" && value) {
//        if (canPredefineCourse()) {
//            m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_autoActions);
//        } else {
//            m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_empty);
//        }
//    } else if (hName == "folded" && value) {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_folded);
//    } else if (hName == "current") {
//        if (value) {
//            m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_course);
//            activateGameActions();
//        } else {
//            const GameAction& course = pl->lastCourse();
//            if (course.first == e_user_action::fold) {
//                m_dPtr->m_roundActionDone = true;
//                m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_folded);
//            } else if (course.first == e_user_action::blind || course.first == e_user_action::guest_bet) {
//                m_dPtr->m_roundActionDone = false;
//                m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_autoActions);
//            } else {
//                m_dPtr->m_roundActionDone = true;
//                m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_empty);
//            }
//        }
//    }
//}

//void UserControls::onMoneyChanged(const QMetaProperty &hint)
//{
//    const TablePlayer* pl = m_dPtr->m_loggedInPlayer;

//    QString hName(hint.name());
////    Money m = qvariant_cast<Money>(hint.read(pl));

//    if (hName == "gameMoney") {
//        gbp_i64 thisTableInGameMoney = pl->totalIngameMoney();
//        if (m_dPtr->m_totalGameMoney != thisTableInGameMoney)
//        {
//            ns_model::user_info_t* usrInfoModel = modelsDB().getUserInfo();

//            if (m_dPtr->m_totalGameMoney == -1) {
//                m_dPtr->m_totalGameMoney = pl->gameMoney();
////                usrInfoModel->moveMoneyToIngameMoney(m_dPtr->m_totalGameMoney);
//            } else {
//                if (pl->roundIndex() == 0) {
//                    gbp_i64 diff = pl->gameMoney() - m_dPtr->m_totalGameMoney;
//                    gbp_i64 totalInGameMoney = usrInfoModel->money(ns_model::user_info_t::field::_in_game_money);
////                    usrInfoModel->updateIngameMoney(totalInGameMoney + diff);
//                    m_dPtr->m_totalGameMoney = pl->gameMoney();
//                }
//            }
//        }
//    }
//}

//void UserControls::onIndexChanged(const QMetaProperty &hint)
//{
//    const TablePlayer* pl = m_dPtr->m_loggedInPlayer;

//    QString hName(hint.name());
//    gbp_i16 index = qvariant_cast<gbp_i16>(hint.read(pl));

//    if (hName == "seatIndex")
//    {
//        if (index == -1) {
//            m_dPtr->gameControls->setCurrentWidget((ns_helpers::freeSeatsCount(gameId()) == 0)
//                                                   ? m_dPtr->page_viewer_full_table
//                                                   : m_dPtr->page_viewer_can_sit);
//        } else {
//            m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_empty);
//        }
//    }
//    else if (hName == "roundIndex")
//    {
//        setRound(index, ns_util::toString(gbp::ns_api::ns_table::e_game_round_type(index)));
//    }
//    else if (hName == "waiterIndex")
//    {
//        if (index == -1) {
//            m_dPtr->gameControls->setCurrentWidget((ns_helpers::freeSeatsCount(gameId()) == 0)
//                                                   ? m_dPtr->page_viewer_full_table
//                                                   : m_dPtr->page_viewer_can_sit);
//        } else {
//            m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_waiter);
//        }
//        setWaitersInfo(waitersCount(), index);
//    }
//}


//void UserControls::showHandButtons()
//{
//    m_dPtr->hand_buttons->show();
//}

//void UserControls::setCurrentActivePlayer(const std::vector<available_user_course_t> &actions, const action_info_t &action_info, Money commonPot)
//{

//    DEBUG.assertX(isCurrent(), __FILELINE__) << "isCurrent";
//    setRound(action_info._current_round, "round");

//    if (QAbstractButton* bttn = m_dPtr->bttnGrp_autoActions->checkedButton()) {
//        int id = m_dPtr->bttnGrp_autoActions->checkedId();
////        e_user_action actionType = e_user_action(m_dPtr->bttnGrp_autoActions->checkedId());

//        if (m_dPtr->bttnGrp_autoActions->checkedButton() == m_dPtr->bttn_autoFold) {
//            m_dPtr->m_triggeredAction = std::make_pair(e_user_action::fold, 0);
//            triggerCourseAction();
//            clearPredefinedActions();
//            return;
//        }

//        if (m_dPtr->bttnGrp_autoActions->checkedButton() == m_dPtr->bttn_autoCheckFold) {
//            for (const available_user_course_t& course : actions) {
//                if (course._ua == e_user_action::check) {
//                    m_dPtr->m_triggeredAction = std::make_pair(e_user_action::check, 0);
//                    triggerCourseAction();
//                    clearPredefinedActions();
//                    return;
//                }
//            }
//            m_dPtr->m_triggeredAction = std::make_pair(e_user_action::fold, 0);
//            triggerCourseAction();
//            clearPredefinedActions();
//            return;
//        }

//        for (const available_user_course_t& course : actions) {
//            if (course._ua == e_user_action::fold)
//            {
//                continue;
//            }
//            if ((1 << underlying_cast(course._ua)) & id) {
//                auto it = m_dPtr->m_predefinedActions.find(course._ua);
//                if (it != m_dPtr->m_predefinedActions.end()) {
//                    if (it->second >= course._min && it->second <= course._max) {
////                        m_dPtr->m_triggeredAction = *it;
//                        m_dPtr->m_triggeredAction = *it;
//                        triggerCourseAction();
//                        clearPredefinedActions();
//                        return;
//                    }
//                }
//            }
//        }
//    }

//    clearPredefinedActions();

//    m_dPtr->betControls->setEnabled(false);

//    GameActions courseActions;
//    for (const available_user_course_t& course: actions)
//    {
//        GameAction courseAction = std::make_pair(course._ua, course._min);
//        courseActions.insert(courseAction);
//        if (course._min < course._max)
//        {
//            Q_ASSERT(course._ua == e_user_action::bet || course._ua == e_user_action::raise);
//            m_dPtr->betControls->setEnabled(true);
//            m_dPtr->betControls->setRange(course._min, course._max);
//            m_dPtr->betControls->setValue(courseAction.second);

//            const auto &moneyInfo = action_info._money_info;
//            if (action_info._current_round == 0 && moneyInfo._current_bet <= moneyInfo._default_bet) {
//                m_dPtr->betControls->enableBlindButtons(moneyInfo._default_bet, moneyInfo._user_bet);
//            } else {
//                m_dPtr->betControls->enablePotButtons( ns_helpers::potBet(commonPot, moneyInfo._current_bet)
//                                                     , ns_helpers::halfPotBet(commonPot, moneyInfo._current_bet));
//            }
//        }
//    }
//    setCourseActions(courseActions);

//    m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_course);
//}

void UserControls::setPredefinedInfo(gbp_i64 defaultBet, gbp_i64 currentBet)
{
//   Q_ASSERT(m_dPtr->m_loggedInPlayer);

//   if (!isInGame()) {
//       return;
//   }

//   gbp_i64 diffToCall = currentBet - roundBet();
//   if (diffToCall > 0 && !gameMoney().isZero()) {
//       m_dPtr->m_roundActionDone = false;
//   }

//   GameActions& actions = m_dPtr->m_predefinedActions;
//   if (!canPredefineCourse()) {
//       clearPredefinedActions();
//   } else {
//       m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_autoActions);
//       GameAction preAction = std::make_pair(e_user_action::fold, 0);
//       auto it = actions.find(preAction.first);

//       if (it == actions.end()) {
//           m_dPtr->bttn_autoFold->show();
//           actions.insert(preAction);
//       }

//       auto limit = ns_helpers::gameLimit(tableId());

//       if (diffToCall > 0)
//       {
//           gbp_i64 cashAfterCall = gameMoney() - diffToCall;
//           if (cashAfterCall > 0)
//           {
//               preAction = std::make_pair(e_user_action::call, diffToCall);
//               it = actions.find(preAction.first);
//               if (it == actions.end())
//               {
//                   actions.insert(preAction);
//                   m_dPtr->bttn_autoCall->show();
//                   m_dPtr->stackOfAutoCall->setCurrentWidget(m_dPtr->page_autoCall);
//                   m_dPtr->bttn_autoCall->setText(tr("@bttn_autoCall_%0").arg(Money(preAction.second).toString()));

//                   it = actions.find(e_user_action::check);
//                   if (it != actions.end()) {
//                       if (m_dPtr->bttn_autoCheck->isChecked()) {
//                           m_dPtr->bttn_autoCheck->click();
//                       }
//                       if (m_dPtr->bttn_autoCheckFold->isChecked()) {
//                           m_dPtr->bttn_autoFold->setChecked(true);
//                       }
//                       if (m_dPtr->bttn_autoCheckCallAny->isChecked()) {
//                           m_dPtr->bttn_autoCall->setChecked(true);
//                       }

//                       m_dPtr->bttn_autoCheck->hide();
//                       m_dPtr->bttn_autoCheckFold->hide();
//                       m_dPtr->bttn_autoCheckCallAny->hide();
//                       actions.erase(it);
//                   }
//               }
//               else
//               {
//                   if (it->second != preAction.second) {
//                       m_dPtr->bttn_autoCall->setText(tr("@bttn_autoCall_%0").arg(Money(preAction.second).toString()));
//                       it->second = preAction.second;
//                       if (m_dPtr->bttn_autoCall->isChecked()) {
//                           m_dPtr->bttn_autoCall->click();
//                       }
//                   }
//               }

//               if (limit == e_game_limit::fixed_limit) {
//                   gbp_i64 maxBet = defaultBet * 4;
//                   if (currentBet < maxBet)
//                   {
//                       if (cashAfterCall >= defaultBet)
//                       {
//                           preAction = std::make_pair(e_user_action::raise, diffToCall + defaultBet);

//                           it = actions.find(preAction.first);
//                           if (it == actions.end())
//                           {
//                               actions.insert(preAction);
//                               m_dPtr->stackOfAutoBetRaise->setCurrentWidget(m_dPtr->page_autoRaise);
//                               m_dPtr->bttn_autoRaise->setText(tr("@bttn_autoRaise_%0").arg(Money(preAction.second).toString()));
//                               m_dPtr->bttn_autoRaise->show();
//                               m_dPtr->bttn_autoRaiseAny->show();

//                               it = actions.find(e_user_action::bet);
//                               if (it != actions.end())
//                               {
//                                   if (m_dPtr->bttn_autoBet->isChecked()) {
//                                       m_dPtr->bttn_autoBet->click();
//                                   }
//                                   m_dPtr->bttn_autoBet->hide();
//                                   actions.erase(it);
//                                   if (m_dPtr->bttn_autoBetRaiseAny->isChecked()) {
//                                       m_dPtr->bttn_autoRaiseAny->setChecked(true);
//                                   }
//                               }
//                           }
//                           else
//                           {
//                               if (it->second != preAction.second) {
//                                   m_dPtr->bttn_autoRaise->setText(tr("@bttn_autoRaise_%0").arg(Money(preAction.second).toString()));
//                                   it->second = preAction.second;
//                                   if (m_dPtr->bttn_autoRaise->isChecked()) {
//                                       m_dPtr->bttn_autoRaise->click();
//                                   }
//                               }
//                           }
//                       }
//                   }
//                   else /* currentBet == maxBet: no RAISE actions available */
//                   {
//                       m_dPtr->stackOfAutoBetRaise->setCurrentIndex(0);
//                       uint flag = ActionFlag::bet | ActionFlag::raise;
//                       if (m_dPtr->bttnGrp_autoActions->checkedId() & flag) {
//                           m_dPtr->bttn_uncheckAutoActions->setChecked(true);
//                       }
//                   }
//               }
//           }
//           else if (gameMoney().isPositive())/*limit != e_game_limit::fixed_limit*/
//           {
//               preAction = std::make_pair(e_user_action::all_in, gameMoney());
//               it = actions.find(preAction.first);
//               if (it == actions.end()) {
//                   actions.insert(preAction);
//               }
//           }
//       }
//       else if (diffToCall == 0)
//       {
//           preAction = std::make_pair(e_user_action::check, 0);
//           it = actions.find(preAction.first);
//           if (it == actions.end()) {
//               actions.insert(preAction);
//           }
//           m_dPtr->bttn_autoCheckCallAny->show();
//           m_dPtr->bttn_autoCheckFold->show();
//           m_dPtr->bttn_autoCheck->show();

//           if (limit == e_game_limit::fixed_limit && gameMoney().isPositive())
//           {
//               if (gameMoney() >= defaultBet) {
//                   if (roundBet().isZero())
//                   {
//                       preAction = std::make_pair(e_user_action::bet, defaultBet);
//                       it = actions.find(preAction.first);
//                       if (it == actions.end())
//                       {
//                           actions.insert(preAction);
//                           m_dPtr->stackOfAutoBetRaise->setCurrentWidget(m_dPtr->page_autoBetRaise);
//                           m_dPtr->bttn_autoBet->show();
//                           m_dPtr->bttn_autoBetRaiseAny->show();
//                           m_dPtr->bttn_autoBet->setText(tr("@bttn_autoBet_%0").arg(Money(preAction.second).toString()));
//                       }
//                       else if (it->second != preAction.second)
//                       {
//                           it->second = preAction.second;
//                           if (m_dPtr->bttn_autoBet->isChecked()) {
//                               m_dPtr->bttn_autoBet->click();
//                           }
//                       }
//                   }
//                   else
//                   {
//                       preAction = std::make_pair(e_user_action::raise, defaultBet);
//                       it = actions.find(preAction.first);
//                       if (it == actions.end())
//                       {
//                           actions.insert(preAction);
//                           m_dPtr->stackOfAutoBetRaise->setCurrentWidget(m_dPtr->page_autoRaise);
//                           m_dPtr->bttn_autoRaise->show();
//                           m_dPtr->bttn_autoRaiseAny->show();
//                           m_dPtr->bttn_autoRaise->setText(tr("@bttn_autoRaise_%0").arg(Money(preAction.second).toString()));
//                       }
//                       else if (it->second != preAction.second)
//                       {
//                           it->second = preAction.second;
//                           if (m_dPtr->bttn_autoRaise->isChecked()) {
//                               m_dPtr->bttn_autoRaise->click();
//                           }
//                       }
//                   }
//               }
////                auto limit = ns_helpers::gameLimit(id().m_gameID.m_tableID);
////                if (limit != e_game_limit::fixed_limit || userCash <= defaultBet) {
////                    actions.insert(std::make_pair(e_user_action::all_in, userCash));
////                }
//           }
//       }
//   }
////    setPredefinedActions(actions);

////    for (const GameAction& course: actions)
////    {

////    }
}

//void UserControls::setSitOutFromNextDistribution(bool checked)
//{
//    static const char* sheet = "::indicator:checked[theme=\"default\"] {image: url(:/textures/game/icon_check_govnocolor.png);}"
//                               "::indicator:!checked[theme=\"default\"] {image: url(:/textures/game/icon_check.png);}";
//    m_dPtr->bttn_checkboxSitOut->setStyleSheet(checked ? sheet : "");
//}


//void UserControls::toViewer(bool /*distributionsReason*/, bool /*rechargeReason*/)
//{
//    setViewer(true);
//    if (ns_helpers::freeSeatsCount(id().m_gameID) > 0) {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_viewer_can_sit);
//    } else {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_viewer_full_table);
//    }
//}

//QString UserControls::takeChatMessage()
//{
//    QString msg = m_dPtr->msgLineEdit->text();
//    m_dPtr->msgLineEdit->clear();
//    return msg;
//}

//QAbstractButton *UserControls::getControl(const QString &name) const {
//    return findChild<QAbstractButton*>(name);
//}

//GameAction UserControls::takeTriggeredAction()
//{
//    auto trAction = m_dPtr->m_triggeredAction;
//    m_dPtr->m_triggeredAction = noAction();
//    return std::move(trAction);
//}

//GameActions UserControls::courseActions() const {
//    return m_dPtr->m_courseActions;
//}

//GameActions UserControls::predefinedActions() const {
//    return m_dPtr->m_predefinedActions;
//}

//void UserControls::setCourseActions(const GameActions& actions) {
////    m_dPtr->m_predefinedActions.clear();
//    m_dPtr->m_courseActions = actions;
//    if (m_dPtr->m_loggedInPlayer->isCurrent()) {
//        activateGameActions();
//        // TODO: должен тригерить из GameHandler
////        m_dPtr->m_loggedInPlayer->toggle();
//    }
//}

//void UserControls::setPredefinedActions(const GameActions& actions) {
//    QButtonGroup* group = m_dPtr->bttnGrp_autoActions;
//    if (group->checkedButton() != nullptr && group->checkedButton() != m_dPtr->bttn_uncheckAutoActions) {
//        if (actions.find(e_user_action(group->checkedId())) == actions.end()) {
//            m_dPtr->bttn_uncheckAutoActions->toggle();
//        }
//    }
//}

//bool UserControls::canPredefineCourse() const {
//    return isInGame() && !isCurrent() && (!m_dPtr->m_roundActionDone);
//}

//void UserControls::setGuestBlindChecked(bool checked)
//{
//    m_dPtr->msg_guestButtons->setCurrentMsgIndex(checked ? 0 : 1);
//    m_dPtr->m_triggeredAction = checked ? std::make_pair(e_user_action::guest_bet, ns_helpers::bigBlind(id().m_gameID)) : noAction();
//}

//void UserControls::enableMask()
//{
////    static const QPixmap maskPixmap(":textures/mockups/mask.png");

//    QPixmap currentView(size());
//    currentView.fill(Qt::transparent);
//    if (!property("mask").isValid()) {
//        render(&currentView, QPoint(), QRegion(), QWidget::DrawChildren);
//    } else {
//        QPainter p(&currentView);
//        p.drawPixmap(rect(), qvariant_cast<QPixmap>(property("mask")));
//    }

//    setMask(currentView.createMaskFromColor(Qt::black, Qt::MaskInColor));
//}

//void UserControls::clearPredefinedActions()
//{
//    m_dPtr->m_predefinedActions.clear();
//    for (QAbstractButton* bttn: m_dPtr->bttnGrp_autoActions->buttons()) {
//        bttn->hide();
//    }
//    m_dPtr->bttn_uncheckAutoActions->show();
//    m_dPtr->bttn_uncheckAutoActions->setChecked(true);

//    m_dPtr->stackOfAutoCall->setCurrentIndex(0);
//    m_dPtr->stackOfAutoBetRaise->setCurrentIndex(0);

//    m_dPtr->page_autoActions->setProperty("checkedButtonName", "");
//}

//void UserControls::setRound(int index, const QString &name)
//{
//    if (m_dPtr->m_roundType == index) {
//        return;
//    }
//    m_dPtr->m_roundType = index;
//    if (index == 0) {
//        m_dPtr->m_roundActionDone = false;
//    } else {
////        if (isInGame()) {
////            m_dPtr->m_roundActionDone = false;
////        }
//    }
//    m_dPtr->roundType->setText(QString("%0. %1").arg(index + 1).arg(name));
//}

//void UserControls::setWaitersInfo(gbp_i16 count, gbp_i16 pos)
//{
//    m_dPtr->msg_queueSize->setText(tr("@msg_queueSize_%0").arg(count));
//    m_dPtr->msg_posInQueue->setText(tr("@msg_pos_%0_in_queue_%1").arg(pos).arg(count));
//}

//void UserControls::toggleChatVisibility(bool bttnState)
//{
//    QPoint shift(0, -86);
//    if (bttnState) {
//        shift.setY(86);
//    }
//    QPropertyAnimation *anim = ns_anim::moveTo(m_dPtr->chat, m_dPtr->chat->pos() + shift, 300);
//    anim->setEasingCurve(QEasingCurve::OutBack);
//    anim->start(QAbstractAnimation::DeleteWhenStopped);
//}

//void UserControls::showSmiles()
//{
//    m_dPtr->smileList->show();
//    m_dPtr->smileList->setProperty("originGeometry", m_dPtr->smileList->geometry());
//    int w = 300;//m_ui->smileList->iconSize().width() * m_ui->smileList->model()->columnCount();
//    int h = 200;//m_ui->smileList->iconSize().height() * m_ui->smileList->model()->rowCount() / (w / m_ui->smileList->iconSize().width());
//    int x = m_dPtr->smileList->x() + m_dPtr->smileList->width() - w;
//    int y = m_dPtr->smileList->y() + m_dPtr->smileList->height() - h;

//    QPropertyAnimation *anim = ns_anim::changeGeometry(m_dPtr->smileList, QRect(x, y, w, h), 350);
//    connect(anim, &QPropertyAnimation::finished, this, [this]{m_dPtr->smileList->doItemsLayout();});
//    anim->setEasingCurve(QEasingCurve::OutBack);
//    anim->start(QAbstractAnimation::DeleteWhenStopped);
//}

//void UserControls::hideSmiles()
//{
//    QPropertyAnimation *anim = ns_anim::changeGeometry(m_dPtr->smileList, m_dPtr->smileList->property("originGeometry").toRect(), 350);
//    connect(anim, &QPropertyAnimation::finished, m_dPtr->smileList, &QListView::hide);
//    anim->setEasingCurve(QEasingCurve::OutBack);
//    anim->start(QAbstractAnimation::DeleteWhenStopped);
//}

//bool UserControls::askBlind(gbp_i64 /*blind*/, bool isBB)
//{
//    return !(isBB && m_dPtr->bttn_checkboxSitOutOnBlind->isChecked());
//}

//void UserControls::enableGuestBlind()
//{
//    m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_frozen);
//}

//void UserControls::disableGuestBlind()
//{
//    if (m_dPtr->bttn_makeGuestBet->isChecked()) {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_empty);
//    } else {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_frozen);
//    }
//}

//void UserControls::addSitOnTableTrigger(QAbstractButton *trigger, gbp_i8 pos)
//{
//    connect(trigger, &QAbstractButton::clicked, this, [this, pos]
//    {
//        if (isViewer()) {
//            if (loginIfNeeded()) {
//                getRpc()->from_viewer_to_player(tableId(), pos);
//            }
//        } else {
//            getRpc()->sit_on_table(tableId(), pos);
//        }
//    });
//}

//void UserControls::insertSmileToChat(QModelIndex index)
//{
//    QString smileSign = m_dPtr->smileList->model()->data(index, Qt::UserRole).toString();
//    m_dPtr->msgLineEdit->setText(m_dPtr->msgLineEdit->text() + smileSign);
//    hideSmiles();
//}

//void UserControls::activateGameActions()
//{
//    int indexCenter = m_dPtr->stackOfGameActionsCenter->count() - 1;
//    int indexRight = m_dPtr->stackOfGameActionsRight->count() - 1;
//    for (const GameAction& action: m_dPtr->m_courseActions) {
//        int id = underlying_cast(action.first);
//        QAbstractButton* bttn = m_dPtr->bttnGrp_gameActions->button(id);
//        Money money = action.second;

//        Q_ASSERT(bttn);
//        if (ns_widget::MoneyButton* moneyBttn = qobject_cast<ns_widget::MoneyButton*>(bttn)) {
//            moneyBttn->setMoney(money);
//        }
//        if (m_dPtr->stackOfGameActionsCenter->isAncestorOf(bttn)) {
//            indexCenter = std::min(indexCenter, m_dPtr->stackOfGameActionsCenter->indexOf(bttn->parentWidget()));
//        } else if(m_dPtr->stackOfGameActionsRight->isAncestorOf(bttn)) {
//            indexRight = std::min(indexRight, m_dPtr->stackOfGameActionsRight->indexOf(bttn->parentWidget()));
//        }
//    }
//    m_dPtr->stackOfGameActionsCenter->setCurrentIndex(indexCenter);
//    m_dPtr->stackOfGameActionsRight->setCurrentIndex(indexRight);
//}

//void UserControls::init()
//{
//    if (isInitialized())
//        return;

//   // ns_debug_util::initDebugOnScreen(this, m_dPtr);

//    m_dPtr->toolbar->init();

//    ns_model::ChatSmilesList* smilesModel = new ns_model::ChatSmilesList(m_dPtr->smileList);
//    m_dPtr->smileList->setModel(smilesModel);
//    smilesModel->setSource(qvariant_cast<QPixmap>(m_dPtr->smileList->property("source")), m_dPtr->smileList->iconSize());
//    connect(m_dPtr->smileList, &QListView::clicked, this, &UserControls::insertSmileToChat);

//    connect(m_dPtr->msgLineEdit, &QLineEdit::returnPressed, this, &UserControls::sendMsg);

//    m_dPtr->betControls->init();
//    connect(m_dPtr->betControls, &ns_panel::BetControls::valueChanged, this, [this](Money m){
//        auto it = m_dPtr->m_courseActions.find(e_user_action::bet);
//        if (it == m_dPtr->m_courseActions.end()) {
//            it = m_dPtr->m_courseActions.find(e_user_action::raise);
//            if (it == m_dPtr->m_courseActions.end()) {
//                return;
//            }
//        }
//        it->second = m;
//        if (QAbstractButton* bttn = m_dPtr->bttnGrp_gameActions->button(underlying_cast(it->first))) {
//            bttn->setProperty("money", QVariant::fromValue(m));
//        }
//    });


//    m_dPtr->bttnGrp_gameActions->setId(m_dPtr->bttn_allin, underlying_cast(e_user_action::all_in));
//    m_dPtr->bttnGrp_gameActions->setId(m_dPtr->bttn_bet,   underlying_cast(e_user_action::bet));
//    m_dPtr->bttnGrp_gameActions->setId(m_dPtr->bttn_call,  underlying_cast(e_user_action::call));
//    m_dPtr->bttnGrp_gameActions->setId(m_dPtr->bttn_check, underlying_cast(e_user_action::check));
//    m_dPtr->bttnGrp_gameActions->setId(m_dPtr->bttn_fold,  underlying_cast(e_user_action::fold));
//    m_dPtr->bttnGrp_gameActions->setId(m_dPtr->bttn_raise, underlying_cast(e_user_action::raise));

//    auto limit = ns_helpers::gameLimit(tableId());

//    m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoFold,        ActionFlag::fold);
//    m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoCheckFold,   ActionFlag::fold | ActionFlag::check);
//    m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoCheck,       ActionFlag::check);
//    m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoCall,        ActionFlag::call);
//    m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoCheckCallAny,ActionFlag::check | ActionFlag::call);

//    if (limit == e_game_limit::fixed_limit)
//    {
//        m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoBet,         ActionFlag::bet);
//        m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoBetRaiseAny, ActionFlag::bet | ActionFlag::raise);
//        m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoRaise,       ActionFlag::raise);
//        m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_autoRaiseAny,    ActionFlag::raise | ActionFlag::all_in);
//    }
//    m_dPtr->bttnGrp_autoActions->setId(m_dPtr->bttn_uncheckAutoActions, ActionFlag::last_flag << 1);

//    connect(m_dPtr->bttnGrp_autoActions, static_cast<void(QButtonGroup::*)(QAbstractButton*, bool)>(&QButtonGroup::buttonToggled), this
//    , [this](QAbstractButton* /*bttn*/, bool /*checked*/) {
//        m_dPtr->bttn_uncheckAutoActions->setText(m_dPtr->bttn_uncheckAutoActions->isChecked() ? "No preAction" : m_dPtr->bttnGrp_autoActions->checkedButton()->objectName());
//    });


//    connect(m_dPtr->bttnGrp_autoActions, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this
//    , [this](QAbstractButton* bttn) {
//        if (m_dPtr->page_autoActions->property("checkedButtonName").toString() == bttn->objectName()) {
//            m_dPtr->bttn_uncheckAutoActions->setChecked(true);
//        }
//        m_dPtr->page_autoActions->setProperty("checkedButtonName", m_dPtr->bttnGrp_autoActions->checkedButton()->objectName());
//    });


//    m_dPtr->bttn_uncheckAutoActions->setAttribute(Qt::WA_TransparentForMouseEvents, true);

//    connect(m_dPtr->bttn_quit, &QPushButton::clicked, this, [this] {
////        getRpc()->close_table(tableId());
//    });
//    connect(m_dPtr->bttn_joinQueue, &QPushButton::clicked, this, [this] {
//        if (loginIfNeeded()) {
//            getRpc()->add_waiter(tableId());
//        }
//    });
//    connect(m_dPtr->bttn_leaveQueue, &QPushButton::clicked, this, [this] {
//        getRpc()->remove_from_waiters(tableId());
//    });

//    connect(m_dPtr->bttn_backToGame, &QPushButton::clicked, this, [this] {
//        getRpc()->user_online(tableId());
//    });
//    connect(m_dPtr->bttn_checkboxSitOut, &QCheckBox::toggled, this, [this](bool checked) {
//        getRpc()->set_sit_out(tableId(), checked);
//    });
//    connect(m_dPtr->bttn_checkboxSitOutOnBlind, &QCheckBox::toggled, this, [this](bool checked) {
//        if (!checked)
//        {
//            if (m_dPtr->m_loggedInPlayer && m_dPtr->m_loggedInPlayer->isSitOut()) {
//                m_dPtr->bttn_backToGame->click();
//            }
//            if (m_dPtr->bttn_checkboxSitOut->isChecked()) {
//                m_dPtr->bttn_checkboxSitOut->setChecked(false);
//            }
//        }
//    });

//    connect(m_dPtr->bttn_waitBigBlind, &QCheckBox::toggled, this, &UserControls::toggleGuestBlind);
//    connect(m_dPtr->bttn_lobby, SIGNAL(clicked()), this, SIGNAL(lobbyOpenRequested()));

////    onSettingsChanged();
//    QWidgetList lst = findChildren<QWidget*>();
//    for (QWidget* w: lst) {
//        w->setProperty("theme", "default");
//    }
//    for (QWidget* w: lst) {
//        w->setStyleSheet(w->styleSheet());
//    }

////    QRect geom = m_dPtr->chatScrollBar->geometry();
////    QWidget* par = m_dPtr->chatScrollBar->parentWidget();
////    m_dPtr->chatList->setVerticalScrollBar(m_dPtr->chatScrollBar);
////    QTimer::singleShot(500, Qt::CoarseTimer, [this, par, geom] {
//////        m_dPtr->chatScrollBar->setParent(par);
////        m_dPtr->chatScrollBar->move(m_dPtr->chatList->width() - m_dPtr->chatScrollBar->width(), 10);
////        m_dPtr->chatScrollBar->show();
////        m_dPtr->chatScrollBar->raise();
////    });

//    if (limit == e_game_limit::fixed_limit) {
//        m_dPtr->betControls->hide();
//    }

//#ifndef ENABLE_VISUAL_DEBUGGING
//    m_dPtr->bttn_uncheckAutoActions->hide();
//#else
//    m_dPtr->bttn_uncheckAutoActions->show();
//#endif // !ENABLE_VISUAL_DEBUGGING

//    onInitialized();
//}

//QTextBrowser *UserControls::chatBrowser() const {
//    return  m_dPtr->chatList;
//}

//QWidget *UserControls::handButtons() const {
//    return m_dPtr->hand_buttons;
//}

//QLabel *UserControls::distributionNumber() const {
//    return m_dPtr->distributionNumber;
//}

//void UserControls::sendMsg()
//{
//    if (!m_dPtr->msgLineEdit->text().isEmpty()) {
//#ifdef ENABLE_VISUAL_DEBUGGING
//        static bool cheatMode = false;
//        QString text = m_dPtr->msgLineEdit->text();
//        if (QRegularExpression("^--cheatmode (true|false|enabled|disabled|0|1)$").match(text).hasMatch())
//        {
//            cheatMode = QRegularExpression("^--cheatmode (true|enabled|1)$").match(text).hasMatch();

//            QTextBrowser* chatBrowser = m_dPtr->chatList;
//            QString msg = QString("<span style = \"color: #ff0000; font-weight: 600; text-decoration: underline\">cheat mode is %0</span></body></html>").arg(cheatMode ? "enabled" : "disabled");
//            QString html = chatBrowser->toHtml().replace("</body></html>", msg);
//            chatBrowser->setHtml(html);
//            chatBrowser->moveCursor(QTextCursor::End);

//            m_dPtr->msgLineEdit->clear();
//            return;
//        }
//        if (cheatMode)
//        {
//            QString result = pApp->cheatRequest(takeChatMessage());

//            QTextBrowser* chatBrowser = m_dPtr->chatList;
//            QString msg = QString("<span style = \"color: #7755ac; font-weight: 600;\">%0</span></body></html>").arg(result);
//            QString html = chatBrowser->toHtml().replace("</body></html>", msg);
//            chatBrowser->setHtml(html);
//            chatBrowser->moveCursor(QTextCursor::End);
//            return;
//        }

//#endif //ENABLE_VISUAL_DEBUGGING
//        getRpc()->send_chat_message(m_gameID.m_tableID, m_dPtr->msgLineEdit->text());
//    }
//}

//void UserControls::setPlayersCourse(int index)
//{
//    e_user_action action = e_user_action(index);

//    ns_widget::MoneyButton* bttn = qobject_cast<ns_widget::MoneyButton*>(m_dPtr->bttnGrp_gameActions->button(index));


//    auto it = m_dPtr->m_courseActions.find(action);
//    if (it != m_dPtr->m_courseActions.end())
//    {
//        if (it->first == e_user_action::bet || it->first == e_user_action::raise) {
//            it->second = bttn->money();
//        }
//        m_dPtr->m_triggeredAction = *it;
////        it->second = (bttn) ? bttn->money().ammount() : 0LL;
//        triggerCourseAction();
//    }
//    else
//    {
//        Logger::error(QString("Course action %0 with id %1 not found in the available actionlist"), __FILELINE__);
//    }
//}

//void UserControls::toggleGuestBlind(bool guestBlind)
//{
//    getRpc()->set_guest_blind(tableId(), guestBlind);
//}

//void UserControls::resetControls()
//{
//    for (QAbstractButton* bttn : m_dPtr->bttnGrp_gameActions->buttons()) {
//        bttn->hide();
//    }

////    updateBet(m_dPtr->m_bet);
//}

//void UserControls::onSettingsChanged(QAction* settings)
//{
////    SharedDataService& sharedDataService = pApp->db().sharedDataService();
////    quintptr id = sharedDataService.singleIdForType<GameTableSettingsData>();
////    GameTableSettingsData* data = sharedDataService.getData<GameTableSettingsData>(id);

////    GameTableSettingsData::Selection selection = /*(m_tournamentId == -1) ?*/ data->cashSetting(tableId())/* : data->tournamentSetting(m_id)*/;
////    QString themeName = data->selectItem(selection).theme;
//    if (settings->data().canConvert<QVariantMap>())
//    {
//        QVariantMap map = settings->data().value<QVariantMap>();
//        QString theme = map.value("theme").toString();
//        QWidgetList lst = findChildren<QWidget*>();
//        for (QWidget* w: lst) {
//            w->setProperty("theme", theme);
//        }
//        for (QWidget* w: lst) {
//            w->setStyleSheet(w->styleSheet());
//        }
//    }
//    update();

////    emit gameSettingsChanged();
//}

//bool UserControls::loginIfNeeded()
//{
//    if (!isLoggedIn()) {
////        window()->showMinimized();
//        MainWindow* mw = pApp->mainWindow();
//        mw->showNormal();
//        mw->raise();
//        if (!mw->openLoginDialog()) {
//            return false;
//        }
//        mw->showMinimized();
//        window()->raise();
////        window()->showNormal();
//    }
//    return true;
//}

void UserControls::changeEvent(QEvent *e)
{
    InitableWidget::changeEvent(e);
    qDebug() << e;
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_impl->retranslateUi(this);
//        resetControls();
        break;
    default:
        break;
    }
}



void UserControls::childEvent(QChildEvent *e)
{
    InitableWidget::childEvent(e);
    if (!e || !e->child()) {
        return;
    }
    QObject* child = e->child();
    switch (e->type()) {
    case QEvent::ChildAdded:
    if (child->isWidgetType()) {
//        m_impl->applyTheme(qobject_cast<QWidget*>(child));
    }
    break;
    case QEvent::ChildPolished:
    {
        qDebug() << child->objectName() << child->property("theme");
        if (child->isWidgetType()) {
//            child->setProperty("theme", m_impl->m_theme);
            m_impl->applyTheme(qobject_cast<QWidget*>(child));
            if (child->objectName() == "hud_low_level") {
                EntityWidget* w = qobject_cast<EntityWidget*>(e->child());
                w->stackUnder(m_impl->buttons_lefttop);
                w->stackUnder(m_impl->buttons_righttop);
            }
        }
        break;
    }
    default:
        break;
    };
}

//void UserControls::actionEvent(QActionEvent *e)
//{
//    if (const TablePlayer* pl = qobject_cast<const TablePlayer*>(e->action()))
//    {
//        switch (e->type()) {
//        case QEvent::ActionAdded:   playerAddEvent(pl); break;
//        case QEvent::ActionChanged: playerChangeEvent(pl); break;
//        case QEvent::ActionRemoved: playerRemoveEvent(pl); break;
//        default:
//            break;
//        }
//    }
//    else
//    {
//        if (e->type() == QEvent::ActionAdded)
//        {
//            if (e->action()->text() == "UpdateGameSettings") {
//                QAction* a = e->action();
//                m_dPtr->toolbar->addAction(a);
//                connect(a, &QAction::triggered, this, [this, a]{ onSettingsChanged(a); });
//            }
//        }
//        else if (e->type() == QEvent::ActionRemoved)
//        {
//            if (e->action()->text() == "UpdateGameSettings") {
//                QAction* a = e->action();
//                this->disconnect(a);
//                m_dPtr->toolbar->removeAction(a);
//            }
//        }
//        InitableWidget::actionEvent(e);
//    }
//}

//void UserControls::playerAddEvent(const TablePlayer *pl)
//{
//    DEBUG.assertX(!m_dPtr->m_loggedInPlayer, __FILELINE__) << "playerAddEvent";
////    Q_ASSERT(!m_dPtr->m_loggedInPlayer);
//    m_dPtr->m_loggedInPlayer = pl;
//    m_rev = pl->changesCountFrom(0);
//    updateGameControls();

//    connect(pl, &TablePlayer::statusChanged, this, &UserControls::onStatusChanged);
//    connect(pl, &TablePlayer::moneyChanged, this, &UserControls::onMoneyChanged);
//    connect(pl, &TablePlayer::indexChanged, this, &UserControls::onIndexChanged);

//}
//void UserControls::playerChangeEvent(const TablePlayer *pl)
//{
//    Q_ASSERT(pl == m_dPtr->m_loggedInPlayer);

//    for (const QMetaProperty& prop: m_dPtr->m_loggedInPlayer->changesFrom(m_rev)) {
//        setProperty(prop.name(), prop.read(m_dPtr->m_loggedInPlayer));
////        updateGameControls(prop);
//        m_rev++;
//    }
//}
//void UserControls::playerRemoveEvent(const TablePlayer *pl)
//{
//    Q_ASSERT(pl == m_dPtr->m_loggedInPlayer);

//    m_dPtr->m_loggedInPlayer->disconnect(this);
//    disconnect(m_dPtr->m_loggedInPlayer);
//    if (m_dPtr->m_loggedInPlayer->seatIndex() != -1) {
//        getRpc()->close_table(m_dPtr->m_loggedInPlayer->tableID());
//    } else {
//        m_dPtr->gameControls->setCurrentWidget(m_dPtr->page_unloginned);
//    }
//}


void UserControls::timelineEvent(const TimeLineEvent &ev)
{
    m_impl->lbl_timebank->setVisible(ev.type == ev.a);
    m_impl->lbl_timebank->setText(tr("@TIMEBANK\n%0").arg(ev.handler->durationFull()/1000));
}

////void Player::on_bttnGrp_gameActions_buttonClicked(QAbstractButton *button)
////{

////}

////void Player::on_bttnGrp_autoActions_buttonToggled(QAbstractButton *button, bool)
////{

////}

////void Player::on_bttnGrp_guestButtons_buttonClicked(QAbstractButton *button)
////{

////}




