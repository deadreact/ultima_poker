#pragma once

#include <tools/types_fwd.hpp>
#include <interface/initable.h>
#include <QFrame>

class QAbstractItemModel;
class QAbstractItemView;
class QAbstractProxyModel;
namespace ns_page
{
    class LobbyBase : public QFrame
    {
        Q_OBJECT
    public:
//        using tournament_stat_t = gbp::ns_api::ns_statistic::tournament_stat_t;
//        using tourn_additional_stat_t = gbp::ns_api::ns_statistic::tourn_additional_stat_t;
    protected:
        gbp_i64 m_tournamentID;
        QScopedPointer<QAbstractProxyModel> m_proxyModel;
    public:
        explicit LobbyBase(QWidget *parent = nullptr);
        virtual ~LobbyBase();

        void init(gbp_i64 tournament_id);

        virtual QAbstractItemView* view() const = 0;
        virtual QAbstractItemModel* model() const = 0;
        virtual QModelIndex rootIndex() const = 0;

        virtual QAbstractProxyModel* createProxyModel() const;
        virtual QModelIndex proxyRootIndex() const final;
    protected:
        virtual void internalInit() = 0;
        virtual void adjustHeader();
        virtual void showEvent(QShowEvent *e) override;
    protected slots:
        void updateView();
    };


} //namespace ns_page

