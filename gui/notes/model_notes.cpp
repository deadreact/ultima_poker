//#include <precompiled.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qset.h>
#include "model_notes.h"
#include "model_markers.h"
#include <basewidgets/types.h>
#include <qstyleditemdelegate.h>
#include <qtimezone.h>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QApplication>

namespace ns_model
{
//    template <>
//    struct ItemData<Notes> {
//        uint markerId;
//        QString nickname;
//        QString note;
//        QDateTime modified;

//    };
    QString Notes::Item::toString() const {
        return QString("{\n\"markerId\": %0,\n"
                       "  \"color\": \"%1\",\n"
                       "  \"nickname\": \"%2\",\n"
                       "  \"note\": \"%3\",\n"
                       "  \"modified\": \"%4\"\n }")
                .arg(markerId)
                .arg(nickname)
                .arg(note)
                .arg(modified.toString("dd.MM hh:mm"));
    }




    // ------------------------------------------------------------------------------------

    Notes::Notes(Markers *sourceModel, QObject *parent)
        : QAbstractTableModel(parent)
        , IOModel(this)
        , m_sourceModel(nullptr)
        , m_data()
    {
        setSourceModel(sourceModel);
    }

    void Notes::setSourceModel(Markers *m)
    {
        if (m_sourceModel != m)
        {
            beginResetModel();
            disconnectFromModel();
            m_sourceModel = m;
            connectToModel();
            endResetModel();
        }
    }

  
    int Notes::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return m_data.size();
    }

    int Notes::columnCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return 2;
    }

    QVariant Notes::data(const QModelIndex &modelIndex, int role) const
    {
        if (!modelIndex.isValid() || modelIndex.parent().isValid()) {
            return QVariant();
        }

        const Item& item = m_data.at(modelIndex.row());

        if (role == UserType::RowItemIdRole) {
            return item.markerId;
        } else if (role == UserType::RowDataRole) {
            return QVariant::fromValue(item);
        } else if (role == Qt::EditRole) {
            return QVariant::fromValue(ItemEditData(item.markerId, item.note));
        }

        if (modelIndex.column() == 0)
        {
            switch (role)
            {
            case Qt::DisplayRole:
                return item.nickname;
            case Qt::ToolTipRole:
                return item.note;
            case Qt::DecorationRole: {
                return m_sourceModel->indexOf(item.markerId).data(Qt::DecorationRole);
            }
            default:
                return QVariant();
            }
        }
        else if (modelIndex.column() == 1)
        {
            switch (role) {
            case Qt::DisplayRole:
                return item.modified.toString("dd.MM.yyyy hh:mm:ss");
            default:
                return QVariant();
            }
        }

        return QVariant();
    }

    QVariant Notes::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation != Qt::Horizontal) {
            return QVariant();
        }
        switch (role) {
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            switch (section) {
            case 0: return tr("@section_player");
            case 1: return tr("@section_modified");
            default:
                return QVariant();
            }
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
        case Qt::CheckStateRole:
        default:
            return QVariant();
        }
    }

    bool Notes::setData(const QModelIndex &index, const QVariant &value, int role)
    {
         if (role == Qt::EditRole && value.canConvert<ItemEditData>()) {
             Item& item = m_data[index.row()];
             ItemEditData editData = value.value<ItemEditData>();
             QVector<int> roles;

             bool modified = false;
             if (editData.markerId != item.markerId)
             {
                 if (const Markers::Item *marker = m_sourceModel->findItemById(editData.markerId))
                 {
                     item.markerId = marker->id;
                     roles << Qt::DecorationRole;
                     modified = true;
                 }
             }
             if (editData.note != item.note)
             {
                 item.note = editData.note;
                 roles << UserType::RowDataRole;
                 modified = true;
             }
             if (modified) {
                item.modified = QDateTime::currentDateTimeUtc().toTimeZone(QTimeZone::systemTimeZone());
                roles << UserType::DateTimeRole;

                emit dataChanged(index, index, roles);
                const QModelIndex dateTimeIndex = index.model()->index(index.row(), 1);
                emit dataChanged(dateTimeIndex, dateTimeIndex, QVector<int>() << Qt::DisplayRole);
                write();
                return true;
             }
         }
        return QAbstractTableModel::setData(index, value, role);
    }

    bool Notes::insertRows(int row, int count, const QModelIndex &parent)
    {
        int last = row + count - 1;
        beginInsertRows(parent, row, last);
        for (;row <= last; row++) {
            m_data.insert(m_data.begin() + row, Item());
        }
        write();
        endInsertRows();
        return true;
//        return QAbstractTableModel::insertRows(row, count, parent);
    }

    bool Notes::removeRows(int row, int count, const QModelIndex &parent)
    {
        if (rowCount(parent) < row + count) {
            return QAbstractTableModel::removeRows(row, count, parent);
        }
        int last = row + count - 1;
        beginRemoveRows(parent, row, last);
        m_data.remove(row, count);
        endRemoveRows();
        write();
        return true;
    }

    Qt::ItemFlags Notes::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        if (index.isValid() && !index.parent().isValid() && index.column() == 0) {
            return flags | Qt::ItemIsEditable;
        }
        return flags;
    }

    QModelIndex Notes::indexOf(const QString &nickname)
    {
        for (int i = 0; i < m_data.size(); i++) {
            if (m_data.at(i).nickname == nickname) {
                return index(i, 0);
            }
        }
        return QModelIndex();
    }

    QColor Notes::markerColor(const QModelIndex &idx) const {
        return m_sourceModel->indexOf(idx.data(UserType::RowItemIdRole).value<uint>()).data(Qt::DecorationRole).value<QColor>();
    }

    QString Notes::note(const QModelIndex &idx) const {
        return data(idx, Qt::ToolTipRole).toString();
    }

    QModelIndex Notes::createNote(const QString &nickname, uint markerId, on_create_item replyHandler)
    {
//        rpc_lobby *rpc = netClient().rpc<rpc_lobby>();
//        rpc->nick_free(nickname, [this](reply_lobby::e_lobby status) {});
        if (insertRow(0))
        {
            Item& item = m_data.first();
            item.nickname = nickname;
            item.markerId = markerId;
            item.note     = tr("@note_placeholder");
            item.modified = QDateTime::currentDateTimeUtc();
            if (replyHandler) {
                replyHandler(true, item);
            }
            return index(0, 0);
        }
        else
        {
            if (replyHandler) {
                replyHandler(false, Item());
            }
        }
        return QModelIndex();
    }

    void Notes::onSourceModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
    {
        if (sender() != m_sourceModel) {
            qFatal("   Loc: [%s:%i]\n(sender() != m_sourceModel)", __FILE__, __LINE__);
        }
        QSet<QModelIndex> indices;
        for (int row = topLeft.row(); row <= bottomRight.row(); row++) {
            uint id = m_sourceModel->rowItem(row).id;
            for (int i = 0; i < rowCount(); i++) {
                if (m_data.at(i).markerId == id) {
                    indices << index(i, 0);
                }
            }
        }
        for (const QModelIndex& modelIndex: indices) {
            emit dataChanged(modelIndex, modelIndex, roles);
        }
    }

    bool Notes::read(QIODevice *iodevice)
    {
        QScopedPointer<QFile> defaultFile;
        if (iodevice == nullptr) {
            defaultFile.reset(new QFile(getPath() + "notes.bd"));
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

    bool Notes::write(QIODevice *iodevice) const
    {
        QScopedPointer<QFile> defaultFile;
        if (iodevice == nullptr) {
            defaultFile.reset(new QFile(getPath() + "notes.bd"));
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

    void Notes::connectToModel()
    {
        if (m_sourceModel != nullptr) {
            connect(m_sourceModel, &Markers::dataChanged, this, &Notes::onSourceModelDataChanged);
            connect(m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
            connect(m_sourceModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
        }
    }

    void Notes::disconnectFromModel()
    {
        if (m_sourceModel != nullptr) {
            disconnect(m_sourceModel, &Markers::dataChanged, this, &Notes::onSourceModelDataChanged);
            disconnect(m_sourceModel, SIGNAL(modelAboutToBeReset()), this, SIGNAL(modelAboutToBeReset()));
            disconnect(m_sourceModel, SIGNAL(modelReset()), this, SIGNAL(modelReset()));
        }
    }

} // namespace ns_model



QDataStream &operator<<(QDataStream &os, const ns_model::Notes::Item &item)
{
    os << ns_model::Notes::serialize_key << QByteArray("item_notes") << QApplication::applicationVersion();
    os << item.markerId << item.nickname << item.note << item.modified;
    return os;
}

QDataStream &operator>>(QDataStream &is, ns_model::Notes::Item &item)
{
    uint key;
    is >> key;

    if (key == ns_model::Notes::serialize_key) {
        QByteArray name;
        QByteArray version;
        is >> name >> version;

        if (name == "item_notes" && version == QApplication::applicationVersion()) {
            is >> item.markerId >> item.nickname >> item.note >> item.modified;
        }
    }
    return is;
}
