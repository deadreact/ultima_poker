#pragma once

#include <QAbstractListModel>

struct GameId;

namespace ns_model
{

    class TableSettingBase : public QAbstractListModel
    {
        Q_OBJECT
        const QString m_name;
        const int m_index;
    public:
        explicit TableSettingBase(const QString& name, int index, QObject *parent = nullptr);
        virtual ~TableSettingBase();

        inline const QString& name() const { return m_name; }
        inline int index() const { return m_index; }
    public:
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &idx, int role) const override;
    };


    class TableSettingFeltColor : public TableSettingBase
    {
        Q_OBJECT
    public:
        TableSettingFeltColor(QObject* parent = nullptr);
        virtual QVariant data(const QModelIndex &index, int role) const override;
    };

    class TableSettingBackground : public TableSettingBase
    {
        Q_OBJECT
    public:
        TableSettingBackground(QObject* parent = nullptr);
    };
    class TableSettingDeck : public TableSettingBase
    {
        Q_OBJECT
    public:
        TableSettingDeck(QObject* parent = nullptr);
    };
    class TableSettingShirt : public TableSettingBase
    {
        Q_OBJECT
    public:
        TableSettingShirt(QObject* parent = nullptr);
    };
    class TableSettingTheme : public TableSettingBase
    {
        Q_OBJECT
    public:
        TableSettingTheme(QObject* parent = nullptr);
    };


} //namespace ns_model
