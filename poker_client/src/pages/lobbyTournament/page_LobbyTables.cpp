#include <precompiled.h>
#include <qidentityproxymodel.h>
#include "page_LobbyTables.h"
#include "ui_page_LobbyTables.h"
#include <api_models/model_tourn_additional_stat_t.hpp>
#include <db/modeldatabase.h>
#include <api_models/itemtype_concept.hpp>

#include <api/lobby_stat/types.h>

Q_DECLARE_METATYPE(ns_statistic::tourn_additional_stat_t)

class TablesProxyModel : public QIdentityProxyModel
{
public:
    TablesProxyModel(QObject* parent = nullptr)
        : QIdentityProxyModel(parent)
    {}
    virtual int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override {
        return 4;
    }
    virtual QVariant data(const QModelIndex& idx, int role) const override
    {
        if (!idx.isValid()) {
            return QVariant();
        }
        if (!idx.parent().isValid() || idx.parent().internalPointer() != nullptr) {
            return QIdentityProxyModel::data(idx, role);
        }

        auto minf = [](const ns_statistic::tourn_additional_stat_t::game& game){
            gbp_i64 min = gbp_u32(-1);
            for (const auto& pl: game._players) {
                if (pl._stack < min) {
                    min = pl._stack;
                }
            }
            return ns_meta::money_type::display(min);
        };
        auto maxf = [](const ns_statistic::tourn_additional_stat_t::game& game){
            gbp_i64 max = 0;
            for (const auto& pl: game._players) {
                if (pl._stack > max) {
                    max = pl._stack;
                }
            }
            return ns_meta::money_type::display(max);
        };

        if (ns_model::tourn_additional_stat_t* m = qobject_cast<ns_model::tourn_additional_stat_t*>(sourceModel())) {
             ns_statistic::tourn_additional_stat_t d = qvariant_cast<ns_statistic::tourn_additional_stat_t>(m->originData());
             if (d._games.size() <= idx.row()) {
                 return QVariant();
             }
             if (role == Qt::DisplayRole) {
                 switch (idx.column()) {
                 case 0: return d._games.at(idx.row())._id;
                 case 1: return QVariant::fromValue(d._games.at(idx.row())._players.size());
                 case 2: return minf(d._games.at(idx.row()));
                 case 3: return maxf(d._games.at(idx.row()));
                 default:
                     return QVariant();
                 }
             }
        }
        return QIdentityProxyModel::data(idx, role);
    }
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        switch (section) {
        case 0: return QApplication::translate("ns_model::LobbyTables", "@name");
        case 1: return QApplication::translate("ns_model::LobbyTables", "@players");
        case 2: return QApplication::translate("ns_model::LobbyTables", "@min_stack");
        case 3: return QApplication::translate("ns_model::LobbyTables", "@max_stack");
        default:
            return QVariant();
        }
    }
};


namespace ns_page
{
    LobbyTables::LobbyTables(QWidget *parent)
        : LobbyBase(parent)
        , m_ui(new Ui::LobbyTables)
    {
        m_ui->setupUi(this);
    }

    LobbyTables::~LobbyTables() {
        delete m_ui;
    }

    void LobbyTables::internalInit()
    {
//        connect()
    }

    QAbstractItemView *LobbyTables::view() const {
        return m_ui->tableView;
    }
    QAbstractItemModel *LobbyTables::model() const {
        return modelsDB().getAdditionalTournStatistic(m_tournamentID);
    }

    QModelIndex LobbyTables::rootIndex() const {
        return model()->index(underlying_cast(ns_model::tourn_additional_stat_t::field::_games), 0);
    }

    QAbstractProxyModel* LobbyTables::createProxyModel() const {
        return new TablesProxyModel;
    }

} //namespace ns_page
