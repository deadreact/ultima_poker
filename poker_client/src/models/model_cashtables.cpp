#include <precompiled.h>
#include "model_cashtables.h"
#include <api_models/itemtype_concept.hpp>
#include <api_models/model_cash_stat_t.hpp>
#include <qdatetime.h>
#include <PokerApplication.h>
#include <utils/IconProvider.h>
#include <api_models/defs_metatype.hpp>

#ifndef Q_MOC_RUN
#include <api/api_common.h>
#include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

Q_DECLARE_METATYPE(games_stat_t::first_type::value_type)

namespace ns_meta
{
    struct blinds_pair : money_type
    {
        using display_type  = QString;

        static display_type display(const value_type& value) {
            return money_type::display(value/2) + "/" + money_type::display(value);
        }
    };

    struct game_desc_type : generic_pair_value<generic_enum_type<gbp::ns_api::ns_game::e_game_limit>, generic_enum_type<gbp::ns_api::ns_game::e_game_type>>
    {
        static display_type display(const value_type &value) {
            return tr_enum_type::display(value.first) + " " + tr_enum_type::display(value.second);
        }
    };

} //namespace ns_meta

Q_DECLARE_METATYPE(ns_meta::game_desc_type::value_type)
Q_DECLARE_METATYPE(ns_meta::game_desc_type::edit_type)
Q_DECLARE_METATYPE(ns_meta::game_desc_type::display_type)

namespace
{
    QString capitalized(const QString &str)
    {
        QString lowerStr = str.toLower();
        lowerStr[0] = lowerStr[0].toTitleCase();
        return lowerStr;
    }

//    QColor titleColor(const gbp::ns_api::ns_statistic::cash_stat_t &item)
//    {
//    //    if (item._types & gbp::ns_api::ns_tournament_types::e_win_result::wr_main) {
//    //        return QColor(0x00aeef);
//    //    } else if (item._types & gbp::ns_api::ns_tournament_types::e_win_result::wr_qualifier) {
//    //        return QColor(0x00c853);
//    //    } else if (item._types & gbp::ns_api::ns_tournament_types::e_money_in::mi_free_roll) {
//    //        return QColor(0xd69e4e);
//    //    } else {
//    //        return QColor(0xfafafa);
//    //    }
//    }
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
    using e_tournament_type   = gbp::ns_api::ns_tournament_types::e_tournament_type;
    using e_start_time        = gbp::ns_api::ns_tournament_types::e_start_time;
    using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_win_prize         = gbp::ns_api::ns_tournament_types::e_win_prize;
    using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_registration      = gbp::ns_api::ns_tournament_types::e_registration;
    using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
    using e_game_type         = gbp::ns_api::ns_game::e_game_type;
    using e_game_limit        = gbp::ns_api::ns_game::e_game_limit;
    using e_game_speed        = gbp::ns_api::ns_game::e_game_speed;

    bool isRebuy(const games_stat_t::second_type::value_type& item) {
        return item._types & e_chips_rebuy::cr_rebuy;
    }
    bool isKnockout(const games_stat_t::second_type::value_type& item) {
        return item._types & e_chips_rebuy::cr_bounty;
    }
    bool isProgressive(const games_stat_t::second_type::value_type& item) {
        return item._types & e_chips_rebuy::cr_progressive;
    }
    bool isMain(const games_stat_t::second_type::value_type& item) {
        return item._types & e_win_result::wr_main;
    }
    bool isQualifier(const games_stat_t::second_type::value_type& item) {
        return item._types & e_win_result::wr_qualifier;
    }
    bool isFreeroll(const games_stat_t::second_type::value_type& item) {
        return item._types & e_money_in::mi_free_roll;
    }
    bool isTurbo(const games_stat_t::second_type::value_type& item) {
        return item._game_speed == gbp::ns_api::ns_game::e_game_speed::turbo;
    }

    float itemWeight(const games_stat_t::second_type::value_type& item) {
        float weight = 0.f;
        if (isRebuy(item))          weight += 1.f/0x02;
        if (isKnockout(item))       weight += 1.f/0x04;
        if (isProgressive(item))    weight += 1.f/0x08;
        if (isMain(item))           weight += 1.f/0x10;
        if (isQualifier(item))      weight += 1.f/0x20;
        if (isFreeroll(item))       weight += 1.f/0x40;
        if (isTurbo(item))          weight += 1.f/0x80;

        return weight;
    }

// ------------------------------------------------------------------------------------
    int headerSizeHint(int section, int page) {
        static int page1[] = {150, 130, 130, 128, 78, 78, 78, 78, 79};
        static int page2[] = {100, 100, 100, 60, 140, 100, 100};
        if (page == 0) {
            if (section >= 0 && section <= 8) {
                return page1[section];
            }
        } else if (page == 1) {
            if (section >= 0 && section <= 7) {
                return page2[section];
            }
        }
        return 0;
    }


} //namespace

namespace ns_model
{

    cash_tables::cash_tables(QObject *parent)
        : QIdentityProxyModel(parent)
        , m_sourceModel(nullptr)
    {}

    int cash_tables::columnCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return underlying_cast(column::count);
    }


    QVariant cash_tables::getData(const games_stat_t::first_type::value_type &item, int col, int role) const
    {
//        auto datTuple = std::make_tuple(item._title
//                      , std::make_pair(item._big_blind/2, item._big_blind)
//                      , std::make_pair(item._limit, item._type)
//                      , item._players_count.second
//                      , item._players_count.first
//                      , item._wait_players
//                      , item._pot
//                      , item._flp
//                      , item._hands_per_hour);
//        std::cout << datTuple << std::endl;

        switch (role) {
        case UserType::RowDataRole:
            return QVariant::fromValue(item);
        case UserType::RowItemIdRole:
            return QVariant::fromValue(item._id);
        case UserType::OriginDataRole:
            switch (column(col)) {
            case column::_title:        return QVariant::fromValue(item._title);
            case column::_bets:         return QVariant::fromValue(std::make_pair(item._big_blind/2, item._big_blind));
            case column::_game:         return QVariant::fromValue(std::make_pair(item._limit, item._type));
            case column::_type:         return QVariant::fromValue(item._players_count.second);
            case column::_players:      return QVariant::fromValue(item._players_count);
            case column::_waiters:      return QVariant::fromValue(item._wait_players);
            case column::_pot:          return QVariant::fromValue(item._pot);
            case column::_flop:         return QVariant::fromValue(item._flp);
            case column::_distribution: return QVariant::fromValue(item._hands_per_hour);
            default:
                return QVariant();
            }
        case Qt::TextAlignmentRole:
            return (col == 0) ? Qt::AlignLeft : Qt::AlignCenter;
        case Qt::DisplayRole:
            switch (column(col)) {
            case  column::_title:        return ns_meta::name_type::display(item._title);
            case  column::_bets:         return ns_meta::blinds_pair::display(item._big_blind);
            case  column::_game:         return ns_meta::game_desc_type::display(std::make_pair(item._limit, item._type));
            case  column::_type:         return QVariant();
            case  column::_players:      return ns_meta::count_type::display(item._players_count.first);
            case  column::_waiters:      return ns_meta::count_type::display(item._wait_players);
            case  column::_pot:          return ns_meta::money_type::display(item._pot);
            case  column::_flop:         return ns_meta::persent_type::display(item._flp);
            case  column::_distribution: return ns_meta::count_type::display(item._hands_per_hour);
            default:
                return QVariant();
            }
        case Qt::ToolTipRole:
            return headerData(col, Qt::Horizontal, Qt::DisplayRole).toString() + ": " + getData(item, col, Qt::DisplayRole).toString();
        case Qt::DecorationRole:
            if (column(col) == column::_type) {
                return pApp->iconProvider().gameTypeIcon(item);
            }
            return QVariant();
        case Qt::EditRole:
            switch (column(col)) {
            case column::_title:        return ns_meta::name_type::edit(item._title);
            case column::_bets:         return ns_meta::blinds_pair::edit(item._big_blind);
            case column::_game:         return QVariant::fromValue(ns_meta::game_desc_type::edit(std::make_pair(item._limit, item._type)));
            case column::_type:         return getData(item, col, Qt::DecorationRole);
            case column::_players:      return ns_meta::count_type::edit(item._players_count.first);
            case column::_waiters:      return ns_meta::count_type::edit(item._wait_players);
            case column::_pot:          return ns_meta::money_type::edit(item._pot);
            case column::_flop:         return ns_meta::persent_type::edit(item._flp);
            case column::_distribution: return ns_meta::count_type::edit(item._hands_per_hour);
            default:
                return QVariant();
            }
        case Qt::ForegroundRole:
            return QColor(0xfafafa);
        case UserType::SortRole:
            switch (column(col)) {
            case column::_title:        return QString(item._title.c_str());
            case column::_bets:         return QVariant::fromValue(item._big_blind);
            case column::_game:         return QString("%0 %1").arg(ns_util::toString(item._limit), ns_util::toString(item._type));
            case column::_type:         return item._players_count.second;
            case column::_players:      return QVariant::fromValue(item._players.size());
            case column::_waiters:      return QVariant::fromValue(item._wait_players);
            case column::_pot:          return QVariant::fromValue(item._pot);
            case column::_flop:         return item._flp;
            case column::_distribution: return QVariant::fromValue(item._hands_per_hour);
            default:
                return QVariant();
            }
        case UserType::FilterRole:
            return UserType::convert(filterValue(item));
        default:
            return QVariant();
        }
    }

    QVariant cash_tables::data(const QModelIndex &index, int role) const
    {
        if (m_sourceModel == nullptr || !index.isValid() || index.parent().isValid()) {
            return QVariant();
        }
        QVariant rowData = QIdentityProxyModel::data(index, UserType::RowDataRole);
        if (role == UserType::RowDataRole) {
            return rowData;
        }
        if (!rowData.canConvert<gbp::ns_api::ns_statistic::cash_stat_t>()) {
            return QVariant();
        }
        gbp::ns_api::ns_statistic::cash_stat_t data = rowData.value<gbp::ns_api::ns_statistic::cash_stat_t>();
        return getData(data, index.column(), role);

    }

    void cash_tables::setSourceModel(QAbstractItemModel *sourceModel)
    {
        if (m_sourceModel != sourceModel) {
            beginResetModel();
            m_sourceModel = qobject_cast<ns_model::cash_stat_t*>(sourceModel);
            QIdentityProxyModel::setSourceModel(m_sourceModel);
            endResetModel();
        }
    }

    UserType::FilterFlags cash_tables::filterValue(const games_stat_t::first_type::value_type &item)
    {
        UserType::FilterFlags currValue = UserType::EFilters::AllUnchecked;

        switch (item._type) {
        case e_game_type::texas_holdem: currValue |= UserType::EFilters::GameType_Holdem; break;
        case e_game_type::omaha:
        case e_game_type::omaha_hi_low: currValue |= UserType::EFilters::GameType_Omaha; break;
        default:
            break;
        }
        switch (item._limit) {
        case e_game_limit::fixed_limit: currValue |= UserType::EFilters::Limit_FixedLimit; break;
        case e_game_limit::pot_limit:   currValue |= UserType::EFilters::Limit_PotLimit; break;
        case e_game_limit::no_limit:    currValue |= UserType::EFilters::Limit_NoLimit; break;
        }
        if (item._is_freeroll) {
            currValue |= UserType::EFilters::BuyIn_FreeRoll;
        } else if (item._big_blind < 10) {
            currValue |= UserType::EFilters::BuyIn_Micro;
        } else if (item._big_blind < 50) {
            currValue |= UserType::EFilters::BuyIn_Low;
        } else if (item._big_blind < 200) {
            currValue |= UserType::EFilters::BuyIn_Medium;
        } else {
            currValue |= UserType::EFilters::BuyIn_High;
        }
        switch (item._players_count.second) {
        case 2: currValue |= UserType::EFilters::SeatsCount_2; break;
        case 6: currValue |= UserType::EFilters::SeatsCount_6; break;
        case 9: currValue |= UserType::EFilters::SeatsCount_9; break;
        default:
            break;
        }
        if (item._players_count.first == 0) {
            currValue |= UserType::EFilters::FreeSeats_Empty;
        } else if (item._players_count.first == item._players_count.second) {
            currValue |= UserType::EFilters::FreeSeats_Full;
        }
        return currValue;
    }

    QModelIndex cash_tables::indexOf(gbp_i64 id) const
    {
        if (m_sourceModel == nullptr) {
            return QModelIndex();
        }
        return mapFromSource(m_sourceModel->indexOf(id));
    }

    QModelIndex cash_tables::indexOf(gbp_i64 id, cash_tables::column field) const {
        QModelIndex idx = indexOf(id);
        return idx.sibling(idx.row(), underlying_cast(field));
    }

    int cash_tables::rowCount(const QModelIndex &parent) const
    {
        return m_sourceModel == nullptr ? 0 : m_sourceModel->rowCount(mapToSource(parent));
    }

    bool cash_tables::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (m_sourceModel == nullptr) {
            return false;
        }
        gbp_i64 id = index.data(UserType::RowItemIdRole).value<gbp_i64>();
//        QModelIndex sourceIndex = mapToSource(index);
        switch (column(index.column())) {
        case column::_title:        return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_title), value, role);
        case column::_bets:         return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_big_blind), value, role);
        case column::_game:
        {
            if (role == Qt::EditRole)
            {
                using T = typename ns_meta::game_desc_type::edit_type;
                if (value.canConvert<T>())
                {
                    T val = value.value<T>();
                    bool hasChanged = m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_limit), val.first, role);
                    hasChanged |= m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_type), val.second, role);
                    return hasChanged;
                }
                return false;
            }
            return false;
        }
        case column::_type:         return false;
        case column::_players:
        {
            auto val = m_sourceModel->item(id).ref()._players_count;
            val.first = value.value<gbp_u8>();
            return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_players_count), QVariant::fromValue(val), role);
        }
        case column::_waiters:      return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_wait_players), value, role);
        case column::_pot:          return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_pot), value, role);
        case column::_flop:         return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_flp), value, role);
        case column::_distribution: return m_sourceModel->setData(m_sourceModel->indexOf(id, cash_stat_t::column::_hands_per_hour), value, role);
        default:
            return false;
        }
    }

    QVariant cash_tables::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            switch (section) {
            case 0: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_table"));
            case 1: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_bets"));
            case 2: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_game"));
            case 3: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_type"));
            case 4: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_players"));
            case 5: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_wait"));
            case 6: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_bank"));
            case 7: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_flop"));
            case 8: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_deal"));
            default:
                return QString();
            }
        case Qt::ForegroundRole:
            return QColor(0xb0bec5);
        default:
            return QVariant();
        }
    }
    QModelIndex cash_tables::parent(const QModelIndex &/*child*/) const {
        return QModelIndex();
    }

    bool cash_tables::hasChildren(const QModelIndex &parent) const {
        return !parent.isValid();
    }
} // namespace ns_model



ns_model::cash_players::cash_players(QObject *parent)
    : QIdentityProxyModel(parent)
//    , m_currentId(-1)
{}

//void ns_model::cash_players::setCurrentId(gbp_i64 id) {
//    if (id != m_currentId) {
//        beginResetModel();
//        m_currentId = id;
//        qDebug() << "curr id" << id;
//        endResetModel();
//    }
//}

//gbp_i64 ns_model::cash_players::currentId() const {
//    return m_currentId;
//}

int ns_model::cash_players::rowCount(const QModelIndex &parent) const {
//    if (parent.isValid()) {
//        return 0;
//    }
    return 9;
}

int ns_model::cash_players::columnCount(const QModelIndex &parent) const
{
//    if (parent.isValid()) {
//        return 0;
//    }
    return 3;
}

QVariant ns_model::cash_players::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    switch (role) {
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (section) {
        case 0: return tr("@h_player");
//        case 1: return "<html><head/><body><p><img src=\":/textures/Common/icons/icon_geo.png\"/></p></body></html>";//tr("@h_country");
        case 2: return tr("@h_stack");
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        if (section == 1) {
            return QPixmap(":/textures/Common/icons/icon_geo.png");
        }
        return QVariant();
    case Qt::SizeHintRole:
        if (section >= 0 && section <= 2) {
            static const int w[] = {176, 24, 100};
            return QSize(w[section], 24);
        }
        return QVariant();
    default:
        return QVariant();
    }
}

QModelIndex ns_model::cash_players::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex idx = QIdentityProxyModel::index(row, column, parent);
    if (!parent.isValid()) {
        if (hasChildren(idx)) {
            return idx;
        }
        return QModelIndex();
    }
    if (QIdentityProxyModel::rowCount(parent) > row) {
        return QIdentityProxyModel::index(row, (column == 1 ? 3 : column), parent);
    }
    return QModelIndex();
}

QVariant ns_model::cash_players::data(const QModelIndex &proxyIndex, int role) const {
    if (proxyIndex.parent().isValid() && proxyIndex.column() == 3) {
        if (role == Qt::DisplayRole) {
            return QVariant();
        } else if (role == Qt::DecorationRole) {
            return pApp->iconProvider().countryIcon(QLocale::Country(QIdentityProxyModel::data(proxyIndex, UserType::OriginDataRole).value<quint16>()));
        }
    }
    return QIdentityProxyModel::data(proxyIndex, role);
}




QModelIndex ns_model::cash_tables::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || row >= rowCount() || column >= columnCount()) {
        return QModelIndex();
    }
    return createIndex(row, column);
}


QModelIndex ns_model::cash_tables::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid() || proxyIndex.model() != this || proxyIndex.column() >= columnCount(proxyIndex.parent()) || proxyIndex.row() >= rowCount(proxyIndex.parent())) {
        return QModelIndex();
    }
    if (ns_model::cash_stat_t* m = qobject_cast<ns_model::cash_stat_t*>(sourceModel()))
    {
        if (!proxyIndex.parent().isValid()) {
            switch (column(proxyIndex.column())) {
            case column::_title:        return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_title));
            case column::_bets:         return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_big_blind));
            case column::_game:         return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_type));
            case column::_type:         return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_players_count));
            case column::_players:      return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_players_count));
            case column::_waiters:      return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_wait_players));
            case column::_pot:          return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_pot));
            case column::_flop:         return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_flp));
            case column::_distribution: return m->index(proxyIndex.row(), underlying_cast(cash_stat_t::column::_hands_per_hour));
            default:
                return QModelIndex();
            }
        }
    }
    return QIdentityProxyModel::mapToSource(proxyIndex);
}

QModelIndex ns_model::cash_tables::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid() || sourceIndex.model() != sourceModel() || sourceIndex.parent().isValid()) {
        return QModelIndex();
    }
    if (ns_model::cash_stat_t* m = qobject_cast<ns_model::cash_stat_t*>(sourceModel()))
    {
        int row = sourceIndex.row();
        switch (cash_stat_t::column(sourceIndex.column())) {
        case cash_stat_t::column::_id:             return QModelIndex();
        case cash_stat_t::column::_title:          return index(row, underlying_cast(column::_title));
        case cash_stat_t::column::_is_freeroll:
        case cash_stat_t::column::_big_blind:      return index(row, column::_bets);
        case cash_stat_t::column::_type:
        case cash_stat_t::column::_limit:          return index(row, column::_game);
        case cash_stat_t::column::_players_count:  return index(row, column::_type);
        case cash_stat_t::column::_pot:            return index(row, column::_pot);
        case cash_stat_t::column::_flp:            return index(row, column::_flop);
        case cash_stat_t::column::_hands_per_hour: return index(row, column::_distribution);
        case cash_stat_t::column::_wait_players:   return index(row, column::_waiters);
        case cash_stat_t::column::_players:
        case cash_stat_t::column::_is_flash:
        case cash_stat_t::column::_flash_id:
        default:
            return QModelIndex();
        }
    }
    return QIdentityProxyModel::mapFromSource(sourceIndex);
}


Qt::ItemFlags ns_model::cash_tables::flags(const QModelIndex &index) const
{
    return QIdentityProxyModel::flags(index) | Qt::ItemNeverHasChildren;
}
