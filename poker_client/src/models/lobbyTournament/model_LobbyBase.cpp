#include <precompiled.h>
#include <core/money.h>
#include "model_LobbyBase.h"
#include <basewidgets/types.h>
#include <db/modeldatabase.h>
#include <api_models/model_tournament_stat_t.hpp>
#include <tools/item_reference.hpp>

namespace ns_model
{
    LobbyBase::LobbyBase(QObject *parent)
        : QAbstractTableModel(parent)
        , m_tournamentID(-1)
    {}

    void LobbyBase::reset(gbp_i64 tournament_id) {
        if (m_tournamentID == tournament_id) {
            return;
        }
        beginResetModel();
        m_tournamentID = tournament_id;
        onReset();
        resetInternalData();
        endResetModel();
    }

    QVariant LobbyBase::data(const QModelIndex &index, int role) const
    {
        if (index.parent().isValid()) {
            return QVariant();
        }

        if (role == UserType::VisibilityRole) {
            return isRowDisplayed(index.row());
        }
        return QVariant();
    }

    bool LobbyBase::isRowDisplayed(int /*rowIndex*/) const {
        return true;
    }

    bool LobbyBase::isRowsDisplayed(int r1, int r2, int r3, int r4) const {
        return isRowDisplayed(r1) && isRowDisplayed(r2) && (r3 == -1 || (isRowDisplayed(r3) && (r4 == -1 || isRowDisplayed(r4))));
    }

    bool LobbyBase::isAnyRowDisplayed(int r1, int r2, int r3, int r4) const {
        return isRowDisplayed(r1) || isRowDisplayed(r2) || (r3 != -1 && (isRowDisplayed(r3) || (r4 != -1 && isRowDisplayed(r4))));
    }

    Accessor<ns_statistic::tournament_stat_t> LobbyBase::statistic() const {
        return modelsDB().getTournamentStatistic()->item(m_tournamentID);
    }

} //namespace ns_model
