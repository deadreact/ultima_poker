#include <precompiled.h>
#include <widgets/widget_ProfilePicture.h>
#include "dialog_changeavatar.h"
#include "ui_dialog_ChangeAvatar.h"
#include <const.h>

namespace ns_dialog
{
    ChangeAvatar::ChangeAvatar(QWidget *parent)
        : InitableDialog(parent)
        , m_ui(new Ui::ChangeAvatar)
    {
        m_ui->setupUi(this);
    }

    ChangeAvatar::~ChangeAvatar()
    {
        delete m_ui;
    }

    void ChangeAvatar::init()
    {
        if (isInitialized()) {
            return;
        }
        m_ui->page_avatarSelect->init();

        connect(m_ui->page_avatarSelect, &ns_page::SelectAvatar::pixmapBrowsed, m_ui->stackOfContent, [this](const QPixmap& avatar){
            m_ui->stackOfContent->setCurrentIndex(1);
            m_ui->page_avatarUpload->init(avatar);
        });
        connect(m_ui->page_avatarSelect->avatarWidget(), &ns_widget::ProfilePicture::iconChanged, m_ui->stackOfContent, [this](QIcon icon){
           m_ui->bttn_ok->setDisabled(icon.isNull());
        });

        onInitialized();
    }

    Avatar ChangeAvatar::avatar() const
    {
        if (m_ui->stackOfContent->currentIndex() == 1) {
            return m_ui->page_avatarUpload->getAvatar();
        } else {
            return m_ui->page_avatarSelect->avatarWidget()->avatar();
        }
    }

    Avatar ChangeAvatar::getAvatar()
    {
        ChangeAvatar dialog;
        dialog.init();
        if (dialog.exec() == QDialog::Accepted) {
            return dialog.avatar();
        }
        return Avatar();
    }

    void ChangeAvatar::reject()
    {
        if (m_ui->stackOfContent->currentIndex() == 1) {
            m_ui->stackOfContent->setCurrentIndex(0);
        } else {
            InitableDialog::reject();
        }
    }

} //namespace ns_dialog
