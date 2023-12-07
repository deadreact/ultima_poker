#include <precompiled.h>
#include "updater_actions.h"

#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_table_state_t.hpp>
#include <models/model_user_additional_info.h>
#include <PokerApplication.h>
#include <QStackedWidget>
#include <qcheckbox.h>
#include <game/GameManager.h>

template <>
inline gbp_u16 get<0>(ns_action::UpdateWaiters* action) {
    return action->waitersCount();
}
template <>
inline gbp_u16 get<1>(ns_action::UpdateWaiters* action) {
    return action->userPosInQueue();
}

namespace ns_action
{
    UpdateWaiters::UpdateWaiters(gbp_i64 cashID, QObject *parent)
        : Action(parent)
        , m_cashID(cashID)
        , m_actualWaitersCount(0)
        , m_actualUserPosInQueue(0)
    {}

    UpdateWaiters::~UpdateWaiters()
    {

    }

    void UpdateWaiters::setWaitersCount(gbp_u16 count) {
        m_actualWaitersCount = count;
    }

    void UpdateWaiters::setUserPosInQueue(gbp_u16 pos) {
        m_actualUserPosInQueue = pos;
    }

    bool UpdateWaiters::updateStatistic()
    {
        ns_model::cash_stat_t* statistic_model = modelsDB().getCashStatistic();
        QModelIndex statistic_index = statistic_model->indexOf(m_cashID, ns_model::cash_stat_t::column::_wait_players);
        return statistic_model->setData(statistic_index, QVariant::fromValue(m_actualWaitersCount), Qt::EditRole);
    }

    bool UpdateWaiters::updateUser()
    {
        if (ns_model::IOModel::userId() == -1) {
            return true;
        }
        ns_model::UserInfo* usrInfoModel = modelsDB().getUserAdditionalInfo();
        QStandardItem* waiting_info = usrInfoModel->category("gameinfo")->field(ns_util::enumToString(ns_model::UserInfo::field::waiter_pos));
        QVariant d = waiting_info->data();
        QMap<gbp_i64, gbp_u16> wInfo;
        if (!d.isValid()) {
            waiting_info->setData(QVariant::fromValue(wInfo));
        } else {
            wInfo = qvariant_cast<QMap<gbp_i64, gbp_u16>>(d);
        }
        wInfo[m_cashID] = m_actualUserPosInQueue;
        waiting_info->setData(QVariant::fromValue(wInfo));
        return true;
    }

    bool UpdateWaiters::updateTable()
    {
        if (pApp->gameManager()->isTableCreated(m_cashID)) {
            auto* m = modelsDB().getTableState(m_cashID);
            return m->setOriginData(m->index_waiters_count(), m_actualWaitersCount);
        }
        return true;
    }

    bool UpdateWaiters::updateViews()
    {
        return true;
    }

    void UpdateWaiters::execute() {
        emit done(updateStatistic() && updateUser() && updateTable() && updateViews());
    }


} // namespace ns_action
