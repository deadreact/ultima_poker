#include <precompiled.h>
#include "TimelineHandler.h"
#include "controlshandler.h"
#include "gamehandlerinterface.hpp"
#include <game/controls/usercontrols.h>

struct ControlsHandler::Impl
{
    UserControls* m_hud;
    gbp_i64 m_userID;
};

ControlsHandler::ControlsHandler(const GameId& id, QObject *parent)
    : QObject(parent)
    , ControlsHandlerInterface(id)
    , m_impl(new Impl)
{}

ControlsHandler::~ControlsHandler()
{
    delete m_impl;
}

void ControlsHandler::initialize()
{

}

void ControlsHandler::timelineEvent(const TimeLineEvent &ev) {
    if (ev.handler->playerID() == m_impl->m_userID) {
        m_impl->m_hud->timelineEvent(ev);
    }
}

void ControlsHandler::openTable() {

}

void ControlsHandler::closeTable() {

}

void ControlsHandler::setHud(UserControls *hud) {
    m_impl->m_hud = hud;
}

void ControlsHandler::onLogout(gbp_i64 playerID)
{
    Q_ASSERT(m_impl->m_userID == playerID);
    m_impl->m_userID = -1;
}

void ControlsHandler::onLogin(gbp_i64 playerID)
{
    Q_ASSERT(m_impl->m_userID == -1);
    m_impl->m_userID = playerID;
}

void ControlsHandler::onModelDataChanged(const QModelIndex &index)
{
}

void ControlsHandler::onActionTriggered(QAction *)
{
}

void ControlsHandler::onActionToggled(QAction *, bool)
{
}

void ControlsHandler::onButtonClicked(QAbstractButton *)
{
}

void ControlsHandler::onButtonToggled(QAbstractButton *, bool)
{
}
