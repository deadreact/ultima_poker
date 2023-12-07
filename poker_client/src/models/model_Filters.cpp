#include <precompiled.h>
#include "model_Filters.h"


namespace ns_model
{

    FiltersItem::FiltersItem(UserType::FilterFlags mask, const QString &text)
        : QStandardItem(text)
        , m_filter(mask)
        , m_mask(mask)
        , m_isCheckedByUser(true)
    {}

    FiltersItem::FiltersItem(const FiltersItem &item)
        : QStandardItem(item)
        , m_filter(item.m_filter)
    {}

    QStandardItem *FiltersItem::clone() const {
        return new FiltersItem(*this);
    }


    FiltersItem *Filters::rootItem() const {
        return m_rootItem;
    }

    void Filters::updateFilter()
    {

    }
    // ---------------------------------------------------------------
    // ---------------------------------------------------------------

    Filters::Filters(const QStringList& names, QObject *parent)
        : QStandardItemModel(32, 1, parent)
        , m_rootItem(new FiltersItem(::UserType::AllChecked, tr("@all")))
    {
        setItem(0, m_rootItem);
        m_rootItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
        m_rootItem->setCheckable(true);
        m_rootItem->setCheckState(Qt::Checked);
        m_rootItem->setBackground(QColor(0xa2afb5));
        m_rootItem->setChild(0, new FiltersItem(::UserType::GameType_Mask | ::UserType::Limit_Mask,  tr("@gameType")));
        m_rootItem->setChild(1, new FiltersItem(::UserType::BuyIn_Mask,     tr("@buyin")));
        m_rootItem->setChild(2, new FiltersItem(::UserType::Speed_Mask,     tr("@speed")));
        m_rootItem->setChild(3, new FiltersItem(::UserType::Type_Mask,      tr("@type")));
        m_rootItem->setChild(4, new FiltersItem(::UserType::Status_Mask,    tr("@status")));

        for (int i = 0; i < 5; i++) {
            m_rootItem->child(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsTristate);
            m_rootItem->child(i)->setCheckable(true);
            m_rootItem->child(i)->setCheckState(Qt::Checked);
            m_rootItem->child(i)->setBackground(QColor(0xa2afb5));
        }
        auto mask = ::UserType::GameType_Holdem | ::UserType::Limit_NoLimit | ::UserType::GameType_Omaha | ::UserType::Limit_PotLimit;
        m_rootItem->child(0)->setChild(0, new FiltersItem(mask,  tr("@all")));
        m_rootItem->child(0)->setChild(1, new FiltersItem(::UserType::GameType_Holdem | ::UserType::Limit_NoLimit,  tr("@holdem")));
        m_rootItem->child(0)->setChild(2, new FiltersItem(::UserType::GameType_Omaha | ::UserType::Limit_PotLimit,  tr("@omaha")));

        m_rootItem->child(1)->setChild(0, new FiltersItem(::UserType::BuyIn_Mask,     tr("@all")));
        m_rootItem->child(1)->setChild(1, new FiltersItem(::UserType::BuyIn_FreeRoll, tr("@freeroll")));
        m_rootItem->child(1)->setChild(2, new FiltersItem(::UserType::BuyIn_Micro,    tr("@micro")));
        m_rootItem->child(1)->setChild(3, new FiltersItem(::UserType::BuyIn_Low,      tr("@low")));
        m_rootItem->child(1)->setChild(4, new FiltersItem(::UserType::BuyIn_Medium,   tr("@medium")));
        m_rootItem->child(1)->setChild(5, new FiltersItem(::UserType::BuyIn_High,     tr("@high")));

        m_rootItem->child(2)->setChild(0, new FiltersItem(::UserType::Speed_Mask, tr("@all")));
        m_rootItem->child(2)->setChild(1, new FiltersItem(::UserType::Speed_Normal, tr("@normal")));
        m_rootItem->child(2)->setChild(2, new FiltersItem(::UserType::Speed_Turbo, tr("@turbo")));

        m_rootItem->child(3)->setChild(0, new FiltersItem(::UserType::Type_Mask,   tr("@all")));
        m_rootItem->child(3)->setChild(1, new FiltersItem(::UserType::Type_Regular,   tr("@regular")));
        m_rootItem->child(3)->setChild(2, new FiltersItem(::UserType::Type_Rebuy,     tr("@rebuy")));
        m_rootItem->child(3)->setChild(3, new FiltersItem(::UserType::Type_Knockout,  tr("@knockout")));
        m_rootItem->child(3)->setChild(4, new FiltersItem(::UserType::Type_Qualifier, tr("@qualifier")));
        m_rootItem->child(3)->setChild(5, new FiltersItem(::UserType::Type_Private,   tr("@private")));

        m_rootItem->child(4)->setChild(0, new FiltersItem(::UserType::Status_Mask,              tr("@all")));
        m_rootItem->child(4)->setChild(1, new FiltersItem(::UserType::Status_Announced,         tr("@announced")));
        m_rootItem->child(4)->setChild(2, new FiltersItem(::UserType::Status_Registration,      tr("@registration")));
        m_rootItem->child(4)->setChild(3, new FiltersItem(::UserType::Status_LateRegistration,  tr("@late_registration")));
        m_rootItem->child(4)->setChild(4, new FiltersItem(::UserType::Status_Started,           tr("@started")));
        m_rootItem->child(4)->setChild(5, new FiltersItem(::UserType::Status_Finished,          tr("@finished")));

        for (int i = 0; i < 3; i++) {
            m_rootItem->child(0)->child(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            m_rootItem->child(0)->child(i)->setCheckable(true);
            m_rootItem->child(0)->child(i)->setCheckState(Qt::Checked);
        }
        for (int i = 0; i < 6; i++) {
            m_rootItem->child(1)->child(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            m_rootItem->child(1)->child(i)->setCheckable(true);
            m_rootItem->child(1)->child(i)->setCheckState(Qt::Checked);
        }
        for (int i = 0; i < 3; i++) {
            m_rootItem->child(2)->child(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            m_rootItem->child(2)->child(i)->setCheckable(true);
            m_rootItem->child(2)->child(i)->setCheckState(Qt::Checked);
        }
        for (int i = 0; i < 6; i++) {
            m_rootItem->child(3)->child(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            m_rootItem->child(3)->child(i)->setCheckable(true);
            m_rootItem->child(3)->child(i)->setCheckState(Qt::Checked);
        }
        for (int i = 0; i < 6; i++) {
            m_rootItem->child(4)->child(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            m_rootItem->child(4)->child(i)->setCheckable(true);
            m_rootItem->child(4)->child(i)->setCheckState(Qt::Checked);
        }
    }



// -----------------------------------------------------------
// -----------------------------------------------------------

    FilterBox::FilterBox(int page, int boxIndex, Filters *sourceModel, QObject *parent)
        : QSortFilterProxyModel(parent)
        , m_items(getInitialData(page, boxIndex))
        , m_mask(0x0)
    {
        for (auto item: m_items) {
            m_mask |= item;
        }
        setSourceModel(sourceModel);
        setFilterRole(UserType::FilterRole);
        sourceModel->setData(sourceModel->index(0, 0), QVariant::fromValue(m_mask), UserType::FilterRole);
    }



} // namespace ns_model

QList<UserType::FilterFlags> ns_model::FilterBox::getInitialData(int page, int boxIndex)
{
    using value_t = UserType::FilterFlags;
    using data_t = QList<value_t>;

    auto val = [](UserType::FilterFlags flags) {
        return flags;
    };

    if (page == 0/*Cash Tables*/) {
        switch (boxIndex) {
        case 0: return data_t() << val(UserType::GameType_Holdem | UserType::Limit_NoLimit) << val(UserType::GameType_Holdem | UserType::Limit_FixedLimit) << val(UserType::GameType_Omaha | UserType::Limit_PotLimit);
        case 1: return data_t() << val(UserType::BuyIn_Micro) << val(UserType::BuyIn_Low) << val(UserType::BuyIn_Medium) << val(UserType::BuyIn_High);
        case 2: return data_t() << val(UserType::SeatsCount_2) << val(UserType::SeatsCount_6) << val(UserType::SeatsCount_9) << val(UserType::FreeSeats_Empty) << val(UserType::FreeSeats_Full);
        default:
            break;
        }
    } else {
        switch (boxIndex) {
        case 0: return data_t() << val(UserType::GameType_Holdem | UserType::Limit_NoLimit) << val(UserType::GameType_Omaha | UserType::Limit_PotLimit);
        case 1: return data_t() << val(UserType::BuyIn_FreeRoll) << val(UserType::BuyIn_Micro) << val(UserType::BuyIn_Low) << val(UserType::BuyIn_Medium) << val(UserType::BuyIn_High);
        case 2: return data_t() << val(UserType::Speed_Normal) << val(UserType::Speed_Turbo);
        case 3: return data_t() << val(UserType::Type_Regular) << val(UserType::Type_Rebuy) << val(UserType::Type_Knockout) << val(UserType::Type_Qualifier) << val(UserType::Type_Private);
        case 4:
            if (page == 1) {
                return data_t() << val(UserType::Status_Announced) << val(UserType::Status_Registration) << val(UserType::Status_LateRegistration) << val(UserType::Status_Started) << val(UserType::Status_Finished);
            } else {
                return data_t() << val(UserType::SNGSize_Pvp) << val(UserType::SNGSize_Single) << val(UserType::SNGSize_Multiple);
            }
        default:
            break;
        }
    }

    return data_t();
}

Qt::CheckState ns_model::FilterBox::checkState(UserType::FilterFlags f) const
{
    UserType::FilterFlags mask = UserType::AllUnchecked;
    for (int i = 1; i < rowCount(); i++) {
        if (index(i, 0).data(Qt::CheckStateRole).value<Qt::CheckState>() == Qt::Checked) {
            mask |= m_items.at(i);
        }
    }
    if ((f & mask) == f) {
        return Qt::Checked;
    } else if (f & mask) {
        return Qt::PartiallyChecked;
    }
    return Qt::Unchecked;
}

bool ns_model::FilterBox::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex sIdx = sourceModel()->index(source_row, 0, source_parent);
    if (sIdx.isValid())
    {
        return m_mask & sIdx.data(filterRole()).value<UserType::FilterFlags>();
    }
    return false;
}

