#pragma once

#include "page_MainLobbyBase.h"

namespace Ui {
    class CashTables;
}

class CashTableView;
namespace ns_model {
    class cash_tables;
}
class QTableView;

namespace ns_page
{
    class CashTables : public MainLobbyBase
    {
        Q_OBJECT
    private:
        Ui::CashTables *m_ui;
    signals:
        void sitOnTableRequested(gbp_i64 tableID, int seatIndex = -1);
        void openTableRequested(gbp_i64 tableID);
    public:
        explicit CashTables(QWidget *parent = nullptr);
        virtual ~CashTables();
        virtual void init() override;

        void updateTexts();

        virtual TableView* tableView() const override;
    protected:
        virtual void changeEvent(QEvent *e) override;
        virtual void showEvent(QShowEvent *e) override;

        virtual void updateFilters() override;
    protected slots:
        virtual void onItemActivated(const QModelIndex& index) override;
        virtual void onCurrentChanged(const QModelIndex &index) override;
        virtual void disableFilters(bool disabled) override;

        // MainLobbyBase interface
    public:
        virtual void initTableView() override;
    private slots:
        void on_bttn_sit_clicked();
        void on_bttn_open_tbl_clicked();
        void on_bttn_joinQueue_clicked();
        void on_bttn_unjoinQueue_clicked();
    };


}// namespace page

