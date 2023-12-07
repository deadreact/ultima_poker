#pragma once

#include <QAbstractItemModel>
#include <qdatetime.h>
#include <qcolor.h>
#include "model_io.hpp"
#include "../../common/api/gbp_int.hpp"

namespace ns_model
{
    class API_MODEL_DLL LogMsgItem
    {
        Q_GADGET
        static gbp_i64 sm_idIncrementer;
    public:
        struct Child {
            QString text;
            QColor foreground;
            QColor background;
            bool underlined;
            bool bold;
            bool italic;
        };

        enum MsgType {
            MsgFunction = 0,
            MsgInfo     = 1,
            MsgWarning  = 2,
            MsgCritical = 3,
            MsgFatal    = 4,
            MsgUser
        };
        Q_ENUM(MsgType)
        const uint m_id;
        const QDateTime m_timeMark;
        MsgType m_type;
        QString m_msg;
        QList<Child> m_children;
    public:
        LogMsgItem(const QString& msg = "<empty>", MsgType type = MsgInfo)
            : m_id(sm_idIncrementer++)
            , m_timeMark(QDateTime::currentDateTimeUtc())
            , m_type(type)
            , m_msg(msg)
        {}
        virtual ~LogMsgItem() {}

        inline uint id() const { return m_id; }
        inline const QDateTime& timeMark() const { return m_timeMark; }


        virtual QVariant data(int role = Qt::DisplayRole) const;
        virtual QVariant childData(int child, int role = Qt::DisplayRole) const;
        virtual void addChild(const QString& text, uint foreground = 0x191919, uint background = 0xfafafa, bool underlined = false, bool bold = false, bool italic = false);

        void setText(const QString& text);
        void setMsgType(MsgType type);

        inline int childrenCount() const {
            return m_children.size();
        }

        inline bool operator ==(const LogMsgItem& item) const {
            return id() == item.id();
        }
        inline bool operator <(const LogMsgItem& item) const {
            return id() < item.id();
        }
        MsgType type() const;
    };

    class API_MODEL_DLL LogModel : public QAbstractItemModel, public ns_model::IOModel
    {
        Q_OBJECT
    private:
        QList<LogMsgItem*> m_messages;
        bool m_useTableRepresentation;
        int m_maxChildren;
    public:
        LogModel(QObject* parent = nullptr);
        virtual ~LogModel();
    protected:
        template <typename T>
        QString toString(const T& arg) {
            QString string;
            QDebug stream(&string);
            stream << arg;
            return string;
        }
    public:
        LogModel& operator<<(const QString& arg);
        LogModel& operator<<(const QUrl& url);
        template <typename T>
        LogModel& operator<<(const T& arg) {
            return operator<<(toString(arg));
        }

        virtual LogModel& newMsg(const QString &text, LogMsgItem::MsgType type = LogMsgItem::MsgInfo);
        LogModel& newMsg(LogMsgItem *item);

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        const LogMsgItem* indexToItem(const QModelIndex& index) const;
        QModelIndex itemToIndex(const LogMsgItem *item) const;

        Q_SLOT void setUseTableRepresentation(bool b);

    protected:
        LogMsgItem* lastItem();
        // QAbstractItemModel interface

    public:
        virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &child) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual bool hasChildren(const QModelIndex &parent) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;
        virtual QMap<int, QVariant> itemData(const QModelIndex &index) const override;
        virtual bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &values) override;

        // IOModel interface
    public:
        virtual bool read(QIODevice *iodevice) override;
        virtual bool write(QIODevice *iodevice) const override;
    };

} //namespace ns_model
