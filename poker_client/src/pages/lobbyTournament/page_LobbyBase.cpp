#include <precompiled.h>
#include "page_LobbyBase.h"
#include <QTableView>
#include <QHeaderView>
#include <basewidgets/types.h>
#include <db/modeldatabase.h>

#include <api_models/model_tournament_stat_t.hpp>

#include <QTimer>
#include <qidentityproxymodel.h>

namespace ns_page
{
    LobbyBase::LobbyBase(QWidget *parent)
        : QFrame(parent)
        , m_tournamentID(-1)
    {}

    LobbyBase::~LobbyBase() {}

    void LobbyBase::init(gbp_i64 tournament_id)
    {
        m_proxyModel.reset(createProxyModel());

        if (tournament_id != m_tournamentID) {
            m_tournamentID = tournament_id;
        }
        view()->setModel(m_proxyModel.data());

        connect(m_proxyModel.data(), &QAbstractProxyModel::modelReset, this, &LobbyBase::updateView);
        connect(m_proxyModel.data(), &QAbstractProxyModel::sourceModelChanged, this, &LobbyBase::updateView);
        connect(m_proxyModel.data(), &QAbstractProxyModel::rowsInserted, this, [this](const QModelIndex& parent, int, int){
            if (parent == m_proxyModel->mapFromSource(rootIndex())) {
                updateView();
            }
        });
        connect(m_proxyModel.data(), &QAbstractProxyModel::rowsRemoved, this, [this](const QModelIndex& parent, int, int){
            if (parent == m_proxyModel->mapFromSource(rootIndex())) {
                updateView();
            }
        });
        connect(m_proxyModel.data(), &QAbstractProxyModel::columnsInserted, this, [this](const QModelIndex& parent, int, int){
            if (parent == m_proxyModel->mapFromSource(rootIndex())) {
                updateView();
            }
        });
        connect(m_proxyModel.data(), &QAbstractProxyModel::columnsRemoved, this, [this](const QModelIndex& parent, int, int){
            if (parent == m_proxyModel->mapFromSource(rootIndex())) {
                updateView();
            }
        });

        internalInit();
    }

    QAbstractProxyModel *LobbyBase::createProxyModel() const {
        return new QIdentityProxyModel;
    }

    QModelIndex LobbyBase::proxyRootIndex() const {
        return m_proxyModel->mapFromSource(rootIndex());
    }

    void LobbyBase::adjustHeader()
    {
        QTableView* tableView = qobject_cast<QTableView*>(view());
        if (!tableView) {
            return;
        }
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for (int i = 0; i < m_proxyModel->columnCount(m_proxyModel->mapFromSource(rootIndex())); i++) {
            tableView->horizontalHeader()->resizeSection(i, (width() - 2) / qMax(1, m_proxyModel->columnCount(m_proxyModel->mapFromSource(rootIndex()))));
        }
    }

    void LobbyBase::showEvent(QShowEvent *e)
    {
        updateView();

        QFrame::showEvent(e);
    }

    void LobbyBase::updateView()
    {
        if (view()->model() != m_proxyModel.data()) {
            view()->setModel(m_proxyModel.data());
        }
        if (m_proxyModel->sourceModel() != model()) {
            m_proxyModel->setSourceModel(model());
        }
        if (proxyRootIndex() != view()->rootIndex()) {
            view()->setRootIndex(proxyRootIndex());
        }

        if (QTableView* tableView = qobject_cast<QTableView*>(view())) {
            adjustHeader();
            for (int i = 0; i < m_proxyModel->rowCount(m_proxyModel->mapFromSource(rootIndex())); i++) {
                tableView->setRowHidden(i, !m_proxyModel->mapFromSource(model()->index(i, 0, rootIndex())).data(UserType::VisibilityRole).toBool());
            }
        }
    }

} //namespace ns_page

