#pragma once
#include <QDialog>

#include <interface/initable.h>

namespace Ui {
class VipSystem;
}

namespace ns_dialog
{
    class VipSystem : public InitableDialog
    {
        Q_OBJECT
    private:
        Ui::VipSystem *m_ui;
    public:
        explicit VipSystem(QWidget *parent = nullptr);
        virtual ~VipSystem();
        virtual void init() override;
    public slots:
        virtual void reject();
    protected:
        virtual void changeEvent(QEvent* e) override;

    };

} //namespace ns_dialog
