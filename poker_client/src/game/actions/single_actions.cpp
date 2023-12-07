#include <precompiled.h>
#include "single_actions.h"
#include <PokerApplication.h>
#include <widgets/MainWindow.h>

#include <db/modeldatabase.h>
#include <api_models/model_user_info_t.hpp>
#include "common.h"

#include <rpc/table/rpc_table.h>
#include <rpc/rpc_common.h>
#include <tools/item_reference.hpp>
#include <qmessagebox.h>
#include <models/model_user_additional_info.h>
#include <dialogs/buyin/dialog_CashBuyIn.h>

#include <helpers/helper_statistic.h>
#include <helpers/helper_enum.h>

#include <api_models/model_cash_stat_t.hpp>

namespace
{
    rpc_table* getRpc() {
        return rpc_object<rpc_table>();
    }
} // namespace

namespace ns_action
{

    LoginRequired::LoginRequired(QWidget *dialogParent, QObject *parent)
        : Action(parent)
        , m_authDialogParent(dialogParent)
    {}
    TableBase::TableBase(const GameId &id, QObject *parent)
        : Action(parent)
        , m_gameID(id)
    {}
    TableBase::TableBase(const TableBase &other)
        : Action(other.parent())
        , m_gameID(other.m_gameID)
    {}
    InitTableWindow::InitTableWindow(const GameId &id, QWidget *tableParent, QObject *parent)
        : TableBase(id, parent)
        , m_tableParent(tableParent)
    {}
    InitPlayerControls::InitPlayerControls(const GameId &id, QObject *parent)
        : TableBase(id, parent)
    {}
    InitViewerControls::InitViewerControls(const GameId &id, QObject *parent)
        : TableBase(id, parent)
    {}
    OpenGameTable::OpenGameTable(const GameId &id, QObject *parent)
        : TableBase(id, parent)
    {}
    CashBuyIn::CashBuyIn(gbp_i64 cashID, QWidget *dialogParent, QObject *parent)
        : TableBase(cashID, parent)
        , m_dialogParent(dialogParent)
        , m_ratTime(-1)
        , m_info(nullptr)
        , m_dialog(nullptr)
        , m_isAlreadySitOnTable(false)
        , m_sitMoney(-1)
    {}
    RemoveWaiter::RemoveWaiter(gbp_i64 cashID, QObject *parent)
        : TableBase(cashID, parent)
        , m_waitersCount(-1)
    {}
    AddWaiter::AddWaiter(gbp_i64 cashID, QObject *parent)
        : TableBase(cashID, parent)
        , m_posInQueue(-1)
    {}
    AddViewer::AddViewer(const GameId &id, QObject *parent)
        : TableBase(id, parent)
    {}
    FetchSitOnTableInfo::FetchSitOnTableInfo(const GameId &id, gbp_i8 pos, QObject *parent)
        : TableBase(id, parent)
        , m_pos(pos)
        , m_ratTime(-1)
        , m_info(nullptr)
        , m_isViewer(false)
        , m_result(false)
    {}
    QSharedPointer<ns_table::recharge_money_t> FetchSitOnTableInfo::info() const {
        Q_ASSERT(m_result);
        return m_info;
    }




    TableBase &TableBase::operator=(const TableBase &other) {
        setParent(other.parent());
        m_gameID = other.m_gameID;
        return *this;
    }
    bool TableBase::isValid() const {
        return m_gameID.isValid();
    }

    void LoginRequired::execute()
    {
        if (!enshureUserLoggedIn(m_authDialogParent)) {
            emit done(false);
            return;
        }
        emit done(true);
    }

    void InitTableWindow::execute() {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }

        GameManager* mgr = pApp->gameManager();
        if (!mgr->isTableCreated(m_gameID)) {
            mgr->initGame(m_gameID);
        }
        GameTable* table = mgr->getGameTable(m_gameID);
        if (table == nullptr) {
            emit done(false);
            return;
        }
        if (!table->isInitialized()) {
            connect(table, &GameTable::initialized, this, [&]{
                emit done(true);
            });
            table->init();
        } else {
            emit done(true);
        }
    }

    void InitPlayerControls::execute()
    {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }
        // FIXME: заинитить контролы игрока
        emit done(true);
    }

    void InitViewerControls::execute()
    {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }
        // FIXME: заинитить контролы вьювера
        emit done(true);
    }

    void OpenGameTable::execute()
    {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }
        if (GameTable* table = pApp->gameManager()->getGameTable(m_gameID)) {
            table->open();
            emit done(true);
        } else {
            emit done(false);
        }
    }

    void CashBuyIn::execute() {
        if (!m_gameID.isValid() || m_info.isNull()) {
            emit done(false);
            return;
        }
        gbp_i64 id = m_gameID.m_tableID;
        m_dialog = new ns_dialog::CashBuyIn(id, (m_dialogParent ? m_dialogParent : pApp->gameManager()->getGameTable(id)), Qt::Dialog);
    //    m_dialog->setAttribute(Qt::WA_DeleteOnClose);

        ns_descriptor::CashBuyIn desc(ns_helpers::tableName(id)
                                    , ns_helpers::tableTitle(id)
                                    , m_info->_money_from
                                    , m_info->_money_to
                                    , m_info->_account_money
                                    , m_ratTime
                                    , m_isAlreadySitOnTable);
        m_dialog->initWithDescriptor(&desc);
        if (m_dialog->exec())
        {
            m_sitMoney = m_dialog->money();
            emit done(true);
            return;
        }
        getRpc()->sit_on_table_canceled(id);
        emit done(false);
    }

    void RemoveWaiter::execute()
    {
        ItemRefCash itemRef(modelsDB().getCashStatistic()->item(m_gameID.m_tableID).ref());
        if (!m_gameID.isValid() || !m_gameID.isCash() || itemRef._wait_players < 1) {
            emit done(false);
            return;
        }
        getRpc()->remove_from_waiters(m_gameID.m_tableID, [this](ns_table::e_status status, gbp_u16 count){
            m_waitersCount = count;
            if (ns_helpers::isOk(status))
            {
                ns_model::UserInfo* usrInfoModel = modelsDB().getUserAdditionalInfo();
                if (!usrInfoModel->categories().contains("gameinfo")) {
                    QStandardItem* waiting_info = usrInfoModel->category("gameinfo")->field(ns_util::enumToString(ns_model::UserInfo::field::waiter_pos));

                    QMap<gbp_i64, gbp_u16> wInfo = qvariant_cast<QMap<gbp_i64, gbp_u16>>(waiting_info->data());

                    wInfo.remove(m_gameID.m_tableID);
                    waiting_info->setData(QVariant::fromValue(wInfo));
                }
                ns_model::cash_stat_t* statistic_model = modelsDB().getCashStatistic();
                QModelIndex statistic_index = statistic_model->indexOf(m_gameID.m_tableID, ns_model::cash_stat_t::column::_wait_players);
                statistic_model->setData(statistic_index, QVariant::fromValue(count), Qt::EditRole);
            }
            emit done(ns_helpers::isOk(status));
        });
    }

    void AddWaiter::execute()
    {
        if (!m_gameID.isValid() || !m_gameID.isCash() || ns_helpers::freeSeatsCount(m_gameID) != 0) {
            emit done(false);
            return;
        }
        getRpc()->add_waiter(m_gameID.m_tableID, [this](ns_table::e_status status, gbp_u16 posInQueue) {
            if (ns_helpers::isOk(status))
            {
                m_posInQueue = posInQueue;
                ns_model::UserInfo* usrInfoModel = modelsDB().getUserAdditionalInfo();
                if (!usrInfoModel->categories().contains("gameinfo")) {
                    QStandardItem* waiting_info = usrInfoModel->category("gameinfo")->field(ns_util::enumToString(ns_model::UserInfo::field::waiter_pos));
                    QVariant d = waiting_info->data();
                    QMap<gbp_i64, gbp_u16> wInfo;
                    if (!d.isValid()) {
                        waiting_info->setData(QVariant::fromValue(wInfo));
                    } else {
                        wInfo = qvariant_cast<QMap<gbp_i64, gbp_u16>>(d);
                    }
                    wInfo[m_gameID.m_tableID] = posInQueue;
                    waiting_info->setData(QVariant::fromValue(wInfo));
                }
                ns_model::cash_stat_t* statistic_model = modelsDB().getCashStatistic();
                QModelIndex statistic_index = statistic_model->indexOf(m_gameID.m_tableID, ns_model::cash_stat_t::column::_wait_players);
                statistic_model->setData(statistic_index, QVariant::fromValue(posInQueue), Qt::EditRole);
            }
            emit done(ns_helpers::isOk(status));
        });
    }

    void AddViewer::execute()
    {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }
        getRpc()->add_viewer(m_gameID, [this](ns_table::e_status status) {
            switch (status)
            {
            case ns_table::e_status::ok:
            case ns_table::e_status::exists:
            case ns_table::e_status::exists_as_player:
            case ns_table::e_status::exists_as_tournament_player:
                emit done(true);
                break;
            default:
                emit done(false);
                break;
            }
        });
    }

    void FetchSitOnTableInfo::execute()
    {
        if (!m_gameID.isValid()) {
            emit done(false);
            return;
        }
        auto callback = [this](ns_table::e_status status, const ns_table::sit_on_table_info_t& info) {
            switch (status) {
            case ns_table::e_status::rat_mode_enabled:
                m_ratTime = info._rat_time;
                Q_FALLTHROUGH();
            case ns_table::e_status::not_enough_money:
            case ns_table::e_status::ok:
            {
                m_result = true;
                m_info.reset(new ns_table::recharge_money_t(info._money));
            } break;
            default:
                m_result = false;
                break;
            }
            emit done(m_result);
        };
        if (m_isViewer) {
            getRpc()->from_viewer_to_player(m_gameID.m_tableID, m_pos, callback);
        } else {
            getRpc()->sit_on_table(m_gameID.m_tableID, m_pos, callback);
        }
    }

    UserClosingTable::UserClosingTable(const GameId &id, QObject *parent)
        : TableBase(id, parent)
    {}

    void UserClosingTable::execute()
    {
        if (false) {
            emit done(true);
            return;
        }
        QWidget* window = pApp->gameManager()->getGameTable(m_gameID);

        int clicked_bttn = QMessageBox::question(window
                                               , tr("@title_question_close_or_hide")
                                               , tr("@text_question_close_or_hide?")
                                               , QMessageBox::Close
                                               , QMessageBox::Ignore
                                               , QMessageBox::Cancel);
        switch (clicked_bttn) {
        case QMessageBox::Close:
        {
            getRpc()->close_table(m_gameID.m_tableID, [&](ns_table::e_status status)
            {
                if (ns_helpers::isOk(status)) {
                    pApp->gameManager()->removeGame(m_gameID);
                    emit done(true);
                } else {
                    emit done(false);
                }
            });
        } break;
        case QMessageBox::Ignore:
        {
            window->showMinimized();
            emit done(true);
        } break;
        case QMessageBox::Cancel:
        {
            emit done(false);
        } break;
        default:
            Q_UNREACHABLE();
        }
    }

} //namespace ns_action
