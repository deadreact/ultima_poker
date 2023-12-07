#pragma once

#include <QAbstractTableModel>


namespace ns_model
{
    class VipStatusInfo : public QAbstractTableModel
    {
        Q_OBJECT

    public:
        explicit VipStatusInfo(QObject *parent = nullptr);

        // Header:
    //    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        // Basic functionality:
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    };
} //namespace ns_model
