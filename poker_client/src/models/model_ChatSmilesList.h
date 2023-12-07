#pragma once

#include <QAbstractListModel>
#include <QPixmap>

namespace ns_model
{

    class ChatSmilesList : public QAbstractListModel
    {
        Q_OBJECT

        QList<QPixmap> m_icons;
    public:
        explicit ChatSmilesList(QObject *parent = 0);

        // Basic functionality:
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        void setSource(const QPixmap& pixmap, const QSize& sizeOfFrame, int framesCount = -1);
    private:
    };

} //namespace ns_model
