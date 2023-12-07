//#include <precompiled.h>
#include <qfile.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include "model_markers.h"
#include <basewidgets/types.h>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QApplication>

namespace
{
    QPixmap createIndicator(const QColor& col, const QSize size = QSize(28, 28)) {
        QPixmap pix(size);
        pix.fill(col);
        QPainter p;
        if (p.isActive()) {
            p.begin(&pix);
            p.setPen(QColor(0x191919));
            p.drawRect(QRect(0, 0, size.width()-1, size.height()-1));
            p.end();
        }
        return pix;
    }

    QColor randomRgb() {
        return QColor(rand()%256, rand()%256, rand()%256);
    }
}

namespace ns_model
{
    uint Markers::Item::sm_idIncrementer = 0;
    const int Markers::sm_minimumItems = 5;
    const int Markers::sm_maximumItems = 15;
    Markers::Item::Item()
        : id(sm_idIncrementer++), m_color(Qt::transparent), name("No marker")
    {}

    Markers::Item::Item(uint id)
        : id(id), m_color(Qt::transparent), name("No marker")
    {
        if (sm_idIncrementer <= id) {
            sm_idIncrementer = id + 1;
        }
    }

    Markers::Item::Item(const Item &item)
        : id(item.id), m_color(item.m_color), name(item.name)
    {}

    Markers::Item::Item(const QColor &color, const QString &name)
        : id(sm_idIncrementer++), m_color(color), name(name)
    {

    }

    void Markers::Item::setColor(const QColor &col) {
        if (col != m_color) {
            m_color = col;
        }
    }

    const Markers::Item &Markers::Item::invalid()
    {
        int val = sm_idIncrementer;
        static const Item invalid(-1);
        sm_idIncrementer = val;

//        if (invalid.pixmap.isNull()) {
//            QSize size(28, 28);
//            invalid.pixmap = QPixmap(size);
//            invalid.pixmap.fill(Qt::transparent);
//            QPainter p;
//            p.begin(&invalid.pixmap);
//            QRect rc(0, 0, size.width()-1, size.height()-1);
//            p.setPen(QColor(0x191919));
//            p.fillRect(rc, QBrush(QColor(0x37474f), Qt::BDiagPattern));
//            p.drawRect(rc);
//            p.end();
//        }

        return invalid;
    }

    // ------------------------------------------------------------------------------------

    Markers::Markers(QObject *parent)
        : QAbstractTableModel(parent)
        , IOModel(this)
        , m_data()
    {
//        static const QIcon icoEdit(":/textures/Common/icons/settings/icon_edit_tag.png");
//        static const QIcon icoRemove(":/textures/Common/icons/settings/icon_delete_tag.png");

        m_data.push_back(Item::invalid());
        m_data.push_back(Item(QColor(0xda4371), "Marker 1"));
        m_data.push_back(Item(QColor(0x43da71), "Marker 2"));
        m_data.push_back(Item(QColor(0x4371da), "Marker 3"));
        m_data.push_back(Item(QColor(0x71da43), "Marker 4"));
        m_data.push_back(Item(QColor(0x009331), "Marker 5"));
        m_data.push_back(Item(QColor(0xf09331), "Marker 6"));

    }

    bool Markers::read(QIODevice *iodevice)
    {
        QScopedPointer<QFile> defaultFile;
        if (iodevice == nullptr) {
            defaultFile.reset(new QFile(getPath() + "markers.bd"));
            iodevice = defaultFile.data();
        }
        if (iodevice->open(QIODevice::ReadOnly)) {
            QDataStream os(iodevice);
            beginResetModel();
            m_data.clear();
            os >> m_data;
            iodevice->close();
            endResetModel();
            return true;
        }
        return false;
    }

    bool Markers::write(QIODevice *iodevice) const
    {
        QScopedPointer<QFile> defaultFile;
        if (iodevice == nullptr) {
            defaultFile.reset(new QFile(getPath() + "markers.bd"));
            iodevice = defaultFile.data();
        }
        if (iodevice->open(QIODevice::WriteOnly)) {
            QDataStream is(iodevice);

            is << m_data;
            iodevice->close();
            return true;
        }
        return false;
    }

  
    int Markers::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return qBound(sm_minimumItems, m_data.size(), sm_maximumItems);
    }

    int Markers::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return 3;
    }

    QVariant Markers::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.parent().isValid()) {
            return QVariant();
        }

        const Item& item = rowItem(index.row());

        if (role == UserType::RowItemIdRole) {
            return item.id;
        }
        if (role == UserType::RowDataRole) {
            return QVariant::fromValue(item);
        }

        if (index.column() == 0)
        {
            switch (role)
            {
            case Qt::DisplayRole:
                return item.name;
            case Qt::DecorationRole: {
//                if (item.pixmap.isNull()) {
//                    item.pixmap = createIndicator(item.color());
//                }
                return item.color();
            }
            default:
                return QVariant();
            }
        }
        else if (index.column() == 1)
        {
            if (role == Qt::DecorationRole)
            {
//                static const QIcon icoEdit(":/textures/Common/icons/settings/icon_edit_tag.png");
//                return icoEdit;
            }
        }
        else if (index.column() == 2)
        {
            if (index.row() > 0 && role == Qt::DecorationRole && rowCount() > sm_minimumItems)
            {
                static const QIcon icoRemove(":/textures/Common/icons/settings/icon_delete_tag.png");
                return icoRemove;
            }
        }

        return QVariant();
    }

    bool Markers::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (role == Qt::EditRole && value.canConvert<Item>()) {
            Item& oldItem = m_data[index.row()];
            Item newItem = value.value<Item>();
            if (newItem.name.isEmpty()) {
                return false;
            }
            QVector<int> roles;
            roles << Qt::EditRole << UserType::RowDataRole;
            if (newItem.color() != oldItem.color()) {
                oldItem.setColor(newItem.color());
                roles << Qt::DecorationRole;
            }
            if (newItem.name != oldItem.name) {
                oldItem.name = newItem.name;
                roles << Qt::DisplayRole;
            }
            emit dataChanged(index, index, roles);
            write();
            return true;
        }
        return QAbstractTableModel::setData(index, value, role);
    }

    bool Markers::insertRows(int row, int count, const QModelIndex &parent)
    {
        if (parent.isValid() || count > 1 || rowCount() == sm_maximumItems) {
            return QAbstractTableModel::insertRows(row, count, parent);
        }
        row = std::max(1, row);
        beginInsertRows(parent, row, row + count - 1);
        m_data.insert(m_data.begin() + row, Item(randomRgb(), "New marker"));
        write();
        endInsertRows();
        return true;
    }

    bool Markers::removeRows(int row, int count, const QModelIndex &parent)
    {
        if (parent.isValid() || row == 0 || rowCount() - count < sm_minimumItems) {
            return QAbstractTableModel::removeRows(row, count, parent);
        }
        beginRemoveRows(parent, row, row + count - 1);
        m_data.remove(row, count);
        write();
        endRemoveRows();
        return true;
    }

    bool Markers::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destParent, int destRow)
    {
        if (sourceParent.isValid() || destParent.isValid() || count != 1 || sourceRow == destRow || sourceRow == 0 || destRow == 0) {
            return QAbstractTableModel::moveRows(sourceParent, sourceRow, count, destParent, destRow);
        }

        beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destParent, destRow);
        std::swap(m_data[sourceRow], m_data[destRow]);
        write();
        endMoveRows();
        return true;
    }

    Qt::ItemFlags Markers::flags(const QModelIndex &index) const
    {
        if (index.row() == 0) {
            return Qt::ItemIsEnabled;
        }
        Qt::ItemFlags flags = QAbstractTableModel::flags(index);
        if (index.isValid() && !index.parent().isValid() && index.column() == 0 && index.row() > 0) {
            return flags | Qt::ItemIsEditable;
        }
        return flags;
    }

    QModelIndex Markers::indexOf(uint id) const
    {
        for (int i = 0; i < m_data.size(); i++) {
            if (m_data.at(i).id == id) {
                return index(i, 0);
            }
        }
        return index(0, 0);
    }

    const Markers::Item *Markers::findItemById(uint id) const
    {
        for (const Item& item: m_data) {
            if (item.id == id) {
                return &item;
            }
        }
        return nullptr;
    }

    const Markers::Item &Markers::rowItem(int row) const {
        return m_data.at(row);
    }

} // namespace ns_model



QDataStream &operator<<(QDataStream &os, const ns_model::Markers::Item &item)
{
    os << ns_model::Markers::serialize_key << QByteArray("item_markers") << QApplication::applicationVersion();
    os << item.id << item.name << item.color();
    return os;
}

QDataStream &operator>>(QDataStream &is, ns_model::Markers::Item &item)
{
    uint key;
    is >> key;

    if (key == ns_model::Markers::serialize_key) {
        QByteArray name;
        QByteArray version;
        is >> name >> version;
        if (name == "item_markers" && version == QApplication::applicationVersion()) {
            QColor col;
            is >> item.id >> item.name >> col;
            item.setColor(col);
        }
    }
    return is;
}

