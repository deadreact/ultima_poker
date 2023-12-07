#include <precompiled.h>
#include "NotificationHandler.h"
#include <PokerApplication.h>
#include <dialogs/dialog_okmessage.h>
#include <core/money.h>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/table/request.h>
    #include <rpc/table/reply.h>
#endif // !Q_MOC_RUN

namespace ns_private
{
    void onRegisterReplied(ns_table::e_status status)
    {
        PokerApplication& app = *pApp;
        ns_dialog::OkMessage::setButtonTheme("green");
        ns_dialog::OkMessage *okMessage = new ns_dialog::OkMessage(app.activeWindow());

        okMessage->init();
        okMessage->resize(378, 153);
        QString title = app.translate("Description", "@tournament_registration");
        QString text = "";
        switch (status) {
        case ns_table::e_status::ok:
        {
            title = app.translate("Description", "Title_TournamentBuyIn_statusOK");
            text  = app.translate("Description", "@Text_TournamentBuyIn_statusOK!");
        } break;
        case ns_table::e_status::already_registered:     text = (app.translate("Description", "@already_registered"));     break;
        case ns_table::e_status::registered_in_main:     text = (app.translate("Description", "@registered_in_main"));     break;
        case ns_table::e_status::registered_in_another:  text = (app.translate("Description", "@registered_in_another"));  break;
        case ns_table::e_status::unknown_error:          text = (app.translate("Description", "@unknown_error"));          break;
        case ns_table::e_status::bad_tournament_id:      text = (app.translate("Description", "@bad_tournament_id"));      break;
        case ns_table::e_status::register_not_available: text = (app.translate("Description", "@register_not_available")); break;
        case ns_table::e_status::too_many_players:       text = (app.translate("Description", "@too_many_players"));       break;
        case ns_table::e_status::not_enough_raiting:     text = (app.translate("Description", "@not_enough_raiting"));     break;
        case ns_table::e_status::not_enough_money:       text = (app.translate("Description", "@not_enough_money"));       break;
        case ns_table::e_status::only_for_invitees:      text = (app.translate("Description", "@only_for_invitees"));      break;
        default:
            break;
        }

        ns_dialog::OkMessage::execMessage(text, title, app.activeWindow());
    }

    void onUnregisterReplied(ns_table::e_status status, gbp_i64 /*id*/, const gbp::ns_api::ns_tourn::cancel_tourn_register_t& info)
    {
        PokerApplication& app = *pApp;

        ns_dialog::OkMessage::setButtonTheme("green");
        ns_dialog::OkMessage *okMessage = new ns_dialog::OkMessage(app.activeWindow());

        okMessage->init();
        okMessage->resize(378, 153);
        okMessage->setTitle(app.translate("Description", "@tournament_registration_canceling"));
        QString returnedFund = (info._money != 0) ? Money(info._money).toString() : QString("ticket \"%0\"").arg(info._ticket_title.c_str());
        switch (status) {
        case ns_table::e_status::ok:                      okMessage->setText(app.translate("Description", "@ok_%0").arg(returnedFund));                      break;
        case ns_table::e_status::bad_tournament_id:       okMessage->setText(app.translate("Description", "@wrong_tourn_id_%0").arg(returnedFund));          break;
        case ns_table::e_status::user_not_registered:     okMessage->setText(app.translate("Description", "@user_not_registered_%0").arg(returnedFund));     break;
        case ns_table::e_status::registered_by_qualifier: okMessage->setText(app.translate("Description", "@registered_by_qualifier_%0").arg(returnedFund)); break;
        case ns_table::e_status::unknown_error:           okMessage->setText(app.translate("Description", "@unknown_error_%0").arg(returnedFund));           break;
        default:
            break;
        }
        okMessage->setAttribute(Qt::WA_DeleteOnClose);
        okMessage->open();
    }
}

NotificationHandler::NotificationHandler(QObject *parent)
    : QObject(parent)
{
    initialize();
}

NotificationHandler::~NotificationHandler()
{

}

void NotificationHandler::initialize()
{
    const reply_table_tournament::emitter_t* emitter = netClient().emitter<reply_table_tournament>();

    connect(emitter, &ns_table::emitter_t::cancel_tourn_register, &ns_private::onUnregisterReplied);
    connect(emitter, &ns_table::emitter_t::tournament_register, &ns_private::onRegisterReplied);
}
