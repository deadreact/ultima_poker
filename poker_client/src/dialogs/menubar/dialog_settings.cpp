#include <precompiled.h>
#include "dialog_settings.h"
#include "ui_dialog_Settings.h"

namespace ns_dialog
{
    Settings::Settings(QWidget *parent)
        : InitableDialog(parent)
        , m_ui(new Ui::Settings)
    {
        m_ui->setupUi(this);
    }

    Settings::~Settings()
    {
        delete m_ui;
    }

    void Settings::init()
    {
        if (isInitialized()) {
            return;
        }

        m_ui->tab_common->init();
        m_ui->tab_autorebuy->init();
        m_ui->tab_history->init();
        m_ui->tab_notes->init();

        onInitialized();
    }

    void Settings::reject() {
      emit rejected();
    }


} //namespace ns_dialog
