#pragma once

#include <QDialog>
#include <interface/initable.h>

namespace Ui {
class MyTournaments;
}
class QAbstractButton;
namespace ns_model {
    class FunctionFilter;
}
namespace ns_dialog
{
    class MyTournaments : public InitableDialog
    {
        Q_OBJECT
    private:
        Ui::MyTournaments *m_ui;
        ns_model::FunctionFilter* m_proxyModel;
    public:
        explicit MyTournaments(QWidget *parent = nullptr);
        virtual ~MyTournaments();

        virtual void init() override;
    public slots:
        virtual void reject();
    private slots:
        void on_bttnGrp_filters_buttonClicked(QAbstractButton* bttn);
        void on_bttn_filterAll_clicked();
        void on_bttn_filterStatusInProgress_clicked();
        void on_bttn_filterStatusRegistration_clicked();
        void on_bttn_unregister_clicked();
        virtual void updateTexts();        

    protected:
        virtual void changeEvent(QEvent* e) override;
    };

} //namespace ns_dialog
