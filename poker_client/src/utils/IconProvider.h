#pragma once

#include <QPixmapCache>
#include <QFileIconProvider>
#include <QLocale>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
#endif // !Q_MOC_RUN

class IconProvider
{
private:
    QPixmapCache m_iconCache;
    QFileIconProvider m_iconProvider;
public:
    QIcon fileIcon(const QString &filename);
    QIcon dirIcon();
    QIcon countryIcon(QLocale::Country country) const;

    QPixmap gameTypeIcon(const games_stat_t::first_type::value_type &item) const;
    QPixmap gameTypeIcon(const games_stat_t::second_type::value_type &item) const;
};
