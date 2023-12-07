#pragma once

#include <QFrame>
#include <basewidgets/entitywidget.h>
#include <db/Avatar.h>

class SharedDB;

namespace ns_widget
{

    class ProfilePicture : public EntityWidget
    {
        Q_OBJECT
    public:

    private:
//        Q_PROPERTY(QPixmap source READ source WRITE setSource NOTIFY sourceChanged)
//        Q_PROPERTY(QPixmap alphaMask READ alphaMask WRITE setAlphaMask)
        Q_PROPERTY(Avatar avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
        Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)
        Q_PROPERTY(QRect viewport READ viewport WRITE setViewport)

        struct PrivateData;
        PrivateData* m_dPtr;
    public:
        ProfilePicture(QWidget *parent = nullptr);
        virtual ~ProfilePicture();
        void init();
        const Avatar& avatar() const;
        QIcon icon() const;
        const QImage &alphaMask() const;
        QRect viewport() const;
    signals:
        void avatarChanged(const Avatar&);
        void iconChanged(const QIcon&);
    public slots:
        void setAvatar(const Avatar& avatar);
        void setIcon(const QIcon &icon);
        void setViewport(const QRect &viewport);
    protected:
        virtual void paintEvent(QPaintEvent *) override;
        virtual void resizeEvent(QResizeEvent *e) override;
    };
} // namespace widget
