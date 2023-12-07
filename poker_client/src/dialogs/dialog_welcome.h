#pragma once

#ifndef NO_REGISTRATION

#include "interface/initable.h"

namespace Ui {
    class Welcome;
}

class Avatar;

namespace ns_dialog
{
    class Welcome : public InitableDialog
    {
        Q_OBJECT

        Q_PROPERTY(QString nickname READ nickname WRITE setNickname)

    private:
        Ui::Welcome *m_ui;
        std::pair<sha1_t, int> m_passinfo;
    public:
        explicit Welcome(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);
        virtual ~Welcome();

        virtual void init() override;
        QString nickname() const;
    public:
        void setAvatar(const Avatar &avatar);
        void setNickname(const QString& nick);
        void setPassinfo(const sha1_t &hash, int len);
    protected:
        void changeEvent(QEvent *e) override;
    private slots:
        void loginUserOnClose();
//        void onLogin(reply_lobby::e_lobby result);
    };
} // namespace dialog


#endif // !NO_REGISTRATION
