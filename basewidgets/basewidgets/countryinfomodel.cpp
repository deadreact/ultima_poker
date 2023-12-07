#include "countryinfomodel.h"
#include "country_combobox.inl"
#include "types.h"

class CountryInfoSourceModel : public QAbstractTableModel
{
private:
    QList<CountryInfo> m_data;
public:
    CountryInfoSourceModel(QObject* parent = nullptr)
        : QAbstractTableModel(parent)
    {
        setObjectName("CountryInfoSourceModel");
    }
    void initData()
    {
        beginResetModel();
        m_data.clear();
        for (int i = 0; i < 257; i++) {
            m_data << countryPhoneCode<CountryInfo>(i);
        }
        endResetModel();
    }

    virtual int rowCount(const QModelIndex &parent) const override {
        return parent.isValid() ? 0 : m_data.size();
    }
    virtual int columnCount(const QModelIndex &parent) const override {
        return parent.isValid() ? 0 : 4;
    }
    virtual QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid() || index.parent().isValid()) {
            return QVariant();
        }
        const CountryInfo& item = m_data.at(index.row());
        switch (role) {
        case UserType::RowDataRole:         return QVariant::fromValue(item);
        case UserType::RowItemIdRole:       return item.id;
        case UserType::InternalIdRole:      return QVariant::fromValue(qMakePair(item.id, index.column()));
        case UserType::PathRole:
            return index.column() == 3 ? QVariant::fromValue(item.icon) : QVariant();
        case UserType::SortRole:
            switch (index.column()) {
            case 0:
            case 3: return QVariant::fromValue(item.id);
            case 1: return QVariant::fromValue(item.name);
            case 2: return QVariant::fromValue(item.code);
            default:
                return QVariant();
            }
        case UserType::OriginDataRole:
        case Qt::EditRole:
            switch (index.column()) {
            case 0: return QVariant::fromValue(item.id);
            case 1: return QVariant::fromValue(item.name);
            case 2: return QVariant::fromValue(item.code);
            case 3: return QVariant::fromValue(item.icon);
            default:
                return QVariant();
            }
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            switch (index.column()) {
            case 0: return QVariant::fromValue(item.id);
            case 1: return QVariant::fromValue(item.name);
            case 2: return QVariant::fromValue("+" + item.code);
            case 3:
            default:
                return QVariant();
            }
        case Qt::DecorationRole:
            return index.column() == 3 ? QVariant::fromValue(QIcon(item.icon)) : QVariant();
        default:
            return QVariant();
        }
    }
};


CountryInfoModel::CountryInfoModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_sourceModel(new CountryInfoSourceModel)
    , m_excludeRegExp("")
    , m_displayFlag(0x0f)
{

}

CountryInfoModel::~CountryInfoModel() {}

void CountryInfoModel::initData()
{
    QSortFilterProxyModel::setSourceModel(m_sourceModel.data());
    m_sourceModel->initData();
}

void CountryInfoModel::setExcludeRegExp(const QString &excludeRegExp) {
    if (m_excludeRegExp != excludeRegExp) {
        beginResetModel();
        m_excludeRegExp = excludeRegExp;
        endResetModel();
    }
//    QSortFilterProxyModel::setFilterRegExp(excludeRegExp);
}

void CountryInfoModel::setDisplayFlag(int f) {
    if (m_displayFlag != f) {
        beginResetModel();
        m_displayFlag = f;
        endResetModel();
    }
}

int CountryInfoModel::displayFlag() const { return m_displayFlag; }

int CountryInfoModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : 1;
}

QVariant CountryInfoModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }


    switch (role) {
    case UserType::RowDataRole:
    case UserType::RowItemIdRole:
        return QSortFilterProxyModel::data(index, role);
    case UserType::OriginDataRole:
        return data(index, UserType::RowDataRole);
    case UserType::InternalIdRole:
        return QSortFilterProxyModel::data(index, UserType::RowItemIdRole);
    case Qt::DisplayRole:
    {
        QStringList lst;
        if (m_displayFlag & 0x01) {
            lst << QSortFilterProxyModel::data(index.sibling(index.row(), 0), role).toString();
        }
        if (m_displayFlag & 0x08) {
            lst << QSortFilterProxyModel::data(index.sibling(index.row(), 2), role).toString();
        }
        if (m_displayFlag & 0x04) {
            lst << QSortFilterProxyModel::data(index.sibling(index.row(), 1), role).toString();
        }
        if (!m_excludeRegExp.isNull()) {
            QRegExp re(m_excludeRegExp);
            for (const QString& str: lst) {
                if (re.exactMatch(str)) {
                    return QVariant();
                }
            }
        }
        return lst.join(" ");
    }
    case Qt::DecorationRole:
        return QSortFilterProxyModel::data(index.sibling(index.row(), 3), role);
    default:
        return QVariant();
    }

    return QVariant();
}
