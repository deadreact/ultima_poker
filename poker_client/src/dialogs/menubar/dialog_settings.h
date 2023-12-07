#pragma once
#include <QDialog>

#include <interface/initable.h>

namespace Ui {
class Settings;
}

namespace ns_dialog
{
    class Settings : public InitableDialog
    {
        Q_OBJECT
    private:
        Ui::Settings *m_ui;
    public:
        explicit Settings(QWidget *parent = nullptr);
        virtual ~Settings();

    public slots:
        virtual void reject();

        virtual void init() override;

    };

} //namespace ns_dialog
