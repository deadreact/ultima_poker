#include <precompiled.h>
#include "model_TableSettings.h"
#include <db/SettingsDatabase.h>
#include <db/SharedDataService.h>
#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
#endif // !Q_MOC_RUN

namespace ns_model
{

    TableSettingBase::TableSettingBase(const QString& name, int index, QObject *parent)
        : QAbstractListModel(parent)
        , m_name(name)
        , m_index(index)
    {

    }
    TableSettingBase::~TableSettingBase()
    {

    }

    int TableSettingBase::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return SettingsDatabase::values(m_name).size();
    }

    QVariant TableSettingBase::data(const QModelIndex &idx, int /*role*/) const
    {
        return SettingsDatabase::value(m_name, idx.row());
    }

} //namespace ns_model


ns_model::TableSettingFeltColor::TableSettingFeltColor(QObject *parent)
    : TableSettingBase(SettingsDatabase::name(0), 0, parent)
{}
ns_model::TableSettingBackground::TableSettingBackground(QObject *parent)
    : TableSettingBase(SettingsDatabase::name(1), 1, parent)
{}
ns_model::TableSettingDeck::TableSettingDeck(QObject *parent)
    : TableSettingBase(SettingsDatabase::name(2), 2, parent)
{}
ns_model::TableSettingShirt::TableSettingShirt(QObject *parent)
    : TableSettingBase(SettingsDatabase::name(3), 3, parent)
{}
ns_model::TableSettingTheme::TableSettingTheme(QObject *parent)
    : TableSettingBase(SettingsDatabase::name(4), 4, parent)
{}


QVariant ns_model::TableSettingFeltColor::data(const QModelIndex &index, int /*role*/) const
{
    return SettingsDatabase::value(name(), index.row());
}
