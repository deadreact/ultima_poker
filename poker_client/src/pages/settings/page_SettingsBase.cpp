#include <QEvent>
#include <precompiled.h>
#include "page_SettingsBase.h"

namespace ns_page
{
    SettingsBase::SettingsBase(QWidget *parent)
        : InitableWidget(parent)
        , m_modified(false)
    {
        connect(this, &SettingsBase::initialized, this, &SettingsBase::load);
    }

    SettingsBase::~SettingsBase() {}

    void SettingsBase::changeEvent(QEvent *e)
    {
        if (e && ((e->type() == QEvent::LanguageChange) || (e->type() == QEvent::LocaleChange))) {
            updateTexts();
        }
        InitableWidget::changeEvent(e);
    }
} //namespace ns_page

