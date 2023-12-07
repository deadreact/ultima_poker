#include "model_vip_system_info.hpp"

#include "api_util.hpp"
#include <qlocale.h>
#include <basewidgets/types.h>
#include <qdatetime.h>
#include <qdebug.h>
#include <QTextStream>
#include <QRegularExpression>
#include "itemtype_concept.hpp"

namespace ns_model
{
QString vip_system_info::indexToKey(const QModelIndex &idx)
{
    if (idx.isValid()) {
        QString id("R_%0_C_%1_P_%2");

        return id.arg(idx.row()).arg(idx.column()).arg(indexToKey(idx.parent()));
    }
    return "false";
}

QModelIndex vip_system_info::keyToIndex(const QString &key) const
{
    QRegularExpression re("R_(\\d+)_C_(\\d+)_P_(\\w+)");
    QRegularExpressionMatch matchResult = re.match(key);
    if(!matchResult.hasMatch()) {
        return QModelIndex();
    }

    int row  = matchResult.captured(1).toInt();
    int col  = matchResult.captured(2).toInt();
    QString parentKey = matchResult.captured(3);

    return index(row, col, keyToIndex(parentKey));
}

vip_system_info::vip_system_info(QObject* parent)
        : QAbstractListModel(parent)
        , m_data(nullptr)
    {}

    vip_system_info::~vip_system_info() {}

    void vip_system_info::resetData(gbp::ns_api::ns_lobby::vip_system_info *newData)
    {
        beginResetModel();
        resetInternalData();
        m_data.reset(newData);
        endResetModel();
    }

    int vip_system_info::rowCount(const QModelIndex &parent) const {
        if (m_data.isNull()) {
            return 0;
        }
        if (parent.isValid()) {
            if (parent.parent().isValid() || field(parent.row()) != field::_bonuses) {
                return 0;
            }
            return m_data->_bonuses.size();
        }
        return underlying_cast(field::count);
    }

    int vip_system_info::columnCount(const QModelIndex &parent) const
    {
        if (m_data.isNull()) {
            return 0;
        }
        if (parent.isValid()) {
            if (parent.parent().isValid() || field(parent.row()) != field::_bonuses) {
                return 0;
            }
            return underlying_cast(bonus_item::count);
        }
        return 1;
    }

    QVariant vip_system_info::data(const QModelIndex &index, int role) const
    {
        if (m_data.isNull() || !index.isValid()) {
            return QVariant();
        }

        auto itemdata = itemData(index);

        if (itemdata.contains(role)) {
            return itemdata.value(role);
        }

        if (index.parent().isValid()) {
            if (index.parent().parent().isValid() || field(index.parent().row()) != field::_bonuses) {
                return additionalData(index, role);
            }

            switch (role) {
            case Qt::DisplayRole:
            case Qt::ToolTipRole:
                return text(index.row(), bonus_item(index.column()));
            default:
                return additionalData(index, role);
            }

            return additionalData(index, role);
        }
        if (index.column() != 0) {
            return additionalData(index, role);
        }
        switch (role)
        {
//        case Qt::ForegroundRole:
//            return QColor(0x37474f);
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return text(field(index.row()));
        default:
            return additionalData(index, role);
        }

        return additionalData(index, role);
    }

    QVariant vip_system_info::additionalData(const QModelIndex &index, int role) const
    {
        if (!index.parent().isValid() && index.column() != 0) {
            return QVariant();
        }
        if (index.parent().parent().isValid() || index.parent() != bonusItemIndex()) {
            return QVariant();
        }
//        qDebug() << index << role;
        auto itemAdditionalData = itemData(index);
        if (!itemAdditionalData.isEmpty() && itemAdditionalData.contains(role)) {
            return itemAdditionalData.value(role);
        }
        return QVariant();
    }

    QString vip_system_info::text(field f) const
    {
        if (m_data.isNull()) {
            return QString();
        }
        switch (f)
        {
            case field::_current_vip_status:        return ns_meta::tr_enum_type::display(m_data->_current_vip_status);
            case field::_current_rake:              return ns_meta::money_type::display(m_data->_current_rake);
            case field::_current_status_money_left: return ns_meta::money_type::display(m_data->_current_status_money_left);
            case field::_rakeback_current_percent:  return ns_meta::persent_type::display(m_data->_rakeback_current_percent);
            case field::_rakeback_current:          return ns_meta::money_type::display(m_data->_rakeback_current);
            case field::_rakeback_current_month:    return ns_meta::money_type::display(m_data->_rakeback_current_month);
            case field::_rakeback_last_month:       return ns_meta::money_type::display(m_data->_rakeback_last_month);
            case field::_rakeback_all_time:         return ns_meta::money_type::display(m_data->_rakeback_all_time);
            case field::_bonuses:
            default:
                return QString();
        }
    }

    gbp_i64 vip_system_info::money(field f) const
    {
        if (m_data.isNull()) {
            return -1;
        }
        switch (f)
        {
        case field::_current_rake:              return m_data->_current_rake;
        case field::_current_status_money_left: return m_data->_current_status_money_left;
        case field::_rakeback_current:          return m_data->_rakeback_current;
        case field::_rakeback_current_month:    return m_data->_rakeback_current_month;
        case field::_rakeback_last_month:       return m_data->_rakeback_last_month;
        case field::_rakeback_all_time:         return m_data->_rakeback_all_time;

        default:
            return -1;
        }
    }

    gbp::ns_vip_system::e_bonus vip_system_info::bonusTypeAtRow(int row) const
    {
        return (m_data.isNull()) ? gbp::ns_vip_system::e_bonus(-1) : m_data->_bonuses.at(row)._type;
    }

    bool vip_system_info::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (m_data.isNull()) {
            return false;
        }
        if (!index.isValid() || index.parent().isValid() || index.column() != 0) {
            return false;
        }

        return QAbstractListModel::setData(index, value, role);
    }

    QModelIndex vip_system_info::bonusItemIndex() const
    {
        return index(underlying_cast(field::_bonuses), 0);
    }

    QString vip_system_info::text(int row, bonus_item f) const
    {
        if (m_data.isNull()) {
            return QString();
        }
        QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);

        auto item = m_data->_bonuses.at(row);
        switch (f) {
        case bonus_item::_type:
            return ns_util::toString(item._type);
        case bonus_item::_datetime:
            return QDateTime::fromTime_t(item._datetime).date().toString(dateFormat);
        case bonus_item::_rakeback_money:
            return (bonusTypeAtRow(row) != gbp::ns_vip_system::e_bonus::rakeback_bonus) ? "" : Money(item._rakeback_money).toString();
        case bonus_item::_tickets_count:
            return QString::number(item._tickets_count);
        case bonus_item::_rakeback_period:
            return (bonusTypeAtRow(row) != gbp::ns_vip_system::e_bonus::rakeback_bonus)
                   ? ""
                   : QString("%0 - %1").arg(QDateTime::fromTime_t(item._rakeback_period.first).date().toString(dateFormat))
                                       .arg(QDateTime::fromTime_t(item._rakeback_period.second).date().toString(dateFormat));

        case bonus_item::_status_bonus_month:
            return QLocale().monthName(item._status_bonus_month);
        default:
            return QString();
        }
    }

    bool vip_system_info::hasChildren(const QModelIndex &parent) const
    {
        return !parent.isValid() || (!parent.parent().isValid() && field(parent.row()) == field::_bonuses);
    }
    QModelIndex vip_system_info::parent(const QModelIndex &child) const {
        if (child.internalPointer() == nullptr) {
            return QModelIndex();
        }
        return bonusItemIndex();
    }
    QModelIndex vip_system_info::index(int row, int column, const QModelIndex &parent) const {
        if (parent.isValid()) {
            return createIndex(row, column, m_data.data());
        }
        return createIndex(row, column, nullptr);
    }

    gbp::ns_api::ns_lobby::vip_system_info API_MODEL_DLL *ns_test::createRandomData()
    {
        gbp::ns_api::ns_lobby::vip_system_info *d = new gbp::ns_api::ns_lobby::vip_system_info;
        d->_current_vip_status          = gbp::ns_vip_system::e_status(rand() % 7);
        d->_current_rake              = rand() % 250000;
        d->_current_status_money_left = rand() % 250000;
        d->_rakeback_current_percent  = rand() % 100;
        d->_rakeback_current          = rand() % 250000;
        d->_rakeback_current_month    = rand() % 25000;
        d->_rakeback_last_month       = rand() % 35000;
        d->_rakeback_all_time         = rand() % 2500000;
        d->_bonuses.resize(rand() % 100);

        static const int periodSec = (90 * 24 * 60 * 60);
        for (auto &item : d->_bonuses)
        {
            item._type               = gbp::ns_vip_system::e_bonus((rand() % 2) + 1);
            item._datetime           = QDateTime::currentDateTimeUtc().toTime_t() - (rand() % periodSec);
            item._rakeback_money     = rand() % 21421;
            item._tickets_count      = rand() % 8;
            item._rakeback_period    = std::make_pair(item._datetime - 24*60*60, item._datetime + 24*6*60*60);
            item._status_bonus_month = rand() % 12;
        }

        return d;
    }

} //namespace ns_model



QVariant ns_model::vip_system_info::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
    {
        switch (role) {
        case Qt::DisplayRole:
            return QString("vip.") + ns_util::enumToString(field(section));
        case Qt::ToolTipRole:
            return QString("gbp::ns_api::ns_lobby::vip_system_info::") + ns_util::enumToString(field(section));
        default:
            return QVariant();
        }
    }
    return QAbstractListModel::headerData(section, orientation, role);
}

QMap<int, QVariant> ns_model::vip_system_info::itemData(const QModelIndex &index) const
{
    QString key = indexToKey(index);
//    qDebug() << key << keyToIndex(key);
//    qDebug() << m_dynamicData;
    if (m_dynamicData.contains(key)) {
        return m_dynamicData.value(key);
    }
    return QMap<int, QVariant>();
}

bool ns_model::vip_system_info::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    QString key = indexToKey(index);
//    qDebug() << key << index;

    m_dynamicData.insert(key, roles);
    emit dataChanged(index, index, roles.keys().toVector());
    return true;
}

bool ns_model::vip_system_info::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent == bonusItemIndex()) {
        beginInsertRows(parent, row, row + count - 1);
        auto it = m_data->_bonuses.begin() + row;
//        auto itEnd = it + count;
        m_data->_bonuses.insert(it, count, gbp::ns_api::ns_lobby::vip_system_info::bonus_item());
//        for (;it != itEnd; it++) {
//        }
        endInsertRows();
        return true;
    }
    return false;
}

bool ns_model::vip_system_info::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent == bonusItemIndex()) {
//        std::vector<int> df;
//        df.erase()
        beginRemoveRows(parent, row, row + count - 1);
        auto it = m_data->_bonuses.begin() + row;

        m_data->_bonuses.erase(it, it + count);
        endRemoveRows();
        return true;
    }
    return false;
}

Qt::ItemFlags ns_model::vip_system_info::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (!index.parent().isValid() || index.column() == 0)
        {
            switch (field(index.row())) {
            case field::_current_vip_status:        return Qt::ItemIsEnabled;
            case field::_current_rake:              return Qt::ItemIsEnabled;
            case field::_current_status_money_left: return Qt::ItemIsEnabled;
            case field::_rakeback_current_percent:  return Qt::ItemIsEnabled;
            case field::_rakeback_current:          return Qt::ItemIsEnabled;
            case field::_rakeback_current_month:    return Qt::ItemIsEnabled;
            case field::_rakeback_last_month:       return Qt::ItemIsEnabled;
            case field::_rakeback_all_time:         return Qt::ItemIsEnabled;
            case field::_bonuses:                   return Qt::ItemIsEnabled;
           default:
               return Qt::NoItemFlags;
            }
        }
        else if (isBonusItem(index))
        {
            return QAbstractListModel::flags(index);
        }
    }
    return QAbstractListModel::flags(index);
}

bool ns_model::vip_system_info::isBonusItem(const QModelIndex &index) const {
    return index.isValid()
            && index.parent().isValid()
            && !index.parent().parent().isValid()
            && field(index.parent().row()) == field::_bonuses;
}
