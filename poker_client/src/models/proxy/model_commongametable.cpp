#include <precompiled.h>
#include "model_commongametable.h"

#include <db/modeldatabase.h>
#include <api_models/model_table_state_t.hpp>
#include <api_models/helper_get_set_data.hpp>
#include <tools/hackindex_p.hpp>

gbp_i64 idFromIndex(int index, const gbp::ns_api::ns_table::table_state_t& data) {
    if (index >= 0 && index < (int)data._players.size()) {
        return data._players.at(index)._id;
    }
    return -1;
}
int indexFromId(gbp_i64 id, const gbp::ns_api::ns_table::table_state_t& data) {
    for (int i = 0; i < data._players.size(); i++) {
        if (id == data._players.at(i)._id) {
            return i;
        }
    }
    return -1;
}
gbp_i8 posFromId(gbp_i64 id, const gbp::ns_api::ns_table::table_state_t& data) {
    for (int i = 0; i < data._players.size(); i++) {
        if (id == data._players.at(i)._id) {
            return data._players.at(i)._pos;
        }
    }
    return -1;
}
gbp_i64 idFromPos(gbp_i8 pos, const gbp::ns_api::ns_table::table_state_t& data) {
    for (int i = 0; i < data._players.size(); i++) {
        if (pos == data._players.at(i)._pos) {
            return data._players.at(i)._id;
        }
    }
    return -1;
}




template <typename identifier_type, typename data_type>
struct converter
{
    static identifier_type proxy_to_source(const data_type& data, const identifier_type& identifier);
    static identifier_type source_to_proxy(const data_type& data, const identifier_type& identifier);
};

template <typename T>
struct converter<T, T>
{
    static T proxy_to_source(const T& proxy);
    static T source_to_proxy(const T& source);
};

template <>
struct converter<std::vector<gbp::ns_api::ns_table::table_player_info_t>, std::vector<gbp::ns_api::ns_table::table_player_info_t>>
{
    static std::vector<gbp::ns_api::ns_table::table_player_info_t> proxy_to_source(const std::vector<gbp::ns_api::ns_table::table_player_info_t>& proxy) {
        std::vector<gbp::ns_api::ns_table::table_player_info_t> source;
        for (const gbp::ns_api::ns_table::table_player_info_t& player: proxy) {
            if (player._id != -1) {
                source.push_back(player);
            }
        }
        return source;
    }

    static std::vector<gbp::ns_api::ns_table::table_player_info_t> source_to_proxy(const std::vector<gbp::ns_api::ns_table::table_player_info_t>& source) {
        std::vector<gbp::ns_api::ns_table::table_player_info_t> proxy;
        proxy.assign(source.back()._pos + 1, gbp::ns_api::ns_table::table_player_info_t());

        for (const gbp::ns_api::ns_table::table_player_info_t& player: source) {
            proxy[player._pos] = player;
        }
        return proxy;
    }
};






//template <>
//int proxy_to_source(int identifier, const std::vector<gbp::ns_api::ns_table::table_player_info_t>& data) {
//    for (int i = 0; i < data.size(); i++) {
//        if (data.at(i)._pos == identifier) {
//            return i;
//        }
//    }
//    return -1;
//}
//template <>
//int source_to_proxy(int identifier, const std::vector<gbp::ns_api::ns_table::table_player_info_t>& data) {
//    if (identifier >= 0 && identifier < (int)data._players.size()) {
//        return data._players.at(identifier)._pos;
//    }
//    return -1;
//}


















int proxy_to_source(int row, const gbp::ns_api::ns_table::table_state_t& data) {
    for (int i = 0; i < data._players.size(); i++) {
        if (row == data._players.at(i)._pos) {
            return i;
        }
    }
    return -1;
}

namespace ns_model
{
    template <typename T, T invalidValue = std::declval<T>()>
    struct Mapping
    {
        using type = T;
        static const T invalid_value = invalidValue;

        T sourceValue;
        T proxyValue;

        Mapping(): sourceValue(invalidValue), proxyValue(invalidValue) {}
        Mapping(T sourceValue, T proxyValue): sourceValue(sourceValue), proxyValue(proxyValue) {}
        void invalidate() {
            sourceValue = proxyValue = invalidValue;
        }
    };
    template <gbp_i8 seats, typename MappingType, typename T = typename MappingType::type, T invalidValue = MappingType::invalid_value>
    struct Mappings
    {
        MappingType mappings[seats];

        MappingType* begin() { return mappings; }
        MappingType* end() { return begin() + seats; }

        void invalidate() {
            for (auto& mapping: mappings) {
                mapping.invalidate();
            }
        }
        T toSource(T proxyValue) const {
            if (proxyValue == invalidValue) {
                return invalidValue;
            }
            for (auto& mapping: mappings) {
                if (mapping.proxyValue == proxyValue) {
                    return mapping.sourceValue;
                }
            }
            return invalidValue;
        }
        T toProxy(T sourceValue) const {
            if (sourceValue == invalidValue) {
                return invalidValue;
            }
            for (auto& mapping: mappings) {
                if (mapping.sourceValue == sourceValue) {
                    return mapping.proxyValue;
                }
            }
            return invalidValue;
        }
    };

    using IntMapping = Mapping<int, -1>;

    struct CommonGameTable::PrivateMappings
    {
        Mappings<9, IntMapping> d_9;

//        int mapToSource(int proxyRow) const {
//            return d_9.mappings
//        }





            };

            CommonGameTable::CommonGameTable(QObject *parent)
                : QIdentityProxyModel(parent)
                , m_gameId(-1)
                , m_mappings(new PrivateMappings)
            {
                connect(this, &CommonGameTable::sourceModelChanged,  this, &CommonGameTable::updateMappings);
            }

            CommonGameTable::~CommonGameTable()
            {
                delete m_mappings;
            }

            void CommonGameTable::setGameID(const GameId &id)
            {
                if (id == gameID()) {
                    return;
                }

                beginResetModel();

                if (sourceModel() != nullptr) {
                    sourceModel()->disconnect(this);
                    this->disconnect(sourceModel());
                }

                if (id.isValid()) {
                    table_state_t* model = modelsDB().getTableState(id);
                    //! пропускаем коннекшны из QIdentityProxyModel
                    QAbstractProxyModel::setSourceModel(model);

                    connect(model, &table_state_t::rowsAboutToBeInserted,    this, &CommonGameTable::onSourceRowsAboutToBeInserted);
                    connect(model, &table_state_t::rowsInserted,             this, &CommonGameTable::onSourceRowsInserted);
                    connect(model, &table_state_t::rowsAboutToBeRemoved,     this, &CommonGameTable::onSourceRowsAboutToBeRemoved);
                    connect(model, &table_state_t::rowsRemoved,              this, &CommonGameTable::onSourceRowsRemoved);
                    connect(model, &table_state_t::rowsAboutToBeMoved,       this, &CommonGameTable::onSourceRowsAboutToBeMoved);
                    connect(model, &table_state_t::rowsMoved,                this, &CommonGameTable::onSourceRowsMoved);

                    connect(model, &table_state_t::modelAboutToBeReset,      this, &CommonGameTable::onSourceModelAboutToBeReset);
                    connect(model, &table_state_t::modelReset,               this, &CommonGameTable::onSourceModelReset);
                    connect(model, &table_state_t::dataChanged,              this, &CommonGameTable::onSourceDataChanged);


                } else {
                    setSourceModel(nullptr);
                }
                endResetModel();
            }

            GameId CommonGameTable::gameID() const
            {
                if (table_state_t* m = sourceModel()) {
                    return GameId(m->origin()._game_id, m->origin()._tournament_id);
                }
                return GameId();
            }

            int CommonGameTable::rowCount(const QModelIndex &parent) const
            {
                if (!sourceModel()) {
                    return 0;
                }
                if (!parent.isValid()) {
                    return 1;
                }
                return sourceModel()->rowsCapacity(parent);
            }

            int CommonGameTable::columnCount(const QModelIndex &parent) const
            {
                if (!sourceModel()) {
                    return 0;
                }
                if (!parent.isValid()) {
                    return 1;
                }
                return sourceModel()->columnsCapacity(parent);
            }

            table_state_t *CommonGameTable::sourceModel() const {
                return qobject_cast<table_state_t*>(QIdentityProxyModel::sourceModel());
            }

            void CommonGameTable::onSourceRowsAboutToBeInserted(const QModelIndex &parent, int first, int last) {
                updateMappings(); //beginInsertRows(parent, first, last);
            }

            void CommonGameTable::onSourceRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last) {
                updateMappings(); //beginRemoveRows(parent, first, last);
            }

            void CommonGameTable::onSourceRowsRemoved(const QModelIndex &/*parent*/, int /*first*/, int /*last*/) {
                updateMappings(); //endRemoveRows();
            }

            void CommonGameTable::onSourceRowsInserted(const QModelIndex &/*parent*/, int /*first*/, int /*last*/) {
                updateMappings(); //endInsertRows();
            }

            void CommonGameTable::onSourceRowsAboutToBeMoved(const QModelIndex &sourceParent, int first, int last, const QModelIndex &destinationParent, int destinationRow) {
                updateMappings(); //beginMoveRows(sourceParent, first, last, destinationParent, destinationRow);
            }

            void CommonGameTable::onSourceRowsMoved(const QModelIndex&, int, int, const QModelIndex&, int) {
                updateMappings(); //endMoveRows();
            }

            void CommonGameTable::onSourceModelAboutToBeReset() {
                beginResetModel();
                updateMappings();
            }

            void CommonGameTable::onSourceModelReset() {
                updateMappings();
                endResetModel();
            }

            void CommonGameTable::onSourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
                emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight), roles);
            }

            void CommonGameTable::updateMappings()
            {
                m_mappings->d_9.invalidate();

                std::vector<gbp::ns_api::ns_table::table_player_info_t>& players = sourceModel()->origin()._players;
                int row = 0;
                for (; row < sourceModel()->playersCount(); row++) {
                    m_mappings->d_9.mappings[row].proxyValue = players.at(row)._pos;
                    m_mappings->d_9.mappings[row].sourceValue = row;
                }
                for (; row < sourceModel()->origin()._seats_count; row++) {
                    m_mappings->d_9.mappings[row].proxyValue = row;
                    m_mappings->d_9.mappings[row].sourceValue = -1;
                }
                for (auto& mapp: m_mappings->d_9) {
//                    qDebug() << mapp.proxyValue << "-" << mapp.sourceValue;
                }
            }


            QModelIndex CommonGameTable::sourceIndex(int row, int column, const QModelIndex &parent) const
            {
                if (sourceModel() == nullptr) {
                    return QModelIndex();
                }

                Q_ASSERT(!parent.isValid() || parent.model() == sourceModel());

                return sourceModel()->index(row, column, parent);
            }
            QModelIndex CommonGameTable::index(int row, int column, const QModelIndex &parent) const
            {
                Q_ASSERT(!parent.isValid() || parent.model() == this);

                const QModelIndex sourceParent = mapToSource(parent);
                return mapFromSource(sourceIndex(row, column, sourceParent));
            }

            QModelIndex CommonGameTable::parent(const QModelIndex &child) const
            {
                return HackIndex(child).with(sourceModel()).parent().with(this);
            }

            QVariant CommonGameTable::data(const QModelIndex &index, int role) const
            {
                if (table_state_t::isRoot(index.parent()) && index.row() == 4 && index.column() == 0 && role == Qt::DisplayRole) {
                    QVariant val = QIdentityProxyModel::data(index, role);

                    if (role == Qt::DisplayRole && !val.isValid()) {
                        return sourceModel()->indexToKey(index);
                    }
                    std::vector<gbp::ns_api::ns_table::table_player_info_t> data = qvariant_cast<std::vector<gbp::ns_api::ns_table::table_player_info_t>>(mapToSource(index).data(UserType::OriginDataRole));
                    std::vector<gbp::ns_api::ns_table::table_player_info_t> proxyData = converter<std::vector<gbp::ns_api::ns_table::table_player_info_t>, std::vector<gbp::ns_api::ns_table::table_player_info_t>>::source_to_proxy(data);

                    return QVariant::fromValue(proxyData);
                }
                return QIdentityProxyModel::data(index, role);
            }

            bool CommonGameTable::setData(const QModelIndex &index, const QVariant &value, int role)
            {
                return QIdentityProxyModel::setData(index, value, role);
            }

            QMap<int, QVariant> CommonGameTable::itemData(const QModelIndex &index) const
            {
                return QIdentityProxyModel::itemData(index);
            }

            bool CommonGameTable::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
            {
                return QIdentityProxyModel::setItemData(index, roles);
            }

            QModelIndex CommonGameTable::mapToSource(const QModelIndex &proxyIndex) const
            {
                if (!proxyIndex.isValid() || proxyIndex.model() != this) {
                    return QModelIndex();
                }
                quint64 id = proxyIndex.internalId();

                if (id == ns_detail::table_state_parent_type::players) {
                    return HackIndex(m_mappings->d_9.toSource(proxyIndex.row()), proxyIndex.column(), proxyIndex.internalPointer(), sourceModel());
                }

                return QIdentityProxyModel::mapToSource(proxyIndex);
            }

            QModelIndex CommonGameTable::mapFromSource(const QModelIndex &sourceIndex) const
            {
                if (!sourceIndex.isValid() || sourceIndex.model() != sourceModel()) {
                    return QModelIndex();
                }
                quint64 id = sourceIndex.internalId();

                if (id == ns_detail::table_state_parent_type::players) {
                    return createIndex(sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
                }

                return QIdentityProxyModel::mapFromSource(sourceIndex);
            }

        } // namespace ns_model



