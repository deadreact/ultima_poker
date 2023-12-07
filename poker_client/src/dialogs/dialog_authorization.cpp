#include <precompiled.h>
#include "dialog_authorization.h"
#include "ui_dialog_authorization.h"

#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_lobby.h>
    #include <rpc/network_client.h>
#endif //Q_MOC_RUN

#include <db/modeldatabase.h>
#include <PokerApplication.h>
#include <const.h>
#include <widgets/MainWindow.h>
#include <styles/shadowedtextstyle.h>
#include <api_models/model_user_info_t.hpp>
#include <models/model_user_additional_info.h>
#include <utils/crypt.h>
#include <hostinfo/hostinfo.h>


namespace
{
    rpc_lobby* getRpc() {
        return rpc_object<rpc_lobby>();
    }
    typedef reply_lobby::e_lobby e_status;
}

namespace ns_dialog {

    Authorization::Authorization(QWidget *parent)
        : InitableDialog(parent)
        , m_ui(new Ui::dialog_Authorization)
        , m_autoLoginRestored(false)
    {
        m_ui->setupUi(this);
    }

    Authorization::~Authorization()
    {
        delete m_ui;
    }

    void Authorization::init()
    {
        if (isInitialized())
            return;
        auto* model = modelsDB().getUserAdditionalInfo();
        CategoryItem* categ = model->category("common");
        categ->load();

        m_ui->check_savepass->setChecked(categ->fieldCheckState("autologin"));
        if (m_ui->check_savepass->isChecked()) {
            m_ui->input_nickname->setText(categ->fieldText("nickname"));
            m_ui->input_password->setText(categ->fieldText("password"));
            m_autoLoginRestored = true;
        } else {
            m_ui->input_nickname->clear();
            m_ui->input_password->clear();
            m_autoLoginRestored = false;
        }



        model->printAll();
        connect(m_ui->check_savepass, &QCheckBox::toggled, this, [this, categ, model](bool toggled) {
            categ->setField("autologin", QVariant::fromValue(toggled ? Qt::Checked : Qt::Unchecked), Qt::CheckStateRole);
        });
        connect(model, &ns_model::UserInfo::itemChanged, this, [this, model, categ](QStandardItem* item){
            QString name = categ->fieldName(item);
            if (name == "autologin") {
                m_ui->check_savepass->setCheckState(categ->fieldCheckState(name));
            } else if (name == "nickname") {
                m_ui->input_nickname->setText(categ->fieldEditText(name));
            } else if (name == "password") {
                m_ui->input_password->setText(categ->fieldEditText(name));
            }
        });
        QStandardItem* passItem = categ->field("password");
        connect(m_ui->input_password, &PasswordLineEdit::textChanged, [passItem](const QString& text){
            passItem->setData(text, Qt::EditRole);
        });
        connect(m_ui->input_nickname, &UsernameLineEdit::textEdited, [categ](const QString& name){
            categ->setField("nickname", name);
        });
//        connect(m_ui->input_password, &PasswordLineEdit::focusToggled, [this, categ](bool focus){
//            if (focus) {
//                qDebug() << "focused...";
//                m_ui->input_password->setText(categ->fieldEditText("password"));
//            } else {
//                qDebug() << "unfocused...";
//                categ->setField("password", m_ui->input_password->text());
//                m_ui->input_password->setText(categ->fieldText("password"));
//            }
//        });

        m_ui->input_confirmation->setPasswordLineEdit(m_ui->input_password);

        connect(m_ui->restore_password_link, &QPushButton::clicked, this, &Authorization::showRecoveryPage);

        connect(m_ui->bttn_finish, &QPushButton::clicked, this, &Authorization::showHomepage);
        connect(m_ui->stackOfControls, &QStackedWidget::currentChanged, this, &Authorization::adjustHeight);

        QList<QAbstractButton*> buttons = findChildren<QAbstractButton*>(QRegExp("bttn_\\w+"));

        for (QAbstractButton* button: buttons) {
            setTextDropShadowStyle(button, QPoint(0, 1), QColor(0, 0, 0, 100));
        }
        m_ui->imsg_loginerror->resetCurrentMsgIndex();

        setTextDropShadowStyle(m_ui->title_login, QPoint(0, 1), 0x191919);
        setTextDropShadowStyle(m_ui->title_logon, QPoint(0, 1), 0x191919);
        setTextDropShadowStyle(m_ui->final_msg, QPoint(0, 1), 0x191919);

        showHomepage();
        onInitialized();
    }

    void Authorization::showRecoveryPage()
    {
        m_ui->stackOfContent->setCurrentIndex(1);
        m_ui->stackOfControls->setCurrentIndex(1);
    }
    void Authorization::accept()
    {
        int currentPage = m_ui->stackOfContent->currentIndex();

        switch (currentPage) {
        case 0: requestLogin(); break;
        case 1:
        case 2:
        case 3: on_bttn_OK_clicked(); break;
        case 4: m_ui->bttn_finish->click(); break;

        default:
            break;
        }
    }

    void Authorization::reject()
    {
        int currentPage = m_ui->stackOfContent->currentIndex();
        if (currentPage > 0 && currentPage < 4) {
            m_ui->bttn_back->click();
            return;
        }
        InitableDialog::reject();
    }

    void Authorization::adjustHeight(int mode)
    {
        // 0 - max, 1,2 - min
        setFixedHeight((mode == 0) ? 358 : 302);
    }

    void Authorization::on_bttn_back_clicked()
    {
        m_ui->stackOfContent->setCurrentIndex(m_ui->stackOfContent->currentIndex() - 1);
        m_ui->stackOfControls->setCurrentIndex(std::min(1, m_ui->stackOfContent->currentIndex()));
    }

    void Authorization::on_bttn_resend_clicked()
    {
        getRpc()->restore_password(m_ui->input_loginOrEmail->text(), [this](e_status result) {
            m_ui->stackOfContent->setStyleSheet(ns_helpers::isOk(result) ? "" : sm_errStyleSheet);
            if (ns_helpers::isOk(result)) {
                m_ui->imsg_error->setCurrentMsgIndex(0);
                m_ui->stackOfContent->setCurrentIndex(2);
            }
            else if (result == reply_lobby::e_lobby::not_registered) {
                m_ui->imsg_error->setCurrentMsgIndex(1);
            } else {
                m_ui->imsg_error->setCurrentMsgIndex(1);
                Logger::error("Unknown Error had been received on password restore request", __FILE__, __LINE__);
            }
        });
        m_ui->imsg_confirmation->setCurrentMsgIndex(2);

    }

    void Authorization::on_bttn_OK_clicked()
    {
        QWidget* currentContent = m_ui->stackOfContent->currentWidget();
         if (currentContent == m_ui->recovery_1) {
            getRpc()->restore_password(m_ui->input_loginOrEmail->text(), [this](e_status result){
                m_ui->stackOfContent->setStyleSheet(ns_helpers::isOk(result) ? "" : sm_errStyleSheet);
                if (ns_helpers::isOk(result)) {
                    m_ui->imsg_error->setCurrentMsgIndex(0);
                    m_ui->stackOfContent->setCurrentIndex(2);
                }
                else if (result == reply_lobby::e_lobby::not_registered) {
                    m_ui->imsg_error->setCurrentMsgIndex(1);
                } else {
                    m_ui->imsg_error->setCurrentMsgIndex(1);
                    Logger::error("Unknown Error had been received on password restore request", __FILE__, __LINE__);
                }
            });
        } else if (currentContent == m_ui->recovery_2) {
             getRpc()->restore_password(m_ui->input_loginOrEmail->text(), m_ui->input_key->text().toLocal8Bit(), [this](e_status result){
                 m_ui->stackOfContent->setStyleSheet(ns_helpers::isOk(result) ? "" : sm_errStyleSheet);
                 m_ui->imsg_confirmation->setCurrentMsgIndex(ns_helpers::isOk(result) ? 0 : 1);
                 if (ns_helpers::isOk(result)) {
                     m_ui->stackOfContent->setCurrentIndex(m_ui->stackOfContent->currentIndex()+1);
                 }
             });
        }
        else if (currentContent == m_ui->recovery_3)
        {
            if ( m_ui->input_newPassword->IsIncorrect()) {
                m_ui->msg_mismatch->setCurrentIndex(1);
                m_ui->input_newPassword->setStyleSheet(sm_errStyleSheet);
                m_ui->input_confirmation->setStyleSheet(sm_errStyleSheet);
            } else if (m_ui->input_confirmation->IsIncorrect()) {
                m_ui->msg_mismatch->setCurrentIndex(2);
                m_ui->input_newPassword->setStyleSheet(sm_errStyleSheet);
                m_ui->input_confirmation->setStyleSheet(sm_errStyleSheet);
            } else {
                m_ui->input_newPassword->setStyleSheet("");
                m_ui->input_confirmation->setStyleSheet("");
                m_ui->msg_mismatch->setCurrentIndex(0);
//                request_lobby* request = pApp->client().request<request_lobby>();
                getRpc()->restore_password(m_ui->input_loginOrEmail->text()
                                         , ns_util::toSha1(m_ui->input_newPassword->text())
                                         , ns_util::toSha1(m_ui->input_confirmation->text())
                                         , m_ui->input_key->text().toLocal8Bit()
                                         , [this](e_status result)
                {
                    m_ui->msg_mismatch->setCurrentIndex(ns_helpers::isOk(result) ? 0 : 1);
                    if (ns_helpers::isOk(result)) {
                        m_ui->stackOfContent->setCurrentIndex(m_ui->stackOfContent->currentIndex() + 1);
                        m_ui->stackOfControls->setCurrentWidget(m_ui->okBttnOnly);
                    }
                });
            }
        }
    }

    void Authorization::requestLogin()
    {
        auto* data = modelsDB().getUserAdditionalInfo()->category("common");
        //qDebug() << data->fieldText("nickname");
        //qDebug() << data->fieldData("password", UserType::HashRole).toByteArray();

        getRpc()->login(data->fieldText("nickname")
                      , data->fieldData("password", UserType::HashRole).toByteArray()
                      , pApp->_host_info.to_string()
            , [this](e_status result, const ns_lobby::user_info_t&){
            m_ui->imsg_loginerror->resetCurrentMsgIndex();
            switch (result) {
            case e_status::ok:
            {
                auto* model = modelsDB().getUserAdditionalInfo();
                CategoryItem* categ = model->category("common");
                categ->field("autologin")->setCheckState(m_ui->check_savepass->checkState());
                categ->setField("nickname", m_ui->input_nickname->text());
                if (!m_autoLoginRestored) {
                    categ->setField("password", m_ui->input_password->text());
                }
                categ->save();

                InitableDialog::accept();

                break;
            }
            case e_status::already_online:
                m_ui->imsg_loginerror->setCurrentMsgIndex(1); break;
            case e_status::not_registered:
                m_ui->imsg_loginerror->setCurrentMsgIndex(2); break;
            case e_status::email_not_verified:
                m_ui->imsg_loginerror->setCurrentMsgIndex(3); break;
            case e_status::user_banned:
                m_ui->imsg_loginerror->setCurrentMsgIndex(4); break;
            case e_status::bad_username_or_password:
                m_ui->imsg_loginerror->setCurrentMsgIndex(5); break;
            case e_status::unknown_error:
            default:
                m_ui->imsg_loginerror->setCurrentMsgIndex(6); break;
            }
            this->adjustSize();
        });
    }

    void Authorization::on_bttn_create_account_clicked()
    {
        if (MainWindow* mw = qobject_cast<MainWindow*>(parentWidget())) {
            hide();
            mw->openRegistrationDialog();
        }
    }

    void Authorization::checkNickname()
    {
        if (m_ui->input_nickname->IsIncorrect()) {
            m_ui->input_nickname->setStyleSheet(sm_errStyleSheet);
            m_ui->bttn_login->setEnabled(false);
        } else {
            m_ui->input_nickname->setStyleSheet("");
            m_ui->bttn_login->setEnabled(m_ui->input_password->isCorrect());
        }
    }

    void Authorization::checkPassword()
    {
        if (m_ui->input_password->IsIncorrect())
        {
            m_ui->input_password->setStyleSheet(sm_errStyleSheet);
            m_ui->bttn_login->setEnabled(false);
        }
        else
        {
            m_ui->input_password->setStyleSheet("");
            m_ui->bttn_login->setEnabled(m_ui->input_nickname->isCorrect());
        }
    }

    void Authorization::showHomepage()
    {
        m_ui->stackOfContent->setCurrentIndex(0);
        m_ui->stackOfControls->setCurrentIndex(0);
        m_ui->imsg_loginerror->resetCurrentMsgIndex();
    }

    void Authorization::clearFakePassword()
    {
        if (m_autoLoginRestored)
        {
            auto* model = modelsDB().getUserAdditionalInfo();
            m_autoLoginRestored = false;
            model->getItem("password", "common")->setData(QVariant(), Qt::EditRole);
            model->getItem("password", "common")->setData(QVariant(), UserType::OriginDataRole);
            m_ui->input_newPassword->clear();
        }
    }

} // namespace ns_dialog


void ns_dialog::Authorization::closeEvent(QCloseEvent *event)
{
    event->accept();
    InitableDialog::reject();
}
