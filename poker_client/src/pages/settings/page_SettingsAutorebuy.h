#pragma once

#include "page_SettingsBase.h"

namespace Ui {
class SettingsAutorebuy;
}


namespace ns_page
{
    struct SettingsAutorebuyPageData
    {
        enum CashTables : gbp_i8 {
            CashTables_AllChipsLost = 0,
            CashTables_LessThan_xx_bigBlinds,
            CashTables_LessThan_xx_fromBuyIn
        };
        enum Tournaments : gbp_i8 {
            Tournaments_FirstOpportunity,
            Tournaments_ZeroChips,
            Tournaments_ZeroChipsDoubleRebuy
        };
        enum AutorebuyLimit : gbp_i8 {
            AutorebuyLimit_TableMinimum,
            AutorebuyLimit_TableMaximum,
            AutorebuyLimit_OriginalBuyIn,
            AutorebuyLimit_BigBlindsCount
        };
        bool enable_cashTables;
        bool enable_tournamentsAutorebuy;
        bool enable_tournamentsAutoaddon;

        CashTables      select_cashTables;
        Tournaments     select_tournaments;
        AutorebuyLimit  select_autorebuyLimit;

        gbp_i8 value_cashTables;
        gbp_i8 value_autorebuyLimit;

        SettingsAutorebuyPageData();
        bool operator==(const SettingsAutorebuyPageData& d) const;
        inline bool operator!=(const SettingsAutorebuyPageData& d) const { return !operator ==(d); }
    };

    class SettingsAutorebuy : public SettingsBase
    {
        Q_OBJECT

    private:
        Ui::SettingsAutorebuy* m_ui;
        SettingsAutorebuyPageData m_data;
    public:
        explicit SettingsAutorebuy(QWidget *parent = nullptr);
        virtual ~SettingsAutorebuy();

        virtual void init() override;
        virtual void updateTexts() override;

        void importData(const SettingsAutorebuyPageData& d);
        void exportData(SettingsAutorebuyPageData& d);

    protected slots:
        virtual void load() override;
        virtual void apply() override;
        virtual void rollback() override;
        virtual void updateIsModified() override;
    };

} //namespace ns_page

QDataStream& operator<<(QDataStream& os, const ns_page::SettingsAutorebuyPageData&  data);
QDataStream& operator>>(QDataStream& is, ns_page::SettingsAutorebuyPageData&  data);
