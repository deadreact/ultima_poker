#include <precompiled.h>
#include "dialog_account.h"
#include "ui_dialog_Account.h"
#include <utils/logger.h>
#include "dialog_okmessage.h"
#include <PokerApplication.h>
#include "../../../EMailSender/emailsender.h"
#ifndef Q_MOC_RUN
    #include <rpc/request_lobby.h>
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_lobby.h>
#endif // Q_MOC_RUN

#include <core/money.h>
#include <pages/page_SelectAvatar.h>
#include <pages/page_UploadAvatar.h>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QPixmapCache>
#include <dialogs/dialog_changeavatar.h>
#include <utils/IconProvider.h>
#include <QMimeData>
#include <QUrl>
#include <api_models/api_models.hpp>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qtimer.h>
#include <db/modeldatabase.h>
#include <models/model_user_additional_info.h>

/*==================================================================================================
 *==================================================================================================
 *==================================================================================================
 *==================================================================================================
 *==================================================================================================
 *==================================================================================================
 */

namespace {
    QString operator*(const QChar& symbol, int len) {
        QString resultString = "";
        for (int i = 0; i < len; i++) {
            resultString += symbol;
        }
        return resultString;
    }

    using e_status = ns_lobby::e_status;

    rpc_lobby* getRpc() {
        return rpc_object<rpc_lobby>();
    }
}

namespace ns_dialog
{
    Account::Account(QWidget *parent, Qt::WindowFlags flags)
        : super(parent, flags)
        , m_ui(new Ui::AccountPage)
        , m_pendingAvatar()
        , m_birthdateStr("")
    {
        m_ui->setupUi(this);
        addAction(m_ui->actionReject);
    }

    Account::~Account()
    {
        delete m_ui;
    }

    void Account::setModel(ns_model::user_info_t *model)
    {
        if (model == nullptr)
        {
            Logger::error("UserInfo is NULL!!", __FILE__, __LINE__);
            return;
        }

        using field = ns_model::user_info_t::field;

        switch (model->data(model->indexOf(field::_verifying), Qt::CheckStateRole).value<Qt::CheckState>()) {
        case Qt::Unchecked:
            m_ui->zstack->setCurrentWidget(m_ui->page_verification);
            m_ui->bttnGrp_categories->setId(m_ui->bttn_verification,  m_ui->zstack->indexOf(m_ui->page_verification));
            break;
        case Qt::PartiallyChecked:
        {
            m_ui->info_verification_status->setCurrentMsgIndex(0);
            m_ui->zstack->setCurrentWidget(m_ui->page_verification_2);
            m_ui->bttnGrp_categories->setId(m_ui->bttn_verification,  m_ui->zstack->indexOf(m_ui->page_verification_2));
            break;
        }
        case Qt::Checked:
        {
            m_ui->info_verification_status->setCurrentMsgIndex(1);
            m_ui->zstack->setCurrentWidget(m_ui->page_verification_2);
            m_ui->bttnGrp_categories->setId(m_ui->bttn_verification,  m_ui->zstack->indexOf(m_ui->page_verification_2));
            break;
        }
        default:
            break;
        }


        ns_model::registerDisplayWidget(model, m_ui->money_cash_available, field::_money);
        ns_model::registerDisplayWidget(model, m_ui->money_cash_ingame,    field::_in_game_money);
        ns_model::registerDisplayWidget(model, m_ui->textlink_nickname,    field::_nickname);
        ns_model::registerDisplayWidget(model, m_ui->info_email,           field::_email);
        ns_model::registerDisplayWidget(model, m_ui->info_gender,          field::_gender);
        ns_model::registerDisplayWidget(model, m_ui->info_birthdate,       field::_birth_date);
        ns_model::registerDisplayWidget(model, m_ui->info_country,         field::_country);
        ns_model::registerDisplayWidget(model, m_ui->info_city,            field::_city);
        ns_model::registerDisplayWidget(model, m_ui->info_address,         field::_mail_address);
        ns_model::registerDisplayWidget(model, m_ui->info_index,           field::_mail_index);
        ns_model::registerDisplayWidget(model, m_ui->info_phone,           field::_phone);
        ns_model::registerDisplayWidget(model, m_ui->bttn_status,          field::_vip_status);

        connect(model, &ns_model::user_info_t::dataChanged, this, [this, model](const QModelIndex& idx){
            if (model->fieldOf(idx) == field::_money || model->fieldOf(idx) == field::_in_game_money) {
                m_ui->money_cash_total->setText(Money(model->money(field::_money) + model->money(field::_in_game_money)).toString());
            }
        });
        m_ui->money_cash_total->setText(Money(model->money(field::_money) + model->money(field::_in_game_money)).toString());

//        m_ui->money_cash_total->setText(Money(model->_money + model->_in_game_money).toString(Money::DisplayType::Full));


        ns_model::registerDisplayWidget(model, m_ui->input_phone,   field::_phone,        Qt::EditRole);
        ns_model::registerDisplayWidget(model, m_ui->input_email,   field::_email,        Qt::EditRole);
        ns_model::registerDisplayWidget(model, m_ui->input_country, field::_country,      Qt::EditRole, "currentIndex");
        ns_model::registerDisplayWidget(model, m_ui->input_city,    field::_city,         Qt::EditRole);
        ns_model::registerDisplayWidget(model, m_ui->input_address, field::_mail_address, Qt::EditRole);
        ns_model::registerDisplayWidget(model, m_ui->input_index,   field::_mail_index,   Qt::EditRole);
//        m_ui->input_email->setText(model->_email.c_str());

//        m_ui->input_country->setCurrentIndex(model->_country);
//        m_ui->input_address->setPlainText(model->_mail_address.c_str());
//        m_ui->input_city->setText(model->_city.c_str());
//        m_ui->input_index->setText(model->_mail_index.c_str());

//        m_ui->input_phone->setText(model->_phone.c_str());
        if (isVisible()) {
            repaint();
        }        
    }

    void Account::init()
    {
        if (m_isInitialized)
            return;

        NetworkClient& client = netClient();
        ns_model::user_info_t* model = modelsDB().getUserInfo();
        const gbp::ns_api::ns_lobby::user_info_t& info = *model->nativeData();

        registerNetworkReplyHandlers(&client);
        setModel(model);

        m_ui->input_confirmation->setPasswordLineEdit(m_ui->input_password_new);

        m_birthdateStr = m_ui->info_birthdate->text();

        sha1_t hash = sha1_t::fromStdString(info._hash_avatar);

        QString avatarName = Avatar::findInPath(":/textures/avatars", hash);

        if (!avatarName.isNull() && !avatarName.isEmpty()) {

        }

        m_ui->profile_picture->setIcon(pApp->db().getAvatarPixmap(hash));

        m_ui->info_firstname->setText(info._username.c_str());
        m_ui->info_lastname->setText(info._last_name.c_str());
        m_ui->info_gender->setText( (((int)info._gender == 0) ? tr("@gender_male") : tr("@gender_female")) );
        m_ui->input_password_current->setText( modelsDB().getUserAdditionalInfo()->getItem("password", "common")->data(Qt::EditRole).toString() );
        m_ui->info_email->setText(info._email.c_str());


        QString passModifiedDate = "";
        if (ns_utildb::restoreState("LastPasswordModifiedDate", passModifiedDate))
        {
    //        m_ui->msg_modified->setProperty("dateTime", passModifiedDate);
        }
    //    QString str = m_ui->msg_modified->property("translatedString").toString();
    //    m_ui->msg_modified->setText(str.arg(passModifiedDate));

    //    m_ui->block_menu->init();

        connect(m_ui->input_country, (void(CountryComboBox::*)(int))&CountryComboBox::currentIndexChanged, this, &Account::onEditAddress);
        connect(m_ui->input_city, &NotEmptyLineEdit::textEdited, this, &Account::onEditAddress);
        connect(m_ui->input_index, &PostIndexLineEdit::textEdited, this, &Account::onEditAddress);
        connect(m_ui->input_address, &NotEmptyPlainTextEdit::textChanged, this, &Account::onEditAddress);

        //----------------------------------------------------------------------------------------

        connect(m_ui->bttn_edit_contacts, &QPushButton::clicked, this, [this]{ m_ui->zstack->setCurrentWidget(m_ui->page_edit_contacts); });
        connect(m_ui->bttn_cancel, &QPushButton::clicked, this, [this]{ m_ui->zstack->setCurrentWidget(m_ui->page_contacts); });


        m_ui->bttnGrp_verificationFiles->setId(m_ui->bttn_remove_file_1, 0);
        m_ui->bttnGrp_verificationFiles->setId(m_ui->bttn_remove_file_2, 1);
        m_ui->bttnGrp_verificationFiles->setId(m_ui->bttn_remove_file_3, 2);

        connect(m_ui->bttnGrp_verificationFiles, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this
        , [this](QAbstractButton* bttn) {
            m_documents.erase(bttn->property("filePath").toString());
            updateUploadedFiles();
        });

        connect(m_ui->bttn_cancel_verification, &QPushButton::clicked, this, [this]{
            m_documents.clear();
            updateUploadedFiles();
        });

        connect(m_ui->page_verification, &DropArea::objectDropped, this, [this](const QMimeData* mimeData) {
            QStringList lst;
            for (const QUrl &url: mimeData->urls()) {
                if (m_ui->page_verification->acceptedFormats().contains(QFileInfo(url.toLocalFile()).suffix())) {
                    lst << url.toLocalFile();
                }
            }
            uploadFiles(std::move(lst));
        });

        //----------------------------------------------------------------------------------------

        m_ui->profile_picture->setParentPainting(E::ParentPainting::PaintParentAfter);

//        int id = 1;

//        m_ui->bttnGrp_categories->setId(m_ui->bttn_verification,  m_ui->zstack->indexOf(m_ui->page_verification));
        m_ui->bttnGrp_categories->setId(m_ui->bttn_personal_info, m_ui->zstack->indexOf(m_ui->page_personal_info));
        m_ui->bttnGrp_categories->setId(m_ui->bttn_contacts,      m_ui->zstack->indexOf(m_ui->page_contacts));
        m_ui->bttnGrp_categories->setId(m_ui->bttn_security,      m_ui->zstack->indexOf(m_ui->page_edit_password));


        connect(m_ui->check_showPassword, &QCheckBox::toggled, this, [this](bool result) {
            QLineEdit::EchoMode echoMode = (result) ? QLineEdit::Normal : QLineEdit::Password;

            m_ui->input_password_current->setEchoMode(echoMode);
            m_ui->input_password_new->setEchoMode(echoMode);
            m_ui->input_confirmation->setEchoMode(echoMode);
        });

        onInitialized();
    }

    void Account::updateUploadedFiles()
    {
        int docsCount = m_documents.size();

        m_ui->bttn_upload_file->setHidden(docsCount == 3);
        m_ui->bttn_send->setDisabled(docsCount == 0);
        m_ui->bttn_cancel_verification->setDisabled(docsCount == 0);
        m_ui->page_verification->setAcceptDrops(docsCount < 3);

        m_ui->bttnGrp_verificationFiles->button(0)->setVisible(docsCount > 0);
        m_ui->bttnGrp_verificationFiles->button(1)->setVisible(docsCount > 1);
        m_ui->bttnGrp_verificationFiles->button(2)->setVisible(docsCount > 2);
        auto it = m_documents.begin();
        for (int id = 0; id < docsCount; id++, it++)
        {
            QAbstractButton* bttn = m_ui->bttnGrp_verificationFiles->button(id);
            QFileInfo fileinfo(*it);

            bttn->setText(fileinfo.fileName());
            bttn->setProperty("filePath", *it);
            QPixmap pixmap(*it);
            bttn->setIcon( pixmap.isNull() ? pApp->iconProvider().fileIcon(*it) : QIcon(pixmap));
        }
    }

    void Account::registerNetworkReplyHandlers(const NetworkClient *client)
    {
        const reply_lobby::emitter_t        *emitter_reply1 = client->reply<reply_lobby>()->emitter();

        connect(emitter_reply1, &reply_lobby::emitter_t::change_password,    this, &Account::change_password, Qt::UniqueConnection);
//        connect(emitter_reply1, &reply_lobby::emitter_t::change_avatar, this, &Account::change_avatar);

        connect(emitter_reply1, &reply_lobby::emitter_t::update_phone_number, this, &Account::update_phone_number);
        connect(emitter_reply1, &reply_lobby::emitter_t::update_address,      this, &Account::update_address);
    }

    bool Account::isAddressInputCorrect() const
    {
        return m_ui->input_address->isCorrect() && m_ui->input_country->isCorrect() && m_ui->input_city->isCorrect() && m_ui->input_index->isCorrect();
    }

    void Account::calcPositions(QPoint& shownPos, QPoint& hiddenPos)
    {
        if (QWidget* parent = parentWidget())
        {
            hiddenPos.setX(-width());
            hiddenPos.setY((parent->height() - height())/2);
            shownPos.setX(10);
            shownPos.setY((parent->height() - height())/2);
        }
    }

    void Account::resetAllPages()
    {
        const gbp::ns_api::ns_lobby::user_info_t* data = modelsDB().getUserInfo()->nativeData();
        m_ui->input_password_current->clear();
        m_ui->input_password_new->clear();
        m_ui->input_confirmation->clear();
        m_ui->input_country->setCurrentIndex(data->_country);
        m_ui->input_city->setText(data->_city.c_str());
        m_ui->input_address->setPlainText(data->_mail_address.c_str());
        m_ui->input_index->setText(data->_mail_index.c_str());
        m_ui->imsg_incorrect_password->resetCurrentMsgIndex();
        m_ui->imsg_incorrect_password_2->resetCurrentMsgIndex();
        m_ui->imsg_mismatch->resetCurrentMsgIndex();
        m_ui->imsg_email->resetCurrentMsgIndex();
    }

    void Account::onEditAddress()
    {
        m_ui->bttn_apply->setEnabled(isAddressInputCorrect());
    }

    void Account::onApplyPassword()
    {
//        m_ui->imsg_incorrect_phone
        if (m_ui->input_password_current->IsIncorrect()) {
            m_ui->imsg_incorrect_password->setCurrentMsgIndex(0);
            m_ui->imsg_incorrect_password_2->resetCurrentMsgIndex();
            m_ui->imsg_mismatch->resetCurrentMsgIndex();
            return;
        }
        if (m_ui->input_password_new->IsIncorrect()) {
            m_ui->imsg_incorrect_password_2->setCurrentMsgIndex(0);
            m_ui->imsg_incorrect_password->resetCurrentMsgIndex();
            m_ui->imsg_mismatch->resetCurrentMsgIndex();
            return;
        }
        if (m_ui->input_confirmation->IsIncorrect())
        {
            m_ui->imsg_incorrect_password->resetCurrentMsgIndex();
            m_ui->imsg_mismatch->setCurrentMsgIndex(0);
            return;
        }
        m_ui->imsg_mismatch->resetCurrentMsgIndex();
//        request_lobby* request = pApp->client().request<request_lobby>();
        sha1_t old_hash = ns_util::toSha1(m_ui->input_password_current->text());
        sha1_t new_hash = ns_util::toSha1(m_ui->input_password_new->text());

        getRpc()->change_password(old_hash, new_hash);
    }

    void Account::onApplyEMail()
    {

        if (m_ui->input_email->IsIncorrect())
        {
            m_ui->imsg_email->setCurrentMsgIndex(0);
            return;
        }
        m_ui->imsg_email->resetCurrentMsgIndex();

        getRpc()->change_email_step1(m_ui->input_email->text(), [this](e_status result) {
            if (ns_helpers::isOk(result)) {
            } else if (result == e_status::new_email_already_exists) {
                m_ui->imsg_email->setCurrentMsgIndex(1);
            } else {
                m_ui->imsg_email->setCurrentMsgIndex(0);
            }
        });
    }

    void Account::onApplyCode()
    {
    //    request_authorization* request = NetworkClient::GetInstance()->GetRequestObject<request_authorization>();
    //    request->change_email_step2();
        m_ui->info_email->setText(m_ui->input_email->text());

        modelsDB().getUserInfo()->updateEmail(m_ui->input_email->text());

        OkMessage::execMessage(tr("@msg_emailchanged"), tr("@Title_emailchanged"));

        resetAllPages();
        m_ui->bttn_cancel->setChecked(true);
    }

    void Account::onApplyPhone()
    {
        if (m_ui->input_phone->IsIncorrect())
        {
            m_ui->imsg_incorrect_phone->show();// setCurrentMsgIndex(0);
            return;
        }
//        m_ui->imsg_incorrect_phone->resetCurrentMsgIndex();
        request_lobby* request = pApp->client().request<request_lobby>();
        request->update_phone_number(m_ui->input_phone->text().toStdString());
    }

    void Account::onApplyAddress()
    {
        const CountryComboBox& country = *m_ui->input_country;
        const NotEmptyLineEdit& city = *m_ui->input_city;
        const PostIndexLineEdit& index = *m_ui->input_index;
        const NotEmptyPlainTextEdit& address = *m_ui->input_address;

        if (!isAddressInputCorrect())
        {
            // Some msg?
            return;
        }

        request_lobby* request = pApp->client().request<request_lobby>();
        request->update_address(country.currentIndex(), city.text().toStdString(), index.text().toStdString(), address.toPlainText().toStdString());
    }

    void Account::onChangeAvatar()
    {
        m_pendingAvatar = ChangeAvatar::getAvatar();
        if (!m_pendingAvatar.icon().isNull())
        {
//            request_lobby* request = pApp->client().request<request_lobby>();

//            request->change_avatar(ns_util::toBase64(pix).toStdString());
//            request->change_avatar(ns_util::toBase64(m_pendingAvatar.icon().pixmap(244)).toStdString());
            getRpc()->change_avatar(m_pendingAvatar.icon().pixmap(244), [this](e_status result){
                if (ns_helpers::isOk(result)) {
                    m_ui->profile_picture->setAvatar(m_pendingAvatar);
                    emit avatarChanged(m_pendingAvatar.icon());

                    m_pendingAvatar.clearPixmaps();
                }
            });
        }

    }

    void Account::onChangeContacts()
    {
        if ( m_ui->info_country->text() != m_ui->input_country->currentText())
        {
            onApplyAddress();
            onApplyPhone();
        }
        else if ( m_ui->info_city->text() != m_ui->input_city->text()
                 || m_ui->info_index->text() != m_ui->input_index->text()
                 || m_ui->info_address->text() != m_ui->input_address->toPlainText())
        {
            onApplyAddress();
            if ( m_ui->info_phone->text() != m_ui->input_phone->text()) {
                onApplyPhone();
            }
        }
        else if ( m_ui->info_phone->text() != m_ui->input_phone->text())
        {
            onApplyPhone();
        }

        if (m_ui->input_email->text() != m_ui->info_email->text()) {
            onApplyEMail();
        }

    }

    void Account::uploadDocument()
    {
        QStringList files = QFileDialog::getOpenFileNames(this, QString(), QString(), "*.png *.jpg *.pdf");

        uploadFiles(std::move(files));
    }

    void Account::uploadFiles(QStringList&& files)
    {
        while (m_documents.size() < 3 && !files.isEmpty()) {
            QString file = files.takeFirst();
            QFileInfo info(file);
            if (/*(info.completeSuffix() == "png"
              || info.completeSuffix() == "jpg"
              || info.completeSuffix() == "pdf")
              && */info.size() < 5 * 1024 * 1024)
            {
    //            QAbstractButton* bttn = m_ui->bttnGrp_verificationFiles->button(m_documents.size());
                m_documents.insert(file);
            }
        }
        updateUploadedFiles();
    }

    void Account::change_password(ns_lobby::e_status result)
    {
        if (!ns_helpers::isOk(result)) {
            m_ui->imsg_incorrect_password->setCurrentMsgIndex(0);
        } else {
            auto* model = modelsDB().getUserAdditionalInfo();
            model->getItem("password", "common")->setData(ns_util::toSha1(m_ui->input_password_new->text()), UserType::OriginDataRole);//, , Qt::DisplayRole);
            model->getItem("password", "common")->setData("********", Qt::EditRole);//, m_ui->input_password_new->text().length(), Qt::DisplayRole);


            OkMessage::execMessage(tr("@msg_passchanged"), tr("@Title_passchanged"));
            resetAllPages();
            m_ui->bttn_cancel->setChecked(true);

            QString stateTimezone;
            QByteArray timezoneId = QTimeZone::systemTimeZoneId();
            if (ns_utildb::restoreState("CurrentTimeZone", stateTimezone))
                timezoneId = stateTimezone.toLatin1();

            QString currentDateTime = QDateTime::currentDateTimeUtc().toTimeZone(QTimeZone(timezoneId)).toString("yy-MM-dd hh:mm:ss");

    //        m_ui->msg_modified->setProperty("dateTime", currentDateTime);
    //        QString str = m_ui->msg_modified->property("translatedString").toString();
    //        m_ui->msg_modified->setText(str.arg(currentDateTime));
            ns_utildb::saveState("LastPasswordModifiedDate", currentDateTime);
        }
    }

    void Account::change_avatar(ns_lobby::e_status result)
    {
        if (ns_helpers::isOk(result)) {
            m_ui->profile_picture->setAvatar(m_pendingAvatar);
            emit avatarChanged(m_pendingAvatar.icon());

            m_pendingAvatar.clearPixmaps();
        }
    }

    void Account::update_phone_number(ns_lobby::e_status status)
    {
        m_ui->imsg_incorrect_phone->setHidden(ns_helpers::isOk(status));
        if (ns_helpers::isOk(status)) {
            QString phone = m_ui->input_phone->text();

            modelsDB().getUserInfo()->updatePhoneNumber(phone);
            m_ui->info_phone->setText(phone);
        } else {
        }
    }

    void Account::update_address(ns_lobby::e_status status)
    {
        if (ns_helpers::isOk(status)) {
            const CountryComboBox& country = *m_ui->input_country;
            const NotEmptyLineEdit& city = *m_ui->input_city;
            const PostIndexLineEdit& index = *m_ui->input_index;
            const NotEmptyPlainTextEdit& address = *m_ui->input_address;

            CountryInfo cInfo = country.view()->currentIndex().data(UserType::RowDataRole).value<CountryInfo>();
            modelsDB().getUserInfo()->updateAddress(QLocale::Country(cInfo.id)
                                                     , city.text()
                                                     , index.text()
                                                     , address.toPlainText());
            m_ui->info_country->setText(cInfo.name);
            m_ui->info_city->setText(city.text());
            m_ui->info_address->setText(address.toPlainText());
            m_ui->info_index->setText(index.text());
        }
    }

    void Account::on_bttn_send_clicked()
    {
        ns_email_sender::message msg;
        ns_email_sender::SMTPInfo info("config.ini");
        //QStringList lst;
        //lst << info;
        //qDebug() << lst;
        msg.text = QString("user id %0").arg(netClient().userId());
        for (const QString& path: m_documents) {
            msg.attachments << path;
        }


        OkMessage* msgDialog = new OkMessage(this);
        msgDialog->setAttribute(Qt::WA_DeleteOnClose);
        msgDialog->setText(tr("@wait")+"...");
        msgDialog->setTitle("@title_verification");
        msgDialog->open();
        msgDialog->setEnabled(false);
        QFuture<int> res = QtConcurrent::run([this, msg]{
            return ns_email_sender::verification(&msg);
        });

        QTimer* timer = new QTimer(this);
        static int counter = 0;
        connect(timer, &QTimer::timeout, this, [this, timer, res, msgDialog]
        {
            QString txt = tr("@wait");
            for (int i =0; i < counter; i++) txt += ".";
            if (counter >= 3) {
                counter = 0;
            } else {
                counter++;
            }
            msgDialog->setText(txt);
            if (res.isRunning()) {
                return;
            }
            int errcode = (int)res;
            if (errcode == 0) {
                msgDialog->setText(tr("@succsesfull"));
                m_documents.clear();
                updateUploadedFiles();
            }
            else
            {
                msgDialog->setText(tr("@fail %0").arg(errcode));
    //                OkMessage::execMessage(tr("@verification_error_%0").arg(errcode), tr("@title_verification"), geometry());
            }
            msgDialog->setEnabled(true);
            timer->stop();
            timer->deleteLater();
        });
        timer->start(500);
    }

    void Account::changeEvent(QEvent *e)
    {
        if(e != nullptr && e->type() == QEvent::LanguageChange)
        {
            m_ui->retranslateUi(this);
        }
        super::changeEvent(e);
    }

    void Account::reject()
    {
        emit rejected();
    }
} //namespace dialog
