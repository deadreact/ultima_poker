#include "model_tournament_user_info_t.h"
#include "model_tournament_stat_t.hpp"

namespace ns_model
{

    tournament_user_info_t::tournament_user_info_t(QObject *parent)
        : QIdentityProxyModel(parent)
        , m_id(-1)
    {}

    void tournament_user_info_t::setSourceModel(QAbstractItemModel *model)
    {
        if (model == qobject_cast<tournament_stat_t*>(model)) {
            QIdentityProxyModel::setSourceModel(model);
        }
    }

    tournament_stat_t *tournament_user_info_t::sourceModel() const
    {
        return qobject_cast<tournament_stat_t*>(QIdentityProxyModel::sourceModel());
    }

    void tournament_user_info_t::setId(gbp_i64 id) {
        if (id != m_id) {
            beginResetModel();
            m_id = id;
            endResetModel();
        }
    }

    gbp_i64 tournament_user_info_t::id() const {
        return m_id;
    }

    QModelIndex tournament_user_info_t::rootIndex() const {
        return sourceModel() == nullptr ? QModelIndex() : sourceModel()->indexOf(m_id, tournament_stat_t::column::_users);
    }

    QModelIndex tournament_user_info_t::mapFromSource(const QModelIndex &idx) const {
        if (idx.isValid() && idx.model() == sourceModel() && idx.parent() == rootIndex()) {
            return index(idx.row(), idx.column());
        }
        return QModelIndex();
    }

    QModelIndex tournament_user_info_t::mapToSource(const QModelIndex &idx) const {

        if (idx.isValid() && idx.model() == this && !idx.parent().isValid()) {
            return (rootIndex().isValid() && sourceModel()->rowCount(rootIndex()) < idx.row()) ? rootIndex().model()->index(idx.row(), idx.column(), rootIndex()) : QModelIndex();
        }
        return QModelIndex();
    }

    QModelIndex tournament_user_info_t::index(int row, int column, const QModelIndex &parent) const
    {
        if (rootIndex().isValid() && sourceModel()->rowCount(rootIndex()) < row && column == 0 && !parent.isValid())
            return createIndex(row, column);
        return QModelIndex();
    }

    QModelIndex tournament_user_info_t::parent(const QModelIndex &/*child*/) const
    {
        return QModelIndex();
    }



} //namespace ns_model
