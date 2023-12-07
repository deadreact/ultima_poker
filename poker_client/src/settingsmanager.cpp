#include <precompiled.h>
#include "settingsmanager.h"
#include <notes/model_markers.h>
#include <notes/model_notes.h>
#include <notes/notescompactview.h>
#include <notes/notespageview.h>


struct SettingsMgr::PrivateData
{
    QSharedPointer<ns_model::Markers> m_modelMarkers;
    QSharedPointer<ns_model::Notes> m_modelNotes;

    PrivateData()
        : m_modelMarkers(new ns_model::Markers)
        , m_modelNotes(new ns_model::Notes(m_modelMarkers.data()))
    {}
};

SettingsMgr::SettingsMgr(QObject *parent)
    : QObject(parent)
    , m_dPtr(new PrivateData)
{}
SettingsMgr::~SettingsMgr()
{
    delete m_dPtr;
}

template <>
bool SettingsMgr::init(NotesCompactView* obj) const {
    obj->setAttribute(Qt::WA_DeleteOnClose);
    obj->resetModels(m_dPtr->m_modelMarkers, m_dPtr->m_modelNotes);
    return true;
}
template <>
bool SettingsMgr::init(NotesPageView* obj) const {
    obj->resetModels(m_dPtr->m_modelMarkers, m_dPtr->m_modelNotes);
    return true;
}
