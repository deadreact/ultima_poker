#include <precompiled.h>
#include <qstandarditemmodel.h>
#include "page_MainLobbyBase.h"
#include <models/model_GamesStatistic.h>
#include <widgets/StatisticTableView.h>
#include <delegates/delegate_GameTableComboBoxItem.h>
#include <delegates/delegate_customcolumn.h>


class FilterItem : public QStandardItem {
public:
    FilterItem(const QString& text = QString())
        : QStandardItem(text)
    {}
    void setFilter(UserType::FilterFlags filter) {
        setData(QVariant::fromValue(filter), UserType::FilterRole);
    }
    UserType::FilterFlags filter() const {
        return data(UserType::FilterRole).value<UserType::FilterFlags>();
    }
};


namespace ns_page
{
    MainLobbyBase::MainLobbyBase(UserType::FilterFlags mask, QWidget *parent)
        : QFrame(parent)
        , m_mask(mask)
        , m_filter(mask)
    {}

    MainLobbyBase::~MainLobbyBase()
    {}

    void MainLobbyBase::init()
    {       
        if (QSortFilterProxyModel* proxy = proxyModel()) {
            proxy->setSortRole(UserType::SortRole);
        }

        initTableView();
        tableView()->setItemDelegateForColumn(0, new ns_delegate::CustomColumn(24));

//        tableView()->setRootModel(m_gamesStatModel);
//        tableView()->setProxyModel(proxyModel());
//        tableView()->setModel(proxyModel());

//        resetFilter();

    }

    void MainLobbyBase::initFilterBox(QComboBox *box, const QStringList &txtlst, const FiltersList &filters)
    {
        QStandardItemModel* m = new QStandardItemModel(txtlst.size() + 1, 1);

        box->setModel(m);
        FilterItem* rootItem = new FilterItem(tr("@all"));
        rootItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        rootItem->setCheckState(Qt::Checked);
        rootItem->setSizeHint(QSize(230, 18));
        rootItem->setBackground(QColor(0xa2afb5));
        rootItem->setForeground(QColor(0x191919));

        box->view()->setItemDelegateForRow(0, new ns_delegate::FilterItem);
        m->setItem(0, 0, rootItem);

        UserType::FilterFlags filterMask = UserType::EFilters::AllUnchecked;
        for (int i = 0; i < txtlst.size(); i++) {
            FilterItem* item = new FilterItem(txtlst.at(i));
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Checked);
            filterMask |= filters.at(i);
            item->setFilter(filters.at(i));
            item->setSizeHint(QSize(200, 18));
            item->setBackground(QColor(0x889aa3));
            item->setForeground(QColor(0x191919));
            m->setItem(i+1, 0, item);
        }

        rootItem->setFilter(filterMask);

        connect(m, &QStandardItemModel::itemChanged, [this, m, rootItem](QStandardItem* item){
            if (item != rootItem)
            {
                UserType::FilterFlags mask = UserType::EFilters::AllUnchecked;
                UserType::FilterFlags filterMask = UserType::EFilters::AllUnchecked;

                bool allChecked = true;
                QString longName = "";
                for (int row = 1; row < m->rowCount(); row++) {
                    FilterItem* filterItem = dynamic_cast<FilterItem*>(m->item(row));
                    mask |= filterItem->filter();
                    if (filterItem->checkState() == Qt::Checked) {
                        filterMask |= filterItem->filter();
                        longName += (longName.isEmpty() ? QString("") : QString(", ")) + filterItem->text();
                    } else {
                        allChecked = false;
                    }
                }
                if (filterMask != rootItem->filter()) {
                    rootItem->setFilter(filterMask);
                    rootItem->setText(allChecked ? tr("@all") : longName);
                    rootItem->setCheckState(allChecked ? Qt::Checked : Qt::Unchecked);

                    updateFilters();
                }
            }
        });

        static auto isRoot = [](const QModelIndex& idx) {
            return idx.isValid() && idx.row() == 0 && idx.column() == 0 && !idx.parent().isValid();
        };

        QAbstractItemView* view = box->view();
        connect(view, &QAbstractItemView::clicked, this, [view](const QModelIndex& index) {
            if (isRoot(index) && index.data(Qt::CheckStateRole).value<Qt::CheckState>() == Qt::Checked) {
                QAbstractItemModel* model = view->model();
                for (int row = 1; row < model->rowCount(); row++) {
                    QModelIndex modelIndex = model->index(row, 0);
                    model->setData(modelIndex, Qt::Checked, Qt::CheckStateRole);
                }
            }
        });
    }

    QSortFilterProxyModel *MainLobbyBase::proxyModel() const {
        return qobject_cast<QSortFilterProxyModel*>(tableView()->model());
    }

    QAbstractItemModel *MainLobbyBase::sourceModel() const {
        if (QSortFilterProxyModel* proxy = proxyModel()) {
            return proxy->sourceModel();
        }
        return nullptr;
    }

    void MainLobbyBase::resetFilter() {
        setFilter(m_mask);
    }

    void MainLobbyBase::setFilter(UserType::EFilters mask)
    {
        if (QSortFilterProxyModel* proxy = proxyModel())
        {
            m_filter = m_mask & mask;
            proxy->setFilterRegExp(UserType::toRegExp(m_filter));
            proxy->setFilterRole(UserType::FilterRole);
        }
    }
    void MainLobbyBase::setFilter(UserType::FilterFlags mask)
    {
        if (QSortFilterProxyModel* proxy = proxyModel())
        {
            m_filter = m_mask & mask;
            proxy->setFilterRegExp(UserType::toRegExp(m_filter));
            proxy->setFilterRole(UserType::FilterRole);
        }
    }


} // namespace page

TableView::TableView(QWidget *parent): QTableView(parent) {}

void TableView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
    QTableView::currentChanged(current, previous);
    emit currentChanged(current);
}
