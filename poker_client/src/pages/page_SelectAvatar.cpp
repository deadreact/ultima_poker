#include <precompiled.h>
#include "page_SelectAvatar.h"
#include "ui_page_SelectAvatar.h"
#include <QFileDialog>
#include <styles/shadowedtextstyle.h>

namespace ns_page
{
    SelectAvatar::SelectAvatar(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::SelectAvatar)
    {
        m_ui->setupUi(this);
    }

    SelectAvatar::~SelectAvatar()
    {
        delete m_ui;
    }

    void SelectAvatar::init()
    {
        m_ui->avatarsLstPage->init(m_ui);
        setTextDropShadowStyle(m_ui->header_common_avatars, QPoint(0, 1), 0x191919);
    }

    ns_widget::ProfilePicture *SelectAvatar::avatarWidget() const
    {
        return m_ui->profile_picture_2;
    }

    void SelectAvatar::browse()
    {
        QString filename = QFileDialog::getOpenFileName(this, "Images", "", "*.png *.jpg *.jpeg *.gif *.bmp");
        if ( !filename.isEmpty() ) {
            QPixmap pixmap(filename);
            if (!pixmap.isNull()) {
                emit pixmapBrowsed(pixmap);
            }
        }
    }
} //namespace ns_page
