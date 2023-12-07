#include <precompiled.h>
#include "model_tournaments.h"
#include <api_models/itemtype_concept.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <qdatetime.h>
#include <PokerApplication.h>
#include <utils/IconProvider.h>

#ifndef Q_MOC_RUN
#include <api/api_common.h>
#include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

Q_DECLARE_METATYPE(games_stat_t::second_type::value_type)

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

    QColor titleColor(const gbp::ns_api::ns_statistic::tournament_stat_t &item)
    {
        if (item._types & gbp::ns_api::ns_tournament_types::e_win_result::wr_main) {
            return QColor(0x00aeef);
        } else if (item._types & gbp::ns_api::ns_tournament_types::e_win_result::wr_qualifier) {
            return QColor(0x00c853);
        } else if (item._types & gbp::ns_api::ns_tournament_types::e_money_in::mi_free_roll) {
            return QColor(0xd69e4e);
        } else {
            return QColor(0xfafafa);
        }
    }
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

Q_DECLARE_METATYPE(e_game_speed)

namespace ns_model
{

    tournaments::tournaments(QObject *parent)
        : QIdentityProxyModel(parent)
        , m_sourceModel(nullptr)
    {}

    int tournaments::columnCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return underlying_cast(column::count);
    }


    QVariant tournaments::getDataFromSource(tournament_stat_t *source, gbp_i64 tournId, column col, int role)
    {
        if (source == nullptr || source->item(tournId).ptr() == nullptr) {
            return QVariant();
        }
        if (role == Qt::CheckStateRole) {
            return QVariant();
        }
        const gbp::ns_api::ns_statistic::tournament_stat_t& item = source->item(tournId).ref();
        if (role == Qt::ForegroundRole) {
            if (col == column::title) {
                 return titleColor(item);
             } else if (col == column::status) {
                 if (item._is_current_user_registered) {
                     return QColor(Qt::red);
                 }
             }
             return QColor(0xfafafa);
        }
        bool sitNGoItem = item._types & e_start_time::st_sit_n_go;

        switch (col) {
        case column::start:
            if (sitNGoItem) {
                switch (role) {
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                    return tr("@SitNGo");
                default:
                    return QVariant();
                }
            }
            else
            {
                return source->indexOf(tournId, tournament_stat_t::column::_start_time).data(role);
            }
        case column::game:
            switch (role) {
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
                return QString("%0 %1").arg(source->indexOf(tournId, tournament_stat_t::column::_game_limit).data(role).toString())
                                       .arg(source->indexOf(tournId, tournament_stat_t::column::_game_type).data(role).toString());
            default:
                return QVariant();
            }
        case column::buyin:
            if (item._buyin == 0) {
                switch (role) {
                case Qt::DisplayRole:
                case Qt::ToolTipRole:
                    return tr("@freeroll");
                default:
                    return source->indexOf(tournId, tournament_stat_t::column::_buyin).data(role);
                }
            }
            else
            {
                return source->indexOf(tournId, tournament_stat_t::column::_buyin).data(role);
            }
        case column::type:
            return (role == Qt::DecorationRole) ? QVariant::fromValue(pApp->iconProvider().gameTypeIcon(item)) : QVariant();
        case column::title:
            return source->indexOf(tournId, tournament_stat_t::column::_title).data(role);
        case column::prize:
            return source->indexOf(tournId, tournament_stat_t::column::_common_prize).data(role);
        case column::status:
            return source->indexOf(tournId, tournament_stat_t::column::_status).data(role);
        case column::players:
            switch (role) {
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
                return QString("%0/%1").arg(source->indexOf(tournId, tournament_stat_t::column::_players_registered_count).data(role).toString())
                                       .arg(source->indexOf(tournId, tournament_stat_t::column::_max_players_count).data(role).toString());
            default:
                return QVariant();
            }
        default:
            return QVariant();
        }

//        _start_time;
//        _game_type,game_limit;
//        _buyin;
//        _types, _game_speed, _seats_count;
//        _title; foreground(_is_current_user_registered)
//        _common_prize;
//        _status;
        //        _players_registered_count, _max_players_count; _sit_n_go_users_count;
    }

//    QVariant tournaments::dataFilterRole(const QModelIndex &idx) const
//    {

//    }

    QVariant tournaments::dataSortRole(const QModelIndex& idx) const
    {
        if (!idx.isValid()) {
            return QVariant();
        }

        gbp_i64 id = idx.data(UserType::RowItemIdRole).value<gbp_i64>();

        switch (column(idx.column())) {
        case column::start:
        case column::buyin:
        case column::prize:
            return data(idx, UserType::OriginDataRole);
        case column::players:
        {
            int max_players  = m_sourceModel->indexOf(id, tournament_stat_t::column::_max_players_count).data(UserType::OriginDataRole).toInt();
            int fact_players = m_sourceModel->indexOf(id, tournament_stat_t::column::_players_registered_count).data(UserType::OriginDataRole).toInt();
            int sit_n_go_users_count = m_sourceModel->indexOf(id, tournament_stat_t::column::_sit_n_go_users_count).data(UserType::OriginDataRole).toInt();
            qDebug() << id << max_players << "/" << fact_players << sit_n_go_users_count;
            return (max_players != 0) ? (float(max_players) + float(fact_players)/max_players) : 0.f;
        }
        case column::type:
        {
            int seats_count = m_sourceModel->indexOf(id, tournament_stat_t::column::_seats_count).data(UserType::OriginDataRole).toInt();
            e_tournament_type type = m_sourceModel->indexOf(id, tournament_stat_t::column::_types).data(UserType::OriginDataRole).value<e_tournament_type>();
            e_game_speed speed = m_sourceModel->indexOf(id, tournament_stat_t::column::_game_speed).data(UserType::OriginDataRole).value<e_game_speed>();

            int sort_value = 1 << (seats_count + 3);
            if (type & e_chips_rebuy::cr_rebuy) {
                sort_value += 1 << 1;
            }
            if (type & e_chips_rebuy::cr_bounty) {
                sort_value += 1 << 2;
            }
            if (speed != e_game_speed::normal) {
                sort_value += 1 << 3;
            }
            return sort_value;
        }
        case column::game:
        case column::title:
        case column::status:
        default:
            return data(idx, Qt::DisplayRole);
        }
    }

    QVariant tournaments::data(const QModelIndex &index, int role) const
    {
        if (m_sourceModel == nullptr || !index.isValid() || index.parent().isValid()) {
            return QVariant();
        }

        switch (role) {
        case Qt::TextAlignmentRole: return Qt::AlignCenter;
        case UserType::SortRole:    return dataSortRole(index);
//        case UserType::FilterRole:  return dataFilterRole(index);
        default:
            if (gbp::ns_api::ns_statistic::tournament_stat_t* itemPtr = m_sourceModel->itemAt(index.row()).ptr())
            {
                switch (role) {
                case UserType::RowDataRole:
                    return QVariant::fromValue(*itemPtr);
                case UserType::RowItemIdRole:
                    return QVariant::fromValue(itemPtr->_id);
                case UserType::FilterRole:
                    return UserType::convert(filterValue(*itemPtr));
                default:
                    return getDataFromSource(m_sourceModel, itemPtr->_id, column(index.column()), role);
                }
            }
        }

        return QVariant();
    }

    void tournaments::setSourceModel(QAbstractItemModel *sourceModel)
    {
        if (m_sourceModel != sourceModel) {
            beginResetModel();
            m_sourceModel = qobject_cast<ns_model::tournament_stat_t*>(sourceModel);
            QIdentityProxyModel::setSourceModel(m_sourceModel);
            endResetModel();
        }
    }

    UserType::FilterFlags tournaments::filterValue(const data_t &item)
    {
        UserType::FilterFlags currValue = UserType::EFilters::AllUnchecked;
        switch (item._game_type) {
        case e_game_type::texas_holdem: currValue |= UserType::EFilters::GameType_Holdem; break;
        case e_game_type::omaha:
        case e_game_type::omaha_hi_low: currValue |= UserType::EFilters::GameType_Omaha; break;
        default:
            break;
        }
        switch (item._game_limit) {
        case e_game_limit::fixed_limit: currValue |= UserType::EFilters::Limit_FixedLimit; break;
        case e_game_limit::pot_limit:   currValue |= UserType::EFilters::Limit_PotLimit; break;
        case e_game_limit::no_limit:    currValue |= UserType::EFilters::Limit_NoLimit; break;
        }
        if (item._types & e_money_in::mi_free_roll) {
            currValue |= UserType::EFilters::BuyIn_FreeRoll;
        } else if (item._buyin < 50) {
            currValue |= UserType::EFilters::BuyIn_Micro;
        } else if (item._buyin < 200) {
            currValue |= UserType::EFilters::BuyIn_Low;
        } else if (item._buyin < 5000) {
            currValue |= UserType::EFilters::BuyIn_Medium;
        } else {
            currValue |= UserType::EFilters::BuyIn_High;
        }
        currValue |= (item._game_speed == gbp::ns_api::ns_game::e_game_speed::turbo ? UserType::EFilters::Speed_Turbo : UserType::EFilters::Speed_Normal);

        currValue |= (item._types & e_chips_rebuy::cr_normal ? UserType::EFilters::Type_Regular : UserType::EFilters::AllUnchecked);
        currValue |= (item._types & e_chips_rebuy::cr_rebuy ? UserType::EFilters::Type_Rebuy : UserType::EFilters::AllUnchecked);
        currValue |= (item._types & e_chips_rebuy::cr_bounty ? UserType::EFilters::Type_Knockout : UserType::EFilters::AllUnchecked);
        currValue |= (item._types & e_win_result::wr_qualifier ? UserType::EFilters::Type_Qualifier : UserType::EFilters::AllUnchecked);
        // TODO: исправить проверку, проверять приватный турнир
        currValue |= (item._types & e_win_result::wr_main ? UserType::EFilters::Type_Private : UserType::EFilters::AllUnchecked);

        if (item._types & e_start_time::st_normal)
        {
            currValue |= UserType::EFilters::Page_Tourn;
            switch (item._status) {
            case e_tournament_status::wait:              currValue |= UserType::EFilters::Status_Announced; break;
            case e_tournament_status::registering:       currValue |= UserType::EFilters::Status_Registration; break;
            case e_tournament_status::late_registration: currValue |= UserType::EFilters::Status_LateRegistration; break;
            case e_tournament_status::started:           currValue |= UserType::EFilters::Status_Started; break;
            case e_tournament_status::finished:          currValue |= UserType::EFilters::Status_Finished; break;
            default:
                break;
            }
        }
        else /*if (item._types & e_start_time::st_sit_n_go)*/
        {
            currValue |= UserType::EFilters::Page_SitNGo;
            if (item._sit_n_go_users_count == 2) {
                currValue |= UserType::EFilters::SNGSize_Pvp;
            } else if (item._sit_n_go_users_count == item._seats_count) {
                currValue |= UserType::EFilters::SNGSize_Single;
            } else {
                currValue |= UserType::EFilters::SNGSize_Multiple;
            }
        }

        if (!item._is_current_user_registered) {
            currValue |= UserType::EFilters::User_DoesNotParticipate;
        }

        return currValue;
    }

    QModelIndex tournaments::indexOf(gbp_i64 id) const
    {
        if (m_sourceModel == nullptr) {
            return QModelIndex();
        }
        return mapFromSource(m_sourceModel->indexOf(id));
    }

    QModelIndex tournaments::indexOf(gbp_i64 id, tournaments::column field) const {
        QModelIndex idx = indexOf(id);
        return idx.sibling(idx.row(), underlying_cast(field));
    }

    int tournaments::rowCount(const QModelIndex &parent) const
    {
        if (sourceModel() == nullptr || parent.isValid()) {
            return 0;
        }
        return m_sourceModel->rowCount();
    }

    bool tournaments::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (m_sourceModel == nullptr || !index.isValid() || index.parent().isValid()) {
            return false;
        }
        switch (role) {
        case UserType::OriginDataRole:
            return m_sourceModel->setData(mapToSource(index), value, role);
        default:
            return false;
        }
        return false;
    }

    QVariant tournaments::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            switch (section) {
            case 0: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_begins"));
            case 1: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_game"));
            case 2: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_buyin"));
            case 3: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_type"));
            case 4: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_tournament"));
            case 5: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_fond"));
            case 6: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_status"));
            case 7: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_players"));
            default:
                return QString();
            }
        case Qt::ForegroundRole:
            return QColor(0xb0bec5);
        default:
            return QVariant();
        }
    }



    QMap<int, QVariant> tournaments::itemData(const QModelIndex &index) const
    {
        return QIdentityProxyModel::itemData(index);
    }

    QSet<int> tournaments::itemRoles(const QModelIndex &index) const
    {
        if (!index.isValid())
        {
            return QSet<int>();
        }
        static QVector<QSet<int>> stableRoles;

        if (stableRoles.isEmpty())
        {
            for (QSet<int>& roles: stableRoles) {
                roles << UserType::RowDataRole << UserType::RowItemIdRole << UserType::SortRole << UserType::FilterRole << UserType::OriginDataRole;
            }

            stableRoles[underlying_cast(column::start)] << Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
            stableRoles[underlying_cast(column::game)]  << Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
            stableRoles[underlying_cast(column::buyin)] << Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
            stableRoles[underlying_cast(column::type)] << Qt::DecorationRole;
            stableRoles[underlying_cast(column::title)] << Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
            stableRoles[underlying_cast(column::status)] << Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
            stableRoles[underlying_cast(column::prize)] << Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
            stableRoles[underlying_cast(column::players)]<< Qt::DisplayRole << Qt::ToolTipRole << Qt::EditRole;
        }
    //    roles << UserType::RowDataRole << UserType::RowItemIdRole << UserType::SortRole << UserType::FilterRole << UserType::OriginDataRole;

    //    switch (column(index.column())) {
    //    case column::start:
    //    case column::game:
    //    case column::buyin:
    //    case column::type:
    //    case column::title:
    //    case column::status:
    //    case column::prize:
    //    case column::players:
    //        return roles;
    //    }
        return stableRoles.at(index.column());
    }


    Qt::ItemFlags tournaments::flags(const QModelIndex &index) const {
        if (index.isValid() && !index.parent().isValid()) {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
        }
        return Qt::NoItemFlags;
    }
} // namespace ns_model


QModelIndex ns_model::tournaments::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid() || proxyIndex.model() != this || sourceModel() == nullptr || proxyIndex.parent().isValid()
     ||  proxyIndex.column() >= columnCount() || proxyIndex.row() >= rowCount()) {
        return QModelIndex();
    }

    if (tournament_stat_t* m = qobject_cast<tournament_stat_t*>(sourceModel()))
    {
        if (!proxyIndex.parent().isValid()) {
            int row = proxyIndex.row();
            bool isSitnGo = m->itemAt(row).ref()._types & e_start_time::st_sit_n_go;
            switch (column(proxyIndex.column())) {
            case column::start:     return (isSitnGo) ? QModelIndex() : m->index(row, underlying_cast(tournament_stat_t::column::_start_time));
            case column::game:      return m->index(row, underlying_cast(tournament_stat_t::column::_types));
            case column::buyin:     return m->index(row, underlying_cast(tournament_stat_t::column::_buyin));
            case column::type:      return m->index(row, underlying_cast(tournament_stat_t::column::_seats_count));
            case column::title:     return m->index(row, underlying_cast(tournament_stat_t::column::_title));
            case column::prize:     return m->index(row, underlying_cast(tournament_stat_t::column::_common_prize));
            case column::status:    return m->index(row, underlying_cast(tournament_stat_t::column::_status));
            case column::players:   return m->index(row, underlying_cast(tournament_stat_t::column::_users));
            default:
                return QModelIndex();
            }
        }
    }
    return QIdentityProxyModel::mapToSource(proxyIndex);
}

QModelIndex ns_model::tournaments::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid() || sourceIndex.model() != sourceModel() || sourceIndex.parent().isValid()) {
        return QModelIndex();
    }
    if (tournament_stat_t* m = qobject_cast<tournament_stat_t*>(sourceModel()))
    {
        int row = sourceIndex.row();
        bool isSitnGo = m->itemAt(row).ref()._types & e_start_time::st_sit_n_go;
        switch (tournament_stat_t::column(sourceIndex.column())) {
        case tournament_stat_t::column::_id:                         return QModelIndex();
        case tournament_stat_t::column::_seats_count:                return index(row, underlying_cast(column::game));
        case tournament_stat_t::column::_max_players_count:          return index(row, underlying_cast(column::players));
        case tournament_stat_t::column::_is_current_user_registered: return index(row, underlying_cast(column::status));
        case tournament_stat_t::column::_players_registered_count:   return index(row, underlying_cast(column::players));
        case tournament_stat_t::column::_sit_n_go_users_count:       return (isSitnGo) ? QModelIndex() : index(row, underlying_cast(column::players));
        case tournament_stat_t::column::_main_tournament_id:         return index(row, underlying_cast(column::type));
        case tournament_stat_t::column::_buyin:                      return index(row, underlying_cast(column::buyin));
        case tournament_stat_t::column::_big_blind:                  return QModelIndex();
        case tournament_stat_t::column::_round_duration:             return QModelIndex();
        case tournament_stat_t::column::_start_stack:                return QModelIndex();
        case tournament_stat_t::column::_prize_places_count:         return QModelIndex();
        case tournament_stat_t::column::_common_prize:               return QModelIndex();
        case tournament_stat_t::column::_types:                      return index(row, underlying_cast(column::type));
        case tournament_stat_t::column::_game_type:                  return index(row, underlying_cast(column::game));
        case tournament_stat_t::column::_game_speed:                 return index(row, underlying_cast(column::type));
        case tournament_stat_t::column::_game_limit:                 return index(row, underlying_cast(column::game));
        case tournament_stat_t::column::_title:                      return index(row, underlying_cast(column::title));
        case tournament_stat_t::column::_status:                     return index(row, underlying_cast(column::status));
        case tournament_stat_t::column::_registration_start_time:    return QModelIndex();
        case tournament_stat_t::column::_start_time:                 return (isSitnGo) ? QModelIndex() : index(row, underlying_cast(column::start));
        case tournament_stat_t::column::_finish_time:                return QModelIndex();
        case tournament_stat_t::column::_button_state:               return QModelIndex();
        case tournament_stat_t::column::_late_registration_duration: return QModelIndex();
        case tournament_stat_t::column::_qualifiers_ids:             return QModelIndex();
        case tournament_stat_t::column::_rebuys:                     return QModelIndex();
        case tournament_stat_t::column::_addon:                      return QModelIndex();
        case tournament_stat_t::column::_bounty:                     return QModelIndex();
        case tournament_stat_t::column::_player_bounty_value:        return QModelIndex();
        case tournament_stat_t::column::_current_level:              return QModelIndex();
        case tournament_stat_t::column::_users:                      return index(row, underlying_cast(column::players));
        default:
            return QModelIndex();
        }
    }
    return QIdentityProxyModel::mapFromSource(sourceIndex);
}


QModelIndex ns_model::tournaments::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid()) {
        return QModelIndex();
    }
    return createIndex(row, column);
}

QModelIndex ns_model::tournaments::parent(const QModelIndex &/*child*/) const {
    return QModelIndex();
}

bool ns_model::tournaments::hasChildren(const QModelIndex &parent) const {
    return !parent.isValid();
}
