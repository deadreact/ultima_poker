#include <precompiled.h>
#include "dialog_registration.h"

#ifndef NO_REGISTRATION

#include "const.h"
#include "dialog_okmessage.h"
#include "ui_dialog_Registration.h"
#include <QTimer>
#include "db/Database.h"
#include "dialog_welcome.h"
#include <PokerApplication.h>
#include <QFileDialog>
#include <models/model_AvatarsList.h>
#include <QToolTip>
#include <styles/shadowedtextstyle.h>
#include <db/SharedDataService.h>

#ifndef Q_MOC_RUN
    #include <api/tools/sha1.h>
    #include "rpc/network_client.h"
#endif //Q_MOC_RUN
#include "widgets/PasswordToolTip.h"

#include <pages/page_SelectAvatar.h>
#include <pages/page_UploadAvatar.h>

#ifndef Q_MOC_RUN
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_lobby.h>
#endif //Q_MOC_RUN

#include <basewidgets/util.h>

namespace {
    void findInputWidgets(QObject* obj, QVector<CorrectInput*>& container) {
        for (QObject* child: obj->children()) {
            if (CorrectInput* inputObj = dynamic_cast<CorrectInput*>(child)) {
                container.push_back(inputObj);
            }
            findInputWidgets(child, container);
        }
    }
}



namespace ns_dialog
{
    Registration::Registration(QWidget *parent, Qt::WindowFlags flags)
        : InitableDialog(parent, flags)
        , m_ui(new Ui::RegistrationForm)
//        , m_pressedButton(nullptr)
        , m_passToolTip(nullptr)
    {
        m_ui->setupUi(this);
//        m_ui->bttn_accept->setEnabled(true);
    }

    Registration::~Registration()
    {
        if (m_passToolTip) {
            delete m_passToolTip;
        }
        delete m_ui;
    }

    void Registration::changeEvent(QEvent *e)
    {
        QWidget::changeEvent(e);
        switch (e->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            onIndicatorChanged(qreal(m_passToolTip->progress())/100.);
            break;
        default:
            break;
        }
    }

    void Registration::initPasswordToolTip()
    {
        if (m_passToolTip == nullptr) {
            m_passToolTip = new PasswordToolTip(m_ui->input_password);
            m_passToolTip->setWindowFlags(Qt::ToolTip);
            QVector<QPoint> maskPoints;
            maskPoints.push_back(QPoint(0, 0));
            maskPoints.push_back(QPoint(263, 0));
            maskPoints.push_back(QPoint(263, 46));
            maskPoints.push_back(QPoint(271, 54));
            maskPoints.push_back(QPoint(263, 63));
            maskPoints.push_back(QPoint(263, 111));
            maskPoints.push_back(QPoint(0, 111));
            m_passToolTip->setMask(QRegion(QPolygon(maskPoints)));
        }
    }

    void Registration::init()
    {
        if (isInitialized())
            return;

        initPasswordToolTip();
        m_ui->input_confirmation->setPasswordLineEdit(m_ui->input_password);

        findInputWidgets(this, m_inputFields);

        auto senderSimpleCheck = [this] {
            if (CorrectInput* input = dynamic_cast<CorrectInput*>(sender())) {
                if (input->IsIncorrect()) {
                    senderIncorrect();
                } else {
                    senderCorrect();
                }
            }
        };


        connect(m_ui->input_firstname, &NotEmptyLineEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_lastname, &NotEmptyLineEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_country, &CountryComboBox::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_city, &NotEmptyLineEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_address, &NotEmptyPlainTextEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_index, &PostIndexLineEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_year, &CheckedComboBox::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_month, &CheckedComboBox::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_day, &CheckedComboBox::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_phone, &PhoneNumberLineEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_accept_rules, &CheckedCheckBox::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_password, &PasswordLineEdit::focused, this, &Registration::senderCorrect);
        connect(m_ui->input_confirmation, &ConfirmationLineEdit::focused, this, &Registration::senderCorrect);

        connect(m_ui->input_firstname, &NotEmptyLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_lastname, &NotEmptyLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_country, &CountryComboBox::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_city, &NotEmptyLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_address, &NotEmptyPlainTextEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_index, &PostIndexLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_year, &CheckedComboBox::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_month, &CheckedComboBox::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_day, &CheckedComboBox::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_phone, &PhoneNumberLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_accept_rules, &CheckedCheckBox::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_password, &PasswordLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });
        connect(m_ui->input_confirmation, &ConfirmationLineEdit::unfocused, this, [this, senderSimpleCheck]{ senderSimpleCheck(); });

        connect(m_ui->input_country, &CountryComboBox::currentTextChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_firstname, &NotEmptyLineEdit::textChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_lastname, &NotEmptyLineEdit::textChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_city, &NotEmptyLineEdit::textChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_address, &NotEmptyPlainTextEdit::textChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_index, &PostIndexLineEdit::textChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_year, &CheckedComboBox::currentTextChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_month, &CheckedComboBox::currentTextChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_day, &CheckedComboBox::currentTextChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_phone, &PhoneNumberLineEdit::textChanged, this, &Registration::updateButtonState);
        connect(m_ui->input_accept_rules, &CheckedCheckBox::toggled, this, &Registration::updateButtonState);
        connect(m_ui->input_password, &PasswordLineEdit::correctStateUpdated, this, &Registration::updateButtonState);
        connect(m_ui->input_confirmation, &ConfirmationLineEdit::correctStateUpdated, this, &Registration::updateButtonState);

        int entryPoint = QDate::currentDate().year() - 18;

        for (int i = entryPoint; i > 1900; i--){
            m_ui->input_year->addItem(QString("%0").arg(i));
        }
        connect(m_ui->input_password, SIGNAL(passwordDifficultyChanged(qreal)), this, SLOT(onIndicatorChanged(qreal)));

        onIndicatorChanged(0);

        if (QWidget* parent = parentWidget()) {
            int wndTitle = parent->frameSize().height() - parent->height();
            QPoint offset((parent->width() - width())/2, (parent->height() - height())/2 + wndTitle);
            move(parent->pos() + offset);
        }

        initAvatarsPage1();
        initAvatarsPage2();
        connect(m_ui->bttn_selectAvatar, &QAbstractButton::clicked, this, [this]{ m_ui->stackOfContent->setCurrentIndex(1); });

        QList<QAbstractButton*> buttons = findChildren<QAbstractButton*>(QRegExp("bttn_\\w+"));

        for (QAbstractButton* button: buttons) {
            setTextDropShadowStyle(button, QPoint(0, 1), QColor(0, 0, 0, 100));
        }
        setTextDropShadowStyle(m_ui->header_1, QPoint(0, 1), 0x191919);
        setTextDropShadowStyle(m_ui->header_2, QPoint(0, 1), 0x191919);

        onInitialized();
    }

    void Registration::reset()
    {
//        hide();

        m_ui->input_accept_rules->setChecked(false);
        m_ui->input_address->clear();
        m_ui->input_bonus->clear();
        m_ui->input_city->clear();
        m_ui->input_confirmation->clear();
        m_ui->input_email->clear();
        m_ui->input_firstname->clear();
        m_ui->input_index->clear();
        m_ui->input_lastname->clear();
        m_ui->input_year->setCurrentIndex(0);
        m_ui->input_month->setCurrentIndex(0);
        m_ui->input_day->setCurrentIndex(0);
        m_ui->input_country->setCurrentIndex(0);
        m_ui->input_nickname->clear();
        m_ui->input_password->clear();
        m_ui->input_phone->clear();
//        m_ui->imsg_email->clear();
//        m_ui->imsg_mismatch->clear();
//        m_ui->imsg_nickname_1->clear();
//        m_ui->imsg_nickname_2->clear();


        unselectAsIncorrect(m_invalidInputLst);

        //m_ui->profile_picture->setPixImage(QPixmap());
//        show();
    }

    void Registration::accept()
    {
//        QWidgetList toCorr, toIncorr;
//        if (!m_invalidInputLst.isEmpty())
//        {
//            for (QWidget* w: m_invalidInputLst)
//            {
//               if (CorrectInput* in = dynamic_cast<CorrectInput*>(w))
//               {
//                   if (in->isCorrect()) toCorr.push_back(w);
//               }
//            }
//            m_invalidInputLst = findInvalidInput();
//            for (QWidget* w: m_invalidInputLst) {
//                if (w->property("incorrect_color_value").toInt() != 250) {
//                    toIncorr.push_back(w);
//                }
//            }
//        } else {
//            m_invalidInputLst = findInvalidInput();
//            toIncorr          = findInvalidInput();
//        }
//        if ( m_invalidInputLst.isEmpty() ) {
//            gbp::ns_api::ns_lobby::user_register_info_t user_info;
//            createDataStruct(user_info);

////            request_lobby* request = pApp->client().request<request_lobby>();
////            request->register_user(user_info);

//            rpc_object<rpc_lobby>()->register_user(user_info, [this](reply_lobby::e_lobby result){
//                if (ns_helpers::isOk(result)) {
//                    this->onUserRegistered();
//                } else {
//                    OkMessage::execMessage("Error! Check all fields and try again (server denied)", "Registration");
//                }
//            });
//        }
//        selectAsIncorrect(toIncorr);
//        unselectAsIncorrect(toCorr);
    }

    void Registration::reject()
    {
        int pageIndex = m_ui->stackOfContent->currentIndex();
        if (pageIndex > 0) {
            m_ui->stackOfContent->setCurrentIndex(pageIndex - 1);
        } else {
            QDialog::reject();
        }
    }


    void Registration::onIndicatorChanged(qreal v)
    {
        m_passToolTip->updateStateByProgress(v*100, m_ui->input_password->text().length());
    }

    QWidgetList Registration::findInvalidInput()
    {
        QWidgetList lst;
        for (CorrectInput* it: m_inputFields) {
            if (it->IsIncorrect()) lst.push_back(dynamic_cast<QWidget*>(it));
        }

        return lst;
    }

    void Registration::createDataStruct(gbp::ns_api::ns_lobby::user_register_info_t &dataStruct) const
    {
//        dataStruct._nick   = m_ui->input_nickname->text().toStdString();
//        dataStruct._pass   = gbp::ns_crypt::to_sha1(m_ui->input_password->text().toStdString());
//        dataStruct._username  = m_ui->input_firstname->text().toStdString();
//        dataStruct._last_name   = m_ui->input_lastname->text().toStdString();
//        dataStruct._email      = m_ui->input_email->text().toStdString();
//        dataStruct._country    = m_ui->input_country->currentData(UserType::RowDataRole).value<CountryInfo>().id;
//        dataStruct._city       = m_ui->input_city->text().toStdString();
//        dataStruct._locale     = QLocale::system().name().toStdString();

////        QDate birthDate(m_ui->input_year->currentText().toInt(), m_ui->input_month->currentText().toInt(), m_ui->input_day->currentText().toInt());

//        dataStruct._birth_date._year  = m_ui->input_year->currentText().toInt();
//        dataStruct._birth_date._month = m_ui->input_month->currentIndex();
//        dataStruct._birth_date._day   = m_ui->input_day->currentIndex();
////        dataStruct._birth_date = QDateTime(birthDate).toTime_t();
//        dataStruct._mail_address = m_ui->input_address->toPlainText().toStdString();
//        dataStruct._gender     = m_ui->input_sex->currentIndex() == 0 ? gbp::ns_details::e_gender::male : gbp::ns_details::e_gender::female;
//        dataStruct._mail_index = m_ui->input_index->text().toStdString();
//        dataStruct._phone      = m_ui->input_phone->text().toStdString();

//        SharedDB& db = pApp->db();
//        QIcon icon = m_ui->profile_picture->icon();
//        if (icon.isNull())
//        {
//            base64_t data = ns_util::toBase64(db.placeholder(E::Placeholder::Avatar));
//            dataStruct._avatar = data.toStdString();
//            db.addAvatar(data);
//        }
//        else
//        {
//            QSize maxSize = QSize();

//            for (const QSize& size : icon.availableSizes()) {
//                if (size.width() > maxSize.width()) {
//                    maxSize = size;
//                }
//            }
//            base64_t data = ns_util::toBase64(icon.pixmap(maxSize));
//            dataStruct._avatar = data.toStdString();
//            db.addAvatar(data);
//        }
    }

    void Registration::resetRedFields()
    {
        for (QWidget* it : m_invalidInputLst) {
            it->setStyleSheet("");
        }
        m_invalidInputLst.clear();
    }

    void Registration::setAvatar(const Avatar& avatar) {
        m_ui->profile_picture->setAvatar(avatar);
    }


    void Registration::checkNickname() {
        QString nickname = m_ui->input_nickname->text();
        if (nickname.isEmpty()) {
            m_ui->input_nickname->setStyleSheet("");
            m_ui->msg_incorrect_nick->resetCurrentMsgIndex();
        } else {
            if (m_ui->input_nickname->isCorrect()) {

                rpc_object<rpc_lobby>()->nick_free(nickname, [this](reply_lobby::e_lobby result){
                    if (ns_helpers::isOk(result)) {
                        m_ui->input_nickname->setStyleSheet("QLineEdit {padding-top: 0px; margin-top: 0px;}");
                    } else {
                        m_ui->msg_incorrect_nick->setCurrentMsgIndex(2);
                        m_ui->input_nickname->setStyleSheet(QString(ns_const::errorStyleSheet).arg(m_ui->input_nickname->objectName()));
                    }
                });


            } else {
                m_ui->msg_incorrect_nick->setCurrentMsgIndex(1);
                m_ui->input_nickname->setStyleSheet(QString(ns_const::errorStyleSheet).arg(m_ui->input_nickname->objectName()));
            }
        }
    }

    void Registration::checkPassword() {
        if (!m_ui->input_password->text().isEmpty() && m_ui->input_password->IsIncorrect()) {
            m_ui->msg_incorrect_pass->setCurrentMsgIndex(1);
        } else {
            m_ui->msg_incorrect_pass->resetCurrentMsgIndex();
            checkConfirmation();
        }
    }

    void Registration::checkConfirmation() {

        if (m_ui->input_password->isCorrect() && !m_ui->input_confirmation->text().isEmpty() && m_ui->input_confirmation->IsIncorrect()) {
            m_ui->msg_incorrect_confirmation->setCurrentMsgIndex(1);
        } else {
            m_ui->msg_incorrect_confirmation->resetCurrentMsgIndex();
        }
    }

    void Registration::checkEmail() {
        QString email = m_ui->input_email->text();
//        m_ui->msg_incorrect_email->setCurrentMsgIndex(1);
        if (email.isEmpty()) {
            m_ui->input_email->setStyleSheet("");

        } else {
            if (m_ui->input_email->isCorrect()) {
                rpc_object<rpc_lobby>()->email_free(email, [this](reply_lobby::e_lobby result){
                    if (ns_helpers::isOk(result)) {
                        m_ui->input_email->setStyleSheet("QLineEdit {padding-top: 0px; margin-top: 0px;}");
                        m_ui->msg_incorrect_email->resetCurrentMsgIndex();
                    } else {
                        m_ui->msg_incorrect_email->setCurrentMsgIndex(2);
                        m_ui->input_email->setStyleSheet(QString(ns_const::errorStyleSheet).arg(m_ui->input_email->objectName()));
                    }
                });
            } else {
                m_ui->msg_incorrect_email->setCurrentMsgIndex(1);
                m_ui->input_email->setStyleSheet(QString(ns_const::errorStyleSheet).arg(m_ui->input_email->objectName()));
            }
        }
    }


    void Registration::updateButtonState() {
        for (CorrectInput* input: m_inputFields) {
            if (input->IsIncorrect()) {
                qDebug() << dynamic_cast<QWidget*>(input)->objectName() << dynamic_cast<QWidget*>(input)->metaObject()->className();
                m_ui->bttn_accept->setEnabled(false);
                return;
            }
        }
        m_ui->bttn_accept->setEnabled(true);
    }
//    void Registration::checkAddress() {

//    }

//    void Registration::checkPersonalInfo() {

//    }

//    void Registration::checkPhone() {

//    }

//    void Registration::checkTermsAndConditions() {

//    }

//    void Registration::nick_free(reply_lobby::e_lobby result)
//    {
//        if (ns_helpers::isOk(result)) {
//            m_ui->input_nickname->setStyleSheet("QLineEdit {padding-top: 0px; margin-top: 0px;}");
//        } else {
//            m_ui->msg_incorrect_nick->setCurrentMsgIndex(2);
//            m_ui->input_nickname->setStyleSheet(QString(ns_const::errorStyleSheet).arg(m_ui->input_nickname->objectName()));
//        }
//    }

//    void Registration::email_free(reply_lobby::e_lobby result)
//    {
//        if (ns_helpers::isOk(result)) {
//            m_ui->input_email->setStyleSheet("QLineEdit {padding-top: 0px; margin-top: 0px;}");
//            m_ui->msg_incorrect_email->resetCurrentMsgIndex();
//        } else {
//            m_ui->msg_incorrect_email->setCurrentMsgIndex(2);
//            m_ui->input_email->setStyleSheet(QString(ns_const::errorStyleSheet).arg(m_ui->input_email->objectName()));
//        }
//    }

    void Registration::onUserRegistered()
    {
        ns_dialog::Welcome* welcomeMessage = new ns_dialog::Welcome;
        welcomeMessage->setAttribute(Qt::WA_DeleteOnClose);

        welcomeMessage->init();
        if (!m_ui->profile_picture->avatar().icon().isNull()) {
            welcomeMessage->setAvatar(m_ui->profile_picture->avatar());
        }
        welcomeMessage->setNickname(m_ui->input_nickname->text());
        welcomeMessage->setPassinfo( ns_util::toSha1(m_ui->input_password->text())
                                   , m_ui->input_password->text().size());
        reset();
        QDialog::accept();

        welcomeMessage->open();
    }

    void Registration::selectAsIncorrect(const QWidgetList &wlist)
    {
        for (QWidget* w : wlist) {
            w->setStyleSheet(w->styleSheet() + QString(ns_const::errorStyleSheet).arg(w->objectName()) );
            m_invalidInputLst.append(w);
        }
    }

    void Registration::unselectAsIncorrect(const QWidgetList &wlist)
    {
        for (QWidget* w : wlist) {
            w->setStyleSheet(w->styleSheet().replace(QString(ns_const::errorStyleSheet).arg(w->objectName()), ""));
            m_invalidInputLst.removeOne(w);
        }
    }

    void Registration::senderCorrect() {
        if (QWidget* w = qobject_cast<QWidget*>(sender())) {
            w->setStyleSheet(w->styleSheet().replace(QString(ns_const::errorStyleSheet).arg(w->objectName()), ""));
            m_invalidInputLst.removeOne(w);
        }
    }

    void Registration::senderIncorrect() {
        if (QWidget* w = qobject_cast<QWidget*>(sender())) {
            w->setStyleSheet(w->styleSheet() + QString(ns_const::errorStyleSheet).arg(w->objectName()) );
            m_invalidInputLst.append(w);
        }
    }

    void Registration::initAvatarsPage1()
    {
        m_ui->page_avatarSelect->init();

        connect(m_ui->page_avatarSelect, &ns_page::SelectAvatar::pixmapBrowsed, this, [this](const QPixmap& pixmap){
            m_ui->page_avatarUpload->init(pixmap);
            m_ui->stackOfContent->setCurrentIndex(2);
        });

        connect(m_ui->page_avatarSelect->avatarWidget(), &ns_widget::ProfilePicture::iconChanged, this, [this](QIcon icon) {
            m_ui->bttn_ok->setDisabled(icon.isNull());
        });
        connect(m_ui->bttn_ok, &QPushButton::clicked, this, [this]{
            m_ui->profile_picture->setIcon(m_ui->page_avatarSelect->avatarWidget()->icon());
//            m_ui->page_avatarSelect->avatarWidget()->setAvatar(nullptr);
            m_ui->stackOfContent->setCurrentIndex(0);
        });
    }

    void Registration::initAvatarsPage2()
    {
        connect(m_ui->bttn_save, &QPushButton::clicked, this, &Registration::uploadAvatar);
    }

    void Registration::uploadAvatar()
    {
        m_ui->profile_picture->setAvatar(m_ui->page_avatarUpload->getAvatar());
        m_ui->stackOfContent->setCurrentIndex(0);
    }

    void Registration::showPasswordToolTip()
    {
//        m_currentToolTip = QString("<html><head/><body><p><span style=\" font-size:12px; font-weight:600; color:#000000;\">"
//                                   "%0</span><span style=\" font-size:12px; font-weight:0; color:#000000;\">"
//                                   " %1</span><br/><img src=\":/textures/pass_weak.png\"/><br><span style=\" font-size:11px; font-weight:0; color:#000000;\">"
//                                   "%2.</span></p></body></html>").arg(tr("@msg_pass_strenght:")).arg(tr("@msg_to_short")).arg("@text_password_description");
        QHelpEvent* e = new QHelpEvent(QEvent::ToolTip, m_ui->input_password->pos(), QCursor::pos());
        pApp->notify(this, e);
    }

    void Registration::setPasswordToolTipVisible(bool visible)
    {
        if (visible) {
            showPasswordToolTip();
        } else {
            hidePasswordToolTip();
        }
    }

    void Registration::hidePasswordToolTip()
    {
        if (m_passToolTip != nullptr) {
            m_passToolTip->hide();
        }
    }

} // namespace dialog




bool ns_dialog::Registration::event(QEvent *e)
{
    if (e != nullptr) {
        if (e->type() == QEvent::ToolTip) {
            QHelpEvent* helpEvent = static_cast<QHelpEvent*>(e);
            Q_UNUSED(helpEvent);
            if (m_ui->input_password == pApp->focusWidget()) {
                m_passToolTip->move(m_ui->input_password->mapToGlobal(m_ui->input_password->pos()) - QPoint(280, 122));
        //        m_passToolTip->move(m_ui->input_password->pos() + QPoint(-100, 10));
                m_passToolTip->show();
                m_passToolTip->raise();
                return true;
            }
            else
            {
//                m_passToolTip->showMinimized();
            }
        } else if (m_passToolTip && m_passToolTip->isVisible()) {
            switch (e->type()) {
            case QEvent::Move:
            case QEvent::Resize:
            case QEvent::FocusIn:
            case QEvent::Show:
            case QEvent::WindowActivate:
            case QEvent::UpdateRequest:
                m_passToolTip->move(m_ui->input_password->mapToGlobal(m_ui->input_password->pos()) - QPoint(280, 122));
                m_passToolTip->setVisible(m_ui->input_password == pApp->focusWidget());
                break;
            case QEvent::Hide:
            case QEvent::FocusOut:
            case QEvent::WindowDeactivate:
                m_passToolTip->hide();
                break;
            default:
                break;
            }
        }
    }
//    return false;
    return InitableDialog::event(e);
}
#endif // !NO_REGISTRATION
