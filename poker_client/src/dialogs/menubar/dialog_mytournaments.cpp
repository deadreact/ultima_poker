#include <precompiled.h>
#include "dialog_mytournaments.h"
#include "ui_dialog_MyTournaments.h"
#include <models/model_GamesStatistic.h>
#include <models/proxy/model_FunctionFilter.h>
#include <dialogs/dialog_okmessage.h>
#include <core/money.h>
#include <widgets/MainWindow.h>
#include <styles/scaledtextstyle.h>
#include <styles/shadowedtextstyle.h>

#ifndef Q_MOC_RUN
#include <db/modeldatabase.h>
    #include <rpc/rpc_common.h>
    #include <rpc/table/rpc_table.h>
#endif // !Q_MOC_RUN

#include <api_models/model_tournament_stat_t.hpp>

Q_DECLARE_METATYPE(games_stat_t::second_type::value_type)

namespace ns_dialog
{
    MyTournaments::MyTournaments(QWidget *parent)
        : InitableDialog(parent)
        , m_ui(new Ui::MyTournaments)
        , m_proxyModel(nullptr)
    {
        m_ui->setupUi(this);
    }

    MyTournaments::~MyTournaments()
    {
        delete m_ui;
    }

    void MyTournaments::init()
    {
        if (isInitialized()) {
            return;
        }


        int counter = 0;
        for (QObject* obj: m_ui->pages->tabBar()->children()) {
            //qDebug() << "tabbar child, name:" << obj->objectName() << "classname:" << obj->metaObject()->className();
            if (QAbstractButton* bttn = qobject_cast<QAbstractButton*>(obj)) {
                bttn->setObjectName(QString("tabBarButton_%0").arg(counter++));
//                bttn->setStyle(style);
            }
        }
        ScaledTextStyle* scaledStyle = new ScaledTextStyle(12, 16);
        ShadowedTextStyle* style = new ShadowedTextStyle(scaledStyle);
        style->setBlending(E::Blending::Overlay);
        style->setShadowOffset(0, 1);
        style->setShadowColor(QColor(0x191919));
        m_ui->pages->setStyle(style);
        m_ui->bttnGrp_filters->setId(m_ui->bttn_filterStatusRegistration, UserType::EFilters::Status_Registration | UserType::EFilters::Status_LateRegistration);
        m_ui->bttnGrp_filters->setId(m_ui->bttn_filterStatusInProgress, UserType::EFilters::Status_Started | UserType::EFilters::Status_LateRegistration);
        m_ui->bttnGrp_filters->setId(m_ui->bttn_filterAll, UserType::EFilters::Status_Mask);

        ns_model::tournament_stat_t* tournModel = modelsDB().getTournamentStatistic();
//        gamesStatModel->setCurrentPage(1);
        m_proxyModel = new ns_model::FunctionFilter(this);
        m_proxyModel->setSourceModel(tournModel);

//        m_ui->tableView->setRootModel(tournModel);
//        m_ui->tableView->setProxyModel(m_proxyModel);
        m_ui->tableView->setModel(m_proxyModel);
//        m_ui->tableView->init();


        auto rowFilter = [this](const QModelIndex& modelIndex) {
            if (modelIndex.data(UserType::RowDataRole).canConvert<games_stat_t::second_type::value_type>()) {
                auto item = modelIndex.data(UserType::RowDataRole).value<games_stat_t::second_type::value_type>();
                if (item._is_current_user_registered) {
                    switch (item._status) {
                    case gbp::ns_api::ns_tournament_types::e_tournament_status::registering:
                        return m_ui->bttn_filterStatusRegistration->isChecked();
                    case gbp::ns_api::ns_tournament_types::e_tournament_status::started:
                        return m_ui->bttn_filterStatusInProgress->isChecked();
                    case gbp::ns_api::ns_tournament_types::e_tournament_status::late_registration:
                        return m_ui->bttn_filterAll->checkState() != Qt::Unchecked;
                    default:
                        return false;
                    }
                }
            }
            return false;
        };

        m_proxyModel->setHandlerForRow(std::move(rowFilter), 6);

        connect(m_ui->tableView, (void(TableView::*)(const QModelIndex&))&TableView::currentChanged, this, [this](const QModelIndex& idx) {
            auto data = idx.data(UserType::RowDataRole).value<games_stat_t::second_type::value_type>();

            m_ui->stackOfBttn->setHidden(data._button_state == gbp::ns_api::ns_statistic::e_tourn_button_type::hidden);

            switch (data._button_state) {
            case gbp::ns_api::ns_statistic::e_tourn_button_type::register_in_tourn:
            case gbp::ns_api::ns_statistic::e_tourn_button_type::late_register_in_tourn:
                m_ui->stackOfBttn->setCurrentWidget(m_ui->page_bttn_register);
                break;
            case gbp::ns_api::ns_statistic::e_tourn_button_type::unregister:
                m_ui->stackOfBttn->setCurrentWidget(m_ui->page_bttn_unregister);
                break;
            case gbp::ns_api::ns_statistic::e_tourn_button_type::back_to_game:
                m_ui->stackOfBttn->setCurrentWidget(m_ui->page_bttn_backToGame);
                break;
            default:
                break;
            }
        });

        connect(m_proxyModel, &ns_model::FunctionFilter::modelReset, this, &MyTournaments::updateTexts);

        updateTexts();

        onInitialized();
    }

    void MyTournaments::updateTexts()
    {
        int registrationCount = 0;
        int inProgressCount = 0;
        //qDebug() << m_proxyModel->rowCount();
        for (int i = 0; i < m_proxyModel->rowCount(); i++) {
            QModelIndex modelIndex = m_proxyModel->index(i, 0);
            auto item = modelIndex.data(UserType::RowDataRole).value<games_stat_t::second_type::value_type>();

            switch (item._status) {
            case gbp::ns_api::ns_tournament_types::e_tournament_status::registering:        registrationCount++; break;
            case gbp::ns_api::ns_tournament_types::e_tournament_status::late_registration:  registrationCount++; Q_FALLTHROUGH();
            case gbp::ns_api::ns_tournament_types::e_tournament_status::started:            inProgressCount++; break;
            default:
                break;
            }
        }

        m_ui->footer_info->setStrings(QStringList() << QString::number(registrationCount) << QString::number(inProgressCount) << QString::number(m_proxyModel->rowCount()));
    }

    void MyTournaments::changeEvent(QEvent *e)
    {
        if (e && e->type() == QEvent::LanguageChange) {
            m_ui->retranslateUi(this);
            updateTexts();
        }
        InitableDialog::changeEvent(e);
    }


    void MyTournaments::on_bttnGrp_filters_buttonClicked(QAbstractButton */*bttn*/) {
        // только для того чтобы обновился фильтр
        m_proxyModel->setFilterRole(m_proxyModel->filterRole() == 0 ? 1 : 0);
    }

    void MyTournaments::on_bttn_filterAll_clicked() {
        m_ui->bttn_filterAll->setTristate(false);
        m_ui->bttn_filterStatusRegistration->setChecked(m_ui->bttn_filterAll->isChecked());
        m_ui->bttn_filterStatusInProgress->setChecked(m_ui->bttn_filterAll->isChecked());

//        setAdditionalFilterValue(m_ui->bttnGrp_filters->id(m_ui->bttn_filterAll));
    }

    void MyTournaments::on_bttn_filterStatusInProgress_clicked() {
        int checkedCount = (m_ui->bttn_filterStatusRegistration->isChecked() ? 1 : 0) + (m_ui->bttn_filterStatusInProgress->isChecked() ? 1 : 0);
        m_ui->bttn_filterAll->setTristate(checkedCount == 1);
        switch (checkedCount) {
        case 2: m_ui->bttn_filterAll->setCheckState(Qt::Checked);           break;
        case 1: m_ui->bttn_filterAll->setCheckState(Qt::PartiallyChecked);  break;
        case 0: m_ui->bttn_filterAll->setCheckState(Qt::Unchecked);         break;
        default:
            break;
        }

    }

    void MyTournaments::on_bttn_filterStatusRegistration_clicked() {
        int checkedCount = (m_ui->bttn_filterStatusRegistration->isChecked() ? 1 : 0) + (m_ui->bttn_filterStatusInProgress->isChecked() ? 1 : 0);
        m_ui->bttn_filterAll->setTristate(checkedCount == 1);
        switch (checkedCount) {
        case 2: m_ui->bttn_filterAll->setCheckState(Qt::Checked);           break;
        case 1: m_ui->bttn_filterAll->setCheckState(Qt::PartiallyChecked);  break;
        case 0: m_ui->bttn_filterAll->setCheckState(Qt::Unchecked);         break;
        default:
            break;
        }
    }

    void MyTournaments::on_bttn_unregister_clicked()
    {
        rpc_table* rpc = rpc_object<rpc_table>();

        rpc->cancel_tourn_register(m_ui->tableView->currentIndex().data(UserType::RowItemIdRole).value<gbp_i64>()/*, &onUnregisterCallback*/);
    }

    void MyTournaments::reject() {
      emit rejected();
    }

} //namespace ns_dialog


