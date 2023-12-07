#include <precompiled.h>
#include "model_QualifiersTable.h"
#include <core/money.h>
#include <utils/Utils.h>
#include <db/modeldatabase.h>
#include <PokerApplication.h>
#include <api_models/api_util.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <tools/item_reference.hpp>

namespace ns_model
{
    QualifiersTable::QualifiersTable(QObject *parent)
        : LobbyBase(parent)
        , m_sourceRootIndex()
    {

    }

    QualifiersTable::~QualifiersTable() {
    }

    void QualifiersTable::onReset() {
        m_sourceRootIndex = modelsDB().getTournamentStatistic()->indexOf(m_tournamentID, tournament_stat_t::column::_qualifiers_ids);
    }

    int QualifiersTable::rowCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return modelsDB().getTournamentStatistic()->rowCount(m_sourceRootIndex);
    }

    int QualifiersTable::columnCount(const QModelIndex &parent) const {
        if (parent.isValid()) {
            return 0;
        }
        return 5;
    }

    QVariant QualifiersTable::data(const QModelIndex &index, int role) const
    {
        if (index.parent().isValid()) {
            return LobbyBase::data(index, role);
        }

        std::vector<gbp_i64> ids = qvariant_cast<std::vector<gbp_i64>>(m_sourceRootIndex.data(UserType::OriginDataRole));
        gbp_i64 id = ids.at(index.row());
        auto* m = modelsDB().getTournamentStatistic();


        TournamentRef d = rowData(index.row());

        switch (index.column()) {
        case 0: return m->indexOf(id, tournament_stat_t::column::_start_time).data(role);
        case 1: return m->indexOf(id, tournament_stat_t::column::_buyin).data(role);
        case 2: return (d._types & gbp::ns_api::ns_tournament_types::e_win_prize::wp_warranty) ? tr("@warranty_value") : QString();
        case 3: return m->indexOf(id, tournament_stat_t::column::_status).data(role);
        case 4: return QString("%0/%1").arg(d._players_registered_count).arg(d._max_players_count);
        default:
            return LobbyBase::data(index, role);
        }/*
        if (role == Qt::DisplayRole)
        {
        }
        else if (role == UserType::SortRole)
        {
            switch (index.column()) {
            case 0: return QVariant::fromValue(d._start_time);
            case 1: return QVariant::fromValue(d._buyin);
            case 2: return (d._types & gbp::ns_api::ns_tournament_types::e_win_prize::wp_warranty);
            case 3: return underlying_cast(d._status);
            case 4: return d._max_players_count + float(d._players_registered_count)/d._max_players_count;
            default:
                return LobbyBase::data(index, role);
            }
        }*/

        return LobbyBase::data(index, role);
    }

    TournamentRef QualifiersTable::rowData(int idx) const {
        if (idx < 0 || idx >= rowCount()) {
            return TournamentRef::invalidItem();
        }

        gbp_i64 id = qvariant_cast<gbp_i64>(m_sourceRootIndex.model()->index(idx, 0, m_sourceRootIndex).data(UserType::OriginDataRole));
        return modelsDB().getTournamentStatistic()->item(id).ref();
    }

} //namespace ns_model


QVariant ns_model::QualifiersTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (section) {
        case 0: return tr("@starts");
        case 1: return tr("@buyin");
        case 2: return tr("@warranty");
        case 3: return tr("@status");
        case 4: return tr("@players_count");
        default:
            return QVariant();
        }
    case Qt::SizeHintRole:
        static const int widths[] = {118, 54, 65, 84, 56};
        return QSize(widths[section], 26);
    default:
        return QVariant();
    }

    return QVariant();
}
