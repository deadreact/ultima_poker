#include <precompiled.h>
#include "page_SettingsNotes.h"

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
#endif // !Q_MOC_RUN

#include <notes/model_markers.h>
#include <notes/model_notes.h>
#include <settingsmanager.h>
#include <PokerApplication.h>

namespace ns_page
{

    SettingsNotes::SettingsNotes(QWidget *parent)
        : NotesPageView(parent)
    {}

    SettingsNotes::~SettingsNotes() {}

    void SettingsNotes::init()
    {
        pApp->settingsManager()->init<NotesPageView>(this);
        NotesPageView::init();

        connect(&netClient(), &NetworkClient::loggedIn, this, [this](const gbp::ns_api::ns_lobby::user_info_t& /*info*/){
            markersModel()->read();
            notesModel()->read();
        });
        connect(&netClient(), &NetworkClient::loggedOut, this, [this]{
            markersModel()->read();
            notesModel()->read();
        });
    }
} //namespace ns_page


