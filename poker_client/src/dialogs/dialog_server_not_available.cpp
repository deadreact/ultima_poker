#include <precompiled.h>

#include "dialog_server_not_available.h"
#include "ui_dialogServerNotAvailable.h"

DialogServerNotAvailable::DialogServerNotAvailable(QWidget* parent)
    : QDialog(parent)
    , m_ui(new Ui::DialogServerNotAvailableUi)
{
    m_ui->setupUi(this);
    connect(m_ui->bttn_close, &QPushButton::clicked, this, &DialogServerNotAvailable::close);
}

DialogServerNotAvailable::~DialogServerNotAvailable() {
    delete m_ui;
}

