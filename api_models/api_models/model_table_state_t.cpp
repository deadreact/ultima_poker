#include "model_table_state_t.hpp"
#include "../poker_client/src/core/money.h"
#include <QLocale>
#include <qdebug.h>
#include "api_util.hpp"
#include "itemtype_concept.hpp"
#include <functional>
#include <qmetatype.h>
#include <qdatastream_io.hpp>
#include "defs_metatype.hpp"
#include "model_log.h"
#include "api_table_types.hpp"
#include <qregularexpression.h>
#include <tools/gameid.h>
#include "macro_table_state.hpp"
#include "helper_get_set_data.hpp"

#include "../../common/api/table/types.h"

Q_DECLARE_METATYPE(gbp::ns_vip_system::e_status)

#define SAFE_LOG if (m_logModel == nullptr) {return false;} m_logModel->newMsg

template <>
struct IdGetter<gbp::ns_api::ns_table::table_state_t>
{
    using id_type = GameId;
    static GameId get_id(const gbp::ns_api::ns_table::table_state_t& v) {
        return GameId(v._game_id, v._tournament_id);
    }
    constexpr static id_type invalidID() {
        return id_type::invalidID();
    }
};


namespace
{
    using parent_type = ns_model::ns_detail::table_state_parent_type;
    bool isTableElement(const QModelIndex& index) {
        return index.internalId() == parent_type::root;
    }
    bool isPlayerElement(const QModelIndex& index) {
        return index.internalId() == parent_type::players;
    }
//    bool isTableCard(const QModelIndex& index) {
//        return index.internalId() == parent_type::tableCards;
//    }
//    bool isTablePot(const QModelIndex& index) {
//        return index.internalId() == parent_type::tablePots;
//    }
//    bool isPlayerStateItem(const QModelIndex& index) {
//        return index.internalId() >= parent_type::player_state[0] && index.internalId() <= parent_type::player_state[8];
//    }
} // namespace

namespace ns_model
{

    template <>
    QModelIndex ApiBase::parentOf(const gbp::ns_api::ns_table::table_player_info_t& /*data*/) const {
        return ((table_state_t*)this)->indexOf(table_state_t::element::_players);
    }

    uint table_state_t::calcMaxIndices(const gbp::ns_api::ns_table::table_state_t &data)
    {
        uint max_table_cards = 5;
        uint max_players_cards = 4;
        uint player_states_count = 7;
        uint max_players = data._seats_count;
        uint max_pots = data._seats_count - 1;
        qDebug() << "max_single_player_items = uint(element_of_players::count) + max_players_cards + player_states_count";
        qDebug() << QString("max_single_player_items = %0 + 4 + 7 = %1").arg(uint(element_of_players::count)).arg(uint(element_of_players::count) + 11);
        uint max_single_player_items = uint(element_of_players::count) + max_players_cards + player_states_count;
        uint max_total_players_items = max_players * max_single_player_items;
        qDebug() << QString("max_total_players_items = max_players * max_single_player_items = %0 * %1 = %2").arg(max_players).arg(max_single_player_items).arg(max_total_players_items);

        uint result = uint(element::count) + max_table_cards + max_pots + max_total_players_items;
        qDebug() << "calcMaxIndices: " << result;


        return result;
    }

    QModelIndex table_state_t::root() const {
        return createIndex(0, 0, parent_type::invalid);
    }

    bool table_state_t::isRoot(const QModelIndex &index)
    {
        return index.row() == 0 && index.column() == 0 && index.internalId() == parent_type::invalid;
    }

    bool table_state_t::isPlayerCard(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return false;
        }
        if (/*const table_state_t* model =*/ qobject_cast<const table_state_t*>(index.model()))
        {
            static const QRegExp re("^\\[[0-3], ?10, ?\\[0, ?4, ?\\[0, ?0\\, ?0x0]\\]\\]$");
            return re.exactMatch(ns_util::traceModelIndex(index));
        }
        return false;
    }

    /***************************************************************************************************************/
    /***************************************************************************************************************/
    /***************************************************************************************************************/
    /***************************************************************************************************************/




    table_state_t::table_state_t(QObject *parent)
        : ApiBase(parent)
        , m_data(new table_state_data)
    {}

    table_state_t::~table_state_t()
    {}

    int table_state_t::playersCount() const {
        return m_data->_players.size();
    }

    int table_state_t::potsCount() const {
        return m_data->_all_pots.size();
    }

    int table_state_t::tableCardsCount() const {
        return m_data->_table_cards.size();
    }

    int table_state_t::playersCardsCount(int playerRow) const {
        return (playersCount() > playerRow) ? m_data->_players.at(playerRow)._cards.size() : 0;
    }

    void table_state_t::update(const table_state_data &data) {
        calcMaxIndices(data);
        if (get_id(&data).ref() != get_id(&origin()).ref()) {
            qDebug() << "reseting table #" << data._game_id;
            beginResetModel();
            origin() = data;
            endResetModel();
        }
        else
        {
            Comparator cmpobj;
            if (cmpobj.compare(origin(), data)) {
                qDebug() << "updating table #" << data._game_id;
                qDebug() << cmpobj.changes();
            }
            setData(root(), QVariant::fromValue(data), UserType::OriginDataRole);
        }
    }
    int table_state_t::rowCount(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return 1;
        }
        if (isRoot(parent)) {
            return underlying_cast(element::count);
        }
        quintptr id = parent.internalId();

        switch (id) {
        case parent_type::root:
            if (parent.column() == 0)
            {
                switch (element(parent.row())) {
                case element::_players:
                    return playersCount();
                case element::_table_cards:
                    return m_data->_table_cards.size();
                case element::_all_pots:
                    return m_data->_all_pots.size();
                default:
                    return 0;
                }
            }
            return 0;
        case parent_type::players:
            if (parent.row() < playersCount())
            {
                switch (element_of_players(parent.column())) {
                case element_of_players::_cards:
                    return m_data->_players.at(parent.row())._cards.size();
                case element_of_players::_state:
                    return 7;
                default:
                    return 0;
                }
            }
            return 0;
        default:
            return 0;
        }
        return 0;
    }

    int table_state_t::columnCount(const QModelIndex &parent) const
    {
        if (!parent.isValid()) {
            return 1;
        }
        if (isRoot(parent)) {
            return 1;
        }
        quintptr id = parent.internalId();

        switch (id) {
        case parent_type::root:
            if (parent.column() == 0)
            {
                switch (element(parent.row())) {
                case element::_players:
                    return underlying_cast(element_of_players::count);
                case element::_table_cards:
                case element::_all_pots:
                    return 1;
                default:
                    return 0;
                }
            }
            return 0;
        case parent_type::players:
            if (parent.row() < playersCount())
            {
                switch (element_of_players(parent.column())) {
                case element_of_players::_cards:
                case element_of_players::_state:
                    return 1;
                default:
                    return 0;
                }
            }
            return 0;
        default:
            return 0;
        }
        return 0;
    }

    QVariant table_state_t::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid()) {
            return QVariant();
        }
        switch (role) {
        case UserType::OriginDataRole: return Helper_TS<UserType::OriginDataRole>::getData(index, origin());
        case UserType::RowDataRole:    return Helper_TS<UserType::RowDataRole>::getData(index, origin());
        case Qt::DisplayRole:          return Helper_TS<Qt::DisplayRole>::getData(index, origin());
        case Qt::DecorationRole:       return Helper_TS<Qt::DecorationRole>::getData(index, origin());
        case Qt::EditRole:             return Helper_TS<Qt::EditRole>::getData(index, origin());
        case Qt::ToolTipRole:          return Helper_TS<Qt::ToolTipRole>::getData(index, origin()).toString() + "\ntrace: " + ns_util::traceModelIndex(index);
        case Qt::BackgroundRole:       return Helper_TS<Qt::BackgroundRole>::getData(index, origin());
        }
        return QVariant();
    }

    bool table_state_t::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        SetDataInfo::Results results = trySetData(index, value, role);
        if (results == SetDataInfo::Empty) {
            SAFE_LOG(QString("can't set data!\n%0:%1").arg(__FILE__).arg((int)results), LogMsgItem::MsgCritical) << data(index, Qt::ToolTipRole).toString() << value << role;
            return false;
        }
        bool finalResult = false;
        int resultsCount = 0;
        if (results & SetDataInfo::Success) {
            resultsCount++;
            SAFE_LOG(QString("data changed!\n%0:%1").arg(SetDataInfo::thisFile()).arg(results), LogMsgItem::MsgInfo) << data(index, Qt::ToolTipRole).toString() << value << role;
//            finalResult = true;
        }
        if (results & SetDataInfo::InvalidIndex) {
            resultsCount++;
            SAFE_LOG(QString("invalid index passed in setData!\n%0:%1").arg(SetDataInfo::thisFile()).arg(results), LogMsgItem::MsgWarning) << data(index, Qt::ToolTipRole).toString() << value << role;
//            finalResult = false;
        }
        if (results & SetDataInfo::SameValue) {
            resultsCount++;
            SAFE_LOG(QString("same data, nothing changed!\n%0:%1").arg(SetDataInfo::thisFile()).arg(results), LogMsgItem::MsgInfo) << data(index, Qt::ToolTipRole).toString() << value << role;
//            finalResult = false;
        }
        if (results & SetDataInfo::TypesMismatchCanConvert) {
            resultsCount++;
            SAFE_LOG(QString("TypesMismatchCanConvert! %0 != %1").arg(data(index, UserType::OriginDataRole).typeName()).arg(value.typeName()), LogMsgItem::MsgCritical) << data(index, Qt::ToolTipRole).toString() << value << role;
//            finalResult = false;
        }
        if (results & SetDataInfo::TypesMismatchNoConverter) {
            resultsCount++;
            SAFE_LOG(QString("TypesMismatchNoConverter! %0 != %1").arg(data(index, UserType::OriginDataRole).typeName()).arg(value.typeName()), LogMsgItem::MsgCritical) << data(index, Qt::ToolTipRole).toString() << value << role;
//            finalResult = false;
        }
        return finalResult;
    }
    SetDataInfo::Results table_state_t::trySetData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (role == UserType::OriginDataRole) {
            return Helper<UserType::OriginDataRole, table_state_data>::setData(index, origin(), value);
        }
        return SetDataInfo::Results(0);
    }


    bool table_state_t::insertRows(int row, int count, const QModelIndex &parent)
    {
        if (!parent.isValid() || row < 0 || count < 1) {
            return false;
        }
        if (isRoot(parent)) {
            return false;
        }
        if (isTableElement(parent)) {
            if (parent.column() != 0) {
                return false;
            }
            switch (element(parent.row())) {
            case element::_players:
                return insertRowsTo(m_data->_players, row, count, parent);
            case element::_table_cards:
                return insertRowsTo(m_data->_table_cards, row, count, parent);
            case element::_all_pots:
                return insertRowsTo(m_data->_all_pots, row, count, parent);
            default:
                return false;
            }
        }
        if (isPlayerElement(parent)) {
            if (parent.row() >= playersCount()) {
                return false;
            }
            switch (element_of_players(parent.column())) {
            case element_of_players::_cards:
                return insertRowsTo(m_data->_players.at(parent.row())._cards, row, count, parent);
            default:
                return false;
            }
        }

        return false;
    }

    bool table_state_t::removeRows(int row, int count, const QModelIndex &parent)
    {
        if (!parent.isValid() || row < 0 || count < 1) {
            return false;
        }
        if (isRoot(parent)) {
            return false;
        }
        if (isTableElement(parent)) {
            if (parent.column() != 0) {
                return false;
            }
            switch (element(parent.row())) {
            case element::_players:
                return removeRowsOf(m_data->_players, row, count, parent);
            case element::_table_cards:
                return removeRowsOf(m_data->_table_cards, row, count, parent);
            case element::_all_pots:
                return removeRowsOf(m_data->_all_pots, row, count, parent);
            default:
                return false;
            }
        }
        if (isPlayerElement(parent)) {
            if (parent.row() >= playersCount()) {
                return false;
            }
            switch (element_of_players(parent.column())) {
            case element_of_players::_cards:
                return removeRowsOf(m_data->_players.at(parent.row())._cards, row, count, parent);
            default:
                return false;
            }
        }

        return false;
    }

    int table_state_t::rowsCapacity(const QModelIndex &parent) const {
        if (isTableElement(parent)) {
            switch (element(parent.row())) {
            case element::_players:
                return m_data->_seats_count;
            case element::_table_cards:
                return 5;
            case element::_all_pots:
                return m_data->_seats_count - 1;
            default:
                return 0;
            }
        }
        if (isPlayerElement(parent)) {
            if (element_of_players(parent.column()) == element_of_players::_cards) {
                return 4;
            }
        }
        return rowCount(parent);
    }
    int table_state_t::columnsCapacity(const QModelIndex &parent) const {
        return columnCount(parent);
    }

    gbp::ns_api::ns_table::table_state_t& table_state_t::origin() {
        return *m_data.data();
    }
    const gbp::ns_api::ns_table::table_state_t& table_state_t::origin() const {
        return *m_data.data();
    }

    QModelIndex table_state_t::findElementOfPlayer(gbp_i64 id, element_of_players el) const
    {
        int row = 0;
        for (const gbp::ns_api::ns_table::table_player_info_t& pl: m_data->_players) {
            if (pl._id == id) {
                return index(row, underlying_cast(el), indexOf(element::_players));
            }
            row++;
        }
        return QModelIndex();
    }

    QModelIndex table_state_t::indexOf(element el) const {
        return createTableElementIndex(underlying_cast(el), 0);
    }

    QModelIndex table_state_t::indexOf(element_of_players el, int player) const {
        return createPlayerElementIndex(player, underlying_cast(el));
    }

    QModelIndex table_state_t::index(int row, int col, const QModelIndex &parent) const
    {
        if (row < 0 || col < 0 || row > underlying_cast(element::count)
                    || col > underlying_cast(element_of_players::count))
        {
            return QModelIndex();
        }
        if (!parent.isValid()) {
            if (col == 0 && row == 0) {
                return root();
            }
            return QModelIndex();
        }
        switch (parent.internalId()) {
        case parent_type::invalid:
            return createTableElementIndex(row, col);
        case parent_type::root:
            switch (element(parent.row())) {
            case element::_players:
                return createPlayerElementIndex(row, col);
            case element::_table_cards:
                return createTableCardIndex(row, col);
            case element::_all_pots:
                return createTablePotIndex(row, col);
            default:
                return QModelIndex();
            }
        case parent_type::players:
            switch (element_of_players(parent.column())) {
            case element_of_players::_cards:
                return createPlayerCardIndex(row, col, parent.row());
            case element_of_players::_state:
                return createPlayerStateIndex(row, col, parent.row());
            default:
                return QModelIndex();
            }
        case parent_type::tableCards:
        case parent_type::tablePots:
        default:
            return QModelIndex();
        }
        return QModelIndex();
    }

    QModelIndex table_state_t::parent(const QModelIndex &child) const
    {
        if (!child.isValid()) {
            return QModelIndex();
        }
        quintptr id = child.internalId();

        switch (id) {
        case parent_type::root:
            return root();
        case parent_type::players:
            return indexOf(element::_players);
        case parent_type::tableCards:
            return indexOf(element::_table_cards);
        case parent_type::tablePots:
            return indexOf(element::_all_pots);
        default:
            break;
        }
        if (id <= parent_type::player_cards[8]) {
            return indexOf(element_of_players::_cards, id - parent_type::player_cards[0]);
        } else if (id <= parent_type::player_state[8]) {
            return indexOf(element_of_players::_state, id - parent_type::player_state[0]);
        }
        return QModelIndex();
    }

    bool table_state_t::hasChildren(const QModelIndex &parent) const
    {
        if (!parent.isValid() || isRoot(parent)) {
            return true;
        }
        if (isTableElement(parent)) {
            switch (element(parent.row())) {
            case element::_players:     return !m_data->_players.empty();
            case element::_table_cards: return !m_data->_table_cards.empty();
            case element::_all_pots:    return !m_data->_all_pots.empty();
            default:
                return false;
            }
        }
        if (isPlayerElement(parent) && playersCount() > parent.row()) {
            switch (element_of_players(parent.column())) {
            case element_of_players::_cards: return !m_data->_players.at(parent.row())._cards.empty();
            case element_of_players::_state: return true;
            default:
                return false;
            }
        }
        return false;
    }

    void table_state_t::refresh()
    {
        beginResetModel();
        resetInternalData();
        endResetModel();
    }

    QModelIndex table_state_t::createTableElementIndex(int row, int column) const
    {
        if (column == 0 && row >= 0 && row < underlying_cast(element::count)) {
            return createIndex(row, column, parent_type::root);
        }
        return QModelIndex();
    }

    QModelIndex table_state_t::createPlayerElementIndex(int row, int column) const
    {
        if (column >= 0 && column <= underlying_cast(element_of_players::count) && !m_data->_players.empty() && row >= 0 && row < playersCount()) {
            return createIndex(row, column, parent_type::players);
        }
        return QModelIndex();
    }

    QModelIndex table_state_t::createTableCardIndex(int row, int column) const
    {
        int size = m_data->_table_cards.size();
        if (column == 0 && (!m_data->_table_cards.empty() && row >= 0 && row < size)) {
            return createIndex(row, column, parent_type::tableCards);
        }
        return QModelIndex();
    }
    QModelIndex table_state_t::createTablePotIndex(int row, int column) const
    {
        int size = m_data->_all_pots.size();
        if (column == 0 && (!m_data->_all_pots.empty() && row >= 0 && row < size)) {
            return createIndex(row, column, parent_type::tablePots);
        }
        return QModelIndex();
    }
    QModelIndex table_state_t::createPlayerCardIndex(int row, int column, int player) const
    {
        if (m_data->_players.empty() || player < 0 || player > playersCount()) {
            return QModelIndex();
        }
        const std::vector<gbp_i8>& cards = m_data->_players.at(player)._cards;
        if (column != 0 || cards.empty() || row < 0 || row >= playersCardsCount(player)) {
            return QModelIndex();
        }
        return createIndex(row, column, parent_type::player_cards[player]);
    }
    QModelIndex table_state_t::createPlayerStateIndex(int row, int column, int player) const
    {
        if (m_data->_players.empty() || player < 0 || player > playersCount()) {
            return QModelIndex();
        }
        if (column != 0 || row < 0 || row > 6) {
            return QModelIndex();
        }
        return createIndex(row, column, parent_type::player_state[player]);
    }

    QModelIndexList table_state_t::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const {
        if (role == UserType::InternalIdRole) {
            return QModelIndexList() << keyToIndex(value.toByteArray()/*, this*/);
        }
        return ApiBase::match(start, role, value, hits, flags);
    }


    bool table_state_t::write(QIODevice *device) const
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

    bool table_state_t::read(QIODevice *device)
    {
        if (device != nullptr && device->open(QIODevice::ReadOnly))
        {
            QDataStream stream(device);
            gbp::ns_api::ns_table::table_state_t buff;
            stream >> buff;
            device->close();
            update(buff);
            return true;
        }
        std::cout << "Read Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    bool table_state_t::isItemOrientedData() const {
        return false;
    }

    int ns_model::table_state_t::rowById(gbp_i64 id, const void *data, const QModelIndex &parent) const
    {
        if (data == nullptr) {
            return -1;
        }
        if (!parent.isValid()) {
            const auto* d = static_cast<const gbp::ns_api::ns_table::table_state_t*>(data);
            return (id == d->_game_id) ? 1 : -1;
        } else if (element(parent.row()) == element::_players) {
            const auto* d = static_cast<const std::vector<gbp::ns_api::ns_table::table_player_info_t>*>(data);
            for (int row = 0; row < rowCount(parent); row++) {
                if (d->at(row)._id == id) {
                    return row;
                }
            }
            return -1;
        }
        return -1;
    }

    int table_state_t::propertyIndex(const QByteArray &name, const QModelIndex &parent) const
    {
        if (!parent.isValid())
        {
            static const PropertyIndex<gbp::ns_api::ns_table::table_state_t> propertyIdx;
            return propertyIdx.indexOf(name);
        }
        if (element(parent.row()) == element::_players)
        {
            static const PropertyIndex<gbp::ns_api::ns_table::table_player_info_t> propertyIdx;
            return propertyIdx.indexOf(name);
        }
        return -1;
    }

    bool table_state_t::removeItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
    {
        bool result = false;
        const void* data = nullptr;
        if (!parent.isValid()) {
            data = m_data.data();
        } else if (element(parent.row()) == element::_players) {
            data = &m_data->_players;
        }
        for (gbp_i64 id: ids) {
            result |= removeRow(rowById(id, data, parent), parent);
        }
        return result;
    }

    bool table_state_t::insertItems(const QList<gbp_i64> &ids, const QModelIndex &parent)
    {
        if (ids.isEmpty()) {
            return false;
        }
        if (parent == indexOf(element::_players)) {
            if (playersCount() + ids.size() >= m_data->_seats_count) {
                return false;
            }
            QSet<gbp_i64> uniqueIds = ids.toSet();
            for (int i = 0; i < playersCount(); i++) {
                if (uniqueIds.contains(m_data->_players.at(i)._id)) {
                    return false;
                }
            }
            int oldCount = playersCount();
            int newCount = oldCount + ids.size() - 1;
            if (m_logModel) {
                m_logModel->newMsg(QString("beginInsertRows(parent, first, last)"), LogMsgItem::MsgInfo) << parent << oldCount << newCount;
            }
            beginInsertRows(parent, oldCount, newCount);
            m_data->_players.resize(newCount);
            QList<gbp_i64> lst = uniqueIds.toList();
            for (int i = oldCount; i < newCount; i++) {
                m_data->_players[i]._id = lst.takeFirst();
            }
            endInsertRows();
            return true;
        }

        bool result = false;
        const void* data = nullptr;
        if (!parent.isValid()) {
            data = m_data.data();
        } else if (element(parent.row()) == element::_players) {
            data = &m_data->_players;
        }
        for (gbp_i64 id: ids) {
            result |= insertRow(rowById(id, data, parent), parent);
        }
        return result;
    }

    template <>
    bool table_state_t::setOriginData(const gbp::ns_api::ns_table::table_player_info_t& value) {
        return setData(findElementOfPlayer(value._id, element_of_players::begin), QVariant::fromValue(value), UserType::RowDataRole);
    }
    template <>
    bool table_state_t::setOriginData(const gbp::ns_api::ns_table::table_state_t& value) {
        if (origin()._game_id != value._game_id || origin()._tournament_id != value._tournament_id)
        {
            beginResetModel();
            origin() = value;
            resetInternalData();
            endResetModel();
        }
        else
        {
            updateRowData(root(), origin(), value);
        }
        return true;
    }
    template <>
    bool table_state_t::setOriginData(const std::vector<gbp::ns_api::ns_table::table_player_info_t>& value) {
        return setOriginData(indexOf(element::_players), value);
    }

} //namespace ns_model


