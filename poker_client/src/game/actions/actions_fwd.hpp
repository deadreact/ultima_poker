#pragma once

namespace ns_action
{
    //! @brief The base class for all actions
    class Action;

    //! @brief The base class for single actions witch depends on GameId
    class TableBase;

    //! @brief The base class for complex actions, consistently executes a chain of actions.
    //!        Each subsequent action is performed if the previous one has been accepted.
    //!        Terminates on first failed action.
    class SequentalActionQueue;

    //! @brief Enshures user logged in. Opens authorization dialog if needed.
    class LoginRequired;

    //! @brief calls rpc_table->add_viewer
    class AddViewer;

    //! @brief calls rpc_table->add_waiter
    class AddWaiter;

    //! @brief calls rpc_table->sit_on_table/rpc_table->from_viewer_to_player
    class FetchSitOnTableInfo;

    //! @brief initializes player's ui controls
    class InitPlayerControls;

    //! @brief creates table window and all needed game handlers
    class InitTableWindow;

    //! @brief performs a buy-in dialog
    class CashBuyIn;

    //! @brief opens game table if exists
    class OpenGameTable;

    //! @brief calls rpc_table->remove_from_waiters
    class RemoveWaiter;    
} //namespace ns_action
