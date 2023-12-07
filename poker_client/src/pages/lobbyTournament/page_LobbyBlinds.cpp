#include <precompiled.h>
#include "page_LobbyBlinds.h"
#include "ui_page_LobbyBlinds.h"
//#include <models/lobbyTournament/model_LobbyBlinds.h>
#include <api_models/model_tourn_additional_stat_t.hpp>
#include <db/modeldatabase.h>
#include <qidentityproxymodel.h>
#include <qstandarditemmodel.h>

class BlindsProxyModel : public QIdentityProxyModel
{
public:
    BlindsProxyModel(QObject* parent = nullptr)
        : QIdentityProxyModel(parent)
    {}

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (orientation != Qt::Horizontal) {
                    return QVariant();
        }
        if (role != Qt::DisplayRole) {
            return QVariant();
        }
        switch (section) {
        case 0: return QApplication::translate("ns_model::LobbyBlinds", "@level");
        case 1: return QApplication::translate("ns_model::LobbyBlinds", "@blind");
        case 2: return QApplication::translate("ns_model::LobbyBlinds", "@ante");
        case 3: return QApplication::translate("ns_model::LobbyBlinds", "@time");
        default:
            return QVariant();
        }
    }
    virtual QVariant data(const QModelIndex &index, int role) const override {
        QVariant value = QIdentityProxyModel::data(index, role);
        if (value.isValid()) {
            return value;
        }
        if (role == Qt::TextAlignmentRole && index.isValid()) {
            return Qt::AlignCenter;
        }
        return QVariant();
    }
};



namespace ns_page
{
    LobbyBlinds::LobbyBlinds(QWidget *parent)
        : LobbyBase(parent)
        , m_ui(new Ui::LobbyBlinds)
    {
        m_ui->setupUi(this);
    }

    LobbyBlinds::~LobbyBlinds() {
        delete m_ui;
    }

    void LobbyBlinds::internalInit() {
    }

    QAbstractItemView *LobbyBlinds::view() const {
        return m_ui->tableView;
    }
    QAbstractItemModel *LobbyBlinds::model() const {
        return modelsDB().getAdditionalTournStatistic(m_tournamentID);
    }

    QModelIndex LobbyBlinds::rootIndex() const {
        return model()->index(underlying_cast(ns_model::tourn_additional_stat_t::field::_blinds), 0);
    }

    QAbstractProxyModel *LobbyBlinds::createProxyModel() const
    {
        return new BlindsProxyModel;
    }

} //namespace ns_page


