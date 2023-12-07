#pragma once

#include <QSortFilterProxyModel>
#include <qbasictimer.h>

#ifndef Q_MOC_RUN
    #include "rpc/reply_statistic.h"
#endif // Q_MOC_RUN

#include <PokerApplication.h>
#include <db/SharedDataService.h>

//QString capitalized(const QString &str);
//QColor titleColor(const games_stat_t::second_type::value_type &item);

// namespace ns_model
// {
//     class cash_stat_t;
//     class tournament_stat_t;

//     class CashTables : public QSortFilterProxyModel
//     {
//         Q_OBJECT

//         ns_model::cash_stat_t* m_sourceModel;

//     public:
//         enum class column {
//             title = 0,
//             bets,
//             game,
//             type,
//             players,
//             waiters,
//             pot,
//             flop,
//             distribution,

//             count
//         };
//     public:
//         CashTables(QObject* parent = nullptr);
//         virtual int columnCount(const QModelIndex &parent) const override;
//         virtual QVariant data(const QModelIndex &index, int role) const override;

//         virtual void setSourceModel(QAbstractItemModel *sourceModel) override;
// //        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
// //        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

//         virtual QModelIndex indexOf(gbp_i64 id) const;
//         QModelIndex indexOf(gbp_i64 id, column field) const;

//         // QAbstractItemModel interface
//     public:
//         virtual int rowCount(const QModelIndex &parent) const override;
//         virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
//         virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
//         static UserType::FilterFlags filterValue(const games_stat_t::first_type::value_type &item);
// //        bool filterAcceptsItem(const games_stat_t::first_type::value_type &item, UserType::FilterFlags filter);
//         QVariant getData(const games_stat_t::first_type::value_type &item, int column, int role) const;
//     protected:
//         virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
//     };


//     class Tournaments : public QSortFilterProxyModel
//     {
//         Q_OBJECT

//         ns_model::tournament_stat_t* m_sourceModel;

//     public:
//         enum class column {
//             title = 0,
//             bets,
//             game,
//             type,
//             players,
//             waiters,
//             pot,
//             flop,
//             distribution,

//             count
//         };
//     public:
//         Tournaments(QObject* parent = nullptr)
//             : QSortFilterProxyModel(parent)
//         {}
// //        virtual int columnCount(const QModelIndex &parent) const override;
// //        virtual QVariant data(const QModelIndex &index, int role) const override;

// //        virtual void setSourceModel(QAbstractItemModel *sourceModel) override;
// //        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
// //        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

// //        virtual QModelIndex indexOf(gbp_i64 id) const;
// //        QModelIndex indexOf(gbp_i64 id, column field) const;
// //    protected:
// //        virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
//     };
    
//    class GamesStatistic : public QAbstractProxyModel
//    {
//        Q_OBJECT
//        Q_PROPERTY(EPage page READ page CONSTANT)
//        bool m_isNowResetting;
//    public:
//        class ECashColumn {
//            enum {
//                title = 0,
//                bets,
//                game,
//                type,
//                players,
//                waiters,
//                pot,
//                flop,
//                distribution
//            };
//        };
//        class ETournColumn {
//            enum {
//                start = 0,
//                game,
//                buyin,
//                type,
//                title,
//                prize,
//                status,
//                players
//            };
//        };

//        enum EPage {Unknown = -1, CashTables, Tournaments};
//        Q_ENUM(EPage)
//    protected:
//        QScopedPointer<ns_model::cash_stat_t> m_cashes;
//        QScopedPointer<ns_model::tournament_stat_t> m_tourns;
//        quintptr m_sharedDataId;
//        games_stat_t m_previuosData;
//        games_stat_t* m_originData;

//        const EPage m_page;

//        std::pair<std::set<gbp_i64>, std::set<gbp_i64>> m_ids;
//    public:
//        GamesStatistic(EPage p, QObject* parent = nullptr);
//        virtual ~GamesStatistic();

//        gbp::ns_api::ns_statistic::e_tourn_button_type buttonStateAtRow(int row) const;

//        virtual QVariant getData(const games_stat_t::first_type::value_type& item, int column, int role = Qt::DisplayRole) const;
//        virtual QVariant getData(const games_stat_t::second_type::value_type& item, int column, int role = Qt::DisplayRole) const;

//        static bool filterAcceptsItem(const games_stat_t::second_type::value_type& item, UserType::FilterFlags filter);
//        static UserType::FilterFlags filterValue(const games_stat_t::first_type::value_type& item);
//        static UserType::FilterFlags filterValue(const games_stat_t::second_type::value_type& item);
//    public:
//        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
//        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//        virtual int rowCount(int page = Unknown) const;
//        virtual int columnCount(int page = Unknown) const;

//        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

//        inline bool isReady() const;
//        inline quintptr sharedDataId() const;

//        QModelIndex indexOf(gbp_i64 id, int page = Unknown) const;
//        gbp_i64 itemId(const QModelIndex& idx) const;

//        EPage page() const;
//    protected slots:
//        virtual void onDataUpdated(const games_stat_t &statistic);
//        virtual void resetData(const games_stat_t::first_type& data);
//        virtual void resetData(const games_stat_t::second_type& data);
//        virtual void updateData(const games_stat_t::first_type& data);
//        virtual void updateData(const games_stat_t::second_type& data);
//    public:
//        virtual void addItem(gbp_i64 id, int page = Unknown);
//        virtual void removeItem(gbp_i64 id, int page = Unknown);
//        virtual void updateItem(gbp_i64 id, int page = Unknown);
//    public:
//        QVariant rowItem(const QModelIndex& index) const;
//        QVariant rowItem(int row, int page = Unknown) const;
//        QVariant previousRowItem(gbp_i64 id, int page = Unknown) const;

//        void doItemChanges(gbp_i64 id, int page = Unknown);


//        // QAbstractItemModel interface
//    public:
//        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
//        virtual QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const override;
//        QString headerText(int section, int page = Unknown) const;

//        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

//        // QAbstractItemModel interface
//    public:
//        virtual QModelIndexList match(const QModelIndex &start
//                                    , int role
//                                    , const QVariant &value
//                                    , int hits               = 1
//                                    , Qt::MatchFlags flags   = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const override;

//        virtual QModelIndex parent(const QModelIndex &child) const override;
//        virtual bool hasChildren(const QModelIndex &parent) const override;

//        // QAbstractProxyModel interface
//    public:
//        virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
//        virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
//    };


//} // namespace model


//inline bool ns_model::GamesStatistic::isReady() const {
//    return (m_sharedDataId != 0) && (m_originData != nullptr);
//}

//inline quintptr ns_model::GamesStatistic::sharedDataId() const  {
//    return m_sharedDataId;
//}
