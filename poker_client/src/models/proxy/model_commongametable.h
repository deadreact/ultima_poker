#pragma once

#include <qidentityproxymodel.h>

class GameId;

namespace ns_model
{

class table_state_t;

class CommonGameTable : public QIdentityProxyModel
{
    Q_OBJECT
    table_state_t* m_sourceModel;
    GameId m_gameId;

    struct PrivateMappings;
    PrivateMappings *m_mappings;
public:
    CommonGameTable(QObject* parent = nullptr);
    virtual ~CommonGameTable();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    table_state_t* sourceModel() const;

    void setGameID(const GameId& id);
    GameId gameID() const;

protected slots:
    virtual void onSourceRowsAboutToBeInserted(const QModelIndex& parent, int first, int last);
    virtual void onSourceRowsAboutToBeRemoved(const QModelIndex& parent, int first, int last);
    virtual void onSourceRowsRemoved(const QModelIndex& parent, int first, int last);
    virtual void onSourceRowsInserted(const QModelIndex& parent, int first, int last);
    virtual void onSourceRowsAboutToBeMoved(const QModelIndex& sourceParent, int first, int last, const QModelIndex& destinationParent, int destinationRow);
    virtual void onSourceRowsMoved(const QModelIndex& sourceParent, int first, int last, const QModelIndex& destinationParent, int destinationRow);
    virtual void onSourceModelAboutToBeReset();
    virtual void onSourceModelReset();
    virtual void onSourceDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& roles = QVector<int>());

    virtual void updateMappings();
public:
    virtual QModelIndex sourceIndex(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex &child = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = UserType::OriginDataRole) override;
    virtual QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    virtual bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;
public:
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    template <typename T>
    bool setOriginData(const QModelIndex& index, const T& value) {
        return setData(index, QVariant::fromValue(value), UserType::OriginDataRole);
    }
};


} //namespace ns_model
