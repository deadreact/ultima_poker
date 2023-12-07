#pragma once

#include "gamehandlerinterface.hpp"
#include <QObject>

class UserControls;

class ControlsHandler : public QObject, public ControlsHandlerInterface
{
    Q_OBJECT
    struct Impl;
    Impl* m_impl;
public:
    explicit ControlsHandler(const GameId& id, QObject *parent = nullptr);
    virtual ~ControlsHandler();

    virtual void initialize() override;
    virtual void timelineEvent(const TimeLineEvent &ev) override;
    virtual void openTable() override;
    virtual void closeTable() override;

    void setHud(UserControls* hud);
public slots:
    virtual void onLogout(gbp_i64 playerID) override;
    virtual void onLogin(gbp_i64 playerID) override;
    virtual void onModelDataChanged(const QModelIndex &index) override;
    virtual void onActionTriggered(QAction *) override;
    virtual void onActionToggled(QAction *, bool) override;
    virtual void onButtonClicked(QAbstractButton *) override;
    virtual void onButtonToggled(QAbstractButton *, bool) override;
};
