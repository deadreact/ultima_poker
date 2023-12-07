#include <precompiled.h>
#include "page_SettingsHistory.h"
#include "ui_page_SettingsHistory.h"
#include <styles/shadowedtextstyle.h>

namespace
{
    template <typename EnumType>
    bool setButtonChecked(QButtonGroup* group, EnumType enumValue) {
        if (QAbstractButton* bttn = group->button(underlying_cast(enumValue))) {
            bttn->setChecked(true);
            return true;
        }
        return false;
    }
}

namespace ns_page
{
    SettingsHistoryPageData::SettingsHistoryPageData()
        : value_concreteDistribution(0)
        , value_tournamentNumber(0)
        , select_cashTables(CashTables_LastHour)
        , select_tournDistr(TournamentDistributions_TournamentNumber)
        , select_tournResults(TournamentResults_LastTournamentsCount_1)
    {}

    bool SettingsHistoryPageData::operator==(const SettingsHistoryPageData &d) const
    {
        return select_cashTables == d.select_cashTables
            && select_tournDistr == d.select_tournDistr
            && select_tournResults == d.select_tournResults
            && (select_cashTables != CashTables_ConcreteDistribution || value_concreteDistribution == d.value_concreteDistribution)
            && (select_tournDistr != TournamentDistributions_TournamentNumber || value_tournamentNumber == d.value_tournamentNumber);
    }

// -------------------------------------------------------------------------
    SettingsHistory::SettingsHistory(QWidget *parent)
        : SettingsBase(parent)
        , m_ui(new Ui::SettingsHistory)
        , m_data()
    {
        m_ui->setupUi(this);
    }

    SettingsHistory::~SettingsHistory() {}


    void SettingsHistory::init()
    {
        if (isInitialized()) {
            return;
        }
        QFont fnt(m_ui->text_historyDescription->font());
        fnt.setWordSpacing(-0.8);
        m_ui->text_historyDescription->setFont(fnt);

        static const QRegularExpression re("(^(msg_|filter_|title_|text_|bttn_)\\w+$)");
        QWidgetList lst = m_ui->subpage_cashTables->findChildren<QWidget*>(re);
        lst << m_ui->subpage_tournaments->findChildren<QWidget*>(re);
        lst << m_ui->text_historyDescription;

        for (QWidget* w: lst) {
            setTextDropShadowStyle(w, QPoint(0, -1), QColor(0x191919), E::Blending::Normal);
        }
        setTextDropShadowStyle(m_ui->bttn_apply, QPoint(0, 1), QColor(0, 0, 0, 255*0.4), E::Blending::Normal);
        setTextDropShadowStyle(m_ui->bttn_cancel, QPoint(0, 1), QColor(0, 0, 0, 255*0.4), E::Blending::Normal);

        updateTexts();

        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_hour, underlying_cast(SettingsHistoryPageData::CashTables_LastHour));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_day, underlying_cast(SettingsHistoryPageData::CashTables_LastDay));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_3days, underlying_cast(SettingsHistoryPageData::CashTables_Last3Days));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_week, underlying_cast(SettingsHistoryPageData::CashTables_LastWeek));

        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_numberLastDistributions_1, underlying_cast(SettingsHistoryPageData::CashTables_LastDistributionsCount_1));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_numberLastDistributions_2, underlying_cast(SettingsHistoryPageData::CashTables_LastDistributionsCount_2));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_numberLastDistributions_3, underlying_cast(SettingsHistoryPageData::CashTables_LastDistributionsCount_3));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_numberLastDistributions_4, underlying_cast(SettingsHistoryPageData::CashTables_LastDistributionsCount_4));
        m_ui->bttnGrp_cashTablesSelection->setId(m_ui->bttn_concreteDistribution, underlying_cast(SettingsHistoryPageData::CashTables_ConcreteDistribution));

        m_ui->bttnGrp_tournDistributions->setId(m_ui->bttn_tournamentNumber, underlying_cast(SettingsHistoryPageData::TournamentDistributions_TournamentNumber));
        m_ui->bttnGrp_tournDistributions->setId(m_ui->bttn_lastDistributionsCount_1, underlying_cast(SettingsHistoryPageData::TournamentDistributions_LastDistributionsCount_1));
        m_ui->bttnGrp_tournDistributions->setId(m_ui->bttn_lastDistributionsCount_2, underlying_cast(SettingsHistoryPageData::TournamentDistributions_LastDistributionsCount_2));
        m_ui->bttnGrp_tournDistributions->setId(m_ui->bttn_lastDistributionsCount_3, underlying_cast(SettingsHistoryPageData::TournamentDistributions_LastDistributionsCount_3));

        m_ui->bttnGrp_tournResults->setId(m_ui->bttn_lastTournamentsCount_1, underlying_cast(SettingsHistoryPageData::TournamentResults_LastTournamentsCount_1));
        m_ui->bttnGrp_tournResults->setId(m_ui->bttn_lastTournamentsCount_2, underlying_cast(SettingsHistoryPageData::TournamentResults_LastTournamentsCount_2));
        m_ui->bttnGrp_tournResults->setId(m_ui->bttn_lastTournamentsCount_3, underlying_cast(SettingsHistoryPageData::TournamentResults_LastTournamentsCount_3));
        m_ui->bttnGrp_tournResults->setId(m_ui->bttn_lastTournamentsCount_4, underlying_cast(SettingsHistoryPageData::TournamentResults_LastTournamentsCount_4));

        connect(m_ui->bttnGrp_cashTablesSelection, SIGNAL(buttonToggled(int, bool)), this, SLOT(updateIsModified()));
        connect(m_ui->bttnGrp_tournDistributions, SIGNAL(buttonToggled(int, bool)), this, SLOT(updateIsModified()));
        connect(m_ui->bttnGrp_tournResults, SIGNAL(buttonToggled(int, bool)), this, SLOT(updateIsModified()));

        connect(m_ui->spinBox_concreteDistribution, SIGNAL(valueChanged(int)), this, SLOT(updateIsModified()));
        connect(m_ui->spinBox_tournamentNumber, SIGNAL(valueChanged(int)), this, SLOT(updateIsModified()));

        onInitialized();
    }

    void SettingsHistory::apply() {
        exportData(m_data);
        QFile f("history.txt");
        if (f.open(QIODevice::WriteOnly)) {
            QDataStream os(&f);
            os << m_data;
            f.close();
        }

        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    void SettingsHistory::rollback() {
        importData(m_data);
        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    void SettingsHistory::updateTexts()
    {
        m_ui->retranslateUi(this);
        m_ui->bttn_lastDistributionsCount_1->setText(m_ui->bttn_lastDistributionsCount_1->text().arg(1));
        m_ui->bttn_lastDistributionsCount_2->setText(m_ui->bttn_lastDistributionsCount_2->text().arg(5));
        m_ui->bttn_lastDistributionsCount_3->setText(m_ui->bttn_lastDistributionsCount_3->text().arg(10));

        m_ui->bttn_lastTournamentsCount_1->setText(m_ui->bttn_lastTournamentsCount_1->text().arg(1));
        m_ui->bttn_lastTournamentsCount_2->setText(m_ui->bttn_lastTournamentsCount_2->text().arg(5));
        m_ui->bttn_lastTournamentsCount_3->setText(m_ui->bttn_lastTournamentsCount_3->text().arg(10));
        m_ui->bttn_lastTournamentsCount_4->setText(m_ui->bttn_lastTournamentsCount_4->text().arg(30));

        m_ui->bttn_numberLastDistributions_1->setText("1");
        m_ui->bttn_numberLastDistributions_2->setText("10");
        m_ui->bttn_numberLastDistributions_3->setText("50");
        m_ui->bttn_numberLastDistributions_4->setText("100");
    }

    void SettingsHistory::importData(const SettingsHistoryPageData &d)
    {
        setButtonChecked(m_ui->bttnGrp_cashTablesSelection, d.select_cashTables);
        setButtonChecked(m_ui->bttnGrp_tournDistributions, d.select_tournDistr);
        setButtonChecked(m_ui->bttnGrp_tournResults, d.select_tournResults);

        m_ui->spinBox_concreteDistribution->setValue(d.value_concreteDistribution);
        m_ui->spinBox_tournamentNumber->setValue(d.value_tournamentNumber);
    }

    void SettingsHistory::exportData(SettingsHistoryPageData &d)
    {
        d.value_concreteDistribution = m_ui->spinBox_concreteDistribution->value();
        d.value_tournamentNumber = m_ui->spinBox_tournamentNumber->value();

        d.select_cashTables   = SettingsHistoryPageData::CashTables(m_ui->bttnGrp_cashTablesSelection->checkedId());
        d.select_tournDistr   = SettingsHistoryPageData::TournamentDistributions(m_ui->bttnGrp_tournDistributions->checkedId());
        d.select_tournResults = SettingsHistoryPageData::TournamentResults(m_ui->bttnGrp_tournResults->checkedId());
    }

    void SettingsHistory::load()
    {
        QFile f("history.txt");

        if (f.exists())
        {
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream is(&f);
                is >> m_data;
                f.close();
            }
        }
        else
        {
            if (f.open(QIODevice::WriteOnly)) {
                QDataStream os(&f);
                os << m_data;
                f.close();
            }
        }
        importData(m_data);

        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
//        QFile f("history.txt");

//        if (f.open(QIODevice::ReadOnly)) {
//            QDataStream is(&f);
//            is >> m_data;
//            importData(m_data);
//            f.close();
//        }

//        m_modified = false;
//        m_ui->footer->setEnabled(m_modified);
//        setWindowModified(m_modified);
    }

    void SettingsHistory::updateIsModified()
    {
        if (!isInitialized()) {
            return;
        }

        SettingsHistoryPageData d;
        exportData(d);

        m_modified = m_data != d;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

} //namespace ns_page



QDataStream &operator<<(QDataStream &os, const ns_page::SettingsHistoryPageData &data)
{
    os << data.value_concreteDistribution << data.value_tournamentNumber << data.select_cashTables << data.select_tournDistr << data.select_tournResults;
    return os;
}

QDataStream &operator>>(QDataStream &is, ns_page::SettingsHistoryPageData &data)
{
    typename std::underlying_type<ns_page::SettingsHistoryPageData::CashTables>::type              select_cashTables;
    typename std::underlying_type<ns_page::SettingsHistoryPageData::TournamentDistributions>::type select_tournDistr;
    typename std::underlying_type<ns_page::SettingsHistoryPageData::TournamentResults>::type       select_tournResults;
    is >> data.value_concreteDistribution >> data.value_tournamentNumber
       >> select_cashTables >> select_tournDistr >> select_tournResults;

    data.select_cashTables   = ns_page::SettingsHistoryPageData::CashTables(select_cashTables);
    data.select_tournDistr   = ns_page::SettingsHistoryPageData::TournamentDistributions(select_tournDistr);
    data.select_tournResults = ns_page::SettingsHistoryPageData::TournamentResults(select_tournResults);
    return is;
}
