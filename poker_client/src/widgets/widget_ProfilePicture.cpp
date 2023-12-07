#include <precompiled.h>
#include "widget_ProfilePicture.h"
#include <db/Database.h>
#include <QPainter>
#include <QBuffer>
#include "db/Database.h"
#include <PokerApplication.h>
#include <basewidgets/util.h>

const char* pattern = "ProfilePicture{ max-width: %0px; min-width: %0px; max-height: %0px; min-height: %0px; border-image: url(%1); border-radius: 10}";

namespace ns_widget
{
    struct ProfilePicture::PrivateData
    {
        Avatar avatar;
        QRect viewport;

        PrivateData(): avatar(), viewport(QRect()) {}
    };

    ProfilePicture::ProfilePicture(QWidget *parent)
        : EntityWidget(parent)
        , m_dPtr(new PrivateData)
    {
        init();
    }

    ProfilePicture::~ProfilePicture() {
        delete m_dPtr;
    }

    void ProfilePicture::init()
    {
        m_parentPainting = E::ParentPainting::IgnoreParentPainting;
    }

    const Avatar &ProfilePicture::avatar() const {
        return m_dPtr->avatar;
    }

    QIcon ProfilePicture::icon() const {
        return m_dPtr->avatar.icon();
    }

    const QImage& ProfilePicture::alphaMask() const {
        return Avatar::alphaMask();
    }

    QRect ProfilePicture::viewport() const {
        return m_dPtr->viewport;
    }

    void ProfilePicture::setAvatar(const Avatar& avatar)
    {
        if (m_dPtr->avatar.icon().cacheKey() == avatar.icon().cacheKey()) {
            return;
        }

        m_dPtr->avatar = avatar;
        update();
        emit avatarChanged(avatar);
        emit iconChanged(icon());
    }

    void ProfilePicture::setIcon(const QIcon &icon) {
        m_dPtr->avatar.setIcon(icon);
        emit iconChanged(icon);
    }

    void ProfilePicture::setViewport(const QRect& viewport)
    {
        if (m_dPtr->viewport != viewport)
        {
            m_dPtr->viewport = viewport;
            update();
        }
    }

    void ProfilePicture::paintEvent(QPaintEvent *e)
    {
        if (m_parentPainting == E::ParentPainting::PaintParentBefore) {
            EntityWidget::paintEvent(e);
        }
        QPainter p(this);
//        p.drawPixmap(m_dPtr->viewport, placeholder.pixmap(m_dPtr->viewport.size()));
        if (!m_dPtr->avatar.draw(&p, m_dPtr->viewport)) {
            p.drawPixmap(m_dPtr->viewport, Avatar::placeholder(m_dPtr->viewport.height()));
        }

        if (m_parentPainting == E::ParentPainting::PaintParentAfter) {
            EntityWidget::paintEvent(e);
        }
    }
} //namespace widget


void ns_widget::ProfilePicture::resizeEvent(QResizeEvent *e)
{
    EntityWidget::resizeEvent(e);
}
