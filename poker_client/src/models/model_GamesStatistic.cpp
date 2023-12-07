#include <precompiled.h>
// #include <qvector.h>
#include "model_GamesStatistic.h"
// #include "utils/logger.h"
// #include <PokerApplication.h>
// #include <QMetaMethod>
// #include <QTime>
// #include <core/money.h>
// #include <utils/IconProvider.h>
// #include <api_models/api_util.hpp>

// #include <api_models/model_cash_stat_t.hpp>
// #include <api_models/model_tournament_stat_t.hpp>
// #include <api_models/itemtype_concept.hpp>

// QString capitalized(const QString &str)
// {
//     QString lowerStr = str.toLower();
//     lowerStr[0] = lowerStr[0].toTitleCase();
//     return lowerStr;
// }

// QColor titleColor(const games_stat_t::second_type::value_type &item)
// {
//     if (item._types & gbp::ns_api::ns_tournament_types::e_win_result::wr_main) {
//         return QColor(0x00aeef);
//     } else if (item._types & gbp::ns_api::ns_tournament_types::e_win_result::wr_qualifier) {
//         return QColor(0x00c853);
//     } else if (item._types & gbp::ns_api::ns_tournament_types::e_money_in::mi_free_roll) {
//         return QColor(0xd69e4e);
//     } else {
//         return QColor(0xfafafa);
//     }
// }


// namespace
// {
//     using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;
//     using e_tournament_type   = gbp::ns_api::ns_tournament_types::e_tournament_type;
//     using e_start_time        = gbp::ns_api::ns_tournament_types::e_start_time;
//     using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
//     using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
//     using e_win_prize         = gbp::ns_api::ns_tournament_types::e_win_prize;
//     using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
//     using e_registration      = gbp::ns_api::ns_tournament_types::e_registration;
//     using e_tourn_button_type = gbp::ns_api::ns_statistic::e_tourn_button_type;
//     using e_game_type         = gbp::ns_api::ns_game::e_game_type;
//     using e_game_limit        = gbp::ns_api::ns_game::e_game_limit;
//     using e_game_speed        = gbp::ns_api::ns_game::e_game_speed;

//     bool isRebuy(const games_stat_t::second_type::value_type& item) {
//         return item._types & e_chips_rebuy::cr_rebuy;
//     }
//     bool isKnockout(const games_stat_t::second_type::value_type& item) {
//         return item._types & e_chips_rebuy::cr_bounty;
//     }
//     bool isProgressive(const games_stat_t::second_type::value_type& item) {
//         return item._types & e_chips_rebuy::cr_progressive;
//     }
//     bool isMain(const games_stat_t::second_type::value_type& item) {
//         return item._types & e_win_result::wr_main;
//     }
//     bool isQualifier(const games_stat_t::second_type::value_type& item) {
//         return item._types & e_win_result::wr_qualifier;
//     }
//     bool isFreeroll(const games_stat_t::second_type::value_type& item) {
//         return item._types & e_money_in::mi_free_roll;
//     }
//     bool isTurbo(const games_stat_t::second_type::value_type& item) {
//         return item._game_speed == gbp::ns_api::ns_game::e_game_speed::turbo;
//     }

//     float itemWeight(const games_stat_t::second_type::value_type& item) {
//         float weight = 0.f;
//         if (isRebuy(item))          weight += 1.f/0x02;
//         if (isKnockout(item))       weight += 1.f/0x04;
//         if (isProgressive(item))    weight += 1.f/0x08;
//         if (isMain(item))           weight += 1.f/0x10;
//         if (isQualifier(item))      weight += 1.f/0x20;
//         if (isFreeroll(item))       weight += 1.f/0x40;
//         if (isTurbo(item))          weight += 1.f/0x80;

//         return weight;
//     }

// // ------------------------------------------------------------------------------------
//     int headerSizeHint(int section, int page) {
//         static int page1[] = {150, 130, 130, 128, 78, 78, 78, 78, 79};
//         static int page2[] = {100, 100, 100, 60, 140, 100, 100};
//         if (page == 0) {
//             if (section >= 0 && section <= 8) {
//                 return page1[section];
//             }
//         } else if (page == 1) {
//             if (section >= 0 && section <= 7) {
//                 return page2[section];
//             }
//         }
//         return 0;
//     }
// }
// //#define STR(__str__) #__str__

// ////    std::cout << msg << " id:" << (__item1._id) << " " << STR(__property) << " changed from: " << __item1.__property << " to: " << __item2.__property << std::endl;

// //#define PRINT_COMPARE(msg, __item1, __item2, __cnt, __column, __role, __property)\
// //if ((__item1.__property) != (__item2.__property)) {\
// //    __cnt[__column].push_back(__role);\
// //}

// //    QMap<int, QVector<int>> getDifference(const games_stat_t::first_type::value_type& item1, const games_stat_t::first_type::value_type& item2)
// //    {
// //        QMap<int, QVector<int>> diffs;

// //        PRINT_COMPARE("cash", item1, item2, diffs, 9, UserType::RowItemIdRole, _id)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 0, Qt::DisplayRole,         _title)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 1, Qt::DisplayRole,         _big_blind)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 2, Qt::DisplayRole,         _type)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 2, Qt::DisplayRole,         _limit)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 4, Qt::DisplayRole,         _players_count.first)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 3, Qt::DecorationRole,      _players_count.second)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 6, Qt::DisplayRole,         _pot)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 7, Qt::DisplayRole,         _flp)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 8, Qt::DisplayRole,         _hands_per_hour)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 5, Qt::DisplayRole,         _wait_players)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 3, Qt::DisplayRole,         _is_flash)
// //        PRINT_COMPARE("cash", item1, item2, diffs, 3, Qt::DisplayRole,         _flash_id)

// //        // if (item1._players.size() != item2._players.size()) { return true; }
// //        // for (int i = 0; i < item1._players.size(); i++) {
// //        //     if (item1._players.at(i) != item2._players.at(i)) { return true; }
// //        // }

// //        return diffs;
// //    }

// //    QMap<int, QVector<int>> getDifference(const games_stat_t::second_type::value_type& item1, const games_stat_t::second_type::value_type& item2)
// //    {
// //        QMap<int, QVector<int>> diffs;

// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowItemIdRole, _id)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 3, Qt::DecorationRole,      _seats_count)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 7, Qt::DisplayRole,         _max_players_count)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 6, Qt::ForegroundRole,      _is_current_user_registered)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 7, Qt::DisplayRole,         _players_registered_count)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 7, Qt::DisplayRole,         _sit_n_go_users_count)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 4, Qt::ForegroundRole,      _main_tournament_id)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 2, Qt::DisplayRole,         _buyin)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 8, Qt::DisplayRole,         _big_blind)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 8, Qt::DisplayRole,         _round_duration)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 8, Qt::DisplayRole,         _start_stack)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 8, Qt::DisplayRole,         _prize_places_count)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 5, Qt::DisplayRole,         _common_prize)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 3, Qt::DecorationRole,      _types)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 1, Qt::DisplayRole,         _game_type)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 3, Qt::DecorationRole,      _game_speed)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 1, Qt::DisplayRole,         _game_limit)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 4, Qt::DisplayRole,         _title)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 6, Qt::DisplayRole,         _status)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _registration_start_time)
// //        PRINT_COMPARE("tournament", item1, item2, diffs, 0, Qt::DisplayRole,         _start_time)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _finish_time)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _button_state)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _late_registration_duration)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _bounty)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _player_bounty_value)
// //        PRINT_COMPARE("tournament", item1, item2, diffs,-1, UserType::RowDataRole,   _current_level)

// //        return diffs;
// //    }

// //}



// //namespace ns_model
// //{
// //    GamesStatistic::GamesStatistic(EPage p, QObject *parent)
// //        : QAbstractProxyModel(parent)
// //        , m_cashes(new ns_model::cash_stat_t)
// //        , m_tourns(new ns_model::tournament_stat_t)
// //        , m_isNowResetting(false)
// //        , m_sharedDataId(0)
// //        , m_originData(nullptr)
// //        , m_page(p)
// //    {
// //        const reply_statistic::emitter_t* emitter_stat = pApp->client().emitter<reply_statistic>();
// //        connect(emitter_stat, &reply_statistic::emitter_t::games_statistic, this, &GamesStatistic::onDataUpdated);
// //    }

// //    GamesStatistic::~GamesStatistic()
// //    {

// //    }

// //    e_tourn_button_type GamesStatistic::buttonStateAtRow(int row) const
// //    {
// //        games_stat_t::second_type::value_type val = rowItem(row, Tournaments).value<games_stat_t::second_type::value_type>();
// //        return val._button_state;
// //    }

// //    int GamesStatistic::rowCount(const QModelIndex &parent) const
// //    {
// //        if (!parent.isValid()) {
// //            return rowCount(m_page);
// //        }
// //        return 0;
// //    }

// //    int GamesStatistic::columnCount(const QModelIndex &parent) const
// //    {
// //        if (!parent.isValid()) {
// //            return columnCount(m_page);
// //        }
// //        return 0;
// //    }

// //    int GamesStatistic::rowCount(int page) const
// //    {
// //        if (page == Unknown) {
// //            return rowCount(m_page);
// //        }
// //        if (!isReady()) {
// //            return 0;
// //        }
// //        return (page == CashTables) ? m_originData->first.size(): m_originData->second.size();
// //    }

// //    int GamesStatistic::columnCount(int page) const
// //    {
// //        if (page == Unknown) {
// //            return columnCount(m_page);
// //        }
// //        return (page == CashTables) ? 9 : 8;
// //    }

// //

// //    QVariant GamesStatistic::getData(const games_stat_t::second_type::value_type &item, int column, int role) const
// //    {
// //        bool sitNGoItem = item._types & e_start_time::st_sit_n_go;
// //        switch (role) {
// //        case UserType::RowDataRole:
// //            return QVariant::fromValue(item);
// //        case UserType::RowItemIdRole:
// //            return QVariant::fromValue(item._id);
// //        case Qt::DisplayRole:
// //        case Qt::ToolTipRole:
// //            switch (column) {
// //            case 0: return (sitNGoItem) ? "SitNGo" : ns_utildb::utcToLocalDateTime(QDateTime::fromTime_t(item._start_time)).toString("dd.MM.yy hh:mm");
// //            case 1: return QString("%0 %1").arg(ns_util::toString(item._game_limit), ns_util::toString(item._game_type));
// //            case 2: return (item._buyin != 0) ? Money(item._buyin).toString() : QObject::tr("@buyin_0");
// //            case 4: return QString::fromStdString(item._title);
// //            case 5: return Money(item._common_prize).toString();
// //            case 6: return ns_util::toString(item._status);
// //            case 7: return (sitNGoItem) ? QVariant(QString("%0/%1").arg(item._players_registered_count).arg(item._sit_n_go_users_count)) : QVariant(item._players_registered_count);
// //            case 3:
// //            default:
// //                return QVariant();
// //            }
// //        case Qt::DecorationRole:
// //            if (column == 2) {
// //            } else if (column == 3) {
// //                return pApp->iconProvider().gameTypeIcon(item);
// //            }
// //            return QVariant();
// //        case Qt::ForegroundRole:
// //            if (column == 4) {
// //                return titleColor(item);
// //            } else if (column == 6) {
// //                if (item._is_current_user_registered/*_button_state == e_tourn_button_type::unregister || item._button_state == e_tourn_button_type::back_to_game*/) {
// //                    return QColor(Qt::red);
// //                }
// //            }
// //            return QColor(0xfafafa);
// //        case UserType::SortRole:
// //            switch (column) {
// //            case 0: return QVariant::fromValue(item._start_time);
// //            case 1: return QString("%0 %1").arg(ns_util::toString(item._game_limit), ns_util::toString(item._game_type));
// //            case 2: return QVariant::fromValue(item._buyin);
// //            case 3: return item._seats_count + itemWeight(item);
// //            case 4: return QString::fromStdString(item._title);
// //            case 5: return QVariant::fromValue(item._common_prize);
// //            case 6: return underlying_cast(item._status);
// //            case 7: return (sitNGoItem) ? ((item._sit_n_go_users_count << 16) + item._players_registered_count) : item._players_registered_count;
// //            default:
// //                return QVariant();
// //            }
// //        case UserType::FilterRole:
// //            return UserType::convert(filterValue(item));
// //        default:
// //            return QVariant();
// //        }
// //    }



// //    UserType::FilterFlags GamesStatistic::filterValue(const games_stat_t::second_type::value_type &item)
// //    {
// //        UserType::FilterFlags currValue = UserType::EFilters::AllUnchecked;
// //        switch (item._game_type) {
// //        case e_game_type::texas_holdem: currValue |= UserType::EFilters::GameType_Holdem; break;
// //        case e_game_type::omaha:
// //        case e_game_type::omaha_hi_low: currValue |= UserType::EFilters::GameType_Omaha; break;
// //        default:
// //            break;
// //        }
// //        switch (item._game_limit) {
// //        case e_game_limit::fixed_limit: currValue |= UserType::EFilters::Limit_FixedLimit; break;
// //        case e_game_limit::pot_limit:   currValue |= UserType::EFilters::Limit_PotLimit; break;
// //        case e_game_limit::no_limit:    currValue |= UserType::EFilters::Limit_NoLimit; break;
// //        }
// //        if (item._types & e_money_in::mi_free_roll) {
// //            currValue |= UserType::EFilters::BuyIn_FreeRoll;
// //        } else if (item._buyin < 50) {
// //            currValue |= UserType::EFilters::BuyIn_Micro;
// //        } else if (item._buyin < 200) {
// //            currValue |= UserType::EFilters::BuyIn_Low;
// //        } else if (item._buyin < 5000) {
// //            currValue |= UserType::EFilters::BuyIn_Medium;
// //        } else {
// //            currValue |= UserType::EFilters::BuyIn_High;
// //        }
// //        currValue |= (item._game_speed == gbp::ns_api::ns_game::e_game_speed::turbo ? UserType::EFilters::Speed_Turbo : UserType::EFilters::Speed_Normal);

// //        currValue |= (item._types & e_chips_rebuy::cr_normal ? UserType::EFilters::Type_Regular : UserType::EFilters::AllUnchecked);
// //        currValue |= (item._types & e_chips_rebuy::cr_rebuy ? UserType::EFilters::Type_Rebuy : UserType::EFilters::AllUnchecked);
// //        currValue |= (item._types & e_chips_rebuy::cr_bounty ? UserType::EFilters::Type_Knockout : UserType::EFilters::AllUnchecked);
// //        currValue |= (item._types & e_win_result::wr_qualifier ? UserType::EFilters::Type_Qualifier : UserType::EFilters::AllUnchecked);
// //        // TODO: исправить проверку, проверять приватный турнир
// //        currValue |= (item._types & e_win_result::wr_main ? UserType::EFilters::Type_Private : UserType::EFilters::AllUnchecked);

// //        if (item._types & e_start_time::st_normal)
// //        {
// //            currValue |= UserType::EFilters::Page_Tourn;
// //            switch (item._status) {
// //            case e_tournament_status::wait:              currValue |= UserType::EFilters::Status_Announced; break;
// //            case e_tournament_status::registering:       currValue |= UserType::EFilters::Status_Registration; break;
// //            case e_tournament_status::late_registration: currValue |= UserType::EFilters::Status_LateRegistration; break;
// //            case e_tournament_status::started:           currValue |= UserType::EFilters::Status_Started; break;
// //            case e_tournament_status::finished:          currValue |= UserType::EFilters::Status_Finished; break;
// //            default:
// //                break;
// //            }
// //        }
// //        else /*if (item._types & e_start_time::st_sit_n_go)*/
// //        {
// //            currValue |= UserType::EFilters::Page_SitNGo;
// //            if (item._sit_n_go_users_count == 2) {
// //                currValue |= UserType::EFilters::SNGSize_Pvp;
// //            } else if (item._sit_n_go_users_count == item._seats_count) {
// //                currValue |= UserType::EFilters::SNGSize_Single;
// //            } else {
// //                currValue |= UserType::EFilters::SNGSize_Multiple;
// //            }
// //        }

// //        if (!item._is_current_user_registered) {
// //            currValue |= UserType::EFilters::User_DoesNotParticipate;
// //        }

// //        return currValue;
// //    }

// //    QVariant GamesStatistic::data(const QModelIndex &index, int role) const
// //    {
// //        if (!isReady() || !index.isValid()) {
// //            return QVariant();
// //        }

// //        if (role == Qt::TextAlignmentRole) {
// //            if (index.column() == 0) {
// //                return Qt::AlignLeft;
// //            }
// //            return Qt::AlignCenter;
// //        }

// //        QVariant rowData = rowItem(index);

// //        if (rowData.canConvert<games_stat_t::first_type::value_type>()) {
// //            return getData(rowData.value<games_stat_t::first_type::value_type>(), index.column(), role);
// //        } else if (rowData.canConvert<games_stat_t::second_type::value_type>()) {
// //            return getData(rowData.value<games_stat_t::second_type::value_type>(), index.column(), role);
// //        }

// //        return QVariant();
// //    }

// //    QModelIndex GamesStatistic::indexOf(gbp_i64 id, int page) const
// //    {
// //        if (!isReady()) {
// //            return QModelIndex();
// //        }
// //        page = (page == Unknown) ? m_page : page;

// //        if (page == CashTables)
// //        {
// //            for (games_stat_t::first_type::size_type i = 0; i < m_originData->first.size(); i++) {
// //                if (m_originData->first.at(i)._id == id) {
// //                    return index(i, 0);
// //                }
// //            }
// //        }
// //        else if (page == Tournaments)
// //        {
// //            for (games_stat_t::second_type::size_type i = 0; i < m_originData->second.size(); i++) {
// //                if (m_originData->second.at(i)._id == id) {
// //                    return index(i, 0);
// //                }
// //            }
// //        }

// //        return QModelIndex();
// //    }

// //    gbp_i64 GamesStatistic::itemId(const QModelIndex &idx) const
// //    {
// //        if (idx.internalPointer() == &m_originData->first) {
// //            return m_originData->first.at(idx.row())._id;
// //        } else if (idx.internalPointer() == &m_originData->second) {
// //            return m_originData->first.at(idx.row())._id;
// //        }
// //        return -1;
// //    }

// //    GamesStatistic::EPage GamesStatistic::page() const
// //    {
// //        return m_page;
// //    }

// //    void GamesStatistic::onDataUpdated(const games_stat_t& statistic/*quintptr sharedDataID*/)
// //    {
// //        SharedDataService& sdserv = pApp->db().sharedDataService();
// //        m_sharedDataId = sdserv.singleIdForType<games_stat_t>();
// //        sdserv.updateSingleData(statistic);
// //        m_originData = sdserv.getData<games_stat_t>(m_sharedDataId);

// //        if (isReady())
// //        {
// //            if (m_ids.first.empty()) {
// //                resetData(m_originData->first);
// //            } else {
// //                updateData(m_originData->first);
// //            }
// //            if (m_ids.second.empty()) {
// //                resetData(m_originData->second);
// //            } else {
// //                updateData(m_originData->second);
// //            }
// //        }
// //        m_previuosData = *m_originData;

// //        m_cashes->updateData(statistic);
// //        m_tourns->updateData(statistic);
// //    }

// //    void GamesStatistic::resetData(const games_stat_t::first_type &data)
// //    {
// //        if (m_page == CashTables) {
// //            beginResetModel();
// //        }
// //        for (const games_stat_t::first_type::value_type& value: data) {
// //            m_ids.first.insert(value._id);
// //        }
// //        if (m_page == CashTables) {
// //            endResetModel();
// //        }
// //    }
// //    void GamesStatistic::resetData(const games_stat_t::second_type &data)
// //    {
// //        if (m_page == Tournaments) {
// //            beginResetModel();
// //        }
// //        for (const games_stat_t::second_type::value_type& value: data) {
// //            m_ids.second.insert(value._id);
// //        }
// //        if (m_page == Tournaments) {
// //            endResetModel();
// //        }
// //    }
// //    void GamesStatistic::updateData(const games_stat_t::first_type &data)
// //    {
// //        std::set<gbp_i64> idsToRemove(m_ids.first);
// //        std::set<gbp_i64> idsToAdd;
// //        std::set<gbp_i64> idsToModify;
// //        for (const games_stat_t::first_type::value_type& value: data) {
// //            if (idsToRemove.find(value._id) != idsToRemove.end()) {
// //                idsToRemove.erase(value._id);
// //                idsToModify.insert(value._id);
// //            } else {
// //                idsToAdd.insert(value._id);
// //            }
// //        }

// //        if (!idsToRemove.empty())
// //        {
// //            for (gbp_i64 id: idsToRemove) {
// //                removeItem(id, CashTables);
// //            }
// //        }

// //        if (!idsToModify.empty())
// //        {
// //            for (gbp_i64 id: idsToModify) {
// //                updateItem(id, CashTables);
// //            }
// //        }

// //        if (!idsToAdd.empty())
// //        {
// //            for (gbp_i64 id: idsToAdd) {
// //                addItem(id, CashTables);
// //            }
// //        }

// //    }
// //    void GamesStatistic::updateData(const games_stat_t::second_type &data)
// //    {
// //        std::set<gbp_i64> idsToRemove(m_ids.second);
// //        std::set<gbp_i64> idsToAdd;
// //        std::set<gbp_i64> idsToModify;
// //        for (const games_stat_t::second_type::value_type& value: data) {
// //            if (idsToRemove.find(value._id) != idsToRemove.end()) {
// //                idsToRemove.erase(value._id);
// //                idsToModify.insert(value._id);
// //            } else {
// //                idsToAdd.insert(value._id);
// //            }
// //        }

// //        if (!idsToRemove.empty())
// //        {
// //            for (gbp_i64 id: idsToRemove) {
// //                removeItem(id, Tournaments);
// //            }
// //        }

// //        if (!idsToModify.empty())
// //        {
// //            for (gbp_i64 id: idsToModify) {
// //                updateItem(id, Tournaments);
// //            }
// //        }

// //        if (!idsToAdd.empty())
// //        {
// //            for (gbp_i64 id: idsToAdd) {
// //                addItem(id, Tournaments);
// //            }
// //        }
// //    }

// //    void GamesStatistic::addItem(gbp_i64 id, int page)
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        int itemCount = rowCount(page);
// //        if (page == m_page) {
// //            beginInsertRows(QModelIndex(), itemCount, itemCount);
// //        }
// //        if (page == CashTables) {
// //            m_ids.first.insert(id);
// //        } else {
// //            m_ids.second.insert(id);
// //        }
// //        if (page == m_page) {
// //            endInsertRows();
// //        }
// //    }
// //    void GamesStatistic::removeItem(gbp_i64 id, int page)
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        QModelIndex idx = indexOf(id, page);

// //        if (idx.isValid())
// //        {
// //            if (page == m_page) {
// //                beginRemoveRows(QModelIndex(), idx.row(), idx.row());
// //            }
// //            if (page == CashTables) {
// //                m_ids.first.erase(id);
// //            } else {
// //                m_ids.second.erase(id);
// //            }
// //            if (page == m_page) {
// //                endRemoveRows();
// //            }
// //        }
// //    }

// //    void GamesStatistic::updateItem(gbp_i64 id, int page)
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        if (page == m_page)
// //        {
// //            doItemChanges(id, page);
// //        }
// //    }

// //    QVariant GamesStatistic::rowItem(const QModelIndex &index) const
// //    {
// //        if (!isReady() || !index.isValid() || index.parent().isValid()) {
// //            return QVariant();
// //        }

// //        if (index.internalPointer() == &m_originData->first) {
// //            return rowItem(index.row(), 0);
// //        } else if (index.internalPointer() == &m_originData->second) {
// //            return rowItem(index.row(), 1);
// //        }

// //        return QVariant();
// //    }

// //    QVariant GamesStatistic::rowItem(int row, int page) const
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        if (page == CashTables) {
// //            return (m_originData->first.size() > row) ? QVariant::fromValue(m_originData->first.at(row)) : QVariant();
// //        } else if (page == Tournaments) {
// //            return (m_originData->second.size() > row) ? QVariant::fromValue(m_originData->second.at(row)) : QVariant();
// //        }

// //        return QVariant();
// //    }

// //    QVariant GamesStatistic::previousRowItem(gbp_i64 id, int page) const
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        if (page == CashTables)
// //        {
// //            for (const games_stat_t::first_type::value_type& item : m_previuosData.first) {
// //                if (item._id == id) {
// //                    return QVariant::fromValue(item);
// //                }
// //            }
// //        }
// //        else if (page == Tournaments)
// //        {
// //            for (const games_stat_t::second_type::value_type& item : m_previuosData.second) {
// //                if (item._id == id) {
// //                    return QVariant::fromValue(item);
// //                }
// //            }
// //        }
// //        return QVariant();
// //    }

// //    void GamesStatistic::doItemChanges(gbp_i64 id, int page)
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        QMap<int, QVector<int>> diff;
// //        if (page == CashTables)
// //        {
// //            const games_stat_t::first_type::value_type& previousItem = previousRowItem(id, 0).value<games_stat_t::first_type::value_type>();
// //            const games_stat_t::first_type::value_type& currentItem  = rowItem(indexOf(id, 0)).value<games_stat_t::first_type::value_type>();

// //            diff = getDifference(previousItem, currentItem);
// //        }
// //        else if (page == Tournaments)
// //        {
// //            const games_stat_t::second_type::value_type& previousItem = previousRowItem(id, 1).value<games_stat_t::second_type::value_type>();
// //            const games_stat_t::second_type::value_type& currentItem  = rowItem(indexOf(id, 1)).value<games_stat_t::second_type::value_type>();

// //            diff = getDifference(previousItem, currentItem);
// //        }
// //        if (!diff.empty())
// //        {
// //            QModelIndex idx = indexOf(id, page);

// //            for (int column: diff.keys()) {
// //                auto value = diff.value(column);
// //                if (column < 0 || column >= columnCount(page) || value.contains(UserType::RowDataRole) || value.contains(UserType::RowItemIdRole)) {
// //                    // обновить всю строку
// //                    emit dataChanged(index(idx.row(), 0), index(idx.row(), columnCount(page) - 1));
// //                    return;
// //                }
// //                QModelIndex i = index(idx.row(), column);
// //                QVector<int> roles = diff.value(column);
// //                emit dataChanged(i, i, roles);
// //            }
// //        }
// //    }

// //    QModelIndex GamesStatistic::index(int row, int column, const QModelIndex &parent) const
// //    {
// //        if (!parent.isValid() && row == 0 && (column == 0 || column == 1)) {
// //            if (column == 0) {
// //                return createIndex(row, column, m_cashes.data());
// //            }
// //            return createIndex(row, column, m_tourns.data());
// //        }
// //        return createIndex(row, column, parent.internalPointer());
// //    }


// //    QVariant GamesStatistic::headerData(int section, Qt::Orientation orientation, int role) const
// //    {
// //        if (orientation != Qt::Horizontal) {
// //            return QVariant();
// //        }
// //        if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
// //            return headerText(section);
// //        } else if (role == Qt::ForegroundRole) {
// //            return QColor(0xb0bec5);
// //        } else if (role == Qt::SizeHintRole) {
// //            return QSize(::headerSizeHint(section, m_page), 24);
// //        }

// //        return QVariant();
// //    }

// //    QString GamesStatistic::headerText(int section, int page) const
// //    {
// //        page = (page == Unknown) ? m_page : page;
// //        if (page == CashTables)
// //        {
// //            switch (section) {
// //            case 0: return capitalized(ns_model::GamesStatistic::tr("@bttn_table"));
// //            case 1: return capitalized(ns_model::GamesStatistic::tr("@bttn_bets"));
// //            case 2: return capitalized(ns_model::GamesStatistic::tr("@bttn_game"));
// //            case 3: return capitalized(ns_model::GamesStatistic::tr("@bttn_type"));
// //            case 4: return capitalized(ns_model::GamesStatistic::tr("@bttn_players"));
// //            case 5: return capitalized(ns_model::GamesStatistic::tr("@bttn_wait"));
// //            case 6: return capitalized(ns_model::GamesStatistic::tr("@bttn_bank"));
// //            case 7: return capitalized(ns_model::GamesStatistic::tr("@bttn_flop"));
// //            case 8: return capitalized(ns_model::GamesStatistic::tr("@bttn_deal"));
// //            default:
// //                return QString();
// //            }
// //        }
// //        else if (page == Tournaments)
// //        {
// //            switch (section) {
// //            case 0: return capitalized(ns_model::GamesStatistic::tr("@bttn_begins"));
// //            case 1: return capitalized(ns_model::GamesStatistic::tr("@bttn_game"));
// //            case 2: return capitalized(ns_model::GamesStatistic::tr("@bttn_buyin"));
// //            case 3: return capitalized(ns_model::GamesStatistic::tr("@bttn_type"));
// //            case 4: return capitalized(ns_model::GamesStatistic::tr("@bttn_tournament"));
// //            case 5: return capitalized(ns_model::GamesStatistic::tr("@bttn_fond"));
// //            case 6: return capitalized(ns_model::GamesStatistic::tr("@bttn_status"));
// //            case 7: return capitalized(ns_model::GamesStatistic::tr("@bttn_players"));
// //            default:
// //                return QString();
// //            }
// //        }

// //        return QString();
// //    }

// //    bool GamesStatistic::setData(const QModelIndex &index, const QVariant &value, int role)
// //    {
// //        if (!index.isValid()) {
// //            return false;
// //        }

// //        if (role == UserType::RowDataRole)
// //        {
// //            if (index.internalPointer() == &m_originData->first) {
// //                games_stat_t::first_type::value_type newData = value.value<games_stat_t::first_type::value_type>();
// //                for (games_stat_t::first_type::value_type& item: m_originData->first) {
// //                    if (item._id == newData._id)
// //                    {
// //                        item = newData;
// //                        updateItem(newData._id, 0);
// //                        return true;
// //                    }
// //                }

// //            } else if (index.internalPointer() == &m_originData->second) {
// //                games_stat_t::second_type::value_type newData = value.value<games_stat_t::second_type::value_type>();
// //                for (games_stat_t::second_type::value_type& item: m_originData->second) {
// //                    if (item._id == newData._id)
// //                    {
// //                        item = newData;
// //                        updateItem(newData._id, 1);
// //                        return true;
// //                    }
// //                }
// //            }
// //        }


// //        return QAbstractProxyModel::setData(index, value, role);
// //    }

// //    QModelIndexList GamesStatistic::match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const
// //    {
// //        if (role == UserType::RowItemIdRole && value.canConvert<gbp_i64>()) {
// //            gbp_i64 id = value.value<gbp_i64>();
// //            for (int i = 0; i < rowCount(0); i++) {
// //                if (m_originData->first.at(i)._id == id) {
// //                    return QModelIndexList() << index(i, 0);
// //                }
// //            }
// //            for (int i = 0; i < rowCount(1); i++) {
// //                if (m_originData->second.at(i)._id == id) {
// //                    return QModelIndexList() << index(i, 0);
// //                }
// //            }
// //            return QModelIndexList();
// //        }
// //        return QAbstractProxyModel::match(start, role, value, hits, flags);
// //    }

// //    QModelIndex GamesStatistic::parent(const QModelIndex &child) const
// //    {

// //    }

// //    bool GamesStatistic::hasChildren(const QModelIndex &parent) const {
// //        return !parent.isValid() || parent.parent().isValid();
// //    }

// //    QModelIndex GamesStatistic::mapToSource(const QModelIndex &proxyIndex) const
// //    {
// //        if (!proxyIndex.isValid() || !proxyIndex.parent().isValid()) {
// //            return QModelIndex();
// //        }

// //        QModelIndex parentSourceIndex = QModelIndex();
// //        if (proxyIndex.parent().parent().isValid()) {
// //            parentSourceIndex = mapToSource(proxyIndex.parent());
// //        }
// //        if (parentSourceIndex.model() == m_cashes.data() || proxyIndex.parent().internalPointer() == m_cashes.data()) {
// //            return m_cashes->index(proxyIndex.row(), proxyIndex.column(), parentSourceIndex);
// //        } else {
// //            return m_tourns->index(proxyIndex.row(), proxyIndex.column(), parentSourceIndex);
// //        }

// //    }

// //    QModelIndex GamesStatistic::mapFromSource(const QModelIndex &sourceIndex) const
// //    {
// //        if (!sourceIndex.parent().isValid()) {
// //            QModelIndex parentIdx = index(0, (sourceIndex.model() == m_cashes.data() ? 0 : 1));
// //            return index(sourceIndex.row(), sourceIndex.column(), parentIdx);
// //        }
// //        return index(sourceIndex.row(), sourceIndex.column(), mapFromSource(sourceIndex.parent()));
// //    }



// namespace
// {
//     using source_column = ns_model::cash_stat_t::column;
//     using proxy_column = ns_model::CashTables::column;

//     QList<source_column> convert(proxy_column col) {
//         switch (col) {
//         case proxy_column::title:        return QList<source_column>() << source_column::_title;
//         case proxy_column::bets:         return QList<source_column>() << source_column::_big_blind;
//         case proxy_column::game:         return QList<source_column>() << source_column::_type << source_column::_limit;
//         case proxy_column::type:         return QList<source_column>() << source_column::_players_count;
//         case proxy_column::players:      return QList<source_column>() << source_column::_players << source_column::_players_count;
//         case proxy_column::waiters:      return QList<source_column>() << source_column::_wait_players;
//         case proxy_column::pot:          return QList<source_column>() << source_column::_pot;
//         case proxy_column::flop:         return QList<source_column>() << source_column::_flp;
//         case proxy_column::distribution: return QList<source_column>() << source_column::_hands_per_hour;
//         default:
//             return QList<source_column>();
//         }
//     }

//     QList<proxy_column> convert(source_column col) {
//         switch (col) {
//         case source_column::_id:			return QList<proxy_column>() << proxy_column(-1);
//         case source_column::_title:			return QList<proxy_column>() << proxy_column::title;
//         case source_column::_big_blind:		return QList<proxy_column>() << proxy_column::bets;
//         case source_column::_type:			return QList<proxy_column>() << proxy_column::game;
//         case source_column::_limit:			return QList<proxy_column>() << proxy_column::game;
//         case source_column::_players_count:	return QList<proxy_column>() << proxy_column::players;
//         case source_column::_pot:			return QList<proxy_column>() << proxy_column::pot;
//         case source_column::_flp:			return QList<proxy_column>() << proxy_column::flop;
//         case source_column::_hands_per_hour:return QList<proxy_column>() << proxy_column::distribution;
//         case source_column::_wait_players:	return QList<proxy_column>() << proxy_column::waiters;
//         case source_column::_players:		return QList<proxy_column>() << proxy_column::players;
//         case source_column::_is_freeroll:
//         case source_column::_is_flash:
//         case source_column::_flash_id:
//         default:
//             return QList<proxy_column>();
//         }
//     }

// } //namespace

// namespace ns_model
// {

//     CashTables::CashTables(QObject *parent)
//         : QSortFilterProxyModel(parent)
//         , m_sourceModel(nullptr)
//     {}

//     int CashTables::columnCount(const QModelIndex &parent) const {
//         if (parent.isValid()) {
//             return 0;
//         }
//         return underlying_cast(column::count);
//     }


//     QVariant CashTables::getData(const games_stat_t::first_type::value_type &item, int column, int role) const
//     {
//         switch (role) {
//         case UserType::RowDataRole:
//             return QVariant::fromValue(item);
//         case UserType::RowItemIdRole:
//             return QVariant::fromValue(item._id);
//         case Qt::DisplayRole:
//         case Qt::ToolTipRole:
//             switch (column) {
//             case 0: return QString(item._title.c_str());
//             case 1: return QString("%0/%1").arg(Money(item._big_blind/2).toString()).arg(Money(item._big_blind).toString());
//             case 2: return QString("%0 %1").arg(ns_util::toString(item._limit), ns_util::toString(item._type));
//             case 4: return QVariant::fromValue(item._players.size());
//             case 5: return QVariant::fromValue(item._wait_players);
//             case 6: return Money(item._pot).toString();
//             case 7: return item._flp;
//             case 8: return QVariant::fromValue(item._hands_per_hour);
//             default:
//                 return QVariant();
//             }
//         case Qt::DecorationRole:
//             if (column == 3) {
//                 return pApp->iconProvider().gameTypeIcon(item);
//             }
//             return QVariant();
//         case Qt::ForegroundRole:
//             return QColor(0xfafafa);
//         case UserType::SortRole:
//             switch (column) {
//             case 0: return QString(item._title.c_str());
//             case 1: return QVariant::fromValue(item._big_blind);
//             case 2: return QString("%0 %1").arg(ns_util::toString(item._limit), ns_util::toString(item._type));
//             case 3: return item._players_count.second;
//             case 4: return QVariant::fromValue(item._players.size());
//             case 5: return QVariant::fromValue(item._wait_players);
//             case 6: return QVariant::fromValue(item._pot);
//             case 7: return item._flp;
//             case 8: return QVariant::fromValue(item._hands_per_hour);
//             default:
//                 return QVariant();
//             }
//         case UserType::FilterRole:
//             return UserType::convert(filterValue(item));
//         default:
//             return QVariant();
//         }
//     }

//     QVariant CashTables::data(const QModelIndex &index, int role) const
//     {
//         if (m_sourceModel == nullptr || !index.isValid() || index.parent().isValid()) {
//             return QVariant();
//         }
//         QVariant rowData = QSortFilterProxyModel::data(index, UserType::RowDataRole);
//         if (role == UserType::RowDataRole) {
//             return rowData;
//         }
//         if (!rowData.canConvert<gbp::ns_api::ns_statistic::cash_stat_t>()) {
//             return QVariant();
//         }
//         gbp::ns_api::ns_statistic::cash_stat_t data = rowData.value<gbp::ns_api::ns_statistic::cash_stat_t>();
//         return getData(data, index.column(), role);
// //        switch (role) {
// //        case Qt::DisplayRole:
// //        case Qt::ToolTipRole:
// //        case Qt::EditRole:
// //            switch (column(index.column())) {
// //            case proxy_column::title:
// //            case proxy_column::waiters:
// //            case proxy_column::pot:
// //            case proxy_column::flop:
// //            case proxy_column::distribution:
// //                return m_sourceModel->indexOf(data._id, convert(column(index.column())).first()).data(role);
// //            case proxy_column::bets:
// //                return QString("%0/%1").arg(ns_meta::money_type::display(data._big_blind/2)).arg(ns_meta::money_type::display(data._big_blind));
// //            case proxy_column::game:
// //                return m_sourceModel->indexOf(data._id, source_column::_limit).data(role).toString() + " "
// //                     + m_sourceModel->indexOf(data._id, source_column::_type).data(role).toString();
// //            case proxy_column::type:
// //                return QVariant();
// //            case proxy_column::players: return ns_meta::bounded_count_type::display(data._players_count);
// //            }
// //        default:
// //            return QVariant();
// //        }
//     }

//     void CashTables::setSourceModel(QAbstractItemModel *sourceModel)
//     {
//         if (m_sourceModel != sourceModel) {
//             beginResetModel();
//             m_sourceModel = qobject_cast<ns_model::cash_stat_t*>(sourceModel);
//             QSortFilterProxyModel::setSourceModel(m_sourceModel);
//             endResetModel();
//         }
//     }

// //    bool CashTables::filterAcceptsItem(const games_stat_t::first_type::value_type &item, UserType::FilterFlags filter)
// //    {
// //        UserType::FilterFlags currValue = filterValue(item);

// //        return currValue & filter;
// //    }

//     UserType::FilterFlags CashTables::filterValue(const games_stat_t::first_type::value_type &item)
//     {
//         UserType::FilterFlags currValue = UserType::EFilters::AllUnchecked;

//         switch (item._type) {
//         case e_game_type::texas_holdem: currValue |= UserType::EFilters::GameType_Holdem; break;
//         case e_game_type::omaha:
//         case e_game_type::omaha_hi_low: currValue |= UserType::EFilters::GameType_Omaha; break;
//         default:
//             break;
//         }
//         switch (item._limit) {
//         case e_game_limit::fixed_limit: currValue |= UserType::EFilters::Limit_FixedLimit; break;
//         case e_game_limit::pot_limit:   currValue |= UserType::EFilters::Limit_PotLimit; break;
//         case e_game_limit::no_limit:    currValue |= UserType::EFilters::Limit_NoLimit; break;
//         }
//         if (item._is_freeroll) {
//             currValue |= UserType::EFilters::BuyIn_FreeRoll;
//         } else if (item._big_blind < 10) {
//             currValue |= UserType::EFilters::BuyIn_Micro;
//         } else if (item._big_blind < 50) {
//             currValue |= UserType::EFilters::BuyIn_Low;
//         } else if (item._big_blind < 200) {
//             currValue |= UserType::EFilters::BuyIn_Medium;
//         } else {
//             currValue |= UserType::EFilters::BuyIn_High;
//         }
//         switch (item._players_count.second) {
//         case 2: currValue |= UserType::EFilters::SeatsCount_2; break;
//         case 6: currValue |= UserType::EFilters::SeatsCount_6; break;
//         case 9: currValue |= UserType::EFilters::SeatsCount_9; break;
//         default:
//             break;
//         }
//         if (item._players_count.first == 0) {
//             currValue |= UserType::EFilters::FreeSeats_Empty;
//         } else if (item._players_count.first == item._players_count.second) {
//             currValue |= UserType::EFilters::FreeSeats_Full;
//         }
//         return currValue;
//     }

// //    QModelIndex CashTables::mapToSource(const QModelIndex &proxyIndex) const
// //    {
// //        if (m_sourceModel == nullptr || !proxyIndex.isValid() || proxyIndex.model() != this) {
// //            return QModelIndex();
// //        }
// //        if (proxyIndex.parent().isValid()) {
// //            return QModelIndex();
// //        }
// //        QList<source_column> lst = convert(proxy_column(proxyIndex.column()));
// //        if (lst.isEmpty()) {
// //            return QModelIndex();
// //        }
// //        QModelIndex idx = QSortFilterProxyModel::mapToSource(proxyIndex);
// //        return idx.sibling(idx.row(), underlying_cast(lst.first()));
// //    }

// //    QModelIndex CashTables::mapFromSource(const QModelIndex &sourceIndex) const
// //    {
// //        if (m_sourceModel == nullptr || !sourceIndex.isValid() || sourceIndex.model() != m_sourceModel) {
// //            return QModelIndex();
// //        }
// //        if (sourceIndex.parent().isValid()) {
// //            return QModelIndex();
// //        }
// //        QList<proxy_column> lst = convert(source_column(sourceIndex.column()));
// //        if (lst.isEmpty()) {
// //            return QModelIndex();
// //        }
// //        QModelIndex idx = QSortFilterProxyModel::mapFromSource(sourceIndex);
// //        return idx.sibling(idx.row(), underlying_cast(lst.first()));
// //    }

//     QModelIndex CashTables::indexOf(gbp_i64 id) const
//     {
//         if (m_sourceModel == nullptr) {
//             return QModelIndex();
//         }
//         return mapFromSource(m_sourceModel->indexOf(id));
//     }

//     QModelIndex CashTables::indexOf(gbp_i64 id, CashTables::column field) const {
//         QModelIndex idx = indexOf(id);
//         return idx.sibling(idx.row(), underlying_cast(field));
//     }

//     bool CashTables::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
//         return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
//     }

// } // namespace ns_model


// int ns_model::CashTables::rowCount(const QModelIndex &parent) const
// {
//     return m_sourceModel == nullptr ? 0 : m_sourceModel->rowCount(mapToSource(parent));
// }

// bool ns_model::CashTables::setData(const QModelIndex &index, const QVariant &value, int role)
// {
//     return QSortFilterProxyModel::setData(index, value, role);
// }

// QVariant ns_model::CashTables::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     if (orientation != Qt::Horizontal) {
//         return QVariant();
//     }
//     switch (role) {
//     case Qt::DisplayRole:
//     case Qt::ToolTipRole:
//         switch (section) {
//         case 0: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_table"));
//         case 1: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_bets"));
//         case 2: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_game"));
//         case 3: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_type"));
//         case 4: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_players"));
//         case 5: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_wait"));
//         case 6: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_bank"));
//         case 7: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_flop"));
//         case 8: return capitalized(QApplication::translate("ns_model::GamesStatistic", "@bttn_deal"));
//         default:
//             return QString();
//         }
//     case Qt::ForegroundRole:
//         return QColor(0xb0bec5);
//     default:
//         return QVariant();
//     }
// }
