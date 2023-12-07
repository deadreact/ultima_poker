#pragma once

#include <QWidget>

namespace Ui {
class SelectAvatar;
}
namespace ns_widget {
    class ProfilePicture;
}

namespace ns_page
{

    class SelectAvatar : public QWidget
    {
        Q_OBJECT
    private:
        Ui::SelectAvatar *m_ui;
    public:
        explicit SelectAvatar(QWidget *parent = nullptr);
        virtual ~SelectAvatar();

        void init();
        ns_widget::ProfilePicture* avatarWidget() const;
    signals:
        void pixmapBrowsed(const QPixmap&);
    private slots:
        void browse();

    };
} // namespace ns_page
