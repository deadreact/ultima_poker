#include <precompiled.h>
#include "model_RakebackHistory.h"
#include <core/money.h>
#include <basewidgets/util.h>
#include <api_models/model_vip_system_info.hpp>

namespace ns_model
{
    RakebackHistory::RakebackHistory(QObject *parent)
        : QAbstractTableModel(parent)
        , m_sourceModel(nullptr)
    {
    }


    int RakebackHistory::rowCount(const QModelIndex &parent) const
    {
        if (m_sourceModel == nullptr || parent.isValid()) {
            return 0;
        }
        return m_sourceModel->rowCount(m_sourceModel->bonusItemIndex());
    }

    int RakebackHistory::columnCount(const QModelIndex &parent) const
    {
        if (m_sourceModel == nullptr || parent.isValid()) {
            return 0;
        }
        return 5;
    }

    void RakebackHistory::refresh()
    {
        beginResetModel();

//        m_data.clear();
//        m_data.push_back(RowItemData::RakebackBonus(QDate(2017, 1, 1), QDate::currentDate(), 45000ll));
//        m_data.push_back(RowItemData::RakebackBonus(QDate(2017, 1, 1), QDate::currentDate(), 2));
//        m_data << (RowItemData::StatusBonus(E::VipStatus::Professional, QDate(2016, 11, 11), 2));
//        m_data.push_back(RowItemData::StatusBonus(E::VipStatus::Master, QDate::currentDate(), 200000ll));

        endResetModel();
    }

    void RakebackHistory::setSourceModel(vip_system_info *model)
    {
        if (m_sourceModel != model) {
            beginResetModel();

            if (m_sourceModel != nullptr)
            {
                disconnect(m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
                disconnect(m_sourceModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
                disconnect(m_sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onSourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
                disconnect(m_sourceModel, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(onSourceRowsAboutToBeInserted(QModelIndex,int,int)));
                disconnect(m_sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(onSourceRowsInserted(QModelIndex,int,int)));
            }
            m_sourceModel = model;
            if (model != nullptr)
            {
                connect(model, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
                connect(model, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
                connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onSourceDataChanged(QModelIndex,QModelIndex,QVector<int>)));
                connect(model, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)), this, SLOT(onSourceRowsAboutToBeInserted(QModelIndex,int,int)));
                connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(onSourceRowsInserted(QModelIndex,int,int)));
            }

            endResetModel();
        }
    }

    QVariant RakebackHistory::data(const QModelIndex &index, int role) const
    {
        if (m_sourceModel == nullptr || !index.isValid() || index.parent().isValid()) {
            return QVariant();
        }

        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        }

//        return m_sourceModel->data(m_sourceModel->index(index.row(), index.column(), m_sourceModel->bonusItemIndex()), role);
//        const RowItemData& d = m_data.at(index.row());
        using field = vip_system_info::bonus_item;
        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            switch (index.column()) {
            case 0: return m_sourceModel->text(index.row(), field::_type) ;//(d.desc.bonusType == EBonus::rakeback) ? tr("@bonus_type_rakeback") : tr("@bonus_type_status");
            case 1: return m_sourceModel->text(index.row(), field::_rakeback_period);//(d.desc.bonusType != EBonus::rakeback)
//                    ? ""
//                    : (d.rakebackPeriodStartDate.toString(Qt::ISODate) + tr("@to") + d.rakebackPeriodStartDate.addDays(7).toString(Qt::ISODate));
            case 2: return m_sourceModel->text(index.row(), field::_datetime);//d.accrualDate;
            case 3:
            {
                QString money = m_sourceModel->text(index.row(), field::_rakeback_money);
                QString tickets = m_sourceModel->text(index.row(), field::_tickets_count);
                if (!money.isEmpty()) {
                    return QString("%0%1").arg(money).arg(tickets != "0" ? QString(", %0").arg(tr("@tickets_prize_%0").arg(tickets)) : "");
                }
                return tr("@tickets_prize_%0").arg(tickets);
            }

//                return m_sourceModel->text(index.row(), field::_rakeback_money)
//                        + (m_sourceModel->text(index.row(), field::_tickets_count) == "0")
//                        ? ", " + tr("@tickets_prize_%0").arg(m_sourceModel->text(index.row(), field::_tickets_count))
//                        : "";
            case 4: return (m_sourceModel->bonusTypeAtRow(index.row()) == gbp::ns_vip_system::e_bonus::status_bonus)
                        ? "TODO: Серега, добавь статус"
                        : "";
                        //(d.desc.bonusType == EBonus::status ? ns_util::enumToString(d.achievedStatus).toUpper() : "");
            default:
                return QVariant();
            }
        default:
            return QVariant();
        }

        return QVariant();
    }

    QVariant RakebackHistory::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        }


        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
        {
            switch (section) {
            case 0: return tr("@bonus_type");
            case 1: return tr("@period");
            case 2: return tr("@accrual_date");
            case 3: return tr("@bonus");
            case 4: return tr("@achieved_status");
            default:
                return QVariant();
            }
        }
        default:
            return QVariant();
        }

        return QVariant();
    }

    void RakebackHistory::onSourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        QModelIndex parent = topLeft.parent();
        if (parent.isValid() && !parent.parent().isValid() && vip_system_info::field(parent.row()) == vip_system_info::field::_bonuses) {
            emit dataChanged(index(topLeft.row(), topLeft.column()), index(bottomRight.row(), bottomRight.column()), roles);
        }
    }

    void RakebackHistory::onSourceRowsAboutToBeInserted(const QModelIndex &parent, int first, int last) {
        if (parent.isValid() && !parent.parent().isValid() && vip_system_info::field(parent.row()) == vip_system_info::field::_bonuses) {
            beginInsertRows(QModelIndex(), first, last);
        }
    }

    void RakebackHistory::onSourceRowsInserted(const QModelIndex &parent, int /*first*/, int /*last*/) {
        if (parent.isValid() && !parent.parent().isValid() && vip_system_info::field(parent.row()) == vip_system_info::field::_bonuses) {
            endInsertRows();
        }
    }

    void RakebackHistory::onSourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last) {
        if (parent.isValid() && !parent.parent().isValid() && vip_system_info::field(parent.row()) == vip_system_info::field::_bonuses) {
            beginRemoveRows(QModelIndex(), first, last);
        }
    }

    void RakebackHistory::onSourceRowsRemoved(const QModelIndex &parent, int /*first*/, int /*last*/) {
        if (parent.isValid() && !parent.parent().isValid() && vip_system_info::field(parent.row()) == vip_system_info::field::_bonuses) {
            endRemoveRows();
        }
    }

} //namespace ns_model
