#include "api_util.hpp"
#include <QDate>

#ifndef Q_MOC_RUN
    #include <api/api_common.h>
    #include <api/lobby/types.h>
    #include <api/lobby_stat/types.h>
#endif // !Q_MOC_RUN

#include <QCoreApplication>
#include <qabstractitemmodel.h>

QModelIndex mapToModel(const QAbstractItemModel *model, const QModelIndex &index)
{
    if (!index.isValid() || model == nullptr) {
        return QModelIndex();
    }
    if (index.model() == model) {
        return index;
    }
    if (const QAbstractProxyModel* proxy = qobject_cast<const QAbstractProxyModel*>(index.model()))
    {
        if (proxy->sourceModel() == model) {
            return proxy->mapToSource(index);
        }
        QModelIndex resultIndex = mapToModel(model, proxy->mapToSource(index));
        if (resultIndex.isValid()) {
            return resultIndex;
        }
    }
    if (const QAbstractProxyModel* proxy = qobject_cast<const QAbstractProxyModel*>(model))
    {
        QModelIndex resultIndex = mapToModel(proxy->sourceModel(), index);
        if (resultIndex.isValid()) {
            return proxy->mapFromSource(resultIndex);
        }
    }

    return QModelIndex();
}


namespace ns_util
{
    template <>
    QString toString(gbp::ns_vip_system::e_bonus e)
    {
        switch (e) {
        case gbp::ns_vip_system::e_bonus::all:            return QCoreApplication::translate("ns_page::VipSystemDescription", "@all");
        case gbp::ns_vip_system::e_bonus::rakeback_bonus: return QCoreApplication::translate("ns_page::VipSystemDescription", "@rakeback_bonus");
        case gbp::ns_vip_system::e_bonus::status_bonus:   return QCoreApplication::translate("ns_page::VipSystemDescription", "@status_bonus");
        default:
            return "";
        }
    }

    template <>
    QString toString(gbp::ns_vip_system::e_status e)
    {
        switch (e) {
        case gbp::ns_vip_system::e_status::all:          return QCoreApplication::translate("ns_page::VipSystemDescription", "@ALL");
        case gbp::ns_vip_system::e_status::beginner:     return QCoreApplication::translate("ns_page::VipSystemDescription", "@BEGINNER");
        case gbp::ns_vip_system::e_status::semi_pro:     return QCoreApplication::translate("ns_page::VipSystemDescription", "@SEMI-PRO");
        case gbp::ns_vip_system::e_status::professional: return QCoreApplication::translate("ns_page::VipSystemDescription", "@PROFESSIONAL");
        case gbp::ns_vip_system::e_status::elite:        return QCoreApplication::translate("ns_page::VipSystemDescription", "@ELITE");
        case gbp::ns_vip_system::e_status::master:       return QCoreApplication::translate("ns_page::VipSystemDescription", "@MASTER");
        case gbp::ns_vip_system::e_status::legend:       return QCoreApplication::translate("ns_page::VipSystemDescription", "@LEGEND");

        default: return "";
        }
    }

    template <>
    QString toString(gbp::ns_api::ns_game::e_game_type e)
    {
        switch (e) {
        case (gbp::ns_api::ns_game::e_game_type::texas_holdem):   return QObject::tr("@pokertype_Holdem");
        case (gbp::ns_api::ns_game::e_game_type::omaha):          return QObject::tr("@pokertype_Omaha");
        case (gbp::ns_api::ns_game::e_game_type::omaha_hi_low):   return QObject::tr("@pokertype_Omaha_HighLow");
        case (gbp::ns_api::ns_game::e_game_type::stud):           return QObject::tr("@pokertype_Stud");
        case (gbp::ns_api::ns_game::e_game_type::stud_hi_low):    return QObject::tr("@pokertype_Stud_HighLow");
        default:                                             return QObject::tr("@pokertype_Unknown");
        }
    }
    template <>
    QString toString(gbp::ns_api::ns_tournament_types::e_tournament_status e) {
        using namespace gbp::ns_api::ns_tournament_types;
        switch (e) {
        case e_tournament_status::not_activated: return QObject::tr("e_tournament_status::not_activated");
        case e_tournament_status::wait: return QObject::tr("e_tournament_status::wait");
        case e_tournament_status::registering: return QObject::tr("e_tournament_status::registering");
        case e_tournament_status::late_registration: return QObject::tr("e_tournament_status::late_registration");
        case e_tournament_status::started: return QObject::tr("e_tournament_status::started");
        case e_tournament_status::finished: return QObject::tr("e_tournament_status::finished");
        }
        return "";
    }

    template <>
    QString toString(gbp::ns_api::ns_game::e_game_speed e)
    {
        switch (e) {
            case (gbp::ns_api::ns_game::e_game_speed::turbo):         return QObject::tr("@turbo");
            case (gbp::ns_api::ns_game::e_game_speed::super_turbo):   return QObject::tr("@super_turbo");
            case (gbp::ns_api::ns_game::e_game_speed::normal):
            default:                                             return "";
        }
    }

    template <>
    QString toString(gbp::ns_api::ns_table::e_game_round_type e)
    {
        using namespace gbp::ns_api::ns_table;
        switch (e) {
        case (e_game_round_type::preflop): return QObject::tr("@gbp::ns_api::ns_table::e_game_round_type::preflop");
        case (e_game_round_type::flop):    return QObject::tr("@gbp::ns_api::ns_table::e_game_round_type::flop");
        case (e_game_round_type::tern):    return QObject::tr("@gbp::ns_api::ns_table::e_game_round_type::tern");
        case (e_game_round_type::river):   return QObject::tr("@gbp::ns_api::ns_table::e_game_round_type::river");
        default:                           return "";
        }
    }

    template <>
    QString toString(gbp::ns_api::ns_game::e_game_limit e)
    {
        switch (e) {
            case (gbp::ns_api::ns_game::e_game_limit::fixed_limit): return QObject::tr("@gbp::ns_api::ns_game::e_game_limit::fixed_limit");
            case (gbp::ns_api::ns_game::e_game_limit::pot_limit):   return QObject::tr("@gbp::ns_api::ns_game::e_game_limit::pot_limit");
            case (gbp::ns_api::ns_game::e_game_limit::no_limit):    return QObject::tr("@gbp::ns_api::ns_game::e_game_limit::no_limit");
            default:                                                return "";
        }
    }

    template <>
    QString toString(gbp::ns_api::ns_table::e_user_action e)
    {
        using namespace gbp::ns_api::ns_table;
        switch (e) {
        case (e_user_action::all_in): return QObject::tr("@gbp::ns_api::ns_table::e_user_action::all_in_%0");
        case (e_user_action::bet):    return QObject::tr("@gbp::ns_api::ns_table::e_user_action::bet_%0");
        case (e_user_action::blind):  return QObject::tr("@gbp::ns_api::ns_table::e_user_action::blind_%0");
        case (e_user_action::call):   return QObject::tr("@gbp::ns_api::ns_table::e_user_action::call_%0");
        case (e_user_action::check):  return QObject::tr("@gbp::ns_api::ns_table::e_user_action::check");
        case (e_user_action::fold):   return QObject::tr("@gbp::ns_api::ns_table::e_user_action::fold");
        case (e_user_action::raise):  return QObject::tr("@gbp::ns_api::ns_table::e_user_action::raise_%0");
        default: return "";
        }
    }

    template <>
    QString toString(gbp::ns_api::ns_table::e_cards_combination e)
    {
        using namespace gbp::ns_api::ns_table;

        switch (e) {
            case (e_cards_combination::hight_card):     return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::hight_card");
            case (e_cards_combination::pair):           return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::pair");
            case (e_cards_combination::doper):          return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::doper");
            case (e_cards_combination::set):            return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::set");
            case (e_cards_combination::wheel):          return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::wheel");
            case (e_cards_combination::straight):       return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::straight");
            case (e_cards_combination::steel_wheel):    return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::steel_wheel");
            case (e_cards_combination::flush):          return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::flush");
            case (e_cards_combination::full_house):     return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::full_house");
            case (e_cards_combination::four):           return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::four");
            case (e_cards_combination::straight_flush): return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::straight_flush");
            case (e_cards_combination::royal_flush):    return QObject::tr("@gbp::ns_api::ns_table::e_cards_combination::royal_flush");
        }
        return "";
    }

    template <>
    QString toString(gbp::ns_api::e_verifying e)
    {
        switch (e)
        {
            case gbp::ns_api::e_verifying::in_process: return QObject::tr("@gbp::ns_api::e_verifying::in_process");
            case gbp::ns_api::e_verifying::verified: return QObject::tr("@gbp::ns_api::e_verifying::verified");
            case gbp::ns_api::e_verifying::not_verified: return QObject::tr("@gbp::ns_api::e_verifying::not_verified");
            default:
                return "";
        }
    }

    template <>
    QString toString(gbp::ns_details::e_gender e)
    {
        switch (e)
        {
            case gbp::ns_details::e_gender::male: return QObject::tr("@male");
            case gbp::ns_details::e_gender::female: return QObject::tr("@female");
            default:
                return "";
        }
    }

    QDate toQDate(const gbp::ns_api::birth_date &bdate) {
        return QDate(bdate._year, bdate._month, bdate._day);
    }

    gbp::ns_api::birth_date toApiBirthDate(const QDate &date)
    {
        gbp::ns_api::birth_date bdate;
        bdate._year = date.year();
        bdate._month = date.month();
        bdate._day = date.day();
        return bdate;
    }

    gbp::ns_api::birth_date apiDate(qint16 year, qint16 month, qint16 day)
    {
        gbp::ns_api::birth_date bdate;
        bdate._year = year;
        bdate._month = month;
        bdate._day = day;
        return bdate;
    }

    QString cardToString(gbp_i8 cardIndex) {
        if (cardIndex < 0 || cardIndex > 51) {
            return "▄▀";
        }
        QString suit = "?";
        switch (cardIndex / 13) {
        case 0: suit = "♣"; break;
        case 1: suit = "♠"; break;
        case 2: suit = "♦"; break;
        case 3: suit = "♥"; break;
        default:
            break;
        }
        return QString("%0%1").arg(suit).arg(std::vector<const char*>({"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"})[cardIndex % 13]);
    }

    QString traceModelIndex(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return "INVALID";
        }
        return QString("(%0, %1)->%2").arg(index.row()).arg(index.column()).arg(traceModelIndex(index.parent()));
    }

    QString traceModelIndexWIthData(const QModelIndex &index)
    {
        return traceModelIndex(index) + " " + index.data().toString();
    }

    QString printItemInfo(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return "INVALID INDEX";
        }
        QVariant indexData = index.data(Qt::WhatsThisRole);
        if (!indexData.isValid()) {
            indexData = index.data(Qt::ToolTipRole);
        }
        if (indexData.canConvert<QString>()) {
            return indexData.toString();
        }
        return traceModelIndexWIthData(index);
    }

} // ns_util
