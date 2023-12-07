#pragma once

#include "notes_global.h"
#include <QSharedPointer>


namespace ns_model
{
    class Markers;
    class Notes;
}
class MarkersList;
class MarkersListEditor;
class NotesList;
class NotesListEditor;

class NOTES_DLL NotesViewInterface
{
protected:
    QSharedPointer<ns_model::Markers> m_markersModel;
    QSharedPointer<ns_model::Notes> m_notesModel;
    bool m_initialized;

    NotesViewInterface();
public:
    virtual ~NotesViewInterface() {}
    inline bool isInitialized() const { return m_initialized; }

    virtual ns_model::Markers*  markersModel() const;
    virtual ns_model::Notes*    notesModel() const;

	virtual MarkersList* 		markersList() const = 0;
	virtual NotesList* 			notesList() const = 0;
	virtual MarkersListEditor* 	markersListEditor() const = 0;
	virtual NotesListEditor* 	notesListEditor() const = 0;

    virtual void resetMarkersModel(ns_model::Markers* m);
    virtual void resetNotesModel(ns_model::Notes* m);
    virtual void resetMarkersList(MarkersList* wgt) = 0;
    virtual void resetNotesList(NotesList* wgt) = 0;

    virtual void init();
    void resetModels(QSharedPointer<ns_model::Markers> markersModel, QSharedPointer<ns_model::Notes> notesModel);
    void resetModelsFrom(NotesViewInterface* other);
};
