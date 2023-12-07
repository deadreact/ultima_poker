#pragma once

#include <QtWidgets/QTabWidget>

namespace Ui {
class TableStateDemo;
}

namespace ns_model {
class table_state_t;
class user_info_t;
class cash_stat_t;
class tournament_stat_t;
}

class TableStateDemo : public QTabWidget
{
    Q_OBJECT

private:
    Ui::TableStateDemo *m_ui;
    ns_model::table_state_t* m_table_state;
    ns_model::user_info_t* m_user_info;
    ns_model::cash_stat_t* m_cash_stat;
    ns_model::tournament_stat_t* m_tournament_stat;

public:
    explicit TableStateDemo(QWidget *parent = nullptr);
    ~TableStateDemo();

protected:
    virtual void changeEvent(QEvent *e) override;
private slots:
    void on__view_activated(const QModelIndex &index);
    void on_bttn_back_clicked();
    void on_bttn_reset_clicked();
private:
    void initPage2();
    void initPage3();
};
