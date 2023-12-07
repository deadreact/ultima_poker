#include <precompiled.h>
#include "complex_actions.h"
#include "subqueue_actions.h"

namespace ns_action
{
    JoinCash::JoinCash(gbp_i64 tableID, QWidget *viewParent, QObject *parent)
        : SequentalActionQueue(parent)
        , m_tableID(tableID)
        , m_viewParent(viewParent)
    {}
    void JoinCash::initialize()
    {
        GameId gameID(m_tableID);

        clear();
        append(new ns_action::LoginRequired(m_viewParent, this));
        append(new ns_action::InitTableWindow(gameID, nullptr, this));
        append(new ns_action::InitPlayerControls(gameID, this));

        ns_action::FetchSitOnTableInfo* getInfo = new ns_action::FetchSitOnTableInfo(gameID, -1, this);
        ns_action::CashBuyIn* makeBuyIn   = new ns_action::CashBuyIn(m_tableID, nullptr, this);

        // не добавляется в очередь
        /*ns_subaction::InitCashBuyIn* subaction = */new ns_subaction::InitCashBuyIn(getInfo, makeBuyIn, makeBuyIn);

        append(getInfo);
        append(new ns_action::OpenGameTable(gameID, this));
        append(makeBuyIn);

    }

    void JoinCash::execute()
    {
        if (m_tableID == -1) {
            emit done(false);
            return;
        }
        SequentalActionQueue::execute();
    }

    WatchCash::WatchCash(gbp_i64 tableID, QWidget *viewParent, QObject *parent)
        : SequentalActionQueue(parent)
        , m_tableID(tableID)
        , m_viewParent(viewParent)
    {

    }

    void WatchCash::initialize()
    {
        GameId gameID(m_tableID);

        clear();
        append(new ns_action::AddViewer(gameID, this));
        append(new ns_action::InitTableWindow(gameID, m_viewParent, this));
        append(new ns_action::InitViewerControls(gameID, this));
        append(new ns_action::OpenGameTable(gameID, this));
    }

    void WatchCash::execute()
    {
        if (m_tableID == -1) {
            emit done(false);
            return;
        }
        SequentalActionQueue::execute();
    }

    RestoreCash::RestoreCash(const GameId &id, QWidget *viewParent, QObject *parent)
        : SequentalActionQueue(parent)
        , m_gameID(id)
        , m_viewParent(viewParent)
    {

    }

    void RestoreCash::initialize()
    {
        clear();
        append(new ns_action::InitTableWindow(m_gameID, m_viewParent, this));
        append(new ns_action::InitPlayerControls(m_gameID, this));
        append(new ns_action::OpenGameTable(m_gameID, this));
    }

    void RestoreCash::execute()
    {
        if (m_gameID.isValid()) {
            emit done(false);
            return;
        }
        SequentalActionQueue::execute();
    }
} //namespace ns_action




