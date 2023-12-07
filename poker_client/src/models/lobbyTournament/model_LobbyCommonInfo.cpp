#include <precompiled.h>
#include <core/money.h>
#include "model_LobbyCommonInfo.h"
#include <utils/Utils.h>
#include <api/lobby_stat/types.h>
#include <db/modeldatabase.h>
#include <api_models/model_tourn_additional_stat_t.hpp>
#include <api_models/itemtype_concept.hpp>
#include <tools/item_reference.hpp>


namespace
{
    using e_tournament_type   = gbp::ns_api::ns_tournament_types::e_tournament_type;
    using e_chips_rebuy       = gbp::ns_api::ns_tournament_types::e_chips_rebuy;
    using e_money_in          = gbp::ns_api::ns_tournament_types::e_money_in;
    using e_win_result        = gbp::ns_api::ns_tournament_types::e_win_result;
    using e_tournament_status = gbp::ns_api::ns_tournament_types::e_tournament_status;

    QString gameTypeName(e_tournament_type types)
    {
        QString name = "";

        if (types & e_money_in::mi_free_roll) {
            name += ns_model::LobbyCommonInfo::tr("@freeroll") + ", ";
        }
        if (types & e_chips_rebuy::cr_normal) {
            name += ns_model::LobbyCommonInfo::tr("@regular");
        } else if (types & e_chips_rebuy::cr_rebuy) {
            name += ns_model::LobbyCommonInfo::tr("@rebuy");
        } else if (types & e_chips_rebuy::cr_progressive) {
            name += ns_model::LobbyCommonInfo::tr("@progressive");
        } else if (types & e_chips_rebuy::cr_bounty) {
            name += ns_model::LobbyCommonInfo::tr("@knockout");
        }

        if (types & e_win_result::wr_main) {
            name += ", " + ns_model::LobbyCommonInfo::tr("@main_event");
        } else if (types & e_win_result::wr_qualifier) {
            name += ", " + ns_model::LobbyCommonInfo::tr("@qualifier");
        }

        return name;
    }
}


namespace ns_model
{

    LobbyCommonInfo::LobbyCommonInfo(QObject *parent)
        : LobbyBase(parent)
    { }

    int LobbyCommonInfo::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return 19;
    }

    int LobbyCommonInfo::columnCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return 2;
    }

    bool LobbyCommonInfo::isRowDisplayed(int index) const
    {
        if (statistic().ptr() == nullptr) {
            return false;
        }
        TournamentRef data(statistic().ref());
        switch (index) {
            case 3:
            case 5:
            case 6: return  data._types & (e_chips_rebuy::cr_bounty | e_chips_rebuy::cr_progressive);
            case 14: return data._late_registration_duration > 0;
            case 15: return data._rebuys._count != 0;
            case 16: return data._addon._is_available;
            case 17: return data._status == e_tournament_status::finished && (isRowDisplayed(15) || isRowDisplayed(16));
            case 18: return data._types & e_win_result::wr_qualifier;
            default:
                return true;
        }
    }

    QModelIndex LobbyCommonInfo::rebuysCount() const {
        return (!additional_statistic()) ? QModelIndex() : additional_statistic()->indexOf(tourn_additional_stat_t::field::_rebuys_count);
    }
    QModelIndex LobbyCommonInfo::addonsCount() const {
        return (!additional_statistic()) ? QModelIndex() : additional_statistic()->indexOf(tourn_additional_stat_t::field::_addons_count);
    }

    void LobbyCommonInfo::onReset()
    {

    }

    QVariant LobbyCommonInfo::data(const QModelIndex &modelIndex, int role) const
    {
        if (modelIndex.column() == 0)
        {
            switch (role) {
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
                switch (modelIndex.row()) {
                case 0: return tr("@game");
                case 1: return tr("@type");
                case 2: return tr("@byuin");
                case 3: return tr("@common_prize_fond");
                case 4: return tr("@default_prize_fond");
                case 5: return tr("@knockout_prize_fond");
                case 6: return tr("@knockout_reward");
                case 7: return tr("@seats_count");
                case 8: return tr("@start_stack");
                case 9: return tr("@minimum_players");
                case 10: return tr("@maximum_players");
                case 11: return tr("@final_table");
                case 12: return tr("@break");
                case 13: return tr("@registration_canceling");
                case 14: return tr("@late_registration");
                case 15: return tr("@rebuy");
                case 16: return tr("@addon");
                case 17: return tr("@rebuy_addon_stat");
                case 18: return tr("@qualifier_of");
                }
                return LobbyBase::data(modelIndex, role);
            case Qt::ForegroundRole:
                return QColor(0x78909c);
            default:
                return LobbyBase::data(modelIndex, role);
            }
        }

        static const int breakDurationMinutes = 5;
        static const int breakFrequenceMinutes = 55;

        TournamentRef dRef(statistic().ref());

        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            switch (modelIndex.row()) {
            case 0:  return ns_util::toString(dRef._game_limit) + " " + ns_util::toString(dRef._game_type);
            case 1:  return gameTypeName(dRef._types);
            case 2:  return ns_meta::money_type::display(dRef._buyin);
            case 3:  return ns_meta::money_type::display(dRef._common_prize);
            case 4:  return ns_meta::money_type::display(dRef._common_prize);
            case 5:  return ns_meta::money_type::display(dRef._common_prize);
            case 6:  return ns_meta::money_type::display(dRef._bounty);
            case 7:  return dRef._seats_count;
            case 8:  return ns_meta::money_type::display(dRef._start_stack);
            case 9:  return dRef._sit_n_go_users_count;
            case 10: return dRef._max_players_count;
            case 11: return dRef._seats_count;
            case 12: return tr("@timebreak_%0_min_every_%1_min").arg(breakDurationMinutes).arg(breakFrequenceMinutes);
            case 13: return ns_meta::duration_type::display(gbp_i64(5*60));
            case 14: return ns_meta::duration_type::display(dRef._late_registration_duration);
            case 15:
            {
                auto info = dRef._rebuys;
                return tr("@rebuys_info_%0_%1_%2").arg(info._count == -1 ? tr("@no_limit") : QString::number(info._count)).arg(info._chips).arg(Money(info._money).toString());
            }
            case 16:
            {
                auto info = dRef._addon;
                return info._is_available ? tr("@addons_info_%0_%1").arg(info._chips).arg(Money(info._money).toString()) : QString();
            }
            case 17: return tr("@rebuys_%0_addons_%1").arg(rebuysCount().data().toString()).arg(addonsCount().data().toString());
            case 18: return dRef._main_tournament_id;
            }
            return LobbyBase::data(modelIndex, role);
        case Qt::ForegroundRole:
            return QColor(0xb0bec5);
        default:
            return LobbyBase::data(modelIndex, role);
        }

        return LobbyBase::data(modelIndex, role);
    }

    tourn_additional_stat_t *LobbyCommonInfo::additional_statistic() const {
        return modelsDB().getAdditionalTournStatistic(m_tournamentID);
    }

} //namespace ns_model
