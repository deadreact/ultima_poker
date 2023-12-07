#include <precompiled.h>
#include "proxyviewhandler.h"

ProxyViewHandler::ProxyViewHandler(const GameId &id)
    : ProxyHandler<GameViewHandlerInterface>(id)
{}

ProxyViewHandler::~ProxyViewHandler()
{
    disconnectFromModel();
}


void ProxyViewHandler::initialize()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->initialize();
    }
    connectToModel();
}

void ProxyViewHandler::timelineEvent(const TimeLineEvent &ev)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->timelineEvent(ev);
    }
}


bool ProxyViewHandler::indexAffectsView(const QModelIndex& index) const
{
    bool result = false;
    for (GameViewHandlerInterface* handler: m_handlers) {
        result |= handler->indexAffectsView(index);
    }
    return result;
}

void ProxyViewHandler::onModelAboutToBeReset()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelAboutToBeReset();
    }
}

void ProxyViewHandler::onModelReset()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelReset();
    }
}

void ProxyViewHandler::onModelDataChanged(const QModelIndexRange &range)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelDataChanged(range);
    }
}

void ProxyViewHandler::onModelAboutToInsertItems(const QModelIndex &parent)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelAboutToInsertItems(parent);
    }
}

void ProxyViewHandler::onModelInsertItems(const QModelIndexRangeList &items)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelInsertItems(items);
    }
}

void ProxyViewHandler::onModelAboutToRemoveItems(const QModelIndexRangeList &items)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelAboutToRemoveItems(items);
    }
}

void ProxyViewHandler::onModelRemoveItems(const QModelIndex &parent)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->onModelRemoveItems(parent);
    }
}

void ProxyViewHandler::setIndexWidget(QWidget *widget, const QModelIndex &index)
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->setIndexWidget(widget, index);
    }
}

QWidget *ProxyViewHandler::indexWidget(const QModelIndex &index) const
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        if (QWidget* widget = handler->indexWidget(index)) {
            return widget;
        }
    }
    return nullptr;
}

void ProxyViewHandler::connectToModel()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->connectToModel();
    }
}

void ProxyViewHandler::disconnectFromModel()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->disconnectFromModel();
    }
}

void ProxyViewHandler::openTable()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->openTable();
    }
}

void ProxyViewHandler::closeTable()
{
    for (GameViewHandlerInterface* handler: m_handlers) {
        handler->closeTable();
    }
}
