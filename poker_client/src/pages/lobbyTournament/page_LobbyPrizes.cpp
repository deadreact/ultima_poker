#include <precompiled.h>
#include "page_LobbyPrizes.h"
#include "ui_page_LobbyPrizes.h"

#include <api_models/model_tourn_additional_stat_t.hpp>
#include <db/modeldatabase.h>
#include <qidentityproxymodel.h>

class PrizesProxyModel : public QIdentityProxyModel
{
public:
    PrizesProxyModel(QObject* parent = nullptr)
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
        case 0: return QApplication::translate("ns_model::LobbyPrizes", "@position");
        case 1: return QApplication::translate("ns_model::LobbyPrizes", "@persentage");
        case 2: return QApplication::translate("ns_model::LobbyPrizes", "@prize");
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
    LobbyPrizes::LobbyPrizes(QWidget *parent)
        : LobbyBase(parent)
        , m_ui(new Ui::LobbyPrizes)
    {
        m_ui->setupUi(this);
    }

    LobbyPrizes::~LobbyPrizes() {
        delete m_ui;
    }

    void LobbyPrizes::internalInit()
    {
    }

    QAbstractProxyModel *LobbyPrizes::createProxyModel() const
    {
        return new PrizesProxyModel;
    }

    QAbstractItemView *LobbyPrizes::view() const {
        return m_ui->tableView;
    }
    QAbstractItemModel *LobbyPrizes::model() const {
        return modelsDB().getAdditionalTournStatistic(m_tournamentID);
    }

    QModelIndex LobbyPrizes::rootIndex() const {
        return model()->index(underlying_cast(ns_model::tourn_additional_stat_t::field::_prizes), 0);
    }

} //namespace ns_page

