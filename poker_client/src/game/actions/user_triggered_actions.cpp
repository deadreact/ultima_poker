#include <precompiled.h>
#include "user_triggered_actions.h"
#include <api_models/model_io.hpp>
#include <PokerApplication.h>
#include <rpc/table/rpc_table.h>
#include <rpc/rpc_common.h>
#include <game/GameManager.h>

#include "updater_actions.h"

namespace
{
    rpc_table* getRpc() { return rpc_object<rpc_table>(); }
} // namespace

namespace ns_action
{
    UserBase::UserBase(const GameId &gameID, QWidget *activeWindow, QObject *parent, bool loginRequired)
        : SequentalActionQueue(parent)
        , m_activeWindow(activeWindow)
        , m_gameID(gameID)
        , m_loginRequired(loginRequired)
    {}

    UserBase::~UserBase()
    {

    }

    void UserBase::initialize()
    {
        LoginRequired* loginRequiredAction = new LoginRequired(m_activeWindow, this);
        append(loginRequiredAction);
    }

    void UserBase::execute()
    {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }
        SequentalActionQueue::execute();
    }

    gbp_i64 UserBase::userId() const {
        return ns_model::IOModel::userId();
    }

    UserJoiningQueue::UserJoiningQueue(gbp_i64 cashId, QObject *parent)
        : UserBase(cashId, nullptr, parent)
    {}

    UserJoiningQueue::~UserJoiningQueue()
    {}

    void UserJoiningQueue::initialize()
    {
        if (pApp->gameManager()->isTableCreated(m_gameID)) {
            m_activeWindow = pApp->gameManager()->getGameTable(m_gameID);
        }
        UserBase::initialize();

        AddWaiter* addWaiter = new AddWaiter(m_gameID.m_tableID, this);
        UpdateWaiters* updWaiters = new UpdateWaiters(m_gameID.m_tableID, this);
        append(addWaiter);
        append(updWaiters);

//        updWaiters->connectView();
    }

    void UserJoiningQueue::execute()
    {
        UserBase::execute();
    }




} //namespace ns_action



