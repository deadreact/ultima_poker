#pragma once

#include <tools/sequentalactionqueue.h>
#include <tools/types_fwd.hpp>

namespace ns_dialog
{
    class CashBuyIn;
} // namespace ns_dialog

namespace ns_action
{
    class  LoginRequired : public Action
    {
        Q_OBJECT
    private:
        QWidget* m_authDialogParent;
    public:
        LoginRequired(QWidget* dialogParent = nullptr, QObject* parent = nullptr);
        virtual void execute() override;
    };

    class TableBase : public Action
    {
        Q_OBJECT
    protected:
        GameId m_gameID;
    public:
        TableBase(const GameId& id, QObject* parent = nullptr);
        TableBase(const TableBase& other);
        TableBase& operator=(const TableBase& other);

        virtual void execute() override = 0;

        bool isValid() const;
    };

    class InitTableWindow : public TableBase
    {
        Q_OBJECT
    private:
        QWidget* m_tableParent;
    public:
        InitTableWindow(const GameId& id, QWidget* tableParent = nullptr, QObject* parent = nullptr);
        virtual void execute() override;
    };

    class InitPlayerControls : public TableBase
    {
        Q_OBJECT
    public:
        InitPlayerControls(const GameId& id, QObject* parent = nullptr);
        virtual void execute() override;
    };

    class InitViewerControls : public TableBase
    {
        Q_OBJECT
    public:
        InitViewerControls(const GameId& id, QObject* parent = nullptr);
        virtual void execute() override;
    };

    class OpenGameTable : public TableBase
    {
        Q_OBJECT
    public:
        OpenGameTable(const GameId& id, QObject* parent = nullptr);
        virtual void execute() override;
    };

    //! calls rpc_table->sit_on_table()/rpc_table->from_viewer_to_player()
    class FetchSitOnTableInfo : public TableBase
    {
        Q_OBJECT
        gbp_i8 m_pos;
        gbp_i64 m_ratTime;
        QSharedPointer<ns_table::recharge_money_t> m_info;
        bool m_isViewer;
        bool m_result;
    public:
        FetchSitOnTableInfo(const GameId& id, gbp_i8 pos = -1, QObject* parent = nullptr);
        virtual void execute() override;
        inline void setPosition(gbp_i8 pos) { m_pos = pos; }
        inline void setViewer(bool b) { m_isViewer = b; }
        QSharedPointer<ns_table::recharge_money_t> info() const;
        inline gbp_i64 ratTime() const { return m_ratTime; }
    };

    class CashBuyIn : public TableBase
    {
        Q_OBJECT
    private:
        QWidget* m_dialogParent;
        gbp_i64 m_ratTime;
        QSharedPointer<ns_table::recharge_money_t> m_info;
        ns_dialog::CashBuyIn* m_dialog;
        bool m_isAlreadySitOnTable;
        gbp_i64 m_sitMoney;
    public:
        CashBuyIn(gbp_i64 cashID, QWidget* dialogParent = nullptr, QObject* parent = nullptr);
        virtual void execute() override;
        inline void setInfo(QSharedPointer<ns_table::recharge_money_t> info) { m_info = info; }
        inline void setAlreadySitOnTable(bool b) { m_isAlreadySitOnTable = b; }
        inline void setRatTime(gbp_i64 t) { m_ratTime = t; }
        inline gbp_i64 sitMoney() const { return m_sitMoney; }
    };


    class AddViewer : public TableBase
    {
        Q_OBJECT
    public:
        AddViewer(const GameId& id, QObject* parent = nullptr);
        virtual void execute() override;
    };
    class AddWaiter : public TableBase
    {
        Q_OBJECT
        int m_posInQueue;
    public:
        AddWaiter(gbp_i64 cashID, QObject* parent = nullptr);
        virtual void execute() override;
    };
    class RemoveWaiter : public TableBase
    {
        Q_OBJECT
        int m_waitersCount;
    public:
        RemoveWaiter(gbp_i64 cashID, QObject* parent = nullptr);
        virtual void execute() override;
    };

    class UserClosingTable : public TableBase
    {
        Q_OBJECT
    public:
        UserClosingTable(const GameId& id, QObject* parent = nullptr);
        virtual void execute() override;
    };

} //namespace ns_action

