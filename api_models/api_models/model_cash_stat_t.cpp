#include "model_cash_stat_t.hpp"
#include "../poker_client/src/core/money.h"
#include <itemtype_concept.hpp>
#include "model_tournament_stat_t.hpp"
#include <QLocale>
#include <qdebug.h>
#include "api_util.hpp"
#include "qdatastream_io.hpp"
#include "defs_metatype.hpp"
#ifndef Q_MOC_RUN
    #include <api/lobby_stat/reply.h>
#endif // !Q_MOC_RUN
#include <qfile.h>

Q_DECLARE_METATYPE(gbp::ns_api::ns_statistic::cash_stat_t)
Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_statistic::cash_stat_t>)

Q_DECLARE_METATYPE(gbp::ns_api::ns_game::e_game_limit)
Q_DECLARE_METATYPE(gbp::ns_api::ns_game::e_game_speed)
Q_DECLARE_METATYPE(gbp::ns_api::ns_game::e_game_type)

namespace ns_model
{
using iterator_type = std::vector<gbp::ns_api::ns_statistic::cash_stat_t>::iterator;
using predicate_type = std::function<bool(const gbp::ns_api::ns_statistic::cash_stat_t&)>;
iterator_type myfind(iterator_type begin, iterator_type end, predicate_type fn) {
    for (auto it = begin; it != end; it++) {
        if (fn(*it)) {
            return it;
        }
    }
    return end;
}



} //namespace ns_model


#define COMPARE(__item1, __item2, __cnt, __role, __property)\
if ((__item1.__property) != (__item2.__property)) {\
    __cnt[underlying_cast(column::__property)].push_back(__role);\
    setData(indexOf(__item1._id, column::__property), QVariant::fromValue(__item2.__property), __role);\
}

namespace
{
    using e_game_type           = gbp::ns_api::ns_game::e_game_type;
    using e_game_limit          = gbp::ns_api::ns_game::e_game_limit;
    using players_lst           = std::vector<gbp::ns_api::ns_statistic::player_t>;
}

namespace ns_model
{

    struct cash_parent_data
    {
        gbp_i64 id;
        int col;
        cash_parent_data(gbp_i64 id, int col) : id(id), col(col) {}
    };


    void cash_stat_t::updateData(const games_stat_t &d) {
        ApiBase::updateItems(*this, m_data, d.first);
    }

    void cash_stat_t::updateData(const data_t &d)
    {
        updateItems(*this, m_data, d);
    }

    cash_stat_t::cash_stat_t(QObject *parent)
        : ApiBase(parent)
        , m_data()
        , m_pData()
    {}

    cash_stat_t::~cash_stat_t()
    {
        qDeleteAll(m_pData);
    }

    void cash_stat_t::init()
    {
        QFile backup("last_cash_statistic.bin");
        read(&backup);
    }

    bool cash_stat_t::write(QIODevice *device) const
    {
        if (device != nullptr && device->open(QIODevice::WriteOnly))
        {
            QDataStream stream(device);
            stream << m_data;
            device->close();
            return true;
        }
        std::cout << "Write Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    bool cash_stat_t::read(QIODevice *device)
    {
        if (device != nullptr && device->open(QIODevice::ReadOnly))
        {
            QDataStream stream(device);
            games_stat_t::first_type buff;
            stream >> buff;
            device->close();
//            beginResetModel();
            updateItems(*this, m_data, buff);
//            m_data = buff;
//            endResetModel();
            return true;
        }
        std::cout << "Read Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    QModelIndex cash_stat_t::indexOf(gbp_i64 id) const {
        return indexOf(id, column::row_data);
    }

    QModelIndex cash_stat_t::indexOf(gbp_i64 id, column field) const
    {
        int col = qMax(0, underlying_cast(field));
        for (int row = 0; row < rowCount(); row++) {
            if (m_data.at(row)._id == id) {
                return index(row, col);
            }
        }
        return QModelIndex();
    }

    int cash_stat_t::rowById(gbp_i64 id, const void *data, const QModelIndex& parent) const
    {
        if (parent.isValid()) {
            return -1;
        }
        const data_t* d = (data != nullptr) ? static_cast<const  data_t*>(data) : (&m_data);
        for (size_t row = 0; row < d->size(); row++) {
            if (d->at(row)._id == id) {
                return row;
            }
        }
        return -1;
    }

    int cash_stat_t::propertyIndex(const QByteArray &name, const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            static const PropertyIndex<gbp::ns_api::ns_statistic::cash_stat_t> propertyIndex;
            return propertyIndex.indexOf(name);
        }
        else if (!parent.parent().isValid() && column(parent.column()) == column::_players) {
            static const PropertyIndex<gbp::ns_api::ns_statistic::player_t> propertyIndex;
            return propertyIndex.indexOf(name);
        }
        return -1;
    }

    Accessor<cash_stat_t::value_t> cash_stat_t::item(gbp_i64 id) const
    {
        for (const gbp::ns_api::ns_statistic::cash_stat_t& d: m_data) {
            if (d._id == id) {
                return Accessor<gbp::ns_api::ns_statistic::cash_stat_t>(&d);
            }
        }
        return Accessor<gbp::ns_api::ns_statistic::cash_stat_t>();
    }

    Accessor<cash_stat_t::value_t> cash_stat_t::itemAt(int row) const {
        return Accessor<gbp::ns_api::ns_statistic::cash_stat_t>(m_data.data() + row);
    }

    const gbp::ns_api::ns_statistic::cash_stat_t &cash_stat_t::defaultRowItem(cash_stat_t* that) {
        static value_t item;
        if (that == nullptr) {
            return item;
        }
        while (that->rowById(item._id) != -1) {
            item._id = -rand();
        }
        return item;
    }

    int cash_stat_t::rowCount(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return m_data.size();
        }
        if (!parent.parent().isValid())
        {
            switch (column(parent.column())) {
            case column::_players_count: return 1;
            case column::_players:
                return (parent.row() < rowCount()) ? m_data.at(parent.row())._players.size() : 0;
            default:
                return 0;
            }
        }
        return 0;
    }

    int cash_stat_t::columnCount(const QModelIndex &parent) const
    {
        if (!parent.isValid())
        {
            return underlying_cast(column::count);
        }

        if (!parent.parent().isValid())
        {
            switch (column(parent.column())) {
            case column::_players_count: return 2;
            case column::_players:       return underlying_cast(players::count);
            default:
                return 0;
            }
        }
        return 0;
    }

    QVariant cash_stat_t::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }

        if (!index.parent().isValid())
        {
            const gbp::ns_api::ns_statistic::cash_stat_t& item = itemAt(index.row()).ref();
            if (role == UserType::RowDataRole) {
                return QVariant::fromValue(item);
            }
            if (role == UserType::RowItemIdRole) {
                return QVariant::fromValue(item._id);
            }
            if (role == UserType::OriginDataRole)
            {
                switch (column(index.column())) {
                case column::_id:               return QVariant::fromValue(item._id);
                case column::_title:            return item._title.c_str();
                case column::_is_freeroll:      return item._is_freeroll;
                case column::_big_blind:        return QVariant::fromValue(item._big_blind);
                case column::_type:             return underlying_cast(item._type);
                case column::_limit:            return underlying_cast(item._limit);
                case column::_players_count:    return QVariant::fromValue(item._players_count);
                case column::_pot:              return QVariant::fromValue(item._pot);
                case column::_flp:              return item._flp;
                case column::_hands_per_hour:   return QVariant::fromValue(item._hands_per_hour);
                case column::_wait_players:     return QVariant::fromValue(item._wait_players);
                case column::_players:          return QVariant::fromValue(item._players);
                case column::_is_flash:         return item._is_flash;
                case column::_flash_id:         return QVariant::fromValue(item._flash_id);
                default:
                    return QVariant();
                }

                return QVariant();
            }
            else if (role == Qt::DisplayRole)
            {               
                switch (column(index.column())) {
                case column::_id:               return ns_meta::id_type::display(item._id);
                case column::_title:            return ns_meta::text_type::display(item._title);
                case column::_is_freeroll:      return ns_meta::flag_type::display(item._is_freeroll);
                case column::_big_blind:        return ns_meta::money_type::display(item._big_blind);
                case column::_type:             return ns_meta::enum_type::display(item._type);
                case column::_limit:            return ns_meta::enum_type::display(item._limit);
                case column::_players_count:    return ns_meta::players_count_type::display(item._players_count);
                case column::_pot:              return ns_meta::money_type::display(item._pot);
                case column::_flp:              return ns_meta::persent_type::display(item._flp);
                case column::_hands_per_hour:   return ns_meta::count_type::display(item._hands_per_hour);
                case column::_wait_players:     return ns_meta::count_type::display(item._wait_players);
                case column::_players:          return ns_meta::generic_container_type<decltype(item._players)>::display(item._players);
                case column::_is_flash:         return ns_meta::flag_type::display(item._is_flash);
                case column::_flash_id:         return ns_meta::id_type::display(item._flash_id);
                default:
                    return QVariant();
                }

                return QVariant();
            }
            else if (role == Qt::EditRole)
            {
                switch (column(index.column())) {
                case column::_id:               return ns_meta::id_type::edit(item._id);
                case column::_title:            return ns_meta::text_type::edit(item._title.c_str());
                case column::_is_freeroll:      return ns_meta::flag_type::edit(item._is_freeroll);
                case column::_big_blind:        return ns_meta::money_type::edit(item._big_blind);
                case column::_type:             return ns_meta::enum_type::edit(item._type);
                case column::_limit:            return ns_meta::enum_type::edit(item._limit);
                case column::_players_count:    return QVariant::fromValue(ns_meta::players_count_type::edit(item._players_count));
                case column::_pot:              return ns_meta::money_type::edit(item._pot);
                case column::_flp:              return ns_meta::persent_type::edit(item._flp);
                case column::_hands_per_hour:   return ns_meta::count_type::edit(item._hands_per_hour);
                case column::_wait_players:     return ns_meta::count_type::edit(item._wait_players);
                case column::_players:          return ns_meta::generic_container_type<decltype(item._players)>::edit(item._players);
                case column::_is_flash:         return ns_meta::flag_type::edit(item._is_flash);
                case column::_flash_id:         return ns_meta::id_type::edit(item._flash_id);
                default:
                    return QVariant();
                }

                return QVariant();
            }
            else if (role == Qt::CheckStateRole)
            {
                switch (column(index.column())) {
                case column::_is_freeroll:      return item._is_freeroll;
                case column::_is_flash:         return item._is_flash;
                default:
                    return QVariant();
                }
            }
        }
        else if (!index.parent().parent().isValid())
        {
            const gbp::ns_api::ns_statistic::cash_stat_t& parentItem = m_data.at(index.parent().row());
            if (role == UserType::OriginDataRole)
            {
                switch (column(index.parent().column())) {
                case column::_players_count: {
                    return index.row() == 0 ? parentItem._players_count.first : parentItem._players_count.second;
                }
                case column::_players:
                {
                    auto playerItem = parentItem._players.at(index.row());
                    switch (players(index.column())) {
                        case players::_nick:     return playerItem._nick.c_str();
                        case players::_pos:      return playerItem._pos;
                        case players::_money:    return QVariant::fromValue(playerItem._money);
                        case players::_country:  return playerItem._country;
                        default:
                            return QVariant();
                    }
                }
                default:
                    return QVariant();
                }
            }
            else if (role == Qt::DisplayRole)
            {
                switch (column(index.parent().column())) {
                case column::_players_count:
                {
                    if (index.row() == 0) {
                        if (index.column() == 0) {
                            return parentItem._players_count.first;
                        } else if (index.column() == 1) {
                            return parentItem._players_count.second;
                        }
                    }
                    return QVariant();
                }
                case column::_players:
                {
                    auto playerItem = parentItem._players.at(index.row());
                    switch (players(index.column())) {
                        case players::_nick:     return QString::fromStdString(playerItem._nick);
                        case players::_pos:      return (int)playerItem._pos;
                        case players::_money:    return Money(playerItem._money).toString();
                        case players::_country:  return QLocale::countryToString(QLocale::Country(playerItem._country));
                        default:
                            return QVariant();
                    }
                }
                default:
                    return QVariant();
                }
            }
        }

        return QVariant();
    }

    bool cash_stat_t::setData(const QModelIndex &idx, const QVariant &value, int role)
    {
        if (!idx.isValid()) {
            return false;
        }
        
        if (data(idx, role) != value) {
            if (!idx.parent().isValid())
            {
                if (role == UserType::RowDataRole)
                {
                    if (value.canConvert<value_t>()) {
                        value_t newItem = value.value<value_t>();
                        value_t& itemRef = m_data.at(idx.row()); 
                        if (itemRef._id == newItem._id) {        
                            itemRef = newItem;                   

                            int colCount = columnCount(idx.parent());
                            if (colCount > 0) {
                                QModelIndex idx1 = idx.sibling(idx.row(), 0);
                                QModelIndex idx2 = idx.sibling(idx.row(), colCount - 1);

                                emit dataChanged(idx1, idx2);
                                return true;
                            }
                        }
                    }
                    return false;
                }
                gbp::ns_api::ns_statistic::cash_stat_t& item = m_data.at(idx.row());
                if (role == Qt::EditRole)
                {
                    bool result = false;
                    switch (column(idx.column())) {
                    case column::_id:             result = ns_meta::editModelData<ns_meta::id_type>(item._id, value);            break;
                    case column::_title:          result = ns_meta::editModelData<ns_meta::name_type>(item._title, value);       break;
                    case column::_is_freeroll:    result = ns_meta::editModelData<ns_meta::flag_type>(item._is_freeroll, value); break;
                    case column::_big_blind:      result = ns_meta::editModelData<ns_meta::money_type>(item._big_blind, value);  break;
                    case column::_type:           result = ns_meta::editModelData<ns_meta::game_type_type>(item._type, value);   break;
                    case column::_limit:          result = ns_meta::editModelData<ns_meta::game_limit_type>(item._limit, value); break;
                    case column::_players_count:
                    {
                        if (ns_meta::editModelData<ns_meta::players_count_type>(item._players_count, value)) {
                            if (item._players_count.first != item._players.size()) {
                                if (item._players_count.first < item._players.size()) {
                                    removeRows(0, item._players.size() - item._players_count.first, indexOf(item._id, column::_players));
                                }  else {
                                    insertRows(0, item._players_count.first - item._players.size(), indexOf(item._id, column::_players));
                                }
                            }
                            emit dataChanged(idx, idx);
                            return true;
                        }
                        return false;
                    }                                                                                                                                                       
                    case column::_pot:            result = ns_meta::editModelData<ns_meta::money_type>(item._pot, value);   break;
                    case column::_flp:            result = ns_meta::editModelData<ns_meta::persent_type>(item._flp, value); break;
                    case column::_hands_per_hour: result = ns_meta::editModelData<ns_meta::generic_count_type<decltype(item._hands_per_hour)>>(item._hands_per_hour, value); break;
                    case column::_wait_players:   result = ns_meta::editModelData<ns_meta::generic_count_type<decltype(item._wait_players)>>(item._wait_players, value); break;
                    case column::_players:
                    {
                        if (ns_meta::editModelData<ns_meta::generic_container_type<players_lst> >(item._players, value)) {
                            if (item._players.size() != item._players_count.first) {
                                item._players_count.first = item._players.size();
                                emit dataChanged(indexOf(item._id, column::_players_count), indexOf(item._id, column::_players_count));
                            }
                            emit dataChanged(idx, idx);
                            return true;
                        } 
                        return false;
                    } 
                    case column::_is_flash:       result = ns_meta::editModelData<ns_meta::flag_type>(item._is_flash, value); break;
                    case column::_flash_id:       result = ns_meta::editModelData<ns_meta::id_type>(item._flash_id, value);   break;
                    default:
                        return false;
                    }
                    if (result) {
                        emit dataChanged(idx, idx);
                    }
                    return result;
                } 
            } 
            else if (!idx.parent().parent().isValid())
            {
                if (role == Qt::DisplayRole)
                {
                    gbp::ns_api::ns_statistic::cash_stat_t& parentItem = m_data.at(idx.parent().row());
                    switch (column(idx.parent().column())) {
                    case column::_players_count:
                    {
                        if (idx.column() != 0 || idx.row() > 1) {
                            return false;
                        }
                        if (idx.row() == 0) {
                            parentItem._players_count.first = value.value<gbp_u8>();
                        } else {
                            parentItem._players_count.second = value.value<gbp_u8>();
                        }
                        break;
                    }
                    case column::_players:
                    {
                        auto& item = parentItem._players.at(idx.row());
                        switch (players(idx.column())) {
                        case players::_nick:    item._nick    = value.toString().toStdString(); break;
                        case players::_pos:     item._pos     = value.value<gbp_i8>(); break;
                        case players::_country: item._country = underlying_cast(value.value<QLocale::Country>()); break;
                        case players::_money:   item._money   = value.value<gbp_i64>(); break;
                        default:
                            return false;
                        }
                        break;
                    }
                    default:
                        return false;
                    }
                    emit dataChanged(idx, idx);
                    return true;
                }
            }
            else
            {
                return false;
            }

            emit dataChanged(idx, idx, QVector<int>() << role);
            return true;
        }
        return false;
    }

    Qt::ItemFlags cash_stat_t::flags(const QModelIndex &index) const
    {
        if (!index.isValid()) {
            return Qt::NoItemFlags;
        }

        auto commonFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (!index.parent().isValid())
        {
            gbp::ns_api::ns_statistic::cash_stat_t item = m_data.at(index.row());
            auto commonFlags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
            switch (column(index.column())) {
            case column::_id:             return commonFlags;
            case column::_title:          return commonFlags | Qt::ItemIsEditable;
            case column::_is_freeroll:    return commonFlags | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
            case column::_big_blind:      return commonFlags | Qt::ItemIsEditable;
            case column::_type:           return commonFlags | Qt::ItemIsEditable;
            case column::_limit:          return commonFlags | Qt::ItemIsEditable;
            case column::_players_count:  return commonFlags | Qt::ItemIsEditable | Qt::ItemIsSelectable;
            case column::_pot:            return commonFlags | Qt::ItemIsEditable;
            case column::_flp:            return commonFlags | Qt::ItemIsEditable;
            case column::_hands_per_hour: return commonFlags | Qt::ItemIsEditable;
            case column::_wait_players:   return commonFlags | Qt::ItemIsEditable;
            case column::_players:        return (item._players.empty() ? Qt::ItemIsEnabled : Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            case column::_is_flash:       return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
            case column::_flash_id:       return (item._is_flash) ? Qt::ItemIsEnabled : Qt::NoItemFlags;
            default:
                break;
            }
        }

        if (!index.parent().parent().isValid())
        {
            if (flags(index.parent()) & Qt::ItemIsEnabled)
            {
                switch (column(index.parent().column())) {
                case column::_players_count: return Qt::ItemIsEnabled;
                case column::_players:
                {
                    gbp::ns_api::ns_statistic::cash_stat_t parentItem = m_data.at(index.parent().row());
                    switch (players(index.column())) {
                    case players::_nick:
                        return commonFlags;
                    case players::_money:
                        return commonFlags | Qt::ItemIsEditable;
                    case players::_country:
                        return commonFlags;
                    default:
                        return Qt::NoItemFlags;
                    }
                }
                default:
                    return Qt::NoItemFlags;
                }
            }
        }

        return Qt::NoItemFlags;
    }

    bool cash_stat_t::removeItems(const QList<gbp_i64>& ids, const QModelIndex &parent)
    {
        bool result = false;
        if (!parent.isValid()) {
            auto& data = m_data;
            for (gbp_i64 id: ids) {
                QModelIndex idx = indexOf(id);
                if (idx.isValid()) {
                    beginRemoveRows(parent, idx.row(), idx.row());
                    data.erase(data.begin() + idx.row());
                    endRemoveRows();
                    result = true;
                }
            }
            return result;
        }
        return false;
    }

    bool cash_stat_t::insertItems(const QList<gbp_i64>& ids, const QModelIndex &parent)
    {
        QList<gbp_i64> copiedIds = ids;
        if (!parent.isValid()) {
            for (gbp_i64 id: ids)
            {
                if (item(id).ptr() != nullptr) {
                    copiedIds.removeAll(id);
                }
            }
            if (copiedIds.empty()) {
                return false;
            }
            auto& data = m_data;
            beginInsertRows(parent, data.size(), data.size() + copiedIds.size() - 1);
            ns_util::push_empty(data, copiedIds.size());
            for (gbp_i64 id: copiedIds)  {
                data.back()._id = id;
            }
            endInsertRows();
            return true;
        }
        return false;
    }

    bool cash_stat_t::insertRows(int row, int count, const QModelIndex &parent)
    {
        try {
        if (count < 1) {
            return false;
        }
        if (!parent.isValid()) {
            beginInsertRows(parent, row, row + count - 1);
            m_data.insert(m_data.begin()+row, count, takeWaitingForInsertItem<value_t>());
            endInsertRows();
            return true;
        }
        if (parent.parent().isValid()) {
            return ApiBase::insertRows(row, count, parent);
        }
        players_lst& lst = m_data.at(parent.row())._players;
        if (column(parent.column()) == column::_players) {
            if (lst.size() + count > 9) {
                return false;
            }
            beginInsertRows(parent, row, row + count - 1);
            lst.insert(lst.begin() + row, count, takeWaitingForInsertItem<players_lst::value_type>());
            endInsertRows();
            return true;
        }
        } catch (std::length_error& err) {
            //qDebug() << row << count << parent << m_waitForInserting;
            std::cout << err.what() << std::endl;
        }

        return false;
    }

    bool cash_stat_t::removeRows(int row, int count, const QModelIndex &parent)
    {
        if (!parent.isValid())
        {
            beginRemoveRows(parent, row, row + count - 1);
            m_data.erase(m_data.begin() + row, m_data.begin() + row + count);
            endRemoveRows();
            return true;
        }
        if (parent.parent().isValid()) {
            return ApiBase::removeRows(row, count, parent);
        }
        players_lst& lst = m_data.at(parent.row())._players;
        if (column(parent.column()) == column::_players) {
            if (int(lst.size() - row) < count) {
                return false;
            }
            beginRemoveRows(parent, row, row + count - 1);
            lst.erase(lst.begin() + row, lst.begin() + row + count);
            endRemoveRows();
            return true;
        }
        return ApiBase::removeRows(row, count, parent);
    }

    bool cash_stat_t::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
    {
        if (sourceParent != destinationParent) {
            return false;
        }
        if (rowCount(destinationParent) >= destinationChild && rowCount(sourceParent) >= sourceRow + count) {
            if (!sourceParent.isValid())
            {
                beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
                data_t buffer;
                auto itFirst = m_data.begin() + sourceRow;
                auto itLast = m_data.begin() + sourceRow + count;
                buffer.assign(itFirst, itLast);
                m_data.erase(itFirst, itLast);
                m_data.insert(m_data.end(), buffer.begin(), buffer.end());
                endMoveRows();
                return true;
            }
            else if (!sourceParent.parent().isValid() && column(sourceParent.column()) == column::_players)
            {
                players_lst &data = itemAt(sourceParent.parent().row()).ref()._players;
                beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild);
                players_lst buffer;
                auto itFirst = data.begin() + sourceRow;
                auto itLast = data.begin() + sourceRow + count;
                buffer.assign(itFirst, itLast);
                data.erase(itFirst, itLast);
                data.insert(data.end(), buffer.begin(), buffer.end());
                endMoveRows();
                return true;
            }
        }
        return false;
    }


    QModelIndex cash_stat_t::index(int row, int col, const QModelIndex &parent) const
    {
        if (row >= rowCount(parent) || col >= columnCount(parent)) {
            return QModelIndex();
        }
        if (!parent.isValid()) {
            return createIndex(row, col, nullptr);
        }
        if (!parent.parent().isValid()) {
            gbp::ns_api::ns_statistic::cash_stat_t parentItem = m_data.at(parent.row());
            switch (column(parent.column())) {
            case column::_players_count:
            case column::_players:
            {
                cash_parent_data* d = new cash_parent_data(parentItem._id, parent.column());
                m_pData << d;
                return createIndex(row, col, d);
            }
            default:
                return QModelIndex();
            }
        }
        return QModelIndex();
    }

    QModelIndex cash_stat_t::parent(const QModelIndex &child) const
    {
        if (child.internalPointer() == nullptr) {
            return QModelIndex();
        }
        const cash_parent_data* d = static_cast<const cash_parent_data*>(child.internalPointer());
        for (size_t i = 0; i < m_data.size(); i++) {
            if (m_data.at(i)._id == d->id) {
                return index(i, d->col, QModelIndex());
            }
        }

        return QModelIndex();
    }

    bool cash_stat_t::hasChildren(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return true;
        }
        if (!parent.parent().isValid())
        {
            switch (column(parent.column())) {
            case column::_players_count:
            case column::_players:
                return true;
            default:
                return false;
            }
        }

        return false;
    }

    void cash_stat_t::refresh()
    {
        beginResetModel();
        endResetModel();
    }

    QVariant cash_stat_t::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return ns_util::enumToString(column(section));
        default:
            return QVariant();
        }

        return QVariant();
    }


    QModelIndexList cash_stat_t::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
    {
        if (role == UserType::RowItemIdRole && value.canConvert<gbp_i64>()) {
            QModelIndexList lst;
            int row = rowById(value.value<gbp_i64>());
            if (row != -1 && (!start.isValid() || start.row() < row || (flags & Qt::MatchRecursive))) {
                int column = (start.isValid()) ? start.column() : 0;
                for (; hits != 0 && column < columnCount(); column++, hits--) {
                    lst << index(row, column);
                }
                if (hits != 0 && start.isValid() && start.column() > 0 && (flags & Qt::MatchRecursive)) {
                    column = 0;
                    for (; hits != 0 && column < start.column(); column++, hits--) {
                        lst << index(row, column);
                    }
                }
            }
            return lst;
        }
        return ApiBase::match(start, role, value, hits, flags);
    }

} //namespace ns_model


