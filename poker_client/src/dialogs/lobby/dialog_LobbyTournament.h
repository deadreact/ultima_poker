#pragma once

#include "dialog_LobbyBase.h"

namespace
{
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
}

namespace Ui {
    class LobbyTournament;
}

namespace ns_dialog
{
    class LobbyTournament : public LobbyBase
    {
        Q_OBJECT
    private:
        Ui::LobbyTournament *m_ui;
    public:
        LobbyTournament(gbp_i64 id, QWidget *parent = nullptr);
        virtual ~LobbyTournament();
        virtual bool initHeader() override;
        virtual bool updateHeader();

        virtual ns_page::LobbyBase *page(int index) const override;
        virtual int pagesCount() const override;
    private:
        void applyShadowsOnTexts();
    protected:
        virtual void updateTexts(bool retranslate = true) override;
    };
} //namespace ns_dialog
