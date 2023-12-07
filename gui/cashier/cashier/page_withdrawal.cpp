#include "page_withdrawal.h"
#include "ui_page_withdrawal.h"
#include <qpropertyanimation.h>
#include <animation/animation.h>

namespace ns_page
{
    Withdrawal::Withdrawal(QWidget *parent)
        : CashierBase(parent)
        , m_ui(new Ui::Withdrawal)
    {
        m_ui->setupUi(this);
    }

    Withdrawal::~Withdrawal()
    {
        delete m_ui;
    }

    void Withdrawal::popupRulesInfo(bool show)
    {
        QFrame* target = m_ui->frame_rules;
        int distance = 120;
        QPoint destination = target->pos() + QPoint(0, show ? distance : -distance);
        QPropertyAnimation* anim = ns_anim::moveTo(target, destination, 320);
        anim->setEasingCurve(QEasingCurve::OutSine);

        anim->start(QAbstractAnimation::DeleteWhenStopped);
    }

    void Withdrawal::init()
    {
        int idx = 0; // default empty page
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_visa, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_mastercard, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_skrill, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_neteller, ++idx);
        m_ui->bttnGrp_paymentSystems->setId(m_ui->bttn_paysafecard, ++idx);

//        m_ui->input_validUntilYear

        connect(m_ui->bttnGrp_paymentSystems, (void(QButtonGroup::*)(QAbstractButton* bttn))&QButtonGroup::buttonClicked, this
        , [this](QAbstractButton* bttn) {
            m_ui->title_depositType->setString(bttn->property("displayName").toString(), 0);
            m_ui->stackOfPaymentForms->setCurrentIndex(m_ui->bttnGrp_paymentSystems->id(bttn));
        });
    }

} //namespace ns_page

