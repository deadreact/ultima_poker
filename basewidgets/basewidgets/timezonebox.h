#pragma once

#include "basewidgets_global.h"
#include <qcombobox.h>
#include <qtimezone.h>
#include "models/model_timezone.h"

class BASEWIDGETS_DLL TimeZoneBox : public QComboBox
{
    Q_OBJECT

    Q_PROPERTY(QByteArray currentTimeZoneId READ currentTimeZoneId)
    Q_PROPERTY(QDateTime currentDateTime READ currentDateTime)
    Q_PROPERTY(ns_model::AvailableZones::Id currentTimezoneIndex READ currentTimezoneIndex WRITE setCurrentTimezone)
    Q_ENUMS(ns_model::AvailableZones::Id)
private:
    ns_model::Timezone* m_model;
    bool m_isModelCreatedHere;
public:
    Q_INVOKABLE TimeZoneBox(QWidget *parent = nullptr);
    virtual ~TimeZoneBox();

    void setModel(ns_model::Timezone *model);
    ns_model::Timezone* model() const;

    QTimeZone currentTimeZone() const;
    QByteArray currentTimeZoneId() const;
    QDateTime currentDateTime() const;
    ns_model::AvailableZones::Id currentTimezoneIndex() const;

public slots:
    void acceptCurrentTimezone();
    void setCurrentTimezone(ns_model::AvailableZones::Id id);
};
