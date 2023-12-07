#pragma once

#ifndef NO_REGISTRATION

#include <interface/initable.h>

namespace Ui {
    class RegistrationForm;
}
class CorrectInput;
namespace ns_widget {
    class ProfilePicture;
}

class PasswordToolTip;
class Avatar;

namespace gbp {
namespace ns_api {
namespace ns_lobby {
    struct user_register_info_t;
} //ns_lobby
} //ns_api
} //gbp

namespace ns_dialog
{
    class Registration : public InitableDialog
    {
        Q_OBJECT
    private:
        Ui::RegistrationForm* m_ui;
        QVector<CorrectInput*> m_inputFields;
        QWidgetList m_invalidInputLst;
        PasswordToolTip* m_passToolTip;
    public:
        explicit Registration(QWidget *parent = 0, Qt::WindowFlags flags = 0);
        virtual ~Registration();
        virtual void init() override;

        QWidgetList findInvalidInput();
        void createDataStruct(gbp::ns_api::ns_lobby::user_register_info_t& dataStruct) const;

        void setShadowOnTexts();
        void initAvatarsPage1();
        void initAvatarsPage2();
    protected:
        virtual void changeEvent(QEvent *e) override;
        void initPasswordToolTip();
    public slots:
        virtual void accept() override;
        virtual void reject() override;

    private slots:
        void reset();
        void onIndicatorChanged(qreal v);
        void resetRedFields();
        void setAvatar(const Avatar &avatar);
        void checkNickname();
        void checkPassword();
        void checkConfirmation();
        void checkEmail();
        void updateButtonState();
        void uploadAvatar();
        void showPasswordToolTip();
        void setPasswordToolTipVisible(bool visible);
        void hidePasswordToolTip();
        // api-sockets
        void onUserRegistered();

        //-------------------------------------
        void selectAsIncorrect(const QWidgetList& wlist);
        void unselectAsIncorrect(const QWidgetList& wlist);

        void senderCorrect();
        void senderIncorrect();

        // QObject interface
    public:
        virtual bool event(QEvent *e) override;
    };

} // namespace dialog

#endif // !NO_REGISTRATION
