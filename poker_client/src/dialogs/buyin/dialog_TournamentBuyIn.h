#pragma once
#include <QDialog>
#include <descriptors/descriptor_SpinNGoBuyIn.h>

#ifndef Q_MOC_RUN
    #include <rpc/table/request.h>
#endif // !Q_MOC_RUN

namespace Ui {
class TournamentBuyIn;
}
class ShadowedTextStyle;

namespace ns_dialog
{
    class TournamentBuyIn : public QDialog
    {
        Q_OBJECT
    public:
        enum CheckedPayment
        {
            Cash = 0,
            Unchecked,
            Ticket
        };
        Q_ENUM(CheckedPayment)
    private:
        Ui::TournamentBuyIn *m_ui;
        const GameId m_gameID;
        CheckedPayment m_checkedPayment;
        ns_callback::on_register m_callback;
        const bool m_callbackValid;
    public:
        explicit TournamentBuyIn(const GameId& gameID, QWidget *parent = nullptr);
        explicit TournamentBuyIn(const GameId& gameID, ns_callback::on_register callback, QWidget *parent = nullptr);
        virtual ~TournamentBuyIn();
        void initWithDescriptor(const ns_descriptor::TournamentBuyIn* desc);
    private slots:
        void onSliderMoved(int);
    public slots:
        virtual void accept() override;
    protected:
        virtual void timerEvent(QTimerEvent *e) override;
    };
} //namespace ns_dialog
