#pragma once

#include "notes_global.h"
#include <basewidgets/entitywidget.h>
#include <qtextbrowser.h>
#include "model_notes.h"

namespace Ui {
class NotesListEditor;
}
namespace ns_model {
class Markers;
//class Notes;
}
class QSortFilterProxyModel;
class QPropertyAnimation;
class QStyleOptionViewItem;

class NOTES_DLL TextBrowser : public QTextBrowser
{
    Q_OBJECT
    int m_minimumHeight;
    int m_maximumHeight;
    bool m_adjustedToContents;
public:
    explicit TextBrowser(QWidget* parent = nullptr);
    virtual bool event(QEvent *e) override;

    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;
    QSize maximumSizeHint() const;

    int actualHeight() const;
    int setAvailableHeight(int minimumHeight, int maximumHeight);
    bool isAdjustedToContents() const;
public slots:
    void setAdjustToContents(bool val);
signals:
    void focused();
    void unfocused();
    void heightDeltaUpdated(int h);
protected:
    virtual void wheelEvent(QWheelEvent *e) override;
};



class NOTES_DLL NotesListEditor : public EntityWidget
{
    Q_OBJECT

    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
//    Q_PROPERTY(int textEditorHeight READ textEditorHeight WRITE setTextEditorAvailableHeight)
    Q_PROPERTY(int heightForEditing READ heightForEditing WRITE setHeightForEditing)
    Q_PROPERTY(int heightForDisplaying READ heightForDisplaying WRITE setHeightForDisplaying)
    Q_PROPERTY(bool hideOnEditingFinished READ hideOnEditingFinished WRITE setHideOnEditingFinished)

private:
    Ui::NotesListEditor *m_ui;
    QScopedPointer<QSortFilterProxyModel> m_proxyMarkersModel;
    ns_model::Notes* m_notesModel;
    ns_model::Notes::ItemEditData m_sourceData;

    int m_heightForEditing;
    int m_heightForDisplaying;
    bool m_hideOnEditingFinished;
public:
    explicit NotesListEditor(QWidget *parent = nullptr);
    virtual ~NotesListEditor();

    void setMarkersModel(ns_model::Markers* model);
    void setNotesModel(ns_model::Notes* model);

    QModelIndex currentMarkerIndex() const;

    QString noteText() const;
    QColor markerColor() const;
    uint markerId() const;
    inline int height() const { return EntityWidget::height(); }

    int textEditorHeight() const;
    int heightForEditing() const;
    int heightForDisplaying() const;


    void setHeightForEditing(int h);
    void setHeightForDisplaying(int h);

    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;

    virtual void updateEditorGeometry(const QStyleOptionViewItem &opt, const QModelIndex &index);

    bool hideOnEditingFinished() const;

    bool isTextEditorAdjustedToContents() const;
    void setTextEditorAdjustedToContents(bool val);

public slots:
    void setCurrentMarker(const QModelIndex& idx);
    void displayNote(const QModelIndex& index);
    void editNote(const QModelIndex& index);
    void submit();
    void revert();

    void setButtonsVisible(bool visible = true);

    void saveState(const char *stateName = "default");
    void restoreState(const char* stateName = "default");
    void setHeight(int h);
    QPropertyAnimation* changeHeightTo(int h, int msec = 420);
    int setTextEditorAvailableHeight(int minimum, int maximum);
    void setHideOnEditingFinished(bool v);

signals:
    void currentMarkerChanged(const QModelIndex&);
    void editingStarted();
    void editingFinished(bool accepted);
    void heightChanged(int height);

protected:
    virtual void changeEvent(QEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;

};




