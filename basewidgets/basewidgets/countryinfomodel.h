#pragma once

#include "basewidgets_global.h"
#include <QSortFilterProxyModel>

class CountryInfoSourceModel;

class BASEWIDGETS_DLL CountryInfoModel : public QSortFilterProxyModel
{
    Q_OBJECT

    QScopedPointer<CountryInfoSourceModel> m_sourceModel;

    QString m_excludeRegExp;
    int m_displayFlag;
public:
    CountryInfoModel(QObject* parent = nullptr);
    virtual ~CountryInfoModel();

    // QAbstractItemModel interface
    void initData();
    const QString& excludeRegExp() const { return m_excludeRegExp; }
public slots:

    void setExcludeRegExp(const QString &excludeRegExp);
public:
    void setDisplayFlag(int f);
    int displayFlag() const;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
};
