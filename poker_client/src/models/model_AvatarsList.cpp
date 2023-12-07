#include <precompiled.h>
#include "model_AvatarsList.h"

#include <QPixmap>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include <const.h>
#include <qregularexpression.h>

namespace
{


}


namespace ns_model
{

    void AvatarsList::loadIcons(const QString &path)
    {
        QAbstractItemModel::beginResetModel();
        m_avatars = Avatar::fromPath(path);
        QAbstractItemModel::endResetModel();
    }

    QPixmap AvatarsList::smallPixmap(int idx) const {
        return icon(idx).pixmap(ns_const::ns_avatar_size::small);
    }

    QPixmap AvatarsList::bigPixmap(int idx) const {
        return icon(idx).pixmap(ns_const::ns_avatar_size::base);
    }

    QIcon AvatarsList::icon(int idx) const {
        if (m_avatars.isEmpty()) {
            static const QIcon invalid;
            return invalid;
        }

        if (idx < 0) {
            idx += (((idx + 1)/m_avatars.size()) * -1) * m_avatars.size();
            idx += m_avatars.size();
        }

        return m_avatars.value(m_avatars.keys().at(idx % m_avatars.size())).icon();
    }

    void AvatarsList::setCurrentIndex(int index) {
        m_currentIndex = index;
    }

    AvatarsList::AvatarsList(QObject *parent)
        : QAbstractListModel(parent)
    {

    }

    int AvatarsList::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;

        return m_avatars.size() + m_extraItemsCount;
    }

    QVariant AvatarsList::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || m_avatars.isEmpty())
            return QVariant();

        int id = index.row();
        switch (role) {
        case Qt::DecorationRole:
            return icon(id);// (m_currentIndex == id) ? bigPixmap(id) : smallPixmap(id);
        default:
            return QVariant();
        }
    }
    QModelIndex AvatarsList::index(int row, int column, const QModelIndex &parent) const
    {
        if (m_avatars.isEmpty()) {
            return QModelIndex();
        }


        if (row < 0) {
            row += (((row + 1)/m_avatars.size()) * -1) * m_avatars.size();
            row += m_avatars.size();
        }

        return QAbstractListModel::index(row % m_avatars.size(), column, parent);
    }



} // namespace ns_model


