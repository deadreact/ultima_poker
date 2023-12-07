#pragma once

#include "gamehandlerinterface.hpp"
#include "proxyhandler.h"

class ProxyViewHandler : public ProxyHandler<GameViewHandlerInterface>
{
public:
    ProxyViewHandler(const GameId& id);
    virtual ~ProxyViewHandler();

    virtual void initialize() override;
    virtual void timelineEvent(const TimeLineEvent &ev) override;
    virtual bool indexAffectsView(const QModelIndex& index) const override;
public slots:
    virtual void onModelAboutToBeReset() override;
    virtual void onModelReset() override;
    virtual void onModelDataChanged(const QModelIndexRange &range) override;
    virtual void onModelAboutToInsertItems(const QModelIndex &parent) override;
    virtual void onModelInsertItems(const QModelIndexRangeList &items) override;
    virtual void onModelAboutToRemoveItems(const QModelIndexRangeList &items) override;
    virtual void onModelRemoveItems(const QModelIndex &parent) override;
    virtual void setIndexWidget(QWidget* widget, const QModelIndex& index = QModelIndex()) override;
    virtual QWidget* indexWidget(const QModelIndex& index = QModelIndex()) const override;
protected:
    virtual void connectToModel() override;
    virtual void disconnectFromModel() override;
public:
    virtual void openTable() override;
    virtual void closeTable() override;
};
