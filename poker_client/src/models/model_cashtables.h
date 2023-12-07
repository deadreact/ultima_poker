#pragma once

#include <QIdentityProxyModel>
#include <tools/types_fwd.hpp>


namespace ns_model
{
    class cash_stat_t;

    class cash_tables : public QIdentityProxyModel
    {
        Q_OBJECT

        ns_model::cash_stat_t* m_sourceModel;

    public:
        enum class column {
            _title = 0,
            _bets,
            _game,
            _type,
            _players,
            _waiters,
            _pot,
            _flop,
            _distribution,

            count
        };
    public:
        cash_tables(QObject* parent = nullptr);
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

        virtual void setSourceModel(QAbstractItemModel *sourceModel) override;


        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

        QVariant getData(const games_stat_t::first_type::value_type &item, int col, int role = UserType::OriginDataRole) const;

        static UserType::FilterFlags filterValue(const games_stat_t::first_type::value_type &item);

        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

        virtual QModelIndex indexOf(gbp_i64 id) const;
        QModelIndex indexOf(gbp_i64 id, column field) const;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        inline QModelIndex index(int row, column col) const {
            return index(row, static_cast<int>(col), QModelIndex());
        }
        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

        // QAbstractItemModel interface
    public:
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    };


    class cash_players : public QIdentityProxyModel
    {
        Q_OBJECT

    public:
        cash_players(QObject* parent = nullptr);

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const override;
    };

} // namespace ns_model
