#include <precompiled.h>
#include <qapplication.h>
#include <api_models/api_util.hpp>
#include <qstyle.h>
#include "logmodel.h"

gbp_i64 LogMsgItem::sm_idIncrementer = 0;

QVariant LogMsgItem::data(int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return m_msg;
    case Qt::ToolTipRole:
        return ns_util::enumToString(m_type) + ": " + m_msg;
    case Qt::DecorationRole:
        switch (m_type) {
        case MsgFunction:  return QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon);
        case MsgInfo:      return QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
        case MsgWarning:   return QApplication::style()->standardIcon(QStyle::SP_MessageBoxWarning);
        case MsgCritical:  return QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
        case MsgFatal:     return QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical);
        default:
            return QVariant();
        }
    case Qt::ForegroundRole:
        switch (m_type) {
        case MsgFunction:  return QColor(0x00aeef);
        case MsgInfo:      return QColor(0x191919);
        case MsgWarning:   return QColor(0xe6b34c);
        case MsgCritical:  return QColor(0xdd0d2c);
        case MsgFatal:     return QColor(0x360000);
        default:
            return QVariant();
        }
    case Qt::BackgroundRole:
        switch (m_type) {
        case MsgFunction:  return QColor(0xfafafa);
        case MsgInfo:      return QColor(0xfafafa);
        case MsgWarning:   return QColor(0xfafafa);
        case MsgCritical:  return QColor(0xfafafa);
        case MsgFatal:     return QColor(0xdd0d2c);
        default:
            return QVariant();
        }
    case UserType::RowItemIdRole:
        return id();
    case UserType::DateTimeRole:
        return timeMark().toString("hh:mm:ss");
    default:
        return QVariant();
    }
}

QVariant LogMsgItem::childData(int child, int role) const
{
    if (child >= m_children.size()){
        return QVariant();
    }
    const Child& item = m_children.at(child);
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return item.text;
    case Qt::ForegroundRole:
        return item.foreground;
    case Qt::BackgroundRole:
        return item.background;
    default:
        return QVariant();
    }
}

void LogMsgItem::addChild(const QString &text, uint foreground, uint background)
{
    m_children << Child{text, foreground, background};
}

void LogMsgItem::setText(const QString &text)
{
    if (m_msg != text) {
        m_msg = text;
    }
}

void LogMsgItem::setMsgType(LogMsgItem::MsgType type)
{
    if (m_type != type) {
        m_type = type;
    }
}

LogModel::LogModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_useTableRepresentation(false)
    , m_maxChildren(7)
{}

LogModel::~LogModel()
{
    qDeleteAll(m_messages);
}

LogModel &LogModel::newMsg(const QString &text, LogMsgItem::MsgType type)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages.append(new LogMsgItem(text, type));
    endInsertRows();

    return *this;
}

LogModel &LogModel::operator<<(const QString &text)
{
    QModelIndex modelIndex = itemToIndex(lastItem());
    if (m_maxChildren <= lastItem()->childrenCount()) {
        if (m_useTableRepresentation) {
            beginInsertColumns(modelIndex, m_maxChildren, lastItem()->childrenCount());
            lastItem()->addChild(text);
        }
        m_maxChildren = lastItem()->childrenCount() + 1;
        if (m_useTableRepresentation) {
            endInsertColumns();
        }
    }
    else
    {
        if (m_useTableRepresentation) {
            lastItem()->addChild(text);
            QModelIndex idx = index(lastItem()->id(), lastItem()->childrenCount());
            emit dataChanged(idx, idx);
        }

    }
    if (!m_useTableRepresentation)
    {
        beginInsertRows(modelIndex, rowCount(modelIndex), rowCount(modelIndex));
        lastItem()->addChild(text);
        endInsertRows();
    }
    return *this;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == UserType::SortRole) {
        return indexToItem(index)->id();
    }
    if (m_useTableRepresentation)
    {
        if (index.parent().isValid()) {
            return QVariant();
        }
        if (index.column() == 0) {
            return indexToItem(index)->data(role);
        }
        return indexToItem(index)->childData(index.column() - 1, role);
    }
    if ((index.column() == 1 && !index.parent().isValid() && role == Qt::DisplayRole) || role == UserType::DateTimeRole) {
        return indexToItem(index)->data(UserType::DateTimeRole);
    }
    if (index.column() == 0)
    {
        if (parent(index).isValid()) {
            return indexToItem(index)->childData(index.row(), role);
        }
        return indexToItem(index)->data(role);
    }
    return QVariant();
}

const LogMsgItem *LogModel::indexToItem(const QModelIndex &index) const {
    return (index.isValid()) ? (index.parent().isValid() ? indexToItem(index.parent()) : m_messages.at(index.row())) : nullptr;
}

QModelIndex LogModel::itemToIndex(const LogMsgItem *item) const {
    return createIndex(item->id(), 0);
}

void LogModel::setUseTableRepresentation(bool b)
{
    if (m_useTableRepresentation != b) {
        beginResetModel();
        resetInternalData();
        m_useTableRepresentation = b;
        endResetModel();
    }
}

LogMsgItem* LogModel::lastItem() {
    return m_messages.last();
}


QModelIndex LogModel::index(int row, int column, const QModelIndex &parent) const
{
    int maxColumns = m_useTableRepresentation ? m_maxChildren : 2;
    if (column >= maxColumns || row >= m_messages.size()) {
        return QModelIndex();
    }
    if (!parent.isValid()) {
        return createIndex(row, column, nullptr);
    }
    if (!m_useTableRepresentation) {
        return createIndex(row, 0, parent.row() + 1);
    }
    return QModelIndex();
}

QModelIndex LogModel::parent(const QModelIndex &child) const
{
    if (m_useTableRepresentation || !child.isValid() ||  child.internalId() == 0) {
        return QModelIndex();
    }
    return createIndex(child.internalId()-1, 0, nullptr);
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_messages.size();
    }
    if (m_useTableRepresentation) {
        return 0;
    }
    return m_messages.at(parent.row())->childrenCount();
}

int LogModel::columnCount(const QModelIndex &/*parent*/) const {
    return (m_useTableRepresentation) ? m_maxChildren : 2;
}

bool LogModel::hasChildren(const QModelIndex &parent) const
{
    return !parent.isValid() || (!m_useTableRepresentation && parent.internalId() == 0);
}



QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical)
    {
        return index(section).data(UserType::RowItemIdRole);
    }
    if (section == 0) {
        return "Message";
    }
    if (m_useTableRepresentation) {
        return QString("arg%0").arg(section);
    }
    return "Time mark";

}

QMap<int, QVariant> LogModel::itemData(const QModelIndex &index) const
{
    QMap<int, QVariant> itemdata;
    if (!index.isValid()) {
        return itemdata;
    }
    static const QVector<int> roles = {Qt::DisplayRole, Qt::DecorationRole, Qt::ToolTipRole, Qt::ForegroundRole, Qt::BackgroundRole
                                     , UserType::SortRole, UserType::RowItemIdRole, UserType::DateTimeRole};
    for (int role: roles) {
        QVariant value = index.data(role);
        if (value.isValid()) {
            itemdata.insert(role, value);
        }
    }
    return itemdata;
}

bool LogModel::read(QIODevice *iodevice)
{
    if (iodevice && (iodevice->isOpen() || iodevice->open(QIODevice::ReadOnly | QIODevice::Text))) {

    }
    return false;
}

bool LogModel::write(QIODevice *iodevice) const
{
    if (iodevice && (iodevice->isOpen() || iodevice->open(QIODevice::WriteOnly | QIODevice::Text))) {
        QTextStream stream(iodevice);
        for (const LogMsgItem* item: m_messages) {
            stream << item->id() << ". " << item->data(UserType::DateTimeRole).toString() << " " << item->data().toString() << "\n";
            QStringList lst;
            stream << "args: ";
            for (int i = 0; i < item->childrenCount(); i++) {
                lst << item->childData(i).toString();
            }
            stream << lst.join(", ") << "\n";
        }
        return true;
    }
    return false;
}
