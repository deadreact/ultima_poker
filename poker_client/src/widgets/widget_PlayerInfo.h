#pragma once

#include <QWidget>
#include <QLocale>
#ifndef Q_MOC_RUN
    #include "rpc/reply_statistic.h"
#endif // Q_MOC_RUN

namespace ns_widget
{
    class PlayerInfo : public QWidget
    {
        Q_OBJECT

        Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName)
        Q_PROPERTY(QString playerMoney READ playerMoney WRITE setPlayerMoney)
        Q_PROPERTY(QLocale::Country playerCountry READ playerCountry WRITE setPlayerCountry)
        Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
        Q_ENUMS(QLocale::Country)

        QString m_playerName;
        QString m_playerMoney;
        QLocale::Country m_playerCountry;
        QPixmap* m_flag;
        int m_index;
    public:
        explicit PlayerInfo(QWidget *parent = 0);
        explicit PlayerInfo(const gbp::ns_api::ns_statistic::player_t& playerInfo, QWidget *parent = 0);

        void assignLocale(const QLocale& locale);
        void update(const gbp::ns_api::ns_statistic::player_t& playerInfo);
        void init();
        QString playerName() const;
        QString playerMoney() const;
        QLocale::Country playerCountry() const;
        int index() const;

        inline bool operator>=(const PlayerInfo& other) const { return m_index >= other.m_index; }
        inline bool operator<=(const PlayerInfo& other) const { return m_index <= other.m_index; }
        inline bool operator >(const PlayerInfo& other) const { return m_index > other.m_index; }
        inline bool operator <(const PlayerInfo& other) const { return m_index < other.m_index; }
        inline bool operator==(const PlayerInfo& other) const { return m_playerName == other.m_playerName; }
        inline bool operator!=(const PlayerInfo& other) const { return !operator==(other); }
    signals:
        void indexChanged(int index);
        void infoUpdated();
    public slots:
        void setPlayerName(const QString &playerName);
        void setPlayerMoney(const QString& playerMoney);
        void setPlayerCountry(QLocale::Country playerCountry);
        void setIndex(int index);

        void clear();
    protected:
        virtual void paintEvent(QPaintEvent*) override;
    };

} //namespace widget
