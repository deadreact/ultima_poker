#include <precompiled.h>
#include "dialog_LobbyBase.h"
#include <core/money.h>
#include <utils/logger.h>
#include <db/modeldatabase.h>
#include <dialogs/dialog_authorization.h>
#include <helpers/helper_enum.h>

#ifndef Q_MOC_RUN
#endif // !Q_MOC_RUN
#include <rpc/reply_statistic.h>
#include <rpc/network_client.h>
#include <rpc/request_statistic.h>
#include <rpc/table/rpc_table.h>

#include <rpc/rpc_common.h>
#include <rpc/rpc_lobby.h>

#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/model_tourn_additional_stat_t.hpp>

#include <tools/item_reference.hpp>

Q_DECLARE_METATYPE(ns_statistic::tourn_additional_stat_t)

namespace
{
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
}

namespace ns_dialog
{
    LobbyBase::LobbyBase(gbp_i64 id, QWidget *parent)
        : InitableDialog(parent)
        , m_tournamentID(id)
    {}

    LobbyBase::~LobbyBase()
    {}

    ns_model::tournament_stat_t *LobbyBase::modelStatistic() const {
        return modelsDB().getTournamentStatistic();
    }
    ns_model::tourn_additional_stat_t *LobbyBase::modelAdditionalStatistic() const {
        return modelsDB().getAdditionalTournStatistic(m_tournamentID);
    }

    Accessor<ns_statistic::tournament_stat_t> LobbyBase::statistic() const
    {
        return modelStatistic()->item(m_tournamentID);
    }

    bool LobbyBase::initPages()
    {
        for (int i = 0; i < pagesCount(); i++) {
            if (ns_page::LobbyBase* p = page(i)) {
                p->init(m_tournamentID);
            } else {
                Logger::warning(QString("page # %0 not found").arg(i), __FILELINE__);
            }
        }

        return true;
    }

    void LobbyBase::on_bttn_register_clicked()
    {
        if (enshureUserLoggedIn())
        {
            rpc_table* rpc = rpc_object<rpc_table>();
            rpc->get_tourn_register_info(m_tournamentID, [this](ns_table::e_status result, const ns_table::tourn_register_info& info){
                if (ns_helpers::isOk(result))
                {
                    // TODO: callback
                }
            });
        }
    }

    void LobbyBase::on_bttn_unregister_clicked()
    {
        if (enshureUserLoggedIn())
        {
            rpc_table* rpc = rpc_object<rpc_table>();
            rpc->cancel_tourn_register(m_tournamentID, [this](ns_table::e_status/*result*/, const ns_table::cancel_tourn_register_t& /*info*/){
                // TODO: callback
            });
        }
    }

    void LobbyBase::on_bttn_backToGame_clicked()
    {
        if (enshureUserLoggedIn())
        {
            auto lambda = [this](ns_table::e_status status
                               , const ns_table::tournament_state_t& tournState
                               , const ns_table::table_state_t& tableState
                               , int playersCount)
            {};
            rpc_table* rpc = rpc_object<rpc_table>();
            rpc->open_own_tournament(m_tournamentID, lambda);
        }
    }

    void LobbyBase::init()
    {
        if (isInitialized()) {
            return;
        }
        initHeader();
        initPages();

//        if (initHeader()) {
            onInitialized();
//        }
    }

    bool LobbyBase::initHeader()
    {
        return false;
    }

    void LobbyBase::changeEvent(QEvent *e)
    {
        if (e != nullptr && e->type() == QEvent::LanguageChange) {
            updateTexts();
        }
        InitableDialog::changeEvent(e);
    }

} //namespace ns_dialog





