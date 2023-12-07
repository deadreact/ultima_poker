#pragma once

#include <QAbstractTableModel>
#include "model_io.h"
#include <qcolor.h>
#include <qpixmap.h>

class QAbstractButton;

namespace ns_model
{
    class NOTES_DLL Markers : public QAbstractTableModel, public IOModel
    {
        Q_OBJECT
    public:
        struct Item {
            uint id;
        private:
            static uint sm_idIncrementer;
            QColor m_color;
        public:
//            mutable QPixmap pixmap;
            QString name;

            Item();
            Item(uint id);
            Item(const Item& rowItem);
            Item(const QColor& color, const QString& name);

            inline QColor color() const { return m_color; }
            void setColor(const QColor& col);

            static const Item& invalid();
        };
        QVector<Item> m_data;

        static const int sm_minimumItems;
        static const int sm_maximumItems;
    public:
        Markers(QObject* parent = nullptr);

        Q_SLOT virtual bool read(QIODevice* iodevice = nullptr) override;
        Q_SLOT virtual bool write(QIODevice* iodevice = nullptr) const override;

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
        virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
        virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destParent, int destRow) override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        QModelIndex indexOf(uint id) const;
        const Item* findItemById(uint id) const;
        const Item& rowItem(int row) const;
        inline const Item& defaultItem() const { return rowItem(0); }

    };
}
Q_DECLARE_METATYPE(ns_model::Markers::Item)

QDataStream& operator<<(QDataStream& os, const ns_model::Markers::Item& item);
QDataStream& operator>>(QDataStream& is, ns_model::Markers::Item& item);
