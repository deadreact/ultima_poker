#pragma once

#include <QAbstractTableModel>
#include <tools/types_fwd.hpp>
#include <tools/item_reference.hpp>

template <class T> class Accessor;

namespace ns_model
{
    class LobbyBase : public QAbstractTableModel
    {
        Q_OBJECT
    protected:
        gbp_i64 m_tournamentID;
    public:
        LobbyBase(QObject *parent = 0);

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        virtual bool isRowDisplayed(int rowIndex) const;
        bool isRowsDisplayed(int, int, int=-1, int=-1) const;
        bool isAnyRowDisplayed(int, int, int=-1, int=-1) const;
        Accessor<ns_statistic::tournament_stat_t> statistic() const;
    public slots:
        void reset(gbp_i64 tournament_id);
    protected:
        virtual void onReset() = 0;
    };

} // namespace ns_model
