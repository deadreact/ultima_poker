#include <precompiled.h>
#include "page_Qualifiers.h"
#include "ui_page_Qualifiers.h"

#include <models/lobbyTournament/model_QualifiersTable.h>

#include <qsortfilterproxymodel.h>

namespace ns_page
{
    Qualifiers::Qualifiers(QWidget *parent)
        : LobbyBase(parent)
        , m_ui(new Ui::Qualifiers)
        , m_model(new ns_model::QualifiersTable)
    {
        m_ui->setupUi(this);
    }

    Qualifiers::~Qualifiers() {
        delete m_ui;
    }

    QAbstractItemView *Qualifiers::view() const {
        return m_ui->tableView_qualifiers;
    }
    QAbstractItemModel *Qualifiers::model() const {
        return m_model;
    }
    QModelIndex Qualifiers::rootIndex() const {
        return QModelIndex();
    }

    void Qualifiers::internalInit()
    {
        m_model->reset(m_tournamentID);
        QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel;
        proxyModel->setSourceModel(m_model);
        proxyModel->setSortRole(UserType::SortRole);
//        m_ui->tableView_qualifiers->setModel(proxyModel);

        for (int i = 0; i < m_model->rowCount(); i++) {
            m_ui->tableView_qualifiers->horizontalHeader()->resizeSection(i, m_model->headerData(i, Qt::Horizontal, Qt::SizeHintRole).toSize().width());
        }
        m_ui->header_qualifiers->setText(tr("@qualifiers_%0").arg(m_model->rowCount()));
    }

    int Qualifiers::headerHeight() const
    {
        return (m_ui && m_ui->header_qualifiers) ? m_ui->header_qualifiers->height() : -1;
    }
} //namespace ns_page




