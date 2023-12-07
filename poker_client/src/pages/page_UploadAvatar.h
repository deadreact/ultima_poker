#pragma once

#include <QWidget>

namespace Ui {
class UploadAvatar;
}

class Avatar;

namespace ns_page
{
    class UploadAvatar : public QWidget
    {
        Q_OBJECT
    private:
        Ui::UploadAvatar *m_ui;
        QPixmap m_selectedFragment;
    public:
        explicit UploadAvatar(QWidget *parent = nullptr);
        virtual ~UploadAvatar();

        Avatar getAvatar() const;

    public slots:
        void init(const QPixmap &pixmap);
    };
} //namespace ns_page

