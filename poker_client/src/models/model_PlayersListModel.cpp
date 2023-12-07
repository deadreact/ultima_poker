#include <precompiled.h>
#include "model_PlayersListModel.h"

#include <QPixmap>
#include <qpushbutton.h>
#include "db/Database.h"
#include <api/lobby_stat/types.h>

namespace ns_model
{
    PlayersListModel::PlayersListModel(const gbp::ns_api::ns_statistic::tournament_stat_t *data, QObject *parent)
        : QAbstractTableModel(parent)
        , m_currentData(data)
    {
        if (data == nullptr) {
            m_data.resize(9);
        } else {
            updateData(data);
        }
    }

    void PlayersListModel::updateData(const gbp::ns_api::ns_statistic::tournament_stat_t *data)
    {
        beginResetModel();
        m_currentData = data;
        m_data.clear();
        m_data.resize(std::max(9, (int)data->_users.size()));
        if (data) {
            for (size_t i = 0; i < data->_users.size(); i++)
            {
                const gbp::ns_api::ns_statistic::tournament_stat_t::user_info& info = data->_users[i];
                m_data[i].set(info);
            }
        }
        endResetModel();

    }

    void PlayersListModel::updateData(const gbp::ns_api::ns_statistic::cash_stat_t *data)
    {
        beginResetModel();
        m_data.clear();
        if (data) {
            m_data.resize(std::max(9, (int)data->_players.size()));
            for (size_t i = 0; i < data->_players.size(); i++)
            {
                const gbp::ns_api::ns_statistic::player_t& info = data->_players[i];
                m_data[i].set(info._nick.c_str(), QLocale::Country(info._country), info._money);
            }
        } else {
            m_data.resize(9);
        }
        endResetModel();
    }

    int PlayersListModel::rowCount(const QModelIndex &/*parent*/) const
    {
        return std::max(9, (int)m_data.size());
    }

    QVariant PlayersListModel::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();
//        if (m_data.size() >= index.row() && index.row() <= 9) {
//            return (role == Qt::DisplayRole) ? "" : QVariant();
//        }
        const PlayersListDataItem& item = m_data.at(index.row());
        if (role == Qt::DisplayRole || role == Qt::ToolTipRole)
        {
            switch (index.column()) {
                case 0: return index.row() + 1;
                case 1: return item.m_nickname;
                case 3: return ((item.m_stack.isPositive()) ? item.m_stack.toString(Money::DisplayType::Short) : QVariant());
                case 2: /*return "";*/
                default: return QVariant();
            }
        }
        else if (role == Qt::DecorationRole)
        {
            switch (index.column()) {;
                case 2: return QPixmap(QString(":textures/Common/icons/ico/%0.ico").arg(QLocale::countryToString(QLocale::Country(item.m_country))));
//                case 1: return QPixmap(":textures/Common/icons/icon_2x4_types").copy(0, 20, 20, 20);
//                case 3:
                default: return QVariant();
            }
        }
//        else if (role == Qt::ForegroundRole)
//        {
//            if (index.column() == 3) {
//                return QColor(227, 218, 145);
//            }
//        }
        return QVariant();
    }

    int PlayersListModel::columnCount(const QModelIndex &/*parent*/) const
    {
        return 4;
    }

    QVariant PlayersListModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation == Qt::Vertical || section > 3) {
            return QVariant();
        }
        QStringList str({tr("@h_num"), tr("@h_player"), tr("@h_country"), tr("@h_stack")});

        static const int sizes[] = {2, 176, 52, 100};
        switch (role) {
        case Qt::DisplayRole:
            if (section == 2) {
                break;
            }
            return QVariant();
        case Qt::ToolTipRole:
            return str[section];
        case Qt::DecorationRole:
            if (section == 2) {
                return QPixmap(":/textures/Common/icons/icon_geo.png");
            }
            return QVariant();
        case Qt::SizeHintRole:
            return QSize(sizes[section], 24);
        default:
            break;
        }

        return QVariant();
    }

    void PlayersListModel::fetchAllPlayers()
    {
        if (m_data.size() != m_currentData->_users.size())
        {
            int diff = m_currentData->_users.size() - m_data.size();
            if (diff > 0)
            {
                beginInsertRows(QModelIndex(), m_data.size(), m_data.size() + diff);

                m_data.resize(m_currentData->_users.size());
                for (size_t i = m_data.size(); i < m_currentData->_users.size(); i++)
                {
                    const gbp::ns_api::ns_statistic::tournament_stat_t::user_info& info = m_currentData->_users[i];
                    m_data[i].set(info);
                }

                endInsertRows();
            }
        }
    }


//    bool PlayersListModel::insertRows(int row, int count, const QModelIndex &parent)
//    {
//        beginInsertRows(parent, row, row + count);

//        endInsertRows();
//    }

} // namespace ns_model


