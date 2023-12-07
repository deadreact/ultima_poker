#include <precompiled.h>
#include "page_VipStatusItem.h"
#include "ui_page_vipstatusitem.h"
#include <QPainter>
#include <QBitmap>
#include <basewidgets/util.h>
#include <qabstractbutton.h>
#include <QGraphicsEffect>
#include <QDataWidgetMapper>

// =================== VipSystemToolBoxStyle ===================

namespace
{
}

namespace ns_page
{
    VipStatusItem::VipStatusItem(QWidget *parent)
        : QWidget(parent)
        , m_ui(new Ui::VipStatusItem)
        , m_model(nullptr)
        , m_displayedRow(-1)
    {
        m_ui->setupUi(this);
    }



    VipStatusItem::~VipStatusItem()
    {
        delete m_ui;
    }


    void VipStatusItem::init()
    {
        //        QToolBox
    }

    void VipStatusItem::setModel(QAbstractTableModel* model, int displayRow)
    {
        if (m_model != model)
        {
            m_model = model;
            if (model != nullptr) {
                displayModelData(displayRow);
            }
        }
    }

    void VipStatusItem::setCurrentIndex(const QModelIndex &modelIndex)
    {
        displayModelData(modelIndex.row());
    }

    void VipStatusItem::displayModelData(int row)
    {
        m_displayedRow = row;
        if (m_model == nullptr)
        {
            return;
        }
        if (row >= 0 && row < m_model->rowCount())
        {
            m_ui->value_description->setText(m_model->index(row, 0).data().toString());
            m_ui->value_neededRake->setText(m_model->index(row, 1).data().toString());
            m_ui->value_rakeback->setText(m_model->index(row, 2).data().toString());
            m_ui->value_statusPrizes->setText(m_model->index(row, 3).data().toString());
            m_ui->value_vipTournaments->setText(m_model->index(row, 4).data().toString());
        }
        else
        {
            m_ui->value_description->clear();
            m_ui->value_neededRake->clear();
            m_ui->value_rakeback->clear();
            m_ui->value_statusPrizes->clear();
            m_ui->value_vipTournaments->clear();
        }
    }

    void VipStatusItem::changeEvent(QEvent *e)
    {
        if (e && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
        }
        displayModelData(m_displayedRow);
        QWidget::changeEvent(e);
    }

} //namespace ns_page

