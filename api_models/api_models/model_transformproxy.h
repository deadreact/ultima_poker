#pragma once

#include "api_models_global.h"
#include <QIdentityProxyModel>

namespace ns_model
{

    class API_MODEL_DLL TransformProxy : public QIdentityProxyModel
    {
        Q_OBJECT

        mutable bool m_transposed;
    public:
        friend class QAbstractItemModel;
        explicit TransformProxy(QObject *parent = nullptr);
        explicit TransformProxy(QAbstractItemModel* sourceModel, QObject *parent = nullptr);
    public:
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &child) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        bool isTransposed() const;
    public slots:
        void setTransposed(bool transposed);
        void toggleTranspose();

        // QAbstractProxyModel interface
    public:
        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

        // QAbstractItemModel interface
    public:
        virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
        virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
        virtual bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) override;
    };

} //namespace ns_model
