#include <precompiled.h>
#include "Utils.h"
#include <QEvent>
#include <map>
#include <QBuffer>
#include <QApplication>
#include <QMetaMethod>
#include <qregularexpression.h>

#ifndef Q_MOC_RUN
    #include "rpc/reply_statistic.h"
    #include <api/table/types.h>
    #include <api/api_common.h>
#endif


namespace ns_debug_util
{
    const char* toCString(const QEvent* e) {
        return QMetaEnum::fromType<QEvent::Type>().valueToKey(e->type());
    }
}

namespace ns_util
{

    QList<QTimeZone> filteredTimeZones()
    {
        QDateTime currDateTime = QDateTime::currentDateTimeUtc();
        QList<QByteArray> lst = QTimeZone::availableTimeZoneIds();
        QList<QTimeZone> lstFiltered;
        QSet<QString> setOfNames;

        QRegularExpression re("^\\(UTC[+-]\\d\\d:\\d\\d\\)");
        for (auto&& item: lst)
        {
            const QTimeZone tzone(item);
            QString name = tzone.displayName(QTimeZone::GenericTime);

            if (re.match(name).hasMatch() && !setOfNames.contains(name))
            {
                setOfNames.insert(name);
                lstFiltered.push_back(tzone);
            }
        }
        std::stable_sort( lstFiltered.begin(), lstFiltered.end()
                          , [&currDateTime](const QTimeZone& a, const QTimeZone& b){ return a.standardTimeOffset(currDateTime) <= b.standardTimeOffset(currDateTime); });

        return lstFiltered;
    }

}






