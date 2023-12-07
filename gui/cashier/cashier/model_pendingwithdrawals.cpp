#include <QDate>
#include <qcolor.h>
#include <qicon.h>
#include <core/money.h>
#include "model_pendingwithdrawals.h"

namespace
{
    QString paymentSystem(int index)
    {
        switch (index) {
        case 1: return "VISA";
        case 2: return "Mastercard";
        default:
            return "";
        }
    }
    QString status(int index)
    {
        switch (index) {
        case 0: return QObject::tr("@withdrawalStatus_pending");
        case 1: return QObject::tr("@withdrawalStatus_checking");
        case 2: return QObject::tr("@withdrawalStatus_success");
        case 3: return QObject::tr("@withdrawalStatus_failed");
        default:
            return "";
        }
    }
}

namespace ns_model
{
    struct PendingWithdrawal::Item
    {
        QDate date;
        int method;
        gbp_i64 transactionID;
        Money amount;
        int status;

        Item(int method, gbp_i64 transId, Money money)
            : date(QDate::currentDate()), method(method), transactionID(transId), amount(money), status(0)
        {}
    };

    PendingWithdrawal::PendingWithdrawal(QObject *parent)
        : QAbstractTableModel(parent)
    {
        Item* item = new Item(1, 234616431, 43000);
        m_data << item;
        item = new Item(2, 234616433, 40000);
        m_data << item;
    }

    QVariant PendingWithdrawal::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }

        if (role != Qt::DisplayRole) {
            return QVariant();
        }

        switch (section) {
        case 0: return tr("@date");
        case 1: return tr("@method");
        case 2: return tr("@transaction_id");
        case 3: return tr("@amount");
        case 4: return tr("@status");
        case 5: return tr("@cancel");
        default:
            return QVariant();
        }
    }

    int PendingWithdrawal::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return m_data.size();
    }

    int PendingWithdrawal::columnCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return 6;
    }

    QVariant PendingWithdrawal::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }

        if (role == Qt::ForegroundRole) {
            if (index.column() == 4)
                return QColor(0xffb74d);
            return QColor(0xfafafa);
        }
        if (role == Qt::TextAlignmentRole) {
            return Qt::AlignCenter;
        }

        const Item* rowItem = m_data.at(index.row());
        if (role == Qt::DisplayRole) {
            switch (index.column()) {
            case 0: return rowItem->date;
            case 1: return paymentSystem(rowItem->method);
            case 2: return QVariant::fromValue(rowItem->transactionID);
            case 3: return rowItem->amount.toString();
            case 4: return status(rowItem->status);
            case 5:
            default:
                return QVariant();
            }
        }
        if (role == Qt::DecorationRole && index.column() == 5) {
            return QIcon(":/textures/Common/icons/icon_notOk.png");
        }

        return QVariant();
    }

} //namespace ns_model
