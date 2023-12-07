#include "timezonebox.h"
#include <qstyleditemdelegate.h>
#include "../../api_models/api_models/api_util.hpp"

TimeZoneBox::TimeZoneBox(QWidget *parent)
    : QComboBox(parent)
    , m_model(nullptr)
    , m_isModelCreatedHere(false)
{
//    setModel(ns_model::Timezone::sharedTimezone());
//    m_isModelCreatedHere = false;

//    connect(m_model, &ns_model::Timezone::currentTimeZoneChanged, this, [this](int index) {
//        if (sender() == model()) {
//            setCurrentIndex(index);
//        }
//    });
}

TimeZoneBox::~TimeZoneBox() {}

void TimeZoneBox::setModel(ns_model::Timezone *model)
{
    if (m_model != model) {
        if (m_model && m_isModelCreatedHere) {
            delete m_model;
            m_isModelCreatedHere = false;
        }
        m_model = model;

        connect(m_model, &ns_model::Timezone::dataChanged, this, [this] {
            setCurrentIndex(m_model->currentRow());
        });
        connect(m_model, &ns_model::Timezone::modelReset, this, [this] {
            setCurrentIndex(m_model->currentRow());
        });
        setItemDelegate(new QStyledItemDelegate);

        QComboBox::setModel(model);
    }
}

ns_model::Timezone *TimeZoneBox::model() const
{
    return m_model;
}

QTimeZone TimeZoneBox::currentTimeZone() const
{
    return m_model ? m_model->currentZone() : QTimeZone::systemTimeZone();
}

QByteArray TimeZoneBox::currentTimeZoneId() const
{
    return m_model ? m_model->currentId() : QByteArray();
}

QDateTime TimeZoneBox::currentDateTime() const
{
    return m_model ? m_model->currentDateTime() : QDateTime::currentDateTimeUtc();
}

ns_model::AvailableZones::Id TimeZoneBox::currentTimezoneIndex() const
{
    return ns_model::AvailableZones::Id(currentIndex());
}

void TimeZoneBox::acceptCurrentTimezone()
{
    m_model->switchTimezone(currentIndex());
}

void TimeZoneBox::setCurrentTimezone(ns_model::AvailableZones::Id id)
{
    if (id != currentTimezoneIndex())
    {
        setCurrentIndex(underlying_cast(id));
    }
}
