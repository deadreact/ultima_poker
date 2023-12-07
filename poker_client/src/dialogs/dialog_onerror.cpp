#include <precompiled.h>
#include "dialog_onerror.h"
#include "ui_dialog_OnError.h"

#include <rpc/network_client.h>
#include <QProgressDialog>
#include <PokerApplication.h>
#include <QTimer>

OnError::OnError(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
    , m_ui(new Ui::OnError)
    , m_direction(1)
    , m_timerID(-1)
{
    m_ui->setupUi(this);
    NetworkClient* client = &pApp->client();
    connect(client, &NetworkClient::connected, this, &OnError::accept);

    m_ui->progressBar->setRange(0, sm_range);
    m_timerID = startTimer(1000/60, Qt::PreciseTimer);
}

OnError::~OnError()
{
    delete m_ui;
}

void OnError::quitApp()
{
    reject();
    pApp->quit();
}

void OnError::timerEvent(QTimerEvent *e)
{
    if (m_direction > 0) {
        int value = m_ui->progressBar->value();
        m_ui->progressBar->setValue(value + 1);
        if (m_ui->progressBar->maximum() - value < sm_minOffset) {
            m_direction = -m_direction;
        }
    }
    else
    {
        int value = m_ui->progressBar->value();
        m_ui->progressBar->setValue(value - 1);
        if (value - m_ui->progressBar->minimum() < sm_minOffset) {
            m_direction = -m_direction;
        }
    }

    QDialog::timerEvent(e);
}


void OnError::accept()
{
    if (pApp->client().isConnected()) {
        killTimer(m_timerID);
        QDialog::accept();
    }
}
void OnError::reject()
{
    killTimer(m_timerID);
    QDialog::reject();
}
