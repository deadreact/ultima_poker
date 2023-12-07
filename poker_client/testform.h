#pragma once

#include <QDialog>

namespace Ui {
class TestForm;
}
namespace ns_model {
class ModelContainer;
class CommonGameTable;
}

class ProxyModel;
class MsgFilterModel;

class TestForm : public QDialog
{
    Q_OBJECT
private:
    Ui::TestForm *m_ui;
    QScopedPointer<ProxyModel> m_model;
    QScopedPointer<ns_model::ModelContainer> m_containerModel;
    MsgFilterModel* m_proxyModel;
    ns_model::CommonGameTable* m_proxyTableModel;
    bool m_choosingNow;
public:
    explicit TestForm(QWidget *parent = nullptr);
    virtual ~TestForm();

    Q_SIGNAL void shown();
private slots:
    void on_bttn_go_clicked();
//    void on_bttn_push_back_clicked();
//    void on_bttn_pop_back_clicked();
    void on_bttn_reparse_clicked();
    void on_bttn_singleColumn_toggled(bool toggle);
    void on_bttn_setModel_toggled(bool toggle);
    void on_bttn_applyStyle_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_tableView_activated(const QModelIndex &index);
    void on_bttn_bg_toggled(bool checked);
    void on_bttn_alternate_toggled(bool checked);

    void on_bttn_back_clicked();
    void on_bttn_next_clicked();
    void on_input_filter_textChanged(const QString &text);
    void on_bttn_enableRegExp_toggled(bool toggle);
    void on_bttn_save_clicked();
    void on_msgTypeFilterChanged(int mask, bool set);
    void on_TestForm_shown();

//    void showFile();
    void on_bttn_l1_clicked();
    void on_input_fontSize_valueChanged(int arg1);
    void on_input_fontWeight_valueChanged(int arg1);
    void on_input_gameID_valueChanged(int arg1);
    void on_input_tournamentID_valueChanged(int arg1);
    void on_bttn_load_clicked();

    void switchSourceModel();
protected:
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    virtual void mouseMoveEvent(QMouseEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;
};
