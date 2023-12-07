#include "common.h"
#include <rpc/network_client.h>
#include <dialogs/dialog_authorization.h>

namespace
{
    bool openAuthorizationDialog(QWidget* parent)
    {
        ns_dialog::Authorization dialog(parent);
        dialog.init();
        return (dialog.exec() == QDialog::Accepted);
    }
} //namespace


bool isUserLoggedIn() {
    return netClient().userId() != -1;
}

bool enshureUserLoggedIn(QWidget *parent) {
    return isUserLoggedIn() || openAuthorizationDialog(parent);
}
