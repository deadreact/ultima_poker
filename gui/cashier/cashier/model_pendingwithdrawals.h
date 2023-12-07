#pragma once

#include "cashier_global.h"
#include <QAbstractTableModel>

namespace ns_model
{
    class CASHIER_DLL PendingWithdrawal : public QAbstractTableModel
    {
        Q_OBJECT
    private:
        struct Item;
        QVector<Item*> m_data;
    public:
        explicit PendingWithdrawal(QObject *parent = 0);

        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    };

} // namespace ns_model
