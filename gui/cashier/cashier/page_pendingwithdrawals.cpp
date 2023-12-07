#include <qpropertyanimation.h>
#include "page_pendingwithdrawals.h"
#include "ui_page_pendingwithdrawals.h"
#include <model_pendingwithdrawals.h>
#include <animation/animation.h>

namespace ns_page
{
    PendingWithdrawals::PendingWithdrawals(QWidget *parent)
        : CashierBase(parent)
        , m_ui(new Ui::PendingWithdrawals)
    {
        m_ui->setupUi(this);
    }

    PendingWithdrawals::~PendingWithdrawals()
    {
        delete m_ui;
    }


    void PendingWithdrawals::init()
    {
        m_ui->tableView->setModel(new ns_model::PendingWithdrawal);
    }

    void PendingWithdrawals::popupInfo(bool show)
    {
        QFrame* target = m_ui->frame_rules;
        int distance = 168;
        QPoint destination = target->pos() + QPoint(0, show ? distance : -distance);
        QPropertyAnimation* anim = ns_anim::moveTo(target, destination, 320);
        anim->setEasingCurve(QEasingCurve::OutSine);

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }
} //namespace ns_page

