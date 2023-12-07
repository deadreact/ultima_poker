#include <precompiled.h>
#include "dialog_LobbySitNGo.h"

namespace
{
    using e_result = gbp::ns_api::ns_tournament_types::e_tournament_status;
}

namespace ns_dialog
{

    LobbySitNGo::LobbySitNGo(gbp_i64 id, QWidget *parent)
        : LobbyBase(id, parent)
    {}
    LobbySitNGo::~LobbySitNGo() {}

    ns_page::LobbyBase *LobbySitNGo::page(int index) const
    {
        if (index < 0 || index >= pagesCount()) {
            return nullptr;
        }
        return nullptr;
    }

    bool LobbySitNGo::initHeader()
    {
        return LobbyBase::initHeader();
    }

    void LobbySitNGo::updateTexts(bool retranslate)
    {
//        m_ui.
    }
} //namespace ns_dialog



