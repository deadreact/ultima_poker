#pragma once

#include <QIdentityProxyModel>
#include <api/gbp_int.hpp>

namespace ns_model
{
    class tournament_stat_t;

    class tournament_user_info_t : public QIdentityProxyModel
    {
        Q_OBJECT

        gbp_i64 m_id;
    public:
        explicit tournament_user_info_t(QObject *parent = 0);

        virtual void setSourceModel(QAbstractItemModel *sourceModel) override;
        tournament_stat_t* sourceModel() const;

        void setId(gbp_i64 id);
        gbp_i64 id() const;

        QModelIndex rootIndex() const;

        virtual QModelIndex mapFromSource(const QModelIndex& idx) const override;
        virtual QModelIndex mapToSource(const QModelIndex& idx) const override;

        virtual QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
    };

} // namespace ns_model
