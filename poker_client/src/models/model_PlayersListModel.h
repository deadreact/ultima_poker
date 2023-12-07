#pragma once

#include <core/money.h>
#include <qlocale.h>
#include <QAbstractTableModel>
#include <tools/types_fwd.hpp>

struct PlayersListDataItem
{
    QString m_nickname;
    QLocale::Country m_country;
    Money m_stack;
    PlayersListDataItem(): m_nickname(""), m_country(QLocale::AnyCountry), m_stack(0) {}
    PlayersListDataItem(const QString& nickname, QLocale::Country country, Money stack)
        : m_nickname(nickname), m_country(country), m_stack(stack)
    {}
    void set(const QString& nickname, QLocale::Country country, Money stack) {
        m_nickname = nickname;
        m_country = country;
        m_stack = stack;
    }
    template <typename T>
    void set(const T& info) {
        m_nickname = QString::fromStdString(info._nickname);
        m_country = QLocale::Country(info._country);
        m_stack = info._money;
    }
};
typedef std::vector<PlayersListDataItem> PlayersListData;

namespace ns_model
{

    class PlayersListModel : public QAbstractTableModel
    {
        Q_OBJECT
        PlayersListData m_data;
        const gbp::ns_api::ns_statistic::tournament_stat_t* m_currentData;
    public:
        PlayersListModel(const gbp::ns_api::ns_statistic::tournament_stat_t* data = nullptr, QObject* parent = nullptr);

        void updateData(const gbp::ns_api::ns_statistic::tournament_stat_t* data);
        void updateData(const gbp::ns_api::ns_statistic::cash_stat_t* data);
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;

        void fetchAllPlayers();
//        virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    };
}
