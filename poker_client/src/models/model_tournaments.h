#pragma once

#include <QIdentityProxyModel>
#include <tools/types_fwd.hpp>


namespace ns_model
{
    class tournament_stat_t;

    class tournaments : public QIdentityProxyModel
    {
        Q_OBJECT

        ns_model::tournament_stat_t* m_sourceModel;
        using data_t = gbp::ns_api::ns_statistic::tournament_stat_t;
    public:
        enum class column {
            start = 0,
            game,
            buyin,
            type,
            title,
            prize,
            status,
            players,

            count
        };
    public:
        tournaments(QObject* parent = nullptr);
        virtual QVariant data(const QModelIndex &index, int role) const override;

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual void setSourceModel(QAbstractItemModel *sourceModel) override;

        virtual QModelIndex indexOf(gbp_i64 id) const;
        QModelIndex indexOf(gbp_i64 id, column field) const;

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;
        static UserType::FilterFlags filterValue(const data_t &item);

        static QVariant getDataFromSource(tournament_stat_t* source, gbp_i64 tournId, column col, int role = Qt::DisplayRole);

        virtual QVariant dataSortRole(const QModelIndex &idx) const;

        virtual QMap<int, QVariant> itemData(const QModelIndex &index) const override;
        virtual QSet<int> itemRoles(const QModelIndex &index) const;

        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;
    };

} // namespace ns_model
