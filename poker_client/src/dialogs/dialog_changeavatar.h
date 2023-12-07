#pragma once

#include <interface/initable.h>

namespace Ui {
class ChangeAvatar;
}
class Avatar;


namespace ns_dialog
{
    class ChangeAvatar : public InitableDialog
    {
        Q_OBJECT
    private:
        Ui::ChangeAvatar *m_ui;

    public:
        explicit ChangeAvatar(QWidget *parent = nullptr);
        virtual ~ChangeAvatar();
        virtual void init() override;
        Avatar avatar() const;

        static Avatar getAvatar();
    public slots:
        virtual void reject() override;
    };


} //namespace ns_dialog
