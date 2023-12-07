#pragma once

#include "page_SettingsBase.h"

namespace Ui {
class SettingsHistory;
}


namespace ns_page
{
    struct SettingsHistoryPageData
    {
        enum CashTables : gbp_i8 {
            CashTables_LastHour = 0,
            CashTables_LastDay,
            CashTables_Last3Days,
            CashTables_LastWeek,
            CashTables_LastDistributionsCount_1,
            CashTables_LastDistributionsCount_2,
            CashTables_LastDistributionsCount_3,
            CashTables_LastDistributionsCount_4,
            CashTables_ConcreteDistribution
        };
        enum TournamentDistributions : gbp_i8 {
            TournamentDistributions_TournamentNumber = 0,
            TournamentDistributions_LastDistributionsCount_1,
            TournamentDistributions_LastDistributionsCount_2,
            TournamentDistributions_LastDistributionsCount_3
        };
        enum TournamentResults {
            TournamentResults_LastTournamentsCount_1,
            TournamentResults_LastTournamentsCount_2,
            TournamentResults_LastTournamentsCount_3,
            TournamentResults_LastTournamentsCount_4
        };

        long long int           value_concreteDistribution;
        long long int           value_tournamentNumber;
        CashTables              select_cashTables;
        TournamentDistributions select_tournDistr;
        TournamentResults       select_tournResults;

        SettingsHistoryPageData();
        bool operator==(const SettingsHistoryPageData& d) const;
        inline bool operator!=(const SettingsHistoryPageData& d) const { return !operator ==(d); }
    };

    class SettingsHistory : public SettingsBase
    {
        Q_OBJECT

    private:
        Ui::SettingsHistory* m_ui;
        SettingsHistoryPageData m_data;
    public:
        explicit SettingsHistory(QWidget *parent = nullptr);
        virtual ~SettingsHistory();

        virtual void init() override;
        virtual void updateTexts() override;

        void importData(const SettingsHistoryPageData& d);
        void exportData(SettingsHistoryPageData& d);

    protected slots:
        virtual void load() override;
        virtual void apply() override;
        virtual void rollback() override;
        virtual void updateIsModified() override;
    };

} //namespace ns_page

QDataStream& operator<<(QDataStream& os, const ns_page::SettingsHistoryPageData&  data);
QDataStream& operator>>(QDataStream& is, ns_page::SettingsHistoryPageData&  data);
