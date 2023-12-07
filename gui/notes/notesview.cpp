#include "notesview.h"
#include "model_io.h"
#include "model_markers.h"
#include "model_notes.h"
#include "markerslist.h"
#include "noteslist.h"
#include "noteslisteditor.h"
#include "markerslisteditor.h"


NotesViewInterface::NotesViewInterface()
    : m_markersModel(nullptr)
    , m_notesModel(nullptr)
    , m_initialized(false)
{

}

ns_model::Markers *NotesViewInterface::markersModel() const {
    return m_markersModel.data();
}
ns_model::Notes *NotesViewInterface::notesModel() const {
    return m_notesModel.data();
}

void NotesViewInterface::resetMarkersModel(ns_model::Markers *m) {
    m_markersModel.reset(m);
    if (m != nullptr) {
        m->read();
    }
    if (!m_notesModel.isNull()) {
        notesModel()->setSourceModel(m);
    }
    if (auto markers = markersList()) {
        markers->resetMarkersModel(m);
    }
    if (auto editor = notesListEditor()) {
        editor->setMarkersModel(m);
    }
}
void NotesViewInterface::resetNotesModel(ns_model::Notes *m) {
    m_notesModel.reset(m);
    if (m != nullptr) {
        m->read();
    }
    if (auto notes = notesList()) {
        notes->resetNotesModel(m);
    }
    if (auto editor = notesListEditor()) {
        editor->setNotesModel(m);
    }
}

void NotesViewInterface::init()
{
    if (isInitialized()) {
        return;
    }
    if (m_markersModel.isNull()) {
        m_markersModel.reset(new ns_model::Markers);
        m_markersModel->read();
    }
    if (m_notesModel.isNull()) {
        m_notesModel.reset(new ns_model::Notes(m_markersModel.data()));
        m_notesModel->read();
    }

    if (auto markers = markersList()) {
        markers->resetMarkersModel(markersModel());
        markers->init();
    }
    if (auto notes = notesList()) {
        notes->resetNotesModel(notesModel());
        notes->init();
    }
    if (auto editor = notesListEditor()) {
        editor->setMarkersModel(markersModel());
        editor->setNotesModel(notesModel());
        editor->init();
    }
}

void NotesViewInterface::resetModels(QSharedPointer<ns_model::Markers> markersModel, QSharedPointer<ns_model::Notes> notesModel)
{
    m_markersModel = markersModel;
    m_notesModel = notesModel;
}

void NotesViewInterface::resetModelsFrom(NotesViewInterface *other)
{
    m_markersModel = other->m_markersModel;
    m_notesModel = other->m_notesModel;
}
