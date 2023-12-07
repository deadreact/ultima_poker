#include "form.h"
#include "ui_form.h"
#include <visitors.hpp>
#include <model_transformproxy.h>
#include <model_cash_stat_t.hpp>
#include <model_tournament_stat_t.hpp>
#include <model_user_info_t.hpp>
#include <model_vip_system_info.hpp>
#include <qsortfilterproxymodel.h>
#include <QTimer>

#include <api_util.hpp>

namespace ns_model
{


    bool registerDisplayWidget(user_info_t* model, QWidget *displayWidget, user_info_t::field f, const QByteArray &property, int role) {
        displayWidget->setProperty(property, model->data(model->index(underlying_cast(f)), role));
        displayWidget->connect(model, &user_info_t::dataChanged, displayWidget
        , [model, displayWidget, f, property, role](const QModelIndex& idx, const QModelIndex&, const QVector<int>& roles) {
            if ((user_info_t::field(idx.row()) == f) && (roles.contains(role) || roles.contains(Qt::EditRole))) {
                QByteArray target = property;
                if ((property.isEmpty() || property.isNull()) && displayWidget->metaObject()->userProperty().isValid()) {
                    target = displayWidget->metaObject()->userProperty().name();
                }
                displayWidget->setProperty(target, model->data(model->index(underlying_cast(f)), role));
            }
        });
        return true;
    }
}

Form::Form(QWidget *parent)
  : QWidget(parent)
  , m_ui(new Ui::Form)
  , m_models()
  , m_cashModel(new ns_model::cash_stat_t)
  , m_tournModel(new ns_model::tournament_stat_t)
{
    for (int i = 0; i < 4; i++) {
        m_models << (new ns_model::TransformProxy);
    }

    m_ui->setupUi(this);

    m_ui->buttonGroup->setId(m_ui->bttn_cash_stat_t, 0);
    m_ui->buttonGroup->setId(m_ui->bttn_tournament_stat_t, 1);
    m_ui->buttonGroup->setId(m_ui->bttn_user_info_t, 2);
    m_ui->buttonGroup->setId(m_ui->bttn_vip_system_description, 3);

    for (int i = 0; i < 4; i++) {
        model(i)->setSourceModel(new QSortFilterProxyModel);
    }
    static_cast<QAbstractProxyModel*>(model(0)->sourceModel())->setSourceModel(m_cashModel);
    static_cast<QAbstractProxyModel*>(model(1)->sourceModel())->setSourceModel(m_tournModel);
    ns_model::user_info_t* info = new ns_model::user_info_t;
    info->resetData(ns_model::ns_test::createRandomUser());
    static_cast<QAbstractProxyModel*>(model(2)->sourceModel())->setSourceModel(info);
    ns_model::vip_system_info* vs_data = new ns_model::vip_system_info;
    vs_data->resetData(ns_model::ns_test::createRandomData());
    QMap<int, QVariant> vs_adata;
    vs_adata.insert(Qt::ForegroundRole, QColor(0x00aeef));
    vs_data->setItemData(vs_data->index(0, 0), vs_adata);
    vs_data->setItemData(vs_data->index(1, 0), vs_adata);
    vs_data->setItemData(vs_data->index(3, 0), vs_adata);

    vs_data->setItemData(vs_data->index(3, 0, vs_data->bonusItemIndex()), vs_adata);

    static_cast<QAbstractProxyModel*>(model(3)->sourceModel())->setSourceModel(vs_data);




    static int counter = 1;
    auto reg_next = [this, info](ns_model::user_info_t::field f)
    {
        ns_model::registerDisplayWidget(info, findChild<QLabel*>(QString("icon_%0").arg(counter)), f, "pixmap", Qt::DecorationRole);
        ns_model::registerDisplayWidget(info, findChild<QLabel*>(QString("text_%0").arg(counter)), f, "text", Qt::DisplayRole);
        counter++;
    };

    reg_next(ns_model::user_info_t::field::_hash_avatar);
    reg_next(ns_model::user_info_t::field::_phone);
    reg_next(ns_model::user_info_t::field::_country);
    reg_next(ns_model::user_info_t::field::_email);

    connect(m_ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(switchModel(int)));

    m_views = m_ui->tab_widget->findChildren<QAbstractItemView*>();
    for (auto view: m_views) {
        connect(view, &QAbstractItemView::activated, [this, view](const QModelIndex& idx){
            if (idx.model()->hasChildren(idx)) {
                view->setRootIndex(idx);
            }
        });
    }



}

Form::~Form()
{
    delete m_ui;
    qDeleteAll(m_models);
}

ns_model::TransformProxy *Form::model(int idx) const
{
    return m_models.at(idx);
}

void Form::switchModel(int idx)
{
    for (QAbstractItemView* view: m_views) {
        view->setModel(model(idx));
    }
    m_ui->comboBox->setModel(model(idx));
    m_ui->spinBox_columns->setMaximum(model(idx)->columnCount()-1);
    m_ui->spinBox_columns->setSuffix(QString("/%0").arg(m_ui->spinBox_columns->maximum()+1));
}

void Form::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Form::on_spinBox_rows_valueChanged(int val) {
    auto* m = model(m_ui->buttonGroup->checkedId())->sourceModel();
    int diff = m->rowCount() - val;
    if (diff > 0) {
        m->removeRows(1, diff);
    } else if (diff < 0) {
        m->insertRows(0, -diff);
    }
}

void Form::on_spinBox_columns_valueChanged(int val)
{
    m_ui->listView->setModelColumn(val);
    m_ui->comboBox->setModelColumn(val);
}

void Form::on_bttn_transpose_toggled(bool val)
{
    for (ns_model::TransformProxy* m: m_models) {
        m->setTransposed(val);
    }
}

void Form::on_tab_widget_customContextMenuRequested(const QPoint &pos)
{
    m_ui->textBrowser->setText(QString("Point(x:%0, y:%1)").arg(pos.x()).arg(pos.y()));
}

void Form::on_bttn_save_clicked() {
    if (!m_ui->lineEdit->text().isEmpty() && m_ui->buttonGroup->checkedButton()) {
        switch (m_ui->buttonGroup->checkedId()) {
        case 0:
            if (m_cashModel->backup(m_ui->lineEdit->text())) {
                m_ui->msg_status->setText("OK");
                m_ui->msg_status->setStyleSheet("QLabel {color: green; }");
            }
            break;
        case 1:
            if (m_tournModel->backup(m_ui->lineEdit->text())) {
                m_ui->msg_status->setText("OK");
                m_ui->msg_status->setStyleSheet("QLabel {color: green; }");
            }
            break;
        default:
            m_ui->msg_status->setText("Fail!");
            m_ui->msg_status->setStyleSheet("QLabel {color: red; }");
        }
    }
}

void Form::on_bttn_load_clicked()
{
    if (!m_ui->lineEdit->text().isEmpty() && m_ui->buttonGroup->checkedButton()) {
        switch (m_ui->buttonGroup->checkedId()) {
        case 0:
            if (m_cashModel->restore(m_ui->lineEdit->text())) {
                m_ui->msg_status->setText("OK");
                m_ui->msg_status->setStyleSheet("QLabel {color: green; }");
                m_ui->spinBox_rows->setValue(m_cashModel->rowCount());
            }
            break;
        case 1:
            if (m_tournModel->restore(m_ui->lineEdit->text())) {
                m_ui->msg_status->setText("OK");
                m_ui->msg_status->setStyleSheet("QLabel {color: green; }");
                m_ui->spinBox_rows->setValue(m_tournModel->rowCount());
            }
            break;
        default:
            m_ui->msg_status->setText("Fail!");
            m_ui->msg_status->setStyleSheet("QLabel {color: red; }");
        }
        QTimer::singleShot(1500, Qt::CoarseTimer, [this]{m_ui->msg_status->clear();});
    }
}

void Form::on_bttn_print_clicked()
{
    QStringList lst;
    if (auto* item = m_cashModel->itemAt(0).ptr()) {
        printTo(lst) << *item;
    }

    while (m_labels.size() < lst.size()) {
        QLabel* label = new QLabel(m_ui->labels);
        m_ui->verticalLayout_2->addWidget(label);
        label->show();
        m_labels.push_back(label);
    }
    while (m_labels.size() > lst.size()) {
        m_labels.takeLast()->deleteLater();
    }

    for (int i = 0; i < lst.size(); i++) {
        m_labels[i]->setProperty("text", lst.at(i));
    }
}
