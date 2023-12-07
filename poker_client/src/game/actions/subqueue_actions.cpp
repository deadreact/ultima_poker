#include <precompiled.h>
#include "subqueue_actions.h"

namespace ns_subaction
{
    InitActionFromAnother::InitActionFromAnother(const Action * const source, Action * const target, QObject *parent)
        : Action(parent)
        , m_source(source)
        , m_target(target)
    {
        connect(source, &Action::done, this, [this](bool b){
            if (b) {
                execute();
            }
        });
    }

    InitCashBuyIn::InitCashBuyIn(SourceType source, TargetType target, QObject *parent)
        : InitActionFromAnother(source, target, parent)
    {
        m_execStarts = ExecutionStarts::Immediately;
    }

    void InitCashBuyIn::execute() {
        SourceType src = qobject_cast<SourceType>(m_source);
        TargetType tgt = qobject_cast<TargetType>(m_target);

        tgt->setInfo(src->info());
        tgt->setRatTime(src->ratTime());
        emit done(true);
    }
} // namespace ns_subaction
