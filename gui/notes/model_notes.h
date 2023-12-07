#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <functional>
#include <qcolor.h>
#include <qlocale.h>
#include "model_io.h"
#include "data.h"


class QAbstractButton;

namespace ns_model
{
    class Markers;

    class NOTES_DLL Notes : public QAbstractTableModel, public IOModel
    {
        Q_OBJECT
    public:
//        using Item = ItemData<Notes>;
        struct Item {
            uint markerId;
            QString nickname;
            QString note;
            QDateTime modified;

            QString toString() const;
        };
        using on_create_item = std::function<void(bool, Item)>;
        struct ItemEditData {
            uint markerId;
            QString note;
            ItemEditData(): markerId(-1), note("") {}
            ItemEditData(uint id, const QString& note): markerId(id), note(note) {}
        };

        Markers *m_sourceModel;
        QVector<Item> m_data;
    public:
        Notes(Markers* sourceModel = nullptr, QObject* parent = nullptr);

        void setSourceModel(Markers* m);
        inline Markers* sourceModel() const { return m_sourceModel; }

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
        virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        QModelIndex indexOf(const QString& nickname);
        QColor markerColor(const QModelIndex& idx) const;
        QString note(const QModelIndex& idx) const;

        Q_SLOT virtual bool read(QIODevice *iodevice = nullptr) override;
        Q_SLOT virtual bool write(QIODevice *iodevice = nullptr) const override;
    protected:
        void connectToModel();
        void disconnectFromModel();
    public:
        QModelIndex createNote(const QString& nickname, uint markerId, on_create_item replyHandler = on_create_item());
    private slots:
        void onSourceModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles);
    };
}

Q_DECLARE_METATYPE(ns_model::Notes::Item)
Q_DECLARE_METATYPE(ns_model::Notes::ItemEditData)

QDataStream& operator<<(QDataStream& os, const ns_model::Notes::Item& item);
QDataStream& operator>>(QDataStream& is, ns_model::Notes::Item& item);
