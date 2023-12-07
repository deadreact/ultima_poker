#include <precompiled.h>
#include "dialog_okmessage.h"
#include "ui_dialog_OkMessage.h"

#include <QLineEdit>
#include <QLabel>
#include <PokerApplication.h>
#include <styles/shadowedtextstyle.h>

namespace ns_dialog
{
    /*static*/ QString OkMessage::sm_buttonThemeName = "orange";

    OkMessage::OkMessage(QWidget *parent, Qt::WindowFlags flags)
        : InitableDialog(parent, flags)
        , m_ui(new Ui::OkMessage)
    {
        m_ui->setupUi(this);
        setTextDropShadowStyle(m_ui->bttn_ok, QPoint(0, 1));
        setTextDropShadowStyle(m_ui->message, QPoint(0, -1));

        startTimer(1000, Qt::VeryCoarseTimer);
    }

    OkMessage::~OkMessage()
    {
        if (m_ui != nullptr) {
            delete m_ui;
        }
    }

    void OkMessage::init()
    {
        if (isInitialized())
            return;
        m_ui->bttn_ok->setProperty("theme", sm_buttonThemeName);

        onInitialized();
    }

    void OkMessage::setTitle(const QString &title){
        setWindowTitle(title);
    }

    void OkMessage::setText(const QString &textToDisplay){
        m_ui->message->setText(textToDisplay);
    }

    /*static*/ QString OkMessage::buttonTheme() {
        return sm_buttonThemeName;
    }

    /*static*/ void OkMessage::setButtonTheme(const QString &name) {
        sm_buttonThemeName = name;
    }

    /*static*/int OkMessage::execMessage(const QString &text, const QString &title, const QRect& rc)
    {
        OkMessage msg(pApp->activeWindow());
        msg.init();
        msg.setText(text);
        msg.setTitle(title);
        msg.setMinimumSize(rc.size());
        msg.setMaximumSize(rc.size());
        msg.setGeometry(rc);
        msg.adjustSize();
        msg.setFixedSize(msg.size());

        return msg.exec();
    }

    /*static*/ int OkMessage::execMessage(const QString &text, const QString &title, QWidget* parent)
    {
        OkMessage msg(parent);
        msg.init();
        msg.setText(text);
        msg.setTitle(title);
        msg.adjustSize();
        msg.setFixedSize(msg.size());

        return msg.exec();
    }

    void OkMessage::accept()
    {
        setTitle(tr("@Title_message"));
        setText(tr("@message"));
        m_isInitialized = false;
        QDialog::accept();
//        this->close();
    }

    void OkMessage::timerEvent(QTimerEvent *e)
    {
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        InitableDialog::timerEvent(e);
    }

} // namespace dialog


