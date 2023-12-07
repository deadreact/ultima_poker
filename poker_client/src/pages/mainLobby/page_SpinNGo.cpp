#include <precompiled.h>
#include "page_SpinNGo.h"
#include "ui_page_SpinNGo.h"

namespace ns_page
{
    SpinNGo::SpinNGo(QWidget *parent)
        : QFrame(parent)
        , m_ui(new Ui::SpinNGo)
    {
        m_ui->setupUi(this);
    }

    SpinNGo::~SpinNGo()
    {
        delete m_ui;
    }

    void SpinNGo::init()
    {

    }

} //namespace ns_page

