#pragma once
#include "single_actions.h"

namespace ns_subaction
{
    using ns_action::Action;
    class InitActionFromAnother : public Action
    {
        Q_OBJECT
    protected:
        const Action* const m_source;
        Action* const m_target;
    public:
        InitActionFromAnother(const Action* const source, Action* const target, QObject* parent = nullptr);
    };

    class InitCashBuyIn : public InitActionFromAnother
    {
        Q_OBJECT
        using SourceType = const ns_action::FetchSitOnTableInfo* const;
        using TargetType = ns_action::CashBuyIn* const;
    public:
        InitCashBuyIn(SourceType source, TargetType target, QObject* parent = nullptr);
        virtual void execute() override;
    };
} // namespace ns_subaction
