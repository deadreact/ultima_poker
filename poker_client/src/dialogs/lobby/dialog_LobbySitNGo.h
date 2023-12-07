#pragma once

#include "dialog_LobbyBase.h"

namespace ns_dialog
{
    class LobbySitNGo : public LobbyBase
    {
        Q_OBJECT
    public:
        LobbySitNGo(gbp_i64 id, QWidget *parent = nullptr);
        virtual ~LobbySitNGo();

        virtual ns_page::LobbyBase *page(int index) const override;
        virtual bool initHeader() override;
    protected:
        virtual void updateTexts(bool retranslate) override;
    };
} //namespace ns_dialog
