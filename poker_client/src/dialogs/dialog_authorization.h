#pragma once

#include <QDialog>
#include <interface/initable.h>

namespace Ui {
class dialog_Authorization;
}

namespace ns_dialog {

    class Authorization : public InitableDialog
    {
        Q_OBJECT

        constexpr static const char* sm_errStyleSheet = "QLineEdit:!focus {"
                                                          "    border: 1px solid #de123a;"
                                                          "    background-color: #ffcdd2;"
                                                          "    background-image: url(:/textures/Common/icons/icon1x2_ok_error.png);"
                                                          "    background-position: right bottom;"
                                                          "    background-repeat: no-repeat;"
                                                          "    background-attachment: fixed;"
                                                          "}";

    private:
        Ui::dialog_Authorization *m_ui;
        bool m_autoLoginRestored;
    signals:
    public:
        explicit Authorization(QWidget *parent = 0);
        virtual ~Authorization();
        virtual void init() override;
    public slots:
        void showRecoveryPage();
        virtual void accept() override;
        virtual void reject() override;
        void adjustHeight(int mode);
    protected:
    protected slots:
//        void login(reply_lobby::e_lobby result, const gbp::ns_api::ns_lobby::user_info_t&);
//        void onServerReplyStep1(reply_lobby::e_lobby result);
//        void onServerReplyStep2(reply_lobby::e_lobby result);
//        void onServerReplyStep3(reply_lobby::e_lobby result);
    private slots:
        void on_bttn_back_clicked();
        void on_bttn_resend_clicked();
        void on_bttn_OK_clicked();
        void requestLogin();
        void on_bttn_create_account_clicked();

        void checkNickname();
        void checkPassword();

        void showHomepage();
        void clearFakePassword();
    protected:
        virtual void closeEvent(QCloseEvent *event) override;
    };
} // ns_dialog
