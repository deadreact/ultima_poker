#pragma once

#include "single_actions.h"

namespace ns_action
{
    class JoinCash : public SequentalActionQueue
    {
        Q_OBJECT
        gbp_i64 m_tableID;
        QWidget* m_viewParent;
    public:
        JoinCash(gbp_i64 tableID, QWidget* viewParent = nullptr, QObject* parent = nullptr);
        virtual void initialize() override;
        virtual void execute() override;
    };

    class WatchCash : public SequentalActionQueue
    {
        Q_OBJECT
        gbp_i64 m_tableID;
        QWidget* m_viewParent;
    public:
        WatchCash(gbp_i64 tableID, QWidget* viewParent = nullptr, QObject* parent = nullptr);
        virtual void initialize() override;
        virtual void execute() override;
    };

    class RestoreCash : public SequentalActionQueue
    {
        Q_OBJECT
        GameId m_gameID;
        QWidget* m_viewParent;
    public:
        RestoreCash(const GameId& id, QWidget* viewParent = nullptr, QObject* parent = nullptr);
        virtual void initialize() override;
        virtual void execute() override;
    };
} //namespace ns_action

