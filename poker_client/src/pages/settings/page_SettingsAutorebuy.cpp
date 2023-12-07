#include <precompiled.h>
#include "page_SettingsAutorebuy.h"
#include "ui_page_SettingsAutorebuy.h"
#include <styles/shadowedtextstyle.h>
#include <api_models/qdatastream_io.hpp>

namespace ns_page
{
    SettingsAutorebuyPageData::SettingsAutorebuyPageData()
    {
        enable_cashTables = true;
        enable_tournamentsAutorebuy = true;
        enable_tournamentsAutoaddon = true;
        select_cashTables = CashTables_AllChipsLost;
        select_tournaments = Tournaments_FirstOpportunity;
        select_autorebuyLimit = AutorebuyLimit_TableMinimum;
        value_cashTables     = 10;
        value_autorebuyLimit = 50;
    }

    bool SettingsAutorebuyPageData::operator==(const SettingsAutorebuyPageData &d) const
    {
        return enable_cashTables == d.enable_cashTables
            && enable_tournamentsAutorebuy == d.enable_tournamentsAutorebuy
            && enable_tournamentsAutoaddon == d.enable_tournamentsAutoaddon
            && select_cashTables == d.select_cashTables
            && select_tournaments == d.select_tournaments
            && select_autorebuyLimit == d.select_autorebuyLimit
            && value_cashTables == d.value_cashTables
            && value_autorebuyLimit == d.value_autorebuyLimit;
    }

// -------------------------------------------------------------------------
    SettingsAutorebuy::SettingsAutorebuy(QWidget *parent)
        : SettingsBase(parent)
        , m_ui(new Ui::SettingsAutorebuy)
    {
        m_ui->setupUi(this);
    }

    SettingsAutorebuy::~SettingsAutorebuy() {}


    void SettingsAutorebuy::init()
    {
        if (isInitialized()) {
            return;
        }

        m_ui->bttnGrp_cashTablesAutorebuy->setId(m_ui->bttn_rebuyWhenAllChipsLost, underlying_cast(SettingsAutorebuyPageData::CashTables_AllChipsLost));
        m_ui->bttnGrp_cashTablesAutorebuy->setId(m_ui->bttn_rebuyWhenLessThan, underlying_cast(SettingsAutorebuyPageData::CashTables_LessThan_xx_bigBlinds));
        m_ui->bttnGrp_cashTablesAutorebuy->setId(m_ui->bttn_rebuyWhenLessThanBuyin, underlying_cast(SettingsAutorebuyPageData::CashTables_LessThan_xx_fromBuyIn));

        m_ui->bttnGrp_tournamentsAutorebuy->setId(m_ui->bttn_firstOpportunity, underlying_cast(SettingsAutorebuyPageData::Tournaments_FirstOpportunity));
        m_ui->bttnGrp_tournamentsAutorebuy->setId(m_ui->bttn_zeroChips, underlying_cast(SettingsAutorebuyPageData::Tournaments_ZeroChips));
        m_ui->bttnGrp_tournamentsAutorebuy->setId(m_ui->bttn_zeroChipsDoubleRebuy, underlying_cast(SettingsAutorebuyPageData::Tournaments_ZeroChipsDoubleRebuy));

        m_ui->bttnGrp_autorebuyLimit->setId(m_ui->bttn_tableMinimum, underlying_cast(SettingsAutorebuyPageData::AutorebuyLimit_TableMinimum));
        m_ui->bttnGrp_autorebuyLimit->setId(m_ui->bttn_tableMaximum, underlying_cast(SettingsAutorebuyPageData::AutorebuyLimit_TableMaximum));
        m_ui->bttnGrp_autorebuyLimit->setId(m_ui->bttn_originalBuyIn, underlying_cast(SettingsAutorebuyPageData::AutorebuyLimit_OriginalBuyIn));
        m_ui->bttnGrp_autorebuyLimit->setId(m_ui->bttn_bigBlinds, underlying_cast(SettingsAutorebuyPageData::AutorebuyLimit_BigBlindsCount));

        static const QRegularExpression re("(^(msg_|filter_|title_|text_|bttn_)\\w+$)");
        QWidgetList lst = m_ui->subpage_cashTables->findChildren<QWidget*>(re);
        lst << m_ui->subpage_tournaments->findChildren<QWidget*>(re);

        for (QWidget* w: lst) {
            setTextDropShadowStyle(w, QPoint(0, -1), QColor(0x191919), E::Blending::Normal);
        }
        setTextDropShadowStyle(m_ui->bttn_apply, QPoint(0, 1), QColor(0, 0, 0, 255*0.4), E::Blending::Normal);
        setTextDropShadowStyle(m_ui->bttn_cancel, QPoint(0, 1), QColor(0, 0, 0, 255*0.4), E::Blending::Normal);

        updateTexts();

        connect(m_ui->bttnGrp_cashTablesAutorebuy, SIGNAL(buttonToggled(int,bool)), this, SLOT(updateIsModified()));
        connect(m_ui->bttnGrp_tournamentsAutorebuy, SIGNAL(buttonToggled(int,bool)), this, SLOT(updateIsModified()));
        connect(m_ui->bttnGrp_autorebuyLimit, SIGNAL(buttonToggled(int,bool)), this, SLOT(updateIsModified()));

        connect(m_ui->bttnGrp_enableSectionSwitchers, SIGNAL(buttonToggled(int,bool)), this, SLOT(updateIsModified()));

        connect(m_ui->comboBox_lessThan_xx_fromBuyIn, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIsModified()));
        connect(m_ui->spinBox_rebuyWhenLessThan_xx_bigBlinds, SIGNAL(valueChanged(int)), this, SLOT(updateIsModified()));
        connect(m_ui->spinBox_autorebuyLimit_xx_bigBlinds, SIGNAL(valueChanged(int)), this, SLOT(updateIsModified()));

        onInitialized();
    }

    void SettingsAutorebuy::apply() {
        exportData(m_data);
        QFile f("autorebuy.txt");
        if (f.open(QIODevice::WriteOnly)) {
            QDataStream os(&f);
            os << m_data;
            f.close();
        }

        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    void SettingsAutorebuy::rollback() {
        importData(m_data);
        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    void SettingsAutorebuy::updateTexts()
    {
        m_ui->retranslateUi(this);
        m_ui->comboBox_lessThan_xx_fromBuyIn->clear();
        m_ui->comboBox_lessThan_xx_fromBuyIn->addItem(tr("comboBox_lessThan_%0%_fromBuyIn").arg(90), 90);
        m_ui->comboBox_lessThan_xx_fromBuyIn->addItem(tr("comboBox_lessThan_%0%_fromBuyIn").arg(50), 50);
        m_ui->comboBox_lessThan_xx_fromBuyIn->addItem(tr("comboBox_lessThan_%0%_fromBuyIn").arg(10), 10);
    }

    void SettingsAutorebuy::importData(const SettingsAutorebuyPageData &d)
    {
        m_ui->bttn_cashTablesAutorebuy->setChecked(d.enable_cashTables);
        m_ui->bttn_tournamentsAutorebuy->setChecked(d.enable_tournamentsAutorebuy);
        m_ui->bttn_tournamentsAutoaddon->setChecked(d.enable_tournamentsAutoaddon);

        m_ui->bttnGrp_cashTablesAutorebuy->button(underlying_cast(d.select_cashTables))->setChecked(true);
        m_ui->bttnGrp_tournamentsAutorebuy->button(underlying_cast(d.select_tournaments))->setChecked(true);
        m_ui->bttnGrp_autorebuyLimit->button(underlying_cast(d.select_autorebuyLimit))->setChecked(true);

        if (d.select_cashTables == SettingsAutorebuyPageData::CashTables_LessThan_xx_fromBuyIn) {
            m_ui->comboBox_lessThan_xx_fromBuyIn->setCurrentText(tr("comboBox_lessThan_%0%_fromBuyIn").arg(d.value_cashTables));
        } else {
            m_ui->spinBox_rebuyWhenLessThan_xx_bigBlinds->setValue(d.value_cashTables);
        }
        m_ui->spinBox_autorebuyLimit_xx_bigBlinds->setValue(d.value_autorebuyLimit);

    }

    void SettingsAutorebuy::exportData(SettingsAutorebuyPageData &d)
    {
        d.enable_cashTables           = m_ui->bttn_cashTablesAutorebuy->isChecked();
        d.enable_tournamentsAutorebuy = m_ui->bttn_tournamentsAutorebuy->isChecked();
        d.enable_tournamentsAutoaddon = m_ui->bttn_tournamentsAutoaddon->isChecked();

        d.select_cashTables     = SettingsAutorebuyPageData::CashTables(m_ui->bttnGrp_cashTablesAutorebuy->checkedId());
        d.select_tournaments    = SettingsAutorebuyPageData::Tournaments(m_ui->bttnGrp_tournamentsAutorebuy->checkedId());
        d.select_autorebuyLimit = SettingsAutorebuyPageData::AutorebuyLimit(m_ui->bttnGrp_autorebuyLimit->checkedId());

        d.value_cashTables = (d.select_cashTables == SettingsAutorebuyPageData::CashTables_LessThan_xx_fromBuyIn)
                           ? m_ui->comboBox_lessThan_xx_fromBuyIn->currentData().toInt()
                           : m_ui->spinBox_rebuyWhenLessThan_xx_bigBlinds->value();

        d.value_autorebuyLimit = m_ui->spinBox_autorebuyLimit_xx_bigBlinds->value();
    }

    void SettingsAutorebuy::load()
    {
        QFile f("autorebuy.txt");

        if (f.open(QIODevice::ReadOnly)) {
            QDataStream is(&f);
            is >> m_data;
            importData(m_data);
            f.close();
        }

        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    void SettingsAutorebuy::updateIsModified()
    {
        if (!isInitialized()) {
            return;
        }

        SettingsAutorebuyPageData d;
        exportData(d);

        m_modified = m_data != d;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

} //namespace ns_page



QDataStream &operator<<(QDataStream &os, const ns_page::SettingsAutorebuyPageData &data)
{
    os << data.enable_cashTables << data.enable_tournamentsAutorebuy << data.enable_tournamentsAutorebuy
       << underlying_cast(data.select_cashTables) << underlying_cast(data.select_tournaments) << underlying_cast(data.select_autorebuyLimit)
       << data.value_cashTables << data.value_autorebuyLimit;
    return os;
}

QDataStream &operator>>(QDataStream &is, ns_page::SettingsAutorebuyPageData &data)
{
    typename std::underlying_type<ns_page::SettingsAutorebuyPageData::CashTables>::type     select_cashTables;
    typename std::underlying_type<ns_page::SettingsAutorebuyPageData::Tournaments>::type    select_tournaments;
    typename std::underlying_type<ns_page::SettingsAutorebuyPageData::AutorebuyLimit>::type select_autorebuyLimit;
    is >> data.enable_cashTables >> data.enable_tournamentsAutorebuy >> data.enable_tournamentsAutorebuy
       >> select_cashTables >> select_tournaments >> select_autorebuyLimit
       >> data.value_cashTables >> data.value_autorebuyLimit;
    data.select_cashTables     = ns_page::SettingsAutorebuyPageData::CashTables(select_cashTables);
    data.select_tournaments    = ns_page::SettingsAutorebuyPageData::Tournaments(select_tournaments);
    data.select_autorebuyLimit = ns_page::SettingsAutorebuyPageData::AutorebuyLimit(select_autorebuyLimit);
    return is;
}
