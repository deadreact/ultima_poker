#pragma once

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include "interface/initable.h"

namespace Ui {
class OkMessage;
}

typedef Ui::OkMessage UiOkMessage;

namespace ns_dialog
{
    class OkMessage : public InitableDialog
    {
        Q_OBJECT
    private:
        UiOkMessage* m_ui;
        static QString sm_buttonThemeName;
    public:
        explicit OkMessage(QWidget *parent = 0, Qt::WindowFlags flags = 0);
        virtual ~OkMessage();
        virtual void init() override;
        void setTitle(const QString &title);
        void setText(const QString &textToDisplay);
        static QString buttonTheme();
        static void setButtonTheme(const QString& name);
        static int execMessage(const QString& text, const QString& title, const QRect& rc);
        static int execMessage(const QString& text=tr("@message"), const QString& title=tr("@Title_message"), QWidget* parent=nullptr);
    signals:
        void deinitialized();
    public slots:
        virtual void accept() override;

    protected:
        virtual void timerEvent(QTimerEvent *e) override;
    };

} // namespace dialog
