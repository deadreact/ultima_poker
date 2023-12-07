#include <precompiled.h>
#include "dialog_welcome.h"

#ifndef NO_REGISTRATION

#include "ui_dialog_Welcome.h"
#include <PokerApplication.h>
#include <db/modeldatabase.h>
#include <api_models/model_user_info_t.hpp>
#include <models/model_user_additional_info.h>

#ifndef Q_MOC_RUN
    #include "rpc/network_client.h"
    #include "rpc/rpc_common.h"
    #include "rpc/rpc_lobby.h"
#endif // Q_MOC_RUN

namespace ns_dialog
{
    Welcome::Welcome(QWidget *parent, Qt::WindowFlags flags)
        : InitableDialog(parent, flags)
        , m_ui(new Ui::Welcome)
        , m_passinfo("", 0)
    {
        m_ui->setupUi(this);
    }

    Welcome::~Welcome()
    {
        delete m_ui;
    }

    void Welcome::changeEvent(QEvent *e)
    {
        if (e != nullptr && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
        }
        QDialog::changeEvent(e);
    }

    void Welcome::loginUserOnClose()
    {
//        NetworkClient* client = &pApp->client();
//        request_lobby* request = client->request<request_lobby>();

//        connect(client->emitter<reply_lobby>(), &emitter_lobby::login, this, &Welcome::onLogin, Qt::UniqueConnection);
//        auto rpc = rpc_object<rpc_lobby>();
//        rpc->login(nickname(), (const QByteArray)m_passinfo.first, QByteArray("")
//        , [this](ns_lobby::e_status result, const ns_lobby::user_info_t&){
//            if (ns_helpers::isOk(result)) {
//                auto* model = modelsDB().getUserAdditionalInfo();
////                model->getItemByKey("nickname", "common")->setData(nickname(), Qt::EditRole);
////                model->getItemByKey("password", "common")->setData((const QByteArray)m_passinfo.first, UserType::OriginDataRole);
//                accept();
//            } else {
//                reject();
//            }
//        });
    }


    void Welcome::init()
    {
        if (isInitialized())
            return;

//        m_ui->profile_picture->setAlphaMask(QPixmap(":textures/Common/icons/icon_avatar_cap.png"));

        onInitialized();
    }

    QString Welcome::nickname() const {
        return m_ui->text_username->text();
    }

    void Welcome::setPassinfo(const sha1_t &hash, int len) {
        m_passinfo = std::make_pair(hash, len);
    }

    void Welcome::setNickname(const QString& nick) {
        m_ui->text_username->setText(nick);
    }

    void Welcome::setAvatar(const Avatar& avatar) {
        m_ui->profile_picture->setAvatar(avatar);
    }

} // namespace dialog

#endif // !NO_REGISTRATION
