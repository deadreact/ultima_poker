#include "model_timezone.h"
#include <QTimeZone>
#include <qsettings.h>
#include "../types.h"
#include "../../../api_models/api_models/api_util.hpp"
//#include

namespace
{
    QByteArrayList ids(const QList<QTimeZone>& lst) {
        QByteArrayList idLst;
        for (const QTimeZone& zone: lst) {
            idLst << zone.id();
        }
        return idLst;
    }

    QByteArrayList fillData()
    {
        QByteArrayList data;
        
        data << "Etc/GMT+12"
        	 << "Etc/GMT+11"
        	 << "Etc/GMT+10"
        	 << "America/Anchorage"
        	 << "America/Dawson"
        	 << "America/Creston"
        	 << "America/Chihuahua"
        	 << "America/Boise"
        	 << "America/Regina"
        	 << "America/Chicago"
        	 << "America/Belize"
        	 << "America/Bahia_Banderas"
        	 << "America/Indiana/Marengo"
        	 << "America/Detroit"
        	 << "America/Cancun"
        	 << "America/Bogota"
        	 << "America/Santiago"
        	 << "America/Glace_Bay"
        	 << "America/Caracas"
        	 << "America/Campo_Grande"
        	 << "America/Asuncion"
        	 << "America/Anguilla"
        	 << "America/St_Johns"
        	 << "America/Sao_Paulo"
        	 << "America/Montevideo"
        	 << "America/Godthab"
        	 << "America/Bahia"
        	 << "America/Argentina/La_Rioja"
        	 << "America/Araguaina"
        	 << "America/Noronha"
        	 << "Atlantic/Cape_Verde"
        	 << "America/Scoresbysund"
        	 << "Atlantic/Canary"
        	 << "Africa/Casablanca"
        	 << "Africa/Abidjan"
        	 << "Europe/Sarajevo"
        	 << "Europe/Belgrade"
        	 << "Arctic/Longyearbyen"
        	 << "Africa/Windhoek"
        	 << "Africa/Ceuta"
        	 << "Africa/Algiers"
        	 << "Europe/Kaliningrad"
        	 << "Europe/Helsinki"
        	 << "Europe/Chisinau"
        	 << "Asia/Nicosia"
        	 << "Asia/Jerusalem"
        	 << "Asia/Damascus"
        	 << "Asia/Beirut"
        	 << "Asia/Amman"
        	 << "Africa/Tripoli"
        	 << "Africa/Cairo"
        	 << "Africa/Blantyre"
        	 << "Europe/Moscow"
        	 << "Europe/Minsk"
        	 << "Europe/Istanbul"
        	 << "Asia/Baghdad"
        	 << "Asia/Aden"
        	 << "Africa/Addis_Ababa"
        	 << "Asia/Tehran"
        	 << "Indian/Mahe"
        	 << "Europe/Samara"
        	 << "Asia/Yerevan"
        	 << "Asia/Tbilisi"
        	 << "Asia/Dubai"
        	 << "Asia/Baku"
        	 << "Asia/Kabul"
        	 << "Asia/Yekaterinburg"
        	 << "Asia/Karachi"
        	 << "Antarctica/Mawson"
        	 << "Asia/Colombo"
        	 << "Asia/Calcutta"
        	 << "Asia/Katmandu"
        	 << "Asia/Dhaka"
        	 << "Antarctica/Vostok"
        	 << "Asia/Rangoon"
        	 << "Asia/Novosibirsk"
        	 << "Asia/Krasnoyarsk"
        	 << "Antarctica/Davis"
        	 << "Asia/Taipei"
        	 << "Asia/Irkutsk"
        	 << "Asia/Hong_Kong"
        	 << "Asia/Choibalsan"
        	 << "Asia/Brunei"
        	 << "Antarctica/Casey"
        	 << "Asia/Pyongyang"
        	 << "Asia/Seoul"
        	 << "Asia/Dili"
        	 << "Asia/Chita"
        	 << "Australia/Darwin"
        	 << "Australia/Adelaide"
        	 << "Australia/Melbourne"
        	 << "Australia/Currie"
        	 << "Australia/Brisbane"
        	 << "Asia/Sakhalin"
        	 << "Antarctica/DumontDUrville"
        	 << "Asia/Srednekolymsk"
        	 << "Asia/Magadan"
        	 << "Antarctica/Macquarie"
        	 << "Pacific/Fiji"
        	 << "Etc/GMT-12"
        	 << "Asia/Anadyr"
        	 << "Antarctica/McMurdo"
        	 << "Pacific/Apia"
        	 << "Etc/GMT-13"
        	 << "Etc/GMT-14";

        return data;
    }
}

namespace ns_model
{
    Timezone::Timezone(QObject *parent)
        : QAbstractListModel(parent)
        , m_timezoneIds(fillData())
        , m_currentTimeZone(-1)
    {
        resetData();
    }

    Timezone::~Timezone()
    {

    }

    Timezone *Timezone::sharedTimezone()
    {
        static Timezone* tmz = new Timezone;
        return tmz;
    }

    int Timezone::rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) {
            return 0;
        }
        return AvailableZones::count;
    }

    QVariant Timezone::data(const QModelIndex &index, int role) const
    {
        if (!index.isValid() || index.parent().isValid() || index.column() != 0) {
            return QVariant();
        }

        switch (role) {
        case UserType::RowItemIdRole:
            return id(index);
        case Qt::DisplayRole:
            return displayName(index);
        case Qt::CheckStateRole:
            return isCurrent(index) ? Qt::Checked : Qt::Unchecked;
        default:
            return QVariant();
        }
        return QVariant();
    }

    void Timezone::resetData()
    {
        beginResetModel();

//        QByteArray id;
//        if (!ns_utildb::restoreState("CurrentTimeZone", id)) {
//            id = QTimeZone::systemTimeZoneId();
//            ns_utildb::saveState("CurrentTimeZone", id);
//        }
//        m_currentTimeZone = indexOf(id).row();
        QSettings s("Ultima Poker", "Ultima Poker");
        if (!s.contains("CurrentTimeZone")) {
            s.setValue("CurrentTimeZone", QTimeZone::systemTimeZoneId());
        }
        QByteArray currentId = s.value("CurrentTimeZone").toByteArray();
        m_currentTimeZone = indexOf(currentId).row();

        endResetModel();
    }

    void Timezone::setCurrentTimeZone(AvailableZones::Id currentTimeZone)
    {
        if (m_currentTimeZone == underlying_cast(currentTimeZone))
            return;

        m_currentTimeZone = underlying_cast(currentTimeZone);
        QSettings s("Ultima Poker", "Ultima Poker");
        s.setValue("CurrentTimeZone", id(index(m_currentTimeZone, 0)));
        emit currentTimeZoneChanged(m_currentTimeZone);
    }

    QModelIndex Timezone::indexOf(const QByteArray &timezoneId) const
    {
        for (int i = 0; i < m_timezoneIds.count(); i++) {
            const QByteArray& id = m_timezoneIds.at(i);
            if (id == timezoneId) {
                return index(i, 0);
            }
        }
        return QModelIndex();
    }
	
	QByteArray Timezone::id(const QModelIndex &idx) const
    {
        if (!idx.isValid()) {
            return QByteArray();
        }
        QString str = ns_util::enumToString(AvailableZones::Id(idx.row()));
        return str.replace("_sl_", "/").replace("_mn_", "-").replace("_pl_", "+").toLatin1();
    }
    


    QTimeZone Timezone::zone(const QModelIndex &idx) const
    {
        return QTimeZone(id(idx));
    }

    QString Timezone::displayName(const QModelIndex &idx) const
    {
        return zone(idx).displayName(QTimeZone::GenericTime);
    }

    QString Timezone::trDisplayName(const QModelIndex &idx) const
    {
        switch (idx.row()) {
        case 0: return tr("Etc/GMT+12");
        case 1: return tr("Etc/GMT+11");
        case 2: return tr("Etc/GMT+10");
        case 3: return tr("America/Anchorage");
        case 4: return tr("America/Dawson");
        case 5: return tr("America/Creston");
        case 6: return tr("America/Chihuahua");
        case 7: return tr("America/Boise");
        case 8: return tr("America/Regina");
        case 9: return tr("America/Chicago");
        case 10: return tr("America/Belize");
        case 11: return tr("America/Bahia_Banderas");
        case 12: return tr("America/Indiana/Marengo");
        case 13: return tr("America/Detroit");
        case 14: return tr("America/Cancun");
        case 15: return tr("America/Bogota");
        case 16: return tr("America/Santiago");
        case 17: return tr("America/Glace_Bay");
        case 18: return tr("America/Caracas");
        case 19: return tr("America/Campo_Grande");
        case 20: return tr("America/Asuncion");
        case 21: return tr("America/Anguilla");
        case 22: return tr("America/St_Johns");
        case 23: return tr("America/Sao_Paulo");
        case 24: return tr("America/Montevideo");
        case 25: return tr("America/Godthab");
        case 26: return tr("America/Bahia");
        case 27: return tr("America/Argentina/La_Rioja");
        case 28: return tr("America/Araguaina");
        case 29: return tr("America/Noronha");
        case 30: return tr("Atlantic/Cape_Verde");
        case 31: return tr("America/Scoresbysund");
        case 32: return tr("Atlantic/Canary");
        case 33: return tr("Africa/Casablanca");
        case 34: return tr("Africa/Abidjan");
        case 35: return tr("Europe/Sarajevo");
        case 36: return tr("Europe/Belgrade");
        case 37: return tr("Arctic/Longyearbyen");
        case 38: return tr("Africa/Windhoek");
        case 39: return tr("Africa/Ceuta");
        case 40: return tr("Africa/Algiers");
        case 41: return tr("Europe/Kaliningrad");
        case 42: return tr("Europe/Helsinki");
        case 43: return tr("Europe/Chisinau");
        case 44: return tr("Asia/Nicosia");
        case 45: return tr("Asia/Jerusalem");
        case 46: return tr("Asia/Damascus");
        case 47: return tr("Asia/Beirut");
        case 48: return tr("Asia/Amman");
        case 49: return tr("Africa/Tripoli");
        case 50: return tr("Africa/Cairo");
        case 51: return tr("Africa/Blantyre");
        case 52: return tr("Europe/Moscow");
        case 53: return tr("Europe/Minsk");
        case 54: return tr("Europe/Istanbul");
        case 55: return tr("Asia/Baghdad");
        case 56: return tr("Asia/Aden");
        case 57: return tr("Africa/Addis_Ababa");
        case 58: return tr("Asia/Tehran");
        case 59: return tr("Indian/Mahe");
        case 60: return tr("Europe/Samara");
        case 61: return tr("Asia/Yerevan");
        case 62: return tr("Asia/Tbilisi");
        case 63: return tr("Asia/Dubai");
        case 64: return tr("Asia/Baku");
        case 65: return tr("Asia/Kabul");
        case 66: return tr("Asia/Yekaterinburg");
        case 67: return tr("Asia/Karachi");
        case 68: return tr("Antarctica/Mawson");
        case 69: return tr("Asia/Colombo");
        case 70: return tr("Asia/Calcutta");
        case 71: return tr("Asia/Katmandu");
        case 72: return tr("Asia/Dhaka");
        case 73: return tr("Antarctica/Vostok");
        case 74: return tr("Asia/Rangoon");
        case 75: return tr("Asia/Novosibirsk");
        case 76: return tr("Asia/Krasnoyarsk");
        case 77: return tr("Antarctica/Davis");
        case 78: return tr("Asia/Taipei");
        case 79: return tr("Asia/Irkutsk");
        case 80: return tr("Asia/Hong_Kong");
        case 81: return tr("Asia/Choibalsan");
        case 82: return tr("Asia/Brunei");
        case 83: return tr("Antarctica/Casey");
        case 84: return tr("Asia/Pyongyang");
        case 85: return tr("Asia/Seoul");
        case 86: return tr("Asia/Dili");
        case 87: return tr("Asia/Chita");
        case 88: return tr("Australia/Darwin");
        case 89: return tr("Australia/Adelaide");
        case 90: return tr("Australia/Melbourne");
        case 91: return tr("Australia/Currie");
        case 92: return tr("Australia/Brisbane");
        case 93: return tr("Asia/Sakhalin");
        case 94: return tr("Antarctica/DumontDUrville");
        case 95: return tr("Asia/Srednekolymsk");
        case 96: return tr("Asia/Magadan");
        case 97: return tr("Antarctica/Macquarie");
        case 98: return tr("Pacific/Fiji");
        case 99: return tr("Etc/GMT-12");
        case 100: return tr("Asia/Anadyr");
        case 101: return tr("Antarctica/McMurdo");
        case 102: return tr("Pacific/Apia");
        case 103: return tr("Etc/GMT-13");
        case 104: return tr("Etc/GMT-14");

        default:
            return QString();
        }
    }

    QTimeZone Timezone::currentZone() const {
        return zone(index(m_currentTimeZone, 0));
    }

    QDateTime Timezone::currentDateTime() const
    {
        return QDateTime::currentDateTimeUtc().toTimeZone(currentZone());
    }

    bool Timezone::isCurrent(const QModelIndex &idx) const
    {
        return (m_currentTimeZone == idx.row()) && (idx.column() == 0);
    }

    bool Timezone::isCurrent(const QByteArray &id) const
    {
        return isCurrent(indexOf(id));
    }

    AvailableZones::Id Timezone::currentTimeZone() const
    {
        return AvailableZones::Id(m_currentTimeZone);
    }

    void Timezone::switchTimezone(const QByteArray &id)
    {
        if (isCurrent(id)) {
            return;
        }

        QModelIndex index1 = current();
        QModelIndex index2 = indexOf(id);

        QSettings s("Ultima Poker", "Ultima Poker");
        s.setValue("CurrentTimeZone", id);
        m_currentTimeZone = indexOf(id).row();

        QVector<int> roles;
        roles  << Qt::DisplayRole << UserType::RowItemIdRole << Qt::CheckStateRole;

        emit dataChanged(index1, index1, roles);
        emit dataChanged(index2, index2, roles);
    }

    void Timezone::switchTimezone(int row)
    {
        if (m_currentTimeZone == row) {
            return;
        }
        int saved = m_currentTimeZone;
        m_currentTimeZone = row;
        QSettings s("Ultima Poker", "Ultima Poker");
        s.setValue("CurrentTimeZone", id(index(row, 0)));

        emit dataChanged(index(saved, 0), index(saved, 0));
        emit dataChanged(index(row, 0), index(row, 0));
    }

    void Timezone::setCurrent(const QModelIndex &idx)
    {
        if (!idx.isValid() || idx.parent().isValid() || idx.column() != 0) {
            switchTimezone(-1);
        } else {
            switchTimezone(idx.row());
        }
    }



} //namespace ns_model


