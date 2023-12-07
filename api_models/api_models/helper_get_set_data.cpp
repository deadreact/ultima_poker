#include "helper_get_set_data.hpp"

#include "api_table_types.hpp"
#include "itemtype_concept.hpp"
#include "api_util.hpp"
#include "visitors.hpp"
#include "model_table_state_t.hpp"

#include <tools/gameid.h>

Q_DECLARE_METATYPE(gbp::ns_vip_system::e_status)

/*******************************************************************************************************/
/****************************************** METATYPES **************************************************/
/*******************************************************************************************************/
namespace ns_meta
{
    QStringList state_type::buff;
    struct money_group_type
    {
        using value_type = std::vector<money_type::value_type>;
        using edit_type = std::vector<money_type::edit_type>;
        using display_type = QString;

        static display_type display(const value_type& value) {
            QList<money_type::display_type> lst;
            for (const money_type::value_type& m: value) {
                lst << money_type::display(m);
            }
            return lst.join(", ");
        }
        static edit_type edit(const value_type& value) {
            edit_type editData;
            for (const auto& val: value) {
                editData.push_back(money_type::edit(val));
            }
            return editData;
        }
    };

    void state_type::operator()(const char *name, bool value)
    {
        if (value) {
            buff << QString::fromLatin1(name);
        }
    }

    state_type::display_type state_type::display(const value_type &value)
    {
        buff.clear();
        value.apply(state_type());
        return buff.join("|");
    }

    state_type::edit_type state_type::edit(const value_type &value)
    {
        edit_type edit_value = state::no_flag;
        edit_value |= (value._left_table) ? state::_left_table : state::no_flag;
        edit_value |= (value._sit_out) ? state::_sit_out : state::no_flag;
        edit_value |= (value._frozen) ? state::_frozen : state::no_flag;
        edit_value |= (value._anticipant) ? state::_anticipant : state::no_flag;
        edit_value |= (value._folded) ? state::_folded : state::no_flag;
        edit_value |= (value._all_ined) ? state::_all_ined : state::no_flag;
        edit_value |= (value._all_ined_in_cur_round) ? state::_all_ined_in_cur_round : state::no_flag;
        return edit_value;
    }
} //namespace ns_meta


/*******************************************************************************************************/
/****************************************** STUFF ******************************************************/
/*******************************************************************************************************/
namespace ns_model
{
//    template <>
//    void ApiBase::updateRowItem(ApiBase& model, std::vector<gbp_i8>& oldItems, const std::vector<gbp_i8> &newItems, size_t row, const QModelIndex& parent)
//    {
//        Q_ASSERT(oldItems.size() == newItems.size() && oldItems.size() > row);

//        if (oldItems[row] != newItems[row]) {
//            oldItems[row] = newItems[row];
//            QModelIndex idx = model.index(row, 0, parent);
//            emit model.dataChanged(idx, idx);
//        }
//    }
//    template <>
//    void ApiBase::updateRowItem(ApiBase& model, std::vector<gbp_i64>& oldItems, const std::vector<gbp_i64> &newItems, size_t row, const QModelIndex& parent)
//    {
//        Q_ASSERT(oldItems.size() == newItems.size() && oldItems.size() > row);

//        if (oldItems[row] != newItems[row]) {
//            oldItems[row] = newItems[row];
//            QModelIndex idx = model.index(row, 0, parent);
//            emit model.dataChanged(idx, idx);
//        }
//    }

    const char *SetDataInfo::thisFile() {
        return __FILE__;
    }

    QList<QVariantList> childrenData(const QModelIndex &parent, int role)
    {
        QList<QVariantList> data;
        if (!parent.isValid()) {
            return data;
        }
        const QAbstractItemModel* model = parent.model();
        if (!model->hasChildren(parent)) {
            return data;
        }
        for (int row = 0; row < model->rowCount(parent); row++) {
            data << QVariantList();
            for (int column = 0; column < model->columnCount(parent); column++) {
                data.last() << model->index(row, column, parent).data(role);
            }
        }
        return data;
    }

    QString makeString(const QList<QVariantList> &childrenData)
    {
        QStringList strlst;
        for (const QVariantList& lst: childrenData) {
            QStringList lastlst;
            for (const QVariant& value: lst) {
                if (value.canConvert<QString>()) {
                    lastlst << value.toString();
                } else {
                    lastlst << "???";
                }
            }
            strlst << lastlst.join(" ");
        }
        return strlst.join("\n");
    }


}


template <>
struct IdGetter<ns_table::table_state_t>
{
    using T = ns_table::table_state_t;
    using type = IdGetter<ns_table::table_state_t>;
    using id_type = GameId;

    constexpr static id_type invalidID() {
        return GameId::invalidID();
    }

    static id_type get_id(const T& v) {
        return GameId(v._game_id, v._tournament_id);
    }
    template <typename Container> static typename Container::const_iterator
    find_id(const Container &items
          , const typename std::enable_if<std::is_same<typename Container::value_type, T>::value, Container>::type& id)
    {
        for (auto it = items.begin(); it != items.end(); it++) {
            if (::get_id(it) == id) {
                return it;
            }
        }
        return items.end();
    }
    template <typename Container> static typename Container::iterator
    find_id(Container &items
          , const typename std::enable_if<std::is_same<typename Container::value_type, T>::value, Container>::type& id)
    {
        for (auto it = items.begin(); it != items.end(); it++) {
            if (::get_id(it) == id) {
                return it;
            }
        }
        return items.end();
    }
};

//template <> const typename IdGetter<ns_table::table_state_t>::id_type
//                           IdGetter<ns_table::table_state_t>::invalidId = GameId::invalidID();

namespace
{
using parent_type = ns_model::ns_detail::table_state_parent_type;

bool isTableElement(const QModelIndex& index) {
    return index.internalId() == parent_type::root;
}
bool isPlayerElement(const QModelIndex& index) {
    return index.internalId() == parent_type::players;
}
bool isTableCard(const QModelIndex& index) {
    return index.internalId() == parent_type::tableCards;
}
bool isTablePot(const QModelIndex& index) {
    return index.internalId() == parent_type::tablePots;
}
bool isPlayerCard(const QModelIndex& index) {
    return index.internalId() >= parent_type::player_cards[0] && index.internalId() <= parent_type::player_cards[8];
    }
    bool isPlayerStateItem(const QModelIndex& index) {
        return index.internalId() >= parent_type::player_state[0] && index.internalId() <= parent_type::player_state[8];
    }

    bool isOfIntType(const QVariant& value)
    {
        switch (value.userType()) {
        case QMetaType::SChar:
        case QMetaType::UChar:
        case QMetaType::Short:
        case QMetaType::UShort:
        case QMetaType::Int:
        case QMetaType::UInt:
        case QMetaType::Long:
        case QMetaType::ULong:
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            return true;
        default:
            return false;
        }
    }
//    bool isTypeDiffOnlyInSign(const QVariant& val1)
    bool intCanConvertWithoutOverflow(const QVariant& value, int typeId)
    {
        if (value.userType() == typeId) {
            return true;
        }
        if (!isOfIntType(value)) {
            return false;
        }
        switch (typeId) {
        case QMetaType::Long:
        case QMetaType::LongLong:
        case QMetaType::ULong:
        case QMetaType::ULongLong:
            return !(qvariant_cast<gbp_i64>(value) - qvariant_cast<gbp_u64>(value));
        case QMetaType::Int:
            return !(qvariant_cast<gbp_u64>(value) - qvariant_cast<gbp_i32>(value));
        case QMetaType::UInt:
            return !(qvariant_cast<gbp_i64>(value) - qvariant_cast<gbp_u32>(value));
        case QMetaType::Short:
            return !(qvariant_cast<gbp_u64>(value) - qvariant_cast<gbp_i16>(value));
        case QMetaType::UShort:
            return !(qvariant_cast<gbp_i64>(value) - qvariant_cast<gbp_u16>(value));
        case QMetaType::SChar:
            return !(qvariant_cast<gbp_u64>(value) - qvariant_cast<gbp_i8>(value));
        case QMetaType::UChar:
            return !(qvariant_cast<gbp_i64>(value) - qvariant_cast<gbp_u8>(value));
        default:
            return false;
        }
    }
}

namespace ns_model
{
    const quintptr ns_detail::table_state_parent_type::player_cards[] = {5, 6, 7, 8, 9, 10, 11, 12, 13};
    const quintptr ns_detail::table_state_parent_type::player_state[] = {14, 15, 16, 17, 18, 19, 20, 21, 22};



    /*******************************************************************************************************/
    /****************************************** GETTERS ****************************************************/
    /*******************************************************************************************************/
    template <> QVariant Helper_TS_DisplayRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (ns_model::table_state_t::isRoot(index)) {
            return QString("table #%0").arg(GameId(data._game_id, data._tournament_id).toString());
        }

        using element = ns_model::table_state_t::element;
        using element_of_players = ns_model::table_state_t::element_of_players;
        if (isTableElement(index)) {
            switch (element(index.row())) {
            case element::_tournament_id:       return ns_meta::id_type::display(data._tournament_id);
            case element::_game_id:             return ns_meta::id_type::display(data._game_id);
            case element::_big_blind:           return ns_meta::money_type::display(data._big_blind);
            case element::_seats_count:         return ns_meta::count_type::display(data._seats_count);
            case element::_players:             return ns_meta::count_type::display(data._players.size());
            case element::_current_user_pos:    return ns_meta::pos_type::display(data._current_user_pos);
            case element::_table_cards:         return ns_meta::cards_type::display(data._table_cards);
            case element::_all_pots:            return ns_meta::money_group_type::display(data._all_pots);
            case element::_waiters_count:       return ns_meta::count_type::display(data._waiters_count);
            case element::_time_short:          return ns_meta::duration_type::display(data._time_short);
            case element::_time_main:           return ns_meta::duration_type::display(data._time_main);
            case element::_time_action_passed:  return ns_meta::duration_type::display(data._time_action_passed);
            case element::_time_action_left:    return ns_meta::duration_type::display(data._time_action_left);
            case element::_distribution_number: return ns_meta::id_type::display(data._distribution_number);
            default:
                return QVariant();
            }
        }
        quintptr id = index.internalId();

        switch (id) {
        case parent_type::players:
        {
            const auto& item = data._players.at(index.row());
            switch (element_of_players(index.column())) {
            case element_of_players::_id:                       return ns_meta::id_type::display(item._id);
            case element_of_players::_is_user:                  return ns_meta::flag_type::display(item._is_user);
            case element_of_players::_pos:                      return ns_meta::pos_type::display(item._pos);
            case element_of_players::_nickname:                 return ns_meta::name_type::display(item._nickname);
            case element_of_players::_country:                  return ns_meta::country_type::display(item._country);
            case element_of_players::_money:                    return ns_meta::money_type::display(item._money);
            case element_of_players::_start_stack:              return ns_meta::money_type::display(item._start_stack);
            case element_of_players::_bounty_progressive_money: return ns_meta::money_type::display(item._bounty_progressive_money);
            case element_of_players::_hash_avatar:              return ns_meta::img_type::display(item._hash_avatar);
            case element_of_players::_is_dealer:                return ns_meta::flag_type::display(item._is_dealer);
            case element_of_players::_cards:                    return ns_meta::cards_type::display(item._cards);
            case element_of_players::_round_bet:                return ns_meta::money_type::display(item._round_bet);
            case element_of_players::_is_current:               return ns_meta::flag_type::display(item._is_current);
            case element_of_players::_vip_status:               return ns_meta::tr_enum_type::display(item._vip_status);
            case element_of_players::_state:                    return QVariant::fromValue(ns_meta::state_type::display(item._state));
            default:
                return QVariant();
            }
        }
        case parent_type::tableCards:
            return ns_meta::card_type::display(data._table_cards.at(index.row()));
        case parent_type::tablePots:
            return ns_meta::money_type::display(data._all_pots.at(index.row()));
        default:
            if (index.parent().row() < 0 || index.parent().row() >= (int)data._players.size()) {
                return QVariant();
            } else if (isPlayerCard(index)) {
                return ns_meta::card_type::display(data._players.at(index.parent().row())._cards.at(index.row()));
            } else if (isPlayerStateItem(index)) {
                switch (index.row()) {
                case 0: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._left_table);
                case 1: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._sit_out);
                case 2: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._frozen);
                case 3: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._anticipant);
                case 4: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._folded);
                case 5: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._all_ined);
                case 6: return ns_meta::flag_type::display(data._players.at(index.parent().row())._state._all_ined_in_cur_round);
                default:
                    return QVariant();
                }
            }
            return QVariant();
        }
        return QVariant();
    }   
    
    template <> QVariant Helper_TS_DecorationRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (isTableElement(index)) {
        }
        if (isPlayerElement(index)) {
        }
        if (isTableCard(index)) {
        }
        if (isTablePot(index)) {
        }
        if (index.parent().row() < 0 || index.parent().row() >= (int)data._players.size()) {
            return QVariant();
        }
        if (isPlayerCard(index)) {
        }
        if (isPlayerStateItem(index)) {

        }
//        return ns_meta::standard_icon_type::decorate(ns_meta::standard_icon_type::value_type(index.internalId()));
        return QVariant();
    }   
    
    template <> QVariant Helper_TS_ToolTipRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (ns_model::table_state_t::isRoot(index)) {
            return QString("table #%0").arg(GameId(data._game_id, data._tournament_id).toString());
        }
        if (isTableElement(index)) {
            static const PropertyIndex<ns_table::table_state_t> propIdx;
            return propIdx.keyAt(index.row()) + ": " + Helper<Qt::DisplayRole, Type>::getData(index, data).toString();
        }
        if (isPlayerElement(index)) {
            PropertyIndex<ns_table::table_player_info_t> propIdx;
            return propIdx.keyAt(index.column()) + ": " + Helper<Qt::DisplayRole, Type>::getData(index, data).toString();
        }
        if (isTableCard(index)) {
            return QString("_table_card #%0: ").arg(index.row()+1) + Helper<Qt::DisplayRole, Type>::getData(index, data).toString();
        }
        if (isTablePot(index)) {
            return QString("_table_pot #%0: ").arg(index.row()+1) + Helper<Qt::DisplayRole, Type>::getData(index, data).toString();
        }
        if (index.parent().row() < 0 || index.parent().row() >= int(data._players.size())) {
            return QVariant();
        }
        if (isPlayerCard(index)) {
            QString nickname = index.parent().sibling(index.parent().row(), underlying_cast(table_state_t::element_of_players::_nickname)).data(Qt::DisplayRole).toString();
            return QString("player %0, card #%1: ").arg(nickname).arg(index.row()+1) + Helper<Qt::DisplayRole, Type>::getData(index, data).toString();
        }
        if (isPlayerStateItem(index)) {
            switch (index.row()) {
            case 0: return QString("player state.%0 %1").arg("_left_table ").arg(index.data(UserType::OriginDataRole).toBool());
            case 1: return QString("player state.%0 %1").arg("_sit_out ").arg(index.data(UserType::OriginDataRole).toBool());
            case 2: return QString("player state.%0 %1").arg("_frozen ").arg(index.data(UserType::OriginDataRole).toBool());
            case 3: return QString("player state.%0 %1").arg("_anticipant ").arg(index.data(UserType::OriginDataRole).toBool());
            case 4: return QString("player state.%0 %1").arg("_folded ").arg(index.data(UserType::OriginDataRole).toBool());
            case 5: return QString("player state.%0 %1").arg("_all_ined ").arg(index.data(UserType::OriginDataRole).toBool());
            case 6: return QString("player state.%0 %1").arg("_all_ined_in_cur_round ").arg(index.data(UserType::OriginDataRole).toBool());
            default:
                return QVariant();
            }
        }
        return QVariant();
    }

    template <> QVariant Helper_TS_EditRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (ns_model::table_state_t::isRoot(index)) {
            return GameId(data._game_id, data._tournament_id).toString();
        }
        using element = ns_model::table_state_t::element;
        using element_of_players = ns_model::table_state_t::element_of_players;
        if (isTableElement(index)) {
            switch (element(index.row())) {
            case element::_tournament_id:       return ns_meta::id_type::edit(data._tournament_id);
            case element::_game_id:             return ns_meta::id_type::edit(data._game_id);
            case element::_big_blind:           return ns_meta::money_type::edit(data._big_blind);
            case element::_seats_count:         return ns_meta::count_type::edit(data._seats_count);
            case element::_players:             return ns_meta::count_type::edit(data._players.size());
            case element::_current_user_pos:    return ns_meta::pos_type::edit(data._current_user_pos);
            case element::_table_cards:         return ns_meta::cards_type::edit(data._table_cards);
            case element::_all_pots:            return QVariant::fromValue(ns_meta::money_group_type::edit(data._all_pots));
            case element::_waiters_count:       return ns_meta::count_type::edit(data._waiters_count);
            case element::_time_short:          return ns_meta::duration_type::edit(data._time_short);
            case element::_time_main:           return ns_meta::duration_type::edit(data._time_main);
            case element::_time_action_passed:  return ns_meta::duration_type::edit(data._time_action_passed);
            case element::_time_action_left:    return ns_meta::duration_type::edit(data._time_action_left);
            case element::_distribution_number: return ns_meta::id_type::edit(data._distribution_number);
            default:
                return QVariant();
            }
        }
        quintptr id = index.internalId();

        switch (id) {
        case parent_type::players:
        {
            const auto& item = data._players.at(index.row());
            switch (element_of_players(index.column())) {
            case element_of_players::_id:                       return ns_meta::id_type::edit(item._id);
            case element_of_players::_is_user:                  return ns_meta::flag_type::edit(item._is_user);
            case element_of_players::_pos:                      return ns_meta::pos_type::edit(item._pos);
            case element_of_players::_nickname:                 return ns_meta::name_type::edit(item._nickname);
            case element_of_players::_country:                  return ns_meta::country_type::edit(item._country);
            case element_of_players::_money:                    return ns_meta::money_type::edit(item._money);
            case element_of_players::_start_stack:              return ns_meta::money_type::edit(item._start_stack);
            case element_of_players::_bounty_progressive_money: return ns_meta::money_type::edit(item._bounty_progressive_money);
            case element_of_players::_hash_avatar:              return ns_meta::img_type::edit(item._hash_avatar);
            case element_of_players::_is_dealer:                return ns_meta::flag_type::edit(item._is_dealer);
            case element_of_players::_cards:                    return displayChildren(index); /*ns_meta::cards_type::edit(item._cards)*/;
            case element_of_players::_round_bet:                return ns_meta::money_type::edit(item._round_bet);
            case element_of_players::_is_current:               return ns_meta::flag_type::edit(item._is_current);
            case element_of_players::_vip_status:               return ns_meta::tr_enum_type::edit(item._vip_status);
            case element_of_players::_state:                    return QVariant::fromValue(ns_meta::state_type::edit(item._state));
            default:
                return QVariant();
            }
        }
        case parent_type::tableCards:
            return ns_meta::card_type::edit(data._table_cards.at(index.row()));
        case parent_type::tablePots:
            return ns_meta::money_type::edit(data._all_pots.at(index.row()));
        default:
            if (index.parent().row() < 0 || index.parent().row() >= int(data._players.size())) {
                return QVariant();
            } else if (isPlayerCard(index)) {
                return ns_meta::card_type::edit(data._players.at(index.parent().row())._cards.at(index.row()));
            } else if (isPlayerStateItem(index)) {
                switch (index.row()) {
                case 0: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._left_table);
                case 1: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._sit_out);
                case 2: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._frozen);
                case 3: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._anticipant);
                case 4: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._folded);
                case 5: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._all_ined);
                case 6: return ns_meta::flag_type::edit(data._players.at(index.parent().row())._state._all_ined_in_cur_round);
                default:
                    return QVariant();
                }
            }
            return QVariant();
        }
        return QVariant();
    }   
    template <> QVariant Helper_TS_FgRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        Q_UNUSED(data)
        if (ns_model::table_state_t::isRoot(index)) {
            return QColor(0xfafafa);
        }
        return QVariant();
    }   
    template <> QVariant Helper_TS_BgRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (ns_model::table_state_t::isRoot(index)) {
            return QColor(0x222a30);
        }
        if (isTableElement(index)) {
            return QColor(Qt::darkMagenta);
        }
        if (isPlayerElement(index)) {
            return QColor(Qt::gray);
        }
        if (isTableCard(index)) {
            return QColor(Qt::yellow);
        }
        if (isTablePot(index)) {
            return QColor(Qt::blue);
        }
        if (index.parent().row() < 0 || index.parent().row() >= int(data._players.size())) {
            return QVariant();
        }
        if (isPlayerCard(index)) {
            return QColor(Qt::green);
        }
        if (isPlayerStateItem(index)) {
            return QColor(Qt::red);
        }
        return QVariant();
    }
    
    template <> QVariant Helper_TS_RowDataRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (ns_model::table_state_t::isRoot(index)) {
            return QVariant();
        }
        if (isTableElement(index)) {
            return index.parent().data(UserType::OriginDataRole);
        }
        if (isTableCard(index)) {
            return index.data(UserType::OriginDataRole);
//            return model->indexOf(table_state_t::element::_table_cards).data(UserType::OriginDataRole);
        }
        if (isTablePot(index)) {
            return index.data(UserType::OriginDataRole);
//            return model->indexOf(table_state_t::element::_all_pots).data(UserType::OriginDataRole);
        }
        if (isPlayerElement(index)) {
            return QVariant::fromValue(data._players.at(index.row()));
        }
        if (isPlayerCard(index) || isPlayerStateItem(index)) {
            return index.data(UserType::OriginDataRole);
        }
        return QVariant();
    }

    template <> QVariant Helper_TS_OriginRole::getData(const QModelIndex& index, const ns_table::table_state_t& data)
    {
        if (!index.isValid()) {
            return QVariant();
        }
        if (ns_model::table_state_t::isRoot(index)) {
            return QVariant::fromValue(data);
        }

        if (isTableElement(index)) {
            switch (index.row()) {
            case 0x0: return QVariant::fromValue(data.get_member<0x0>());
            case 0x1: return QVariant::fromValue(data.get_member<0x1>());
            case 0x2: return QVariant::fromValue(data.get_member<0x2>());
            case 0x3: return QVariant::fromValue(data.get_member<0x3>());
            case 0x4: return QVariant::fromValue(data.get_member<0x4>());
            case 0x5: return QVariant::fromValue(data.get_member<0x5>());
            case 0x6: return QVariant::fromValue(data.get_member<0x6>());
            case 0x7: return QVariant::fromValue(data.get_member<0x7>());
            case 0x8: return QVariant::fromValue(data.get_member<0x8>());
            case 0x9: return QVariant::fromValue(data.get_member<0x9>());
            case 0xa: return QVariant::fromValue(data.get_member<0xa>());
            case 0xb: return QVariant::fromValue(data.get_member<0xb>());
            case 0xc: return QVariant::fromValue(data.get_member<0xc>());
            case 0xd: return QVariant::fromValue(data.get_member<0xd>());
            default:
                return QVariant();
            }
        }
        quintptr id = index.internalId();

        switch (id) {
        case parent_type::players:
        {
            const auto& item = data._players.at(index.row());
            switch (index.column()) {
            case 0x0: return QVariant::fromValue(item.get_member<0x0>());
            case 0x1: return QVariant::fromValue(item.get_member<0x1>());
            case 0x2: return QVariant::fromValue(item.get_member<0x2>());
            case 0x3: return QVariant::fromValue(item.get_member<0x3>());
            case 0x4: return QVariant::fromValue(item.get_member<0x4>());
            case 0x5: return QVariant::fromValue(item.get_member<0x5>());
            case 0x6: return QVariant::fromValue(item.get_member<0x6>());
            case 0x7: return QVariant::fromValue(item.get_member<0x7>());
            case 0x8: return QVariant::fromValue(item.get_member<0x8>());
            case 0x9: return QVariant::fromValue(item.get_member<0x9>());
            case 0xa: return QVariant::fromValue(item.get_member<0xa>());
            case 0xb: return QVariant::fromValue(item.get_member<0xb>());
            case 0xc: return QVariant::fromValue(item.get_member<0xc>());
            case 0xd: return QVariant::fromValue(item.get_member<0xd>());
            case 0xe: return QVariant::fromValue(item.get_member<0xe>());
            default:
                return QVariant();
            }
        }
        case parent_type::tableCards:
            return QVariant::fromValue(data._table_cards.at(index.row()));
        case parent_type::tablePots:
            return QVariant::fromValue(data._all_pots.at(index.row()));
        default:
            if (index.parent().row() < 0 || index.parent().row() >= index.model()->rowCount(index.parent())/*data._players.size()*/) {
                return QVariant();
            } else if (isPlayerCard(index)) {
                return QVariant::fromValue(data._players.at(index.parent().row())._cards.at(index.row()));
            } else if (isPlayerStateItem(index)) {
                switch (index.row()) {
                case 0: return QVariant::fromValue(data._players.at(index.parent().row())._state._left_table);
                case 1: return QVariant::fromValue(data._players.at(index.parent().row())._state._sit_out);
                case 2: return QVariant::fromValue(data._players.at(index.parent().row())._state._frozen);
                case 3: return QVariant::fromValue(data._players.at(index.parent().row())._state._anticipant);
                case 4: return QVariant::fromValue(data._players.at(index.parent().row())._state._folded);
                case 5: return QVariant::fromValue(data._players.at(index.parent().row())._state._all_ined);
                case 6: return QVariant::fromValue(data._players.at(index.parent().row())._state._all_ined_in_cur_round);
                default:
                    return QVariant();
                }
            }
            return QVariant();
        }
        return QVariant();
    }


    /*******************************************************************************************************/
    /****************************************** SETTERS ****************************************************/
    /*******************************************************************************************************/

    SetDataInfo::Results success(const QModelIndex& index) {
        if (ApiBase* model = qobject_cast<ApiBase*>(const_cast<QAbstractItemModel*>(index.model()))) {
            emit model->dataChanged(index, index);
            return SetDataInfo::Success;
        }
        return SetDataInfo::Fail;
    }

    template <typename T>
    SetDataInfo::Results setManually(const QModelIndex& index, const QVariant& newValue, T& oldValue) {
        oldValue = qvariant_cast<T>(newValue);
        return success(index);
    }

    template <int R, typename T>
    SetDataInfo::Results setDataCommon(const QModelIndex& index, T& data, const QVariant& value)
    {
        if (!index.isValid()) {
            return SetDataInfo::InvalidIndex;
        }
        QVariant prevValue = Helper<R, T>::getData(index, data);
        if (prevValue == value) {
            return SetDataInfo::SameValue;
        }
        if (prevValue.userType() != value.userType()) {
            return value.canConvert(prevValue.userType()) ? SetDataInfo::TypesMismatchCanConvert : SetDataInfo::TypesMismatchNoConverter;
        }
        return SetDataInfo::Success;
    }

    template <> SetDataInfo::Results Helper_TS_RowDataRole::setData(const QModelIndex& index, ns_table::table_state_t& data, const QVariant& value)
    {
        SetDataInfo::Results result = setDataCommon<role, Type>(index, data, value);
        if (result != SetDataInfo::Success) {
            return result;
        }

        if (isPlayerElement(index))
        {
            Comparator comp;
            if (!comp.compare(data._players.at(index.row()), qvariant_cast<ns_table::table_player_info_t>(value))) {
                return SetDataInfo::SameValue;
            }
            static const PropertyIndex<ns_table::table_player_info_t> propIdx;
            SetDataInfo::Results result = SetDataInfo::Empty;
            for (const QByteArray& key: comp.changes().uniqueKeys())
            {
                result |= setData(index.model()->index(index.row(), propIdx.indexOf(key), index), data, comp.changes().value(key).second);
            }
            return result;
        }

        return SetDataInfo::Fail;
    }

    template <> SetDataInfo::Results Helper_TS_OriginRole::setData(const QModelIndex& index, ns_table::table_state_t& data, const QVariant& value)
    {
        SetDataInfo::Results result = setDataCommon<role, Type>(index, data, value);
        if (result != SetDataInfo::Success) {
            if (result == SetDataInfo::TypesMismatchCanConvert)
            {
                int id = getData(index, data).userType();
                if (intCanConvertWithoutOverflow(value, id))
                {
                    QVariant newValue = value;
                    newValue.convert(id);
                    return setData(index, data, newValue) | SetDataInfo::TypesMismatchCanConvert;
                }
            }
            return result;
        }

        table_state_t* model = (table_state_t*)index.model();
        switch (index.internalId()) {
        case parent_type::invalid:
        {
            ModelDataIndex dataIndex("ns_table::table_state_t");
            ModelDataSetter<ns_table::table_state_t> dSetter(&dataIndex);
            qDebug() << "ModelDataSetter dSetter FOR TABLE!!";
            if (dSetter.setData(data, qvariant_cast<ns_table::table_state_t>(value)) > 0) {
                qDebug() << dSetter.changes();
            }
            static const PropertyIndex<Type> propIdx;
            Type newData = qvariant_cast<Type>(value);
            Comparator comp;
            comp.compare(data, newData);

            SetDataInfo::Results localResult = SetDataInfo::Empty;
            for (const QByteArray& key: comp.changes().uniqueKeys()) {
                localResult |= setData(index.model()->index(propIdx.indexOf(key), 0, index), data, comp.changes().value(key).second);
            }
            return localResult;
        }
        case parent_type::root:
        {
            using element = table_state_t::element;
            switch (element(index.row())) {
            case element::_tournament_id:       return setManually(index, value, data._tournament_id);
            case element::_game_id:             return setManually(index, value, data._game_id);
            case element::_big_blind:           return setManually(index, value, data._big_blind);
            case element::_seats_count:         return setManually(index, value, data._seats_count);
            case element::_players:
            {
                std::vector<ns_table::table_player_info_t> newPlayers = qvariant_cast<std::vector<ns_table::table_player_info_t>>(value);
                table_state_t::updateItems(*model, data._players, newPlayers, index);
                return success(index);
            }
            case element::_current_user_pos:
                return setManually(index, value, data._current_user_pos);
            case element::_table_cards:
            {
                table_state_t::updatePrimitives(*model, data._table_cards, qvariant_cast<decltype(data._table_cards)>(value), index);
                return success(index);
            }
            case element::_all_pots:
            {
                auto all_pots = qvariant_cast<std::vector<gbp_i64>>(value);
                if (data._all_pots.size() != all_pots.size()) {
                    table_state_t::updatePrimitives(*model, data._all_pots, qvariant_cast<decltype(data._all_pots)>(value), index);
                } else {
                    data._all_pots = all_pots;
                }
                return success(index);
            }
            case element::_waiters_count:
            {
                return setManually(index, value, data._waiters_count);
            }
            case element::_time_short:          return setManually(index, value, data._time_short);
            case element::_time_main:           return setManually(index, value, data._time_main);
            case element::_time_action_passed:  return setManually(index, value, data._time_action_passed);
            case element::_time_action_left:    return setManually(index, value, data._time_action_left);
            case element::_distribution_number: return setManually(index, value, data._distribution_number);
            default:
                return SetDataInfo::Results(__LINE__);
            }
        }
        case parent_type::tableCards:
        {
            return setManually(index, value, data._table_cards[index.row()]);
        }
        case parent_type::tablePots:
        {
            return setManually(index, value, data._all_pots[index.row()]);
        }
        case parent_type::players:
        {
            if (index.row() >= static_cast<int>(data._players.size())) {
                return SetDataInfo::Results(__LINE__);
            }
            ns_table::table_player_info_t& player = data._players[index.row()];
            using element_of_players = table_state_t::element_of_players;
            switch (element_of_players(index.column())) {
            case element_of_players::_id:                         return setManually(index, value, player._id);
            case element_of_players::_is_user:                    return setManually(index, value, player._is_user);
            case element_of_players::_pos:                        return setManually(index, value, player._pos);
            case element_of_players::_nickname:                   return setManually(index, value, player._nickname);
            case element_of_players::_country:                    return setManually(index, value, player._country);
            case element_of_players::_money:                      return setManually(index, value, player._money);
            case element_of_players::_start_stack:                return setManually(index, value, player._start_stack);
            case element_of_players::_bounty_progressive_money:   return setManually(index, value, player._bounty_progressive_money);
            case element_of_players::_hash_avatar:                return setManually(index, value, player._hash_avatar);
            case element_of_players::_is_dealer:                  return setManually(index, value, player._is_dealer);
            case element_of_players::_round_bet:                  return setManually(index, value, player._round_bet);
            case element_of_players::_is_current:                 return setManually(index, value, player._is_current);
            case element_of_players::_vip_status:                 return setManually(index, value, player._vip_status);
            case element_of_players::_cards:
            {
                table_state_t::updatePrimitives(*model, player._cards, qvariant_cast<decltype(player._cards)>(value), index);
                return success(index);
            }
            case element_of_players::_state:
            {
                return setManually(index, value, player._state);
            }
            default:
                return SetDataInfo::Results(__LINE__);
            }
            return SetDataInfo::Results(__LINE__);
        }
        default:
            if (table_state_t::isPlayerCard(index)) {
                return setManually(index, value, data._players[index.parent().row()]._cards[index.row()]);
            } else if (isPlayerStateItem(index)) {
                ns_table::user_state_t& state = data._players[index.parent().row()]._state;
                switch (index.row()) {
                case ns_meta::state_type::_left_table:            return setManually(index, value, state._left_table);
                case ns_meta::state_type::_sit_out:               return setManually(index, value, state._sit_out);
                case ns_meta::state_type::_frozen:                return setManually(index, value, state._frozen);
                case ns_meta::state_type::_anticipant:            return setManually(index, value, state._anticipant);
                case ns_meta::state_type::_folded:                return setManually(index, value, state._folded);
                case ns_meta::state_type::_all_ined:              return setManually(index, value, state._all_ined);
                case ns_meta::state_type::_all_ined_in_cur_round: return setManually(index, value, state._all_ined_in_cur_round);
                default:
                    return SetDataInfo::Results(__LINE__);
                }
            }
            return SetDataInfo::Results(__LINE__);
        }
    }

} // namespace ns_model


//if (role == UserType::RowDataRole)
//{
//    if (isRoot(index.parent())) {
//        setData(index.parent(), value, UserType::OriginDataRole);
//    }
//}
//if (role == UserType::OriginDataRole || role == Qt::EditRole)
//{
//    switch (index.internalId()) {
//    case parent_type::invalid:
//    {
//        if (!value.canConvert<ns_table::table_state_t>()) {
//            return __LINE__;
//        }
//        beginResetModel();
//        m_data.reset(new ns_table::table_state_t(qvariant_cast<ns_table::table_state_t>(value)));
//        endResetModel();
//        return 0;
//    }
//    case parent_type::root:
//    {
//        int line = 0;
//        switch (element(index.row())) {
//        case element::_tournament_id:       return __LINE__;
//        case element::_game_id:             return __LINE__;
//        case element::_big_blind:           return __LINE__;
//        case element::_seats_count:         return __LINE__;
//        case element::_players:
//        {
//            if (!value.canConvert<std::vector<ns_table::table_player_info_t>>()) {
//                return __LINE__;
//            }
//            std::vector<ns_table::table_player_info_t> newPlayers = qvariant_cast<std::vector<ns_table::table_player_info_t>>(value);
//            updateItems(*this, m_data->_players, newPlayers, index.parent());
//            return returnDataChanged(index);
//        }
//        case element::_current_user_pos:
//        {
//            if (!value.canConvert<decltype(m_data->_current_user_pos)>()) {
//                return __LINE__;
//            }
//            m_data->_current_user_pos = qvariant_cast<decltype(m_data->_current_user_pos)>(value);
//            return returnDataChanged(index);
//        }
//        case element::_table_cards:
//        {
//            if (!value.canConvert<std::vector<gbp_i8>>()) {
//                return __LINE__;
//            }
//            std::vector<gbp_i8> newCards = qvariant_cast<std::vector<gbp_i8>>(value);
//            updateItems(*this, m_data->_table_cards, newCards, index.parent());
//            return returnDataChanged(index);
//        }
//        case element::_all_pots:
//        {

//            if (value.canConvert<std::vector<gbp_i64>>()) {
//                std::vector<gbp_i64> newPots = qvariant_cast<std::vector<gbp_i64>>(value);
//                updateItems(*this, m_data->_all_pots, newPots, index.parent());
//                return returnDataChanged(index);
//            } else if (!value.canConvert<gbp_i64>()) {
//                // std::vector<gbp_i64> newPots = {qvariant_cast<gbp_i64>(value)};
//                // updateItems(*this, m_data->_all_pots, newPots, index.parent());
//                // emit dataChanged(index, index);
//                // return 0;
//                return __LINE__;
//            } else {
//                return __LINE__;
//            }
//        }
//        case element::_waiters_count:       line = ns_meta::generic_count_type<decltype(m_data->_waiters_count)>::modify(m_data->_waiters_count, value) ? 0 : __LINE__; break;
//        case element::_time_short:          line = ns_meta::duration_type::modify(m_data->_time_short, value) ? 0 : __LINE__; break;
//        case element::_time_main:           line = ns_meta::duration_type::modify(m_data->_time_main, value) ? 0 : __LINE__; break;
//        case element::_time_action_passed:  line = ns_meta::duration_type::modify(m_data->_time_action_passed, value) ? 0 : __LINE__; break;
//        case element::_time_action_left:    line = ns_meta::duration_type::modify(m_data->_time_action_left, value) ? 0 : __LINE__; break;
//        case element::_distribution_number: line = ns_meta::id_type::modify(m_data->_distribution_number, value) ? 0 : __LINE__; break;
//        default:
//            return __LINE__;
//        }
//        if (line == 0) {
//            return returnDataChanged(index);
//        }
//        return __LINE__;
//    }
//    default:
//        return __LINE__;
//    }
//}
//quintptr id = index.internalId();
//switch (id) {
//case parent_type::players:
//{
//    auto& item = m_data->_players.at(index.row());
//    int line = 0;
//    switch (element_of_players(index.column())) {
//    case element_of_players::_id:                       line = (ns_meta::id_type::modify(item._id, value)) ? 0 : __LINE__; break;
//    case element_of_players::_is_user:                  return __LINE__;
//    case element_of_players::_pos:                      line = ns_meta::modify_common(item._pos, value) ? 0 : __LINE__; break;
//    case element_of_players::_nickname:                 line = ns_meta::modify_common(item._nickname, value) ? 0 : __LINE__; break;
//    case element_of_players::_country:                  line = ns_meta::country_type::modify(item._country, value) ? 0 : __LINE__; break;
//    case element_of_players::_money:                    line = ns_meta::money_type::modify(item._money, value) ? 0 : __LINE__; break;
//    case element_of_players::_start_stack:              line = ns_meta::money_type::modify(item._start_stack, value) ? 0 : __LINE__; break;
//    case element_of_players::_bounty_progressive_money: line = ns_meta::money_type::modify(item._bounty_progressive_money, value) ? 0 : __LINE__; break;
//    case element_of_players::_hash_avatar:              line = ns_meta::img_type::modify(item._hash_avatar, value) ? 0 : __LINE__; break;
//    case element_of_players::_is_dealer:                line = ns_meta::flag_type::modify(item._is_dealer, value) ? 0 : __LINE__; break;
//    case element_of_players::_cards:
//    {
//        if (!value.canConvert<std::vector<gbp_i8>>()) {
//            return __LINE__;
//        }
//        if (playersCount() >= index.parent().row()) {
//            return __LINE__;
//        }
//        std::vector<gbp_i8> newCards = qvariant_cast<std::vector<gbp_i8>>(value);
//        updateItems(*this, m_data->_players[index.parent().row()]._cards, newCards, index.parent());
//        return returnDataChanged(index);
//    }
//    case element_of_players::_round_bet:                line = ns_meta::money_type::modify(item._round_bet, value) ? 0 : __LINE__; break;
//    case element_of_players::_is_current:               line = ns_meta::flag_type::modify(item._is_current, value) ? 0 : __LINE__; break;
//    case element_of_players::_vip_status:               line = ns_meta::modify_common(item._vip_status, value) ? 0 : __LINE__; break;
//    case element_of_players::_state:
//    {
//        if (playersCount() >= index.parent().row()) {
//            return __LINE__;
//        }
//        if (ns_meta::modify_common(m_data->_players[index.parent().row()]._state, value)) {
//            return returnDataChanged(index);
//        }
//        return __LINE__;
//    }
//    default:
//        return __LINE__;
//    }
//    if (line == 0) {
//        return returnDataChanged(index);
//    }
//    return line;
//}
//case parent_type::tableCards:
//    if (ns_meta::card_type::modify(m_data->_table_cards.at(index.row()), value)) {
//        return returnDataChanged(index);
//    }
//    return __LINE__;
//case parent_type::tablePots:
//    if (ns_meta::money_type::modify(m_data->_all_pots.at(index.row()), value)) {
//        return returnDataChanged(index);
//    }
//    return __LINE__;
//default:
//    if (index.parent().row() < 0 || index.parent().row() >= playersCount()) {
//        return __LINE__;
//    } else if (isPlayerCard(index)) {
//        if (ns_meta::card_type::modify(m_data->_players[index.parent().row()]._cards[index.row()], value)) {
//            return returnDataChanged(index);
//        }
//        return __LINE__;
//    } else if (isPlayerStateItem(index)) {
//        if (value.type() == QVariant::Bool) {
//            auto& state = m_data->_players.at(index.parent().row())._state;
//            switch (index.row()) {
//            case 0: state._left_table = value.toBool(); break;
//            case 1: state._sit_out = value.toBool(); break;
//            case 2: state._frozen = value.toBool(); break;
//            case 3: state._anticipant = value.toBool(); break;
//            case 4: state._folded = value.toBool(); break;
//            case 5: state._all_ined = value.toBool(); break;
//            case 6: state._all_ined_in_cur_round = value.toBool(); break;
//            default:
//                return __LINE__;
//            }
//            return returnDataChanged(index);
//        }
//        return __LINE__;
//    }
//    return __LINE__;
//}
//return __LINE__;






