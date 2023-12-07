#pragma once
#include <QWidget>
#include <model_transformproxy.h>

namespace Ui {
class Form;
}

class QAbstractItemView;
namespace ns_model
{
    class cash_stat_t;
    class tournament_stat_t;
} //namespace ns_model
class Form : public QWidget
{
    Q_OBJECT

private:
    Ui::Form *m_ui;
    QList<ns_model::TransformProxy*> m_models;
    QList<QAbstractItemView*> m_views;
    ns_model::cash_stat_t* m_cashModel;
    ns_model::tournament_stat_t* m_tournModel;
    QList<QWidget*> m_labels;
public:
    explicit Form(QWidget *parent = nullptr);
    virtual ~Form();

    ns_model::TransformProxy* model(int idx) const;
public slots:
    void switchModel(int idx);
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_spinBox_rows_valueChanged(int val);
    void on_spinBox_columns_valueChanged(int val);
    void on_bttn_transpose_toggled(bool val);
    void on_tab_widget_customContextMenuRequested(const QPoint &pos);
    void on_bttn_save_clicked();
    void on_bttn_load_clicked();
    void on_bttn_print_clicked();
};
