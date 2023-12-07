#include <precompiled.h>
#include <qsortfilterproxymodel.h>
#include "page_LobbyPlayers.h"
#include "ui_page_LobbyPlayers.h"
#include <tools/types_fwd.hpp>
#include <db/modeldatabase.h>
#include <models/model_PlayersTable.h>
#include <api_models/model_tournament_stat_t.hpp>


class PlayersProxyModel : public QSortFilterProxyModel
{
public:
    PlayersProxyModel(QObject* parent = nullptr)
        : QSortFilterProxyModel(parent)
    {}
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (orientation != Qt::Horizontal || section > 6) {
            return QSortFilterProxyModel::headerData(section, orientation, role);
        }

        switch (role) {
        case Qt::DisplayRole:
            switch (section) {
            case 0: return QObject::tr("@h_id");
            case 1: return QObject::tr("@h_player");
            case 5: return QObject::tr("@h_money");
            default:
                return QSortFilterProxyModel::headerData(section, orientation, role);
            }
        case Qt::SizeHintRole:
            switch (section) {
            case 0: return QSize(32, 24);
            case 1: return QSize(140, 24);
            case 2: return QSize(32, 24);
            case 3: return QSize(56, 24);
            case 4: return QSize(56, 24);
            default:
                return QVariant();
            }
        default:
            return QSortFilterProxyModel::headerData(section, orientation, role);
        }
    }
};


namespace ns_page
{
    LobbyPlayers::LobbyPlayers(QWidget *parent)
        : LobbyBase(parent)
        , m_ui(new Ui::LobbyPlayers)
//        , m_proxyModel(new QSortFilterProxyModel)
    {
        m_ui->setupUi(this);
    }

    LobbyPlayers::~LobbyPlayers() {
        delete m_ui;
    }

    QAbstractProxyModel *LobbyPlayers::createProxyModel() const
    {
        return new PlayersProxyModel;
    }

    QAbstractItemView *LobbyPlayers::view() const {
        return m_ui->tableView;
    }
    QAbstractItemModel *LobbyPlayers::model() const {
        return playersListModel();// m_proxyModel;
    }

    QModelIndex LobbyPlayers::rootIndex() const {
        return (playersListModel()->root());
    }

    ns_model::players_list *LobbyPlayers::playersListModel() const {
//        static ns_model::players_list plModel;
//        plModel.setCurrentId(m_tournamentID);
//        return &plModel;

                return modelsDB().getTournamentPlayersList(m_tournamentID);
    }

    QSortFilterProxyModel *LobbyPlayers::proxyModel() const {
        return qobject_cast<QSortFilterProxyModel*>(m_proxyModel.data());
    }

    void LobbyPlayers::filterPlayers(const QString &fixedString) {
        qDebug() << "FILTER" << fixedString;
        proxyModel()->setFilterFixedString(fixedString);
    }

    void LobbyPlayers::internalInit()
    {
//        m_proxyModel->setSourceModel(sourceModel());
        proxyModel()->setSortRole(UserType::SortRole);
        proxyModel()->setFilterKeyColumn(1);
        proxyModel()->setFilterRole(UserType::FilterRole);
        proxyModel()->setFilterCaseSensitivity(Qt::CaseInsensitive);
    }

    void LobbyPlayers::showEvent(QShowEvent *e)
    {
        LobbyBase::showEvent(e);
        uint types = qvariant_cast<uint>(modelsDB().getTournamentStatistic()->indexOf(m_tournamentID, ns_model::tournament_stat_t::column::_types));
        m_ui->tableView->horizontalHeader()->setSectionHidden(3, !(types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_bounty));
        m_ui->tableView->horizontalHeader()->setSectionHidden(4, !(types & gbp::ns_api::ns_tournament_types::e_chips_rebuy::cr_progressive));
    }

    void LobbyPlayers::adjustHeader()
    {
        m_ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for (int i = 0; i < m_proxyModel->columnCount(m_proxyModel->mapFromSource(rootIndex())); i++) {
            m_ui->tableView->horizontalHeader()->resizeSection(i, proxyModel()->headerData(i, Qt::Horizontal, Qt::SizeHintRole).toSize().width());
        }
    }
} //namespace ns_page
