#pragma once

#include <tools/types_fwd.hpp>
#include <interface/initable.h>
#include <interface/NetworkHandlers.h>
#include <QDialog>
#include <db/Avatar.h>

#ifndef Q_MOC_RUN
//    #include "rpc/reply_lobby.h"
//    #include "rpc/network_client.h"
    #include <rpcbase/emitter_lobby.h>
#endif // Q_MOC_RUN

namespace Ui {
class AccountPage;
}
namespace ns_model {
class user_info_t;
}
class Avatar;

namespace ns_dialog
{
    class Account : public InitableDialog, protected IReplyHandler
    {
        Q_OBJECT
    private:
        typedef InitableDialog super;
        Ui::AccountPage *m_ui;
        Avatar m_pendingAvatar;
        QString m_birthdateStr;
        std::set<QString> m_documents;
    signals:
        void closed();
        void avatarChanged(const QIcon&);
    public:
        explicit Account(QWidget *parent = 0, Qt::WindowFlags flags = 0);
        virtual ~Account();

        virtual void init() override;
        virtual void registerNetworkReplyHandlers(const NetworkClient* client);

        void setModel(ns_model::user_info_t *model);
        bool isAddressInputCorrect() const;

        void calcPositions(QPoint &shownPos, QPoint &hiddenPos);
    public slots:
        virtual void reject() override;
    protected slots:
        void resetAllPages();

        void onEditAddress();
        void onApplyPassword();
        void onApplyEMail();
        void onApplyCode();
        void onApplyPhone();
        void onApplyAddress();
        void onChangeAvatar();
        void onChangeContacts();
        void uploadDocument();
        void updateUploadedFiles();
    private slots:
        void change_password(ns_lobby::e_status result);
        void change_avatar(ns_lobby::e_status result);
        void update_phone_number(ns_lobby::e_status status);
        void update_address(ns_lobby::e_status status);
        void on_bttn_send_clicked();
    protected:
        virtual void changeEvent(QEvent *e) override;
        void uploadFiles(QStringList &&files);
    };
} // namespace dialog
