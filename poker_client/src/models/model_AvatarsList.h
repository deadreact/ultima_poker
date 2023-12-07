#pragma once

#include <QAbstractListModel>
#include <utility>
#include <iostream>
#include <QPixmap>
#include <db/Avatar.h>

namespace ns_widget {
    class ProfilePicture;
}

namespace ns_model
{
    class AvatarsList : public QAbstractListModel
    {
        Q_OBJECT
        
        int m_currentIndex;
        int m_extraItemsCount;
    private:
        QMap<QString, Avatar> m_avatars;
//        QScopedPointer<AvatarGroup> m_avatarsGroup;
    public:
        explicit AvatarsList(QObject *parent = nullptr);
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        void loadIcons(const QString& path);


        QPixmap smallPixmap(int idx) const;
        QPixmap bigPixmap(int idx) const;
        QIcon icon(int idx) const;

        int extraItemsCount() const { return m_extraItemsCount; }
        virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;

    public slots:
        void setCurrentIndex(int index);
    };
} // namespace ns_model
