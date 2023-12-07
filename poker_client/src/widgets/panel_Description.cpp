#include <precompiled.h>
#include <dialogs/dialog_okmessage.h>
#include "panel_Description.h"

#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_statistic.h>
    #include <rpc/table/rpc_table.h>
#endif // !Q_MOC_RUN

#include <helpers/helper_enum.h>

#include <core/money.h>
#include <dialogs/buyin/dialog_TournamentBuyIn.h>
#include <dialogs/dialog_authorization.h>
#include <models/model_GamesStatistic.h>
#include <qaction.h>
#include <qactiongroup.h>
#include <api_models/api_util.hpp>

Q_DECLARE_METATYPE(games_stat_t::second_type::value_type)

namespace
{
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_win_prize         = gbp::ns_api::ns_tournament_types::e_win_prize;
    using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_registration      = gbp::ns_api::ns_tournament_types::e_registration;

    rpc_table* rpcTable() { return rpc_object<rpc_table>(); }
//    rpc_statistic* rpcStatistic() { return rpc_object<rpc_statistic>(); }

}

Description::Description(QWidget *parent)
    : QFrame(parent)
    , m_actionGroup(new QActionGroup(this))
    , m_currentId(-1)
{}

Description::~Description() {}

//QAbstractItemModel *Description::model() const {
//    return m_model;
//}

//void Description::setModel(QAbstractItemModel *model)
//{
//    if (m_model != model) {
//        m_model = model;
//    }
//}

QAction* Description::createAction(ActionName name) {
    QAction *action = new QAction(ns_util::enumToString(name), m_actionGroup);
    action->setObjectName(ns_util::enumToString(name));
    m_actionGroup->addAction(action);
    m_actions.insert(name, action);
    return action;
}

void Description::initActions() {
    createAction(action_get_info);
    createAction(action_register);
    createAction(action_unregister);
    createAction(action_back_to_game);

    connect(m_actionGroup, &QActionGroup::triggered, this, &Description::onActionTriggered);

}

gbp_i64 Description::currentId() const {
    return m_currentId;
}

void Description::setCurrentId(gbp_i64 id) {
    if (m_currentId != id) {
        m_currentId = id;
        onCurrentIdChanged();
    }
}

QVariant Description::rootData() const {
    return currentIndex().data(UserType::RowDataRole);
}

void Description::openCurrentLobby()
{
    emit openLobbyRequested(currentIndex());
}

void Description::setCurrentIndex(const QModelIndex &idx)
{
    if (!idx.isValid()) {
        setCurrentId(-1);
    } else {
        setCurrentId(idx.data(UserType::RowItemIdRole).value<gbp_i64>());
    }
}

void Description::onActionTriggered(QAction *action)
{
    if (!enshureUserLoggedIn(this)) {
        return;
    }
    if (action == m_actions.value(action_get_info))
    {
        gbp_i64 id = currentId();

        rpcTable()->get_tourn_register_info(currentId(), [this, id](ns_table::e_status status, const ns_table::tourn_register_info& info){
            if (ns_helpers::isOk(status)) {
                QVariant data = rootData();

                if (data.isValid())
                {
                    auto val = data.value<games_stat_t::second_type::value_type>();
                    ns_descriptor::TournamentBuyIn desc("name", val._title.c_str(), info._user_available_money
                                                        , info._tournament_price
                                                        , info._user_tickets_count
                                                        , -1
                                                        , val._types & e_chips_rebuy::cr_rebuy);

                    auto callback = [this, &val](ns_table::e_status status
                                              , e_tourn_button_type bttn_state
                                              , e_tournament_status tourn_status) {
                        if (ns_helpers::isOk(status)) {
                            val._button_state = bttn_state;
                            val._status = tourn_status;
//                            QModelIndexList lst = rootModel()->match(QModelIndex(), UserType::RowItemIdRole, val._id);
//                            if (!lst.isEmpty()) {
//                                rootModel()->setData(lst.first(), QVariant::fromValue(val), UserType::RowDataRole);
//                            }
                        }
                    };

                    ns_dialog::TournamentBuyIn *buyin = new ns_dialog::TournamentBuyIn(GameId(-1, currentId()), callback, nullptr);
                    buyin->initWithDescriptor(&desc);
                    buyin->setAttribute(Qt::WA_DeleteOnClose);
                    buyin->open();
                }
            }
        });
    }
    else if (action == m_actions.value(action_register))
    {
//        rpcTable()->tournament_register(rootId());
    }
    else if (action == m_actions.value(action_unregister))
    {
        rpcTable()->cancel_tourn_register(currentId());
    }
    else if (action == m_actions.value(action_back_to_game))
    {
        rpcTable()->open_own_tournament(currentId());
    }
}

void Description::init()
{
    initLeft();
    initCentral();
    initPlayers();
    initTexts();

    m_actions.insert(action_get_info,     new QAction("action_get_info"));        m_actionGroup->addAction(m_actions.last());
    m_actions.insert(action_register,     new QAction("action_register"));        m_actionGroup->addAction(m_actions.last());
    m_actions.insert(action_unregister,   new QAction("action_unregister"));      m_actionGroup->addAction(m_actions.last());
    m_actions.insert(action_back_to_game, new QAction("action_back_to_game"));    m_actionGroup->addAction(m_actions.last());
}
