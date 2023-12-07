#pragma once

#include <notes/notespageview.h>

namespace ns_page
{
    class SettingsNotes : public NotesPageView
    {
        Q_OBJECT
    public:
        explicit SettingsNotes(QWidget *parent = nullptr);
        virtual ~SettingsNotes();

        virtual void init() override;
    };

} //namespace ns_page

