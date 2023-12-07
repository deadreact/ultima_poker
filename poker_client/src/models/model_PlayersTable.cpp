#include <precompiled.h>
#include "model_PlayersTable.h"

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/request_statistic.h>
    #include <rpc/rpc_common.h>
    #include <rpc/rpc_statistic.h>
#endif // Q_MOC_RUN

#include <QPixmap>
#include <api_models/itemtype_concept.hpp>
#include <qpushbutton.h>
#include <db/Database.h>
#include <db/SharedDataService.h>
#include <db/modeldatabase.h>
#include <utils/IconProvider.h>
#include <core/money.h>
#include <PokerApplication.h>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/api_util.hpp>
#include <api_models/defs_metatype.hpp>
#include <tools/item_reference.hpp>

Q_DECLARE_METATYPE(std::vector<gbp::ns_api::ns_statistic::tournament_stat_t::user_info>)

namespace ns_model
{
    bool players_list::addPlayer(const tourn_user_info_t &pl)
    {
        if (m_data.contains(pl._id)) {
            updatePlayer(pl);
            return false;
        }
//        beginInsertRows(index(0, 0, QModelIndex()), m_data.size(), m_data.size());
        Player player(pl._nickname.c_str());
        player._money   = pl._money;
        player._country = pl._country;
        player._flags   = (pl._is_ticket) ? Player::_is_ticket : Player::_no_flag;
        if (pl._bounty_progr_money != 0) {
            player._bounty_progr_money = pl._bounty_progr_money;
        } else if (pl._bounty_knocked_out_count != 0) {
            player._bounty_knocked_out_count = pl._bounty_knocked_out_count;
        }
        m_data.insert(pl._id, player);
//        endInsertRows();
        return true;
    }

    void players_list::updatePlayer(const tourn_user_info_t &pl)
    {
        setData(index(pl._id, 3), pl._bounty_knocked_out_count, UserType::OriginDataRole);
        setData(index(pl._id, 4), pl._bounty_progr_money,       UserType::OriginDataRole);
        setData(index(pl._id, 5), pl._money,                    UserType::OriginDataRole);
    }

    QModelIndex players_list::index(gbp_i64 playerId, int column) const
    {
        for (int row = 0; row < m_positions.size(); row++) {
            if (m_positions[row] == playerId) {
                return index(row, column);
            }
        }
        return QModelIndex();
    }

    void players_list::updatePositions()
    {
        QList<gbp_i64> positions = m_data.uniqueKeys();
        std::sort(positions.begin(), positions.end(), [this](gbp_i64 l, gbp_i64 r){ return m_data[l]._money > m_data[r]._money; });

        if (m_positions.size() < positions.size()) {
            QSet<gbp_i64> posSet1 = m_positions.toSet();
            QSet<gbp_i64> posSet2 = positions.toSet();
            QSet<gbp_i64> sub = posSet2.subtract(posSet1);
            m_positions.append(sub.toList());
        }

        for (int row = 0; row < positions.size(); row++)
        {
            gbp_i64 id = positions.at(row);
            if (m_positions.at(row) != id) {
                int pos = index(id).row();
                if (pos != -1) {
                    beginMoveRows(index(0, 0, QModelIndex()), pos, pos,index(0, 0, QModelIndex()), row);
                    while (pos != row) {
//                        qDebug() << "move" << pos << "->" << row;
                        std::swap(m_positions[pos], m_positions[pos-1]);
                        pos--;
                    }
                    endMoveRows();
                }
            }
        }
        m_positions = positions;
    }

players_list::players_list(QObject *parent)
        : QAbstractTableModel(parent)
        , m_id(-1)
        , m_colsType(Common)
        , m_fetched(false)
{}

bool players_list::isRoot(const QModelIndex &idx) const
{
    return idx.isValid() && idx.row() == 0 && idx.column() == 0 && idx.internalId() == quintptr(m_id);
}

QModelIndex players_list::root() const {
    return createIndex(0, 0, m_id);
}


    gbp_i64 players_list::currentId() const {
        return m_id;
    }

    bool players_list::setCurrentId(gbp_i64 id)
    {
        ns_model::tournament_stat_t* model = modelsDB().getTournamentStatistic();

        if (id != m_id) {
            beginResetModel();
            m_id = id;
            m_fetched = false;
            m_data.clear();
            m_colsType = Common;
            if (currentTournStat()->_types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_bounty) {
                m_colsType = Bounty;
            } else if (currentTournStat()->_types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_progressive) {
                m_colsType = Progressive;
            }
            if (model->usersRootOf(id).isValid()) {
                for (const auto& pl: model->item(id).ref()._users) {
                    addPlayer(pl);
                }
            }
            updatePositions();
            endResetModel();
            return true;
//            if (!m_fetched) {
//                fetchMore();
//            }
        }
        return false;
    }

    Accessor<ns_statistic::tournament_stat_t> players_list::currentTournStat() const {
        return modelsDB().getTournamentStatistic()->item(m_id);
    }


} // namespace ns_model



int ns_model::players_list::rowCount(const QModelIndex &parent) const
{
    if (isRoot(parent)) {
        return m_data.size();
    }
    if (!parent.isValid()) {
        return 1;
    }
    return 0;
}

int ns_model::players_list::columnCount(const QModelIndex &/*parent*/) const
{
    switch (m_colsType) {
    case Common:
        return 4;
    case Bounty:
    case Progressive:
        return 5;
    case All:
        return 6;
    default:
        return 0;
    }
//    if (isRoot(parent)) {
//        return 6;
//    }
//    if (!parent.isValid()) {
//        return 1;
//    }
//    return 0;
}


QVariant ns_model::players_list::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.column() == 3) {
        if (m_colsType == Common) {
            return index.sibling(index.row(), 5).data(role);
        }
        if  (m_colsType == Progressive) {
            return index.sibling(index.row(), 4).data(role);
        }
    }
    if (index.column() == 4) {
        if (m_colsType == Common) {
            return QVariant();
        }
        if  (m_colsType == Bounty) {
            return index.sibling(index.row(), 5).data(role);
        }
    }

    auto item = m_data.value(m_positions.at(index.row()));

    TournamentRef itemRef(currentTournStat().ref());

    static_check(itemRef._item);

    if (role == UserType::VisibilityRole) {
        switch (index.column()) {
        case 3: return itemRef._types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_bounty;// item._bounty_knocked_out_count > 0;
        case 4: return itemRef._types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_progressive;
        default:
            return true;
        }
        return true;
    }
    if (isRoot(index))
    {
        if (index.row() == 0 && index.column() == 0) {
            switch (role) {
            case UserType::RowItemIdRole: return m_id;
            default:
                return QVariant();
            }
        }
        return QVariant();
    }
    if (!isRoot(index.parent())) {
        return QVariant();
    }


    int column = 0;
    switch (index.column()) {
    case 0:
        if (role == Qt::DisplayRole) {
            return index.row() + 1;
        }
        break;
    case 1: column = underlying_cast(tournament_stat_t::users::_nickname); break;
    case 2: column = underlying_cast(tournament_stat_t::users::_country);  break;
    case 3: column = underlying_cast(tournament_stat_t::users::_bounty_knocked_out_count); break;
    case 4: column = underlying_cast(tournament_stat_t::users::_bounty_progr_money); break;
    case 5: column = underlying_cast(tournament_stat_t::users::_money);    break;
    default:
        return QVariant();
    }

    switch (role) {
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
    case UserType::FilterRole:
    {
        return index.data(Qt::DisplayRole).toString().trimmed();
    }
    case UserType::SortRole:
        return item._money;
    case Qt::ForegroundRole: {
        QColor col = QColor(0xb0bec5);
//        if (sourceIndex.data(UserType::OriginDataRole).value<gbp_i64>() == 0) {
//            col.setAlphaF(0.5f);
//        }
        return col;
    }
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return index.row() + 1;
        case 1: return item._nickname;
        case 2: return QVariant();
        case 3: return ns_meta::money_type::display(item._bounty_knocked_out_count);
        case 4: return ns_meta::money_type::display(item._bounty_progr_money);
        case 5: return ns_meta::money_type::display(item._money);
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        if (index.column() == 2) {
            return pApp->iconProvider().countryIcon(QLocale::Country(item._country));
        }
        return QVariant();
    default:
        return QVariant();
    }
    return QVariant();
}

bool ns_model::players_list::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }
    if (!index.parent().isValid()) {
        if (index.row() == 0 && index.column() == 0) {
            switch (role) {
            case UserType::RowItemIdRole:
                return value.canConvert<gbp_i64>() && setCurrentId(value.value<gbp_i64>());
            default:
                return false;
            }
        }
    }
    if (index.parent().parent().isValid() || index.parent().data(UserType::RowItemIdRole).value<gbp_i64>() != m_id) {
        return false;
    }
    if (data(index, role) != value) {
        auto val = modelsDB().getTournamentStatistic()->item(m_id).ref();

        switch (role) {
        case UserType::OriginDataRole:
            switch (index.column()) {
            case 3:
                if (true) {
                    m_data[index.data(UserType::RowItemIdRole).value<gbp_i64>()]._money = value.value<gbp_i64>();
                    updatePositions();
                    emit dataChanged(index, index);
                    return true;
                }
                return false;
            case 4:
                if (val._types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_bounty) {
                    m_data[index.data(UserType::RowItemIdRole).value<gbp_i64>()]._bounty_knocked_out_count = value.toInt();
                    emit dataChanged(index, index);
                    return true;
                }
                return false;
            case 5:
                if (val._types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_progressive) {
                    m_data[index.data(UserType::RowItemIdRole).value<gbp_i64>()]._bounty_progr_money = value.value<gbp_i64>();
                    emit dataChanged(index, index);
                    return true;
                }
                return false;
            default:
                return false;
            }
        default:
            return false;
        }
    }

    return false;
}


void ns_model::players_list::fetchMore(const QModelIndex &parent)
{
    if (!canFetchMore(parent)) {
        return;
    }
    rpc_statistic* rpc = rpc_object<rpc_statistic>();
    gbp_i64 id = m_id;
    rpc->get_tourn_users_stat(id
    , [this, id, parent](ns_statistic::e_status status, const std::vector<tourn_user_info_t>& vec) {
        if (ns_helpers::isOk(status)) {
            int rCount = rowCount(root());
            if (static_cast<int>(vec.size()) > rCount)
            {
                tournament_stat_t* sourceModel = modelsDB().getTournamentStatistic();
                QModelIndex sourceParent = sourceModel->indexOf(id, tournament_stat_t::column::_users);
                if (!sourceParent.isValid()) {
                    return;
                }
                if (rCount < 10) {
                    beginResetModel();
                    resetInternalData();
                } else {
                    beginInsertRows(root(), rCount, vec.size()-1);
                }
                for (const auto& player: vec) {
                    addPlayer(player);
                }
                m_fetched = true;
//                if (sourceModel->setData(sourceParent, QVariant::fromValue(vec), UserType::OriginDataRole)) {

//                }
                updatePositions();
                if (rCount <= 10) {
                    endResetModel();
                } else {
                    endInsertRows();
                }
            }
        }
    });
}

bool ns_model::players_list::canFetchMore(const QModelIndex &parent) const
{
    if (isRoot(parent)) {
        return currentTournStat().isValid() && rowCount(parent) < currentTournStat()->_players_registered_count;
    }
    return false;
}

//QString headerText(int section)
//{
//    switch (section) {
//    case 0: return ns_model::players_list::tr("@h_num");
//    case 1: return ns_model::players_list::tr("@h_player");
//    case 5: return ns_model::players_list::tr("@h_stack");
//    default:
//        return QString();
//    }
//}

QVariant ns_model::players_list::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        switch (section) {
            case 0: return tr("@h_num");
            case 1: return tr("@h_player");
            case 5: return tr("@h_stack");
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        switch (section) {
        case 2: return QIcon(":/textures/Common/icons/icon_geo.png");
        case 3:
        case 4: return QIcon(":/textures/Common/Tournament/icon_ko.png");
        default:
            return QVariant();
        }
    case Qt::SizeHintRole:
        switch (section) {
        case 0: return QSize(30, 24);
        case 1: return QSize(120, 24);
        case 2: return QSize(50, 24);
        case 3: return QSize(50, 24);
        case 4: return QSize(50, 24);
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }

    return QVariant();
}


QModelIndex ns_model::players_list::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        if (row != 0 || column != 0) {
            return QModelIndex();
        }
        return root();
    }
    if (columnCount(parent) > column && rowCount(parent) > row) {
        return createIndex(row, column, ((row << columnCount(parent)) + column));
    }
    return QModelIndex();
}

QModelIndex ns_model::players_list::parent(const QModelIndex &child) const
{
    if (!child.isValid()) {
        return QModelIndex();
    }

    if (child.internalId() == ((child.row() << columnCount(root())) + child.column())) {
        return root();
    }
    if (isRoot(child)) {
        return QModelIndex();
    }
    return QModelIndex();
}

bool ns_model::players_list::hasChildren(const QModelIndex &parent) const
{
    return !parent.isValid() || isRoot(parent);
}


//QModelIndex ns_model::players_list::mapToSource(const QModelIndex &proxyIndex) const
//{
//    if (!proxyIndex.isValid() || proxyIndex.model() != this) {
//        return QModelIndex();        // QAbstractProxyModel interface
//        public:
//            virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
//            virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
//    }
//    ns_model::tournament_stat_t* model = modelsDB().getTournamentStatistic();
//    if (sourceModel() != model) {
//        return QModelIndex();
//    }
//    if (isRoot(proxyIndex)) {
//        return model->usersRootOf(currentId());
//    }
//    if (proxyIndex.internalId() == ((proxyIndex.row() << columnCount(proxyIndex.parent())) + proxyIndex.column())) {
//        auto it = m_data.begin();
//        for (int i = 0; i < proxyIndex.row(); i++, it++);
//        gbp_i64 userId = it.key();

//        switch (proxyIndex.column()) {
//        case 0: return QModelIndex();
//        case 1: return model->indexOfUser(m_id, userId, tournament_stat_t::users::_nickname);
//        case 2: return model->indexOfUser(m_id, userId, tournament_stat_t::users::_country);
//        case 3: return model->indexOfUser(m_id, userId, tournament_stat_t::users::_bounty_knocked_out_count);
//        case 4: return model->indexOfUser(m_id, userId, tournament_stat_t::users::_bounty_progr_money);
//        case 5: return model->indexOfUser(m_id, userId, tournament_stat_t::users::_money);
//        default:
//            return QModelIndex();
//        }
//    }
//    return QModelIndex();
//}

//QModelIndex ns_model::players_list::mapFromSource(const QModelIndex &sourceIndex) const
//{
//    if (!sourceIndex.isValid() || sourceIndex.model() != modelsDB().getTournamentStatistic()) {
//        return QModelIndex();
//    }
//    ns_model::tournament_stat_t* model = modelsDB().getTournamentStatistic();
//    if (sourceModel() != model) {
//        return QModelIndex();
//    }
//    if (sourceIndex.internalPointer() == nullptr && sourceIndex.data(UserType::RowItemIdRole).value<gbp_i64>() == m_id) {
//        return index(0, 0, QModelIndex());
//    }
//    if (sourceIndex.parent() == model->usersRootOf(m_id))
//    {
//        gbp_i64 userId = sourceIndex.data(UserType::RowItemIdRole).value<gbp_i64>();

//        if (!m_data.contains(userId)) {
//            return QModelIndex();
//        }
//        switch (tournament_stat_t::users(sourceIndex.column())) {
//        case tournament_stat_t::users::_nickname:                 return index(userId, 1);
//        case tournament_stat_t::users::_country:                  return index(userId, 2);
//        case tournament_stat_t::users::_bounty_knocked_out_count: return index(userId, 3);
//        case tournament_stat_t::users::_bounty_progr_money:       return index(userId, 4);
//        case tournament_stat_t::users::_money:                    return index(userId, 5);
//        default:
//            return QModelIndex();
//        }
//    }
//    return QModelIndex();
//}
