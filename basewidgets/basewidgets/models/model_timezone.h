#pragma once

#include "../basewidgets_global.h"
#include <QAbstractTableModel>
//#include <types.h>
#include <QDate>



namespace ns_model
{
    class BASEWIDGETS_DLL AvailableZones
    {
        Q_GADGET
    public:
        enum Id
        {
            Etc_sl_GMT_pl_12 = 0,
            Etc_sl_GMT_pl_11,
            Etc_sl_GMT_pl_10,
            America_sl_Anchorage,
            America_sl_Dawson,
            America_sl_Creston,
            America_sl_Chihuahua,
            America_sl_Boise,
            America_sl_Regina,
            America_sl_Chicago,
            America_sl_Belize,
            America_sl_Bahia_Banderas,
            America_sl_Indiana_sl_Marengo,
            America_sl_Detroit,
            America_sl_Cancun,
            America_sl_Bogota,
            America_sl_Santiago,
            America_sl_Glace_Bay,
            America_sl_Caracas,
            America_sl_Campo_Grande,
            America_sl_Asuncion,
            America_sl_Anguilla,
            America_sl_St_Johns,
            America_sl_Sao_Paulo,
            America_sl_Montevideo,
            America_sl_Godthab,
            America_sl_Bahia,
            America_sl_Argentina_sl_La_Rioja,
            America_sl_Araguaina,
            America_sl_Noronha,
            Atlantic_sl_Cape_Verde,
            America_sl_Scoresbysund,
            Atlantic_sl_Canary,
            Africa_sl_Casablanca,
            Africa_sl_Abidjan,
            Europe_sl_Sarajevo,
            Europe_sl_Belgrade,
            Arctic_sl_Longyearbyen,
            Africa_sl_Windhoek,
            Africa_sl_Ceuta,
            Africa_sl_Algiers,
            Europe_sl_Kaliningrad,
            Europe_sl_Helsinki,
            Europe_sl_Chisinau,
            Asia_sl_Nicosia,
            Asia_sl_Jerusalem,
            Asia_sl_Damascus,
            Asia_sl_Beirut,
            Asia_sl_Amman,
            Africa_sl_Tripoli,
            Africa_sl_Cairo,
            Africa_sl_Blantyre,
            Europe_sl_Moscow,
            Europe_sl_Minsk,
            Europe_sl_Istanbul,
            Asia_sl_Baghdad,
            Asia_sl_Aden,
            Africa_sl_Addis_Ababa,
            Asia_sl_Tehran,
            Indian_sl_Mahe,
            Europe_sl_Samara,
            Asia_sl_Yerevan,
            Asia_sl_Tbilisi,
            Asia_sl_Dubai,
            Asia_sl_Baku,
            Asia_sl_Kabul,
            Asia_sl_Yekaterinburg,
            Asia_sl_Karachi,
            Antarctica_sl_Mawson,
            Asia_sl_Colombo,
            Asia_sl_Calcutta,
            Asia_sl_Katmandu,
            Asia_sl_Dhaka,
            Antarctica_sl_Vostok,
            Asia_sl_Rangoon,
            Asia_sl_Novosibirsk,
            Asia_sl_Krasnoyarsk,
            Antarctica_sl_Davis,
            Asia_sl_Taipei,
            Asia_sl_Irkutsk,
            Asia_sl_Hong_Kong,
            Asia_sl_Choibalsan,
            Asia_sl_Brunei,
            Antarctica_sl_Casey,
            Asia_sl_Pyongyang,
            Asia_sl_Seoul,
            Asia_sl_Dili,
            Asia_sl_Chita,
            Australia_sl_Darwin,
            Australia_sl_Adelaide,
            Australia_sl_Melbourne,
            Australia_sl_Currie,
            Australia_sl_Brisbane,
            Asia_sl_Sakhalin,
            Antarctica_sl_DumontDUrville,
            Asia_sl_Srednekolymsk,
            Asia_sl_Magadan,
            Antarctica_sl_Macquarie,
            Pacific_sl_Fiji,
            Etc_sl_GMT_mn_12,
            Asia_sl_Anadyr,
            Antarctica_sl_McMurdo,
            Pacific_sl_Apia,
            Etc_sl_GMT_mn_13,
            Etc_sl_GMT_mn_14
        };
        Q_ENUM(Id)

        static const int count = Id::Etc_sl_GMT_mn_14 + 1;
    };

    class BASEWIDGETS_DLL Timezone : public QAbstractListModel
    {
        Q_OBJECT

        Q_PROPERTY(AvailableZones::Id currentTimeZone READ currentTimeZone WRITE setCurrentTimeZone NOTIFY currentTimeZoneChanged)

        QByteArrayList m_timezoneIds;
        int m_currentTimeZone;

    public:
        Timezone(QObject* parent = nullptr);
        virtual ~Timezone();

        static Timezone* sharedTimezone();

        virtual int rowCount(const QModelIndex &parent) const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;

        QModelIndex indexOf(const QByteArray& timezoneId) const;

        inline int currentRow() const { return m_currentTimeZone; }
        inline QModelIndex current() const { return index(currentRow(), 0); }

        QByteArray id(const QModelIndex& idx) const;
        QTimeZone zone(const QModelIndex& idx) const;
        QString displayName(const QModelIndex& idx) const;
        QString trDisplayName(const QModelIndex& idx) const;

        inline QByteArray currentId() const { return id(index(m_currentTimeZone, 0)); }
        QTimeZone currentZone() const;
        inline QString currentDisplayName() const { return displayName(index(m_currentTimeZone, 0)); }
        inline QString currentTrDisplayName() const { return trDisplayName(index(m_currentTimeZone, 0)); }

        QDateTime currentDateTime() const;

        bool isCurrent(const QModelIndex& idx) const;
        bool isCurrent(const QByteArray& id) const;
        AvailableZones::Id currentTimeZone() const;

    public slots:
        void switchTimezone(const QByteArray& id);
        void switchTimezone(int row);
        void setCurrent(const QModelIndex& idx);
        void resetData();
        void setCurrentTimeZone(AvailableZones::Id currentTimeZone);
    signals:
        void currentTimeZoneChanged(int currentTimeZone);
    };

} //namespace ns_model
