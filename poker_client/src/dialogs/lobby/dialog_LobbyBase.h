#pragma once

#include <QDialog>
#include <pages/lobbyTournament/page_LobbyBase.h>
#include <tools/types_fwd.hpp>
#include <interface/initable.h>
#include <common.h>
#include <tools/accessor.hpp>
#include <tools/item_reference.hpp>
namespace
{
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
}

namespace ns_model {
    class tournament_stat_t;
    class tourn_additional_stat_t;
} //namespace ns_model

namespace ns_dialog
{
    class LobbyBase : public InitableDialog
    {
        Q_OBJECT
    protected:
        const gbp_i64 m_tournamentID;
    public:
        LobbyBase(gbp_i64 id, QWidget *parent = nullptr);
        virtual ~LobbyBase();

        ns_model::tournament_stat_t*       modelStatistic() const;
        ns_model::tourn_additional_stat_t* modelAdditionalStatistic() const;

        Accessor<ns_statistic::tournament_stat_t> statistic() const;

        virtual ns_page::LobbyBase* page(int index) const = 0;
        virtual int pagesCount() const { return 5; }

        virtual void init() override;
        virtual bool initHeader();
    protected:  
        void updatePages();
        virtual void updateTexts(bool retranslate = true) = 0;
        virtual void changeEvent(QEvent *e) override;

    protected slots:
        virtual void on_bttn_register_clicked();
        virtual void on_bttn_unregister_clicked();
        virtual void on_bttn_backToGame_clicked();
    private slots:
        bool initPages();
    };
} //namespace ns_dialog
