#include <precompiled.h>
#include "model_VipStatusInfo.h"
#include <core/money.h>

namespace ns_model
{
    VipStatusInfo::VipStatusInfo(QObject *parent)
        : QAbstractTableModel(parent)
    {
    }


    int VipStatusInfo::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        return 6;
    }

    int VipStatusInfo::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;
        return 5;
    }

    QVariant VipStatusInfo::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.parent().isValid()) {
            return QVariant();
        }

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
            case 0: return tr("@vipStatusItem_description");
            case 1:
            {
                static const Money neededRake[] = {0, 2000, 20000, 80000, 300000, 800000};
                return neededRake[index.row()].toString();
            }
            case 2:
            {
                static const int rakebeckPersent[] = {10, 15, 20, 25, 30, 40};
                return QString("%%0").arg(rakebeckPersent[index.row()]);
            }
            case 3:
            {
                switch (index.row()) {
                case 0: return tr("@no_prize");
                case 1: return tr("@cheap_ticket_x%0").arg(1);
                case 2: return tr("@cheap_ticket_x%0").arg(2);
                case 3: return tr("@cheap_ticket_x%0").arg(2) + tr("@medium_ticket_x%0").arg(1);
                case 4: return tr("@cheap_ticket_x%0").arg(5) + tr("@medium_ticket_x%0").arg(2);
                case 5: return tr("@cheap_ticket_x%0").arg(10) + tr("@medium_ticket_x%0").arg(4) + tr("@expensive_ticket_x%0").arg(1) + tr("@personal_manager");
                default:
                    return QVariant();
                }
            }
            case 4:
                return tr("@link_details");
            default:
                return QVariant();
            }
        }

        return QVariant();
    }

} //namespace ns_model
