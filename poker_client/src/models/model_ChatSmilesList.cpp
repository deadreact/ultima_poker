#include <precompiled.h>
#include "model_ChatSmilesList.h"
#include <qpixmap.h>

namespace ns_model
{
    ChatSmilesList::ChatSmilesList(QObject *parent)
        : QAbstractListModel(parent)
    {
    }

    int ChatSmilesList::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid())
            return 0;

        return m_icons.size();
    }

    QVariant ChatSmilesList::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid())
            return QVariant();

        if (role == Qt::DecorationRole)
        {
            return m_icons.at(index.row());
        } else if (role == Qt::UserRole) {
            return QLatin1String(":)");
        }
        // FIXME: Implement me!
        return QVariant();
    }

    void ChatSmilesList::setSource(const QPixmap &pixmap, const QSize &sizeOfFrame, int framesCount)
    {
        beginResetModel();

        int iBorder = pixmap.height() - sizeOfFrame.height();
        int jBorder = pixmap.width() - sizeOfFrame.width();

        for (int i = 0; i < iBorder; i += sizeOfFrame.height()) {
            for (int j = 0; j < jBorder; j += sizeOfFrame.width()) {
                m_icons << pixmap.copy(QRect(QPoint(j, i), sizeOfFrame));
                if (m_icons.size() == framesCount) {
                    endResetModel();
                    return;
                }
            }
        }
        endResetModel();

    }
} //namespace ns_model
