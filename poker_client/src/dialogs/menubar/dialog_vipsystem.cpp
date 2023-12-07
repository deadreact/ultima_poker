#include <precompiled.h>
#include "dialog_vipsystem.h"
#include "ui_dialog_VipSystem.h"

namespace ns_dialog
{
    VipSystem::VipSystem(QWidget *parent)
        : InitableDialog(parent)
        , m_ui(new Ui::VipSystem)
    {
        m_ui->setupUi(this);
    }

    VipSystem::~VipSystem()
    {
        delete m_ui;
    }

    void VipSystem::init()
    {
        if (isInitialized()) {
            return;
        }

        m_ui->tab_vipSystemDesc->init();
        m_ui->tab_vipStatusDesc->init();

        onInitialized();
    }

    void VipSystem::changeEvent(QEvent* e)
    {
        if (e && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
        }
    }

    void VipSystem::reject() {
      emit rejected();
    }

} //namespace ns_dialog
