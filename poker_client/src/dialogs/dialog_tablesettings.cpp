#include <precompiled.h>
#include "dialog_tablesettings.h"
#include "ui_dialog_TableSettings.h"
#include <QStyledItemDelegate>
#include <qfiledialog.h>
#include <qlistview.h>
#include <qtableview.h>
#include <QDebug>
#include <models/model_TableSettings.h>
#include <delegates/delegate_GameTableComboBoxItem.h>
#include <dialogs/dialog_gametable.h>
#include <widgets/StatisticTableView.h>
#include <db/GameTableSettingsData.h>
#include <db/SettingsDatabase.h>
#include <db/SharedDataService.h>
#include <PokerApplication.h>
#include <qevent.h>
#include <api_models/api_util.hpp>

//TableView::TableView(QWidget *parent)
//    : QTableView(parent)
//{}

//void TableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
//{
//    QTableView::currentChanged(current, previous);
//    emit currentIndexChanged(current, previous);
//}

namespace ns_dialog
{
    TableSettings::TableSettings(QWidget *parent)
        : InitableDialog(parent)
        , m_ui(new Ui::TableSettings)
        , m_sharedDataId(pApp->db().sharedDataService().singleIdForType<GameTableSettingsData>())
        , m_actionOnSettingsApplied(nullptr)
    {
        m_ui->setupUi(this);
    }

    TableSettings::~TableSettings()
    {
        delete m_ui;
    }



    void TableSettings::init()
    {
        if (isInitialized()) {
            return;
        }

        m_ui->bttnGrp_flags->setId(m_ui->bttn_showDealerMsgs, underlying_cast(GameTableSettingsData::ShowDealerMsg));
        m_ui->bttnGrp_flags->setId(m_ui->bttn_showPlayersMsgs, underlying_cast(GameTableSettingsData::ShowPlayersMsg));
        m_ui->bttnGrp_flags->setId(m_ui->bttn_displayEmoticone, underlying_cast(GameTableSettingsData::DisplayEmoticone));
        m_ui->bttnGrp_flags->setId(m_ui->bttn_dropLooserHand, underlying_cast(GameTableSettingsData::DropLooserHand));
        m_ui->bttnGrp_flags->setId(m_ui->bttn_dontShowWinningHand, underlying_cast(GameTableSettingsData::DontShowWinningHand));
        m_ui->bttnGrp_flags->setId(m_ui->bttn_dontShowLastFold, underlying_cast(GameTableSettingsData::DontShowLastFold));

        auto *m = new ns_model::TableSettingFeltColor;
        //qDebug() << "m->rowCount()" << m->rowCount();
        m_ui->comboBox_feltColor->setModel(m);
        m_ui->comboBox_background->setModel(new ns_model::TableSettingBackground);

        onInitialized();
    }
    void TableSettings::accept()
    {
        QVariantMap map = SettingsDatabase::values(SettingsDatabase::names());
        m_actionOnSettingsApplied->setData(map);

        //qDebug() << map;

        m_actionOnSettingsApplied->trigger();

        InitableDialog::accept();
    }

}//namespace ns_dialog


void ns_dialog::TableSettings::actionEvent(QActionEvent *e)
{
    if (e->type() == QEvent::ActionAdded) {
        m_actionOnSettingsApplied = e->action();
        QVariantMap map = SettingsDatabase::values(SettingsDatabase::names());
        m_actionOnSettingsApplied->setData(map);
    }
}
