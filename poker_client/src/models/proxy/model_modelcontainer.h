#pragma once

#include <QIdentityProxyModel>

using InternalData = QPair<QAbstractItemModel*, QList<QPoint> >;

namespace ns_model
{

    class ModelContainer : public QIdentityProxyModel
    {
        Q_OBJECT
        QList<QAbstractItemModel*> m_models;
        mutable QList<InternalData> m_internal;
    public:
        ModelContainer(QObject* parent = nullptr);

        QModelIndex indexOf(const QAbstractItemModel *m, const QList<QPoint> &parentInfo = QList<QPoint>()) const;

        void push_back(QAbstractItemModel* model);
        void pop_back();
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual void setSourceModel(QAbstractItemModel *sourceModel) override;
        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
        virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

        virtual QVariant data(const QModelIndex &proxyIndex, int role) const override;
    protected slots:
        void connectTo(QAbstractItemModel* model);
        void disconnectFrom(const QAbstractItemModel *model);
    private slots:
        void on_sourceRowsAboutToBeInserted(const QModelIndex& parent, int first, int last);
        void on_sourceRowsInserted(const QModelIndex& parent, int first, int last);
        void on_sourceRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
        void on_sourceRowsRemoved(const QModelIndex& parent, int first, int last);
        void on_sourceRowsAboutToBeMoved(const QModelIndex& sourceParent, int sourceFirst, int sourceLast, const QModelIndex& destinationParent, int destinationRow);
        void on_sourceRowsMoved(const QModelIndex& index, int row, int count,QModelIndex,int);
        void on_sourceColumnsAboutToBeInserted(const QModelIndex& parent, int first, int last);
        void on_sourceColumnsInserted(const QModelIndex& parent, int first, int last);
        void on_sourceColumnsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
        void on_sourceColumnsRemoved(const QModelIndex& parent, int first, int last);
        void on_sourceColumnsAboutToBeMoved(const QModelIndex& sourceParent, int sourceFirst, int sourceLast, const QModelIndex& destinationParent, int destinationRow);
        void on_sourceColumnsMoved(const QModelIndex& index, int row, int count,QModelIndex,int);
        void on_sourceDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
        void on_sourceHeaderDataChanged(Qt::Orientation orientation, int first, int last);
        void on_sourceModelAboutToBeReset();
        void on_sourceModelReset();

    };


} //namespace ns_model
