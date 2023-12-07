#include <precompiled.h>
#include "page_LobbyCommonInfo.h"
#include "page_Qualifiers.h"
#include "ui_page_LobbyCommonInfo.h"

#include <models/lobbyTournament/model_LobbyCommonInfo.h>
#include <models/lobbyTournament/model_QualifiersTable.h>
#include <qsortfilterproxymodel.h>
#include <utils/Utils.h>
#include <qpropertyanimation.h>
#include <animation/animation.h>

#include <api_models/model_tournament_stat_t.hpp>
#include <db/modeldatabase.h>

#include <api/lobby_stat/types.h>

namespace
{
    using e_tournament_type   = gbp::ns_api::ns_tournament_types::e_tournament_type;
    using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
}

namespace ns_page
{
    LobbyCommonInfo::LobbyCommonInfo(QWidget *parent)
        : LobbyBase(parent)
        , m_ui(new Ui::LobbyCommonInfo)
        , m_model(new ns_model::LobbyCommonInfo)
    {
        m_ui->setupUi(this);
    }

    LobbyCommonInfo::~LobbyCommonInfo() {
        delete m_ui;
    }

    QAbstractItemView *LobbyCommonInfo::view() const {
        return m_ui->tableView_info;
    }

    QAbstractItemModel *LobbyCommonInfo::model() const {
        return m_model.data();
    }
    QModelIndex LobbyCommonInfo::rootIndex() const {
        return QModelIndex();
    }

    void LobbyCommonInfo::popupQualifiers(bool show)
    {
        ns_page::Qualifiers* target = m_ui->qualifiers_frame;
        int distance = target->height() - target->headerHeight();
        QPoint destination = target->pos() + QPoint(0, show ? distance : -distance);
        QPropertyAnimation* anim = ns_anim::moveTo(target, destination, 350);
        anim->setEasingCurve(QEasingCurve::OutBack);

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }


    void LobbyCommonInfo::internalInit()
    {
        m_model->reset(m_tournamentID);
//        view()->setModel(model());

        const gbp::ns_api::ns_statistic::tournament_stat_t& data = modelsDB().getTournamentStatistic()->item(m_tournamentID).ref();

        const std::vector<gbp_i64> qualifiersIds = data._qualifiers_ids;
        m_ui->qualifiers_frame->setHidden(qualifiersIds.empty());

        if (data._types & e_win_result::wr_main)
        {
            m_ui->qualifiers_frame->init(m_tournamentID);
//            m_ui->qualifiers_frame->model().
//            m_ui->qualifiers_frame->tableModel()->refresh();
        }
    }

} //namespace ns_page
