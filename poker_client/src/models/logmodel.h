#pragma once

#include <QAbstractItemModel>
#include <qdatetime.h>
#include <api_models/model_io.hpp>
#include "../../../common/api/gbp_int.hpp"

class LogMsgItem
{
    Q_GADGET
    static gbp_i64 sm_idIncrementer;
public:
    struct Child {
        QString text;
        QColor foreground;
        QColor background;
    };

    enum MsgType {
        MsgFunction,
        MsgInfo,
        MsgWarning,
        MsgCritical,
        MsgFatal
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
    virtual void addChild(const QString& text, uint foreground = 0x191919, uint background = 0xfafafa);

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
};

class LogModel : public QAbstractItemModel, public ns_model::IOModel
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
public:
    virtual LogModel& newMsg(const QString &text, LogMsgItem::MsgType type = LogMsgItem::MsgInfo);
    virtual LogModel& operator<<(const QString& text);
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

    // IOModel interface
public:
    virtual bool read(QIODevice *iodevice) override;
    virtual bool write(QIODevice *iodevice) const override;
};
