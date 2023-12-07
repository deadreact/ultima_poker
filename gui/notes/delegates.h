#pragma once

#include "notes_global.h"
#include <qstyleditemdelegate.h>
#include <qitemdelegate.h>
#include "markerslisteditor.h"
#include "model_markers.h"


class NOTES_DLL Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    Delegate(QObject* parent = nullptr);

    virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
};

class NOTES_DLL EditorDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:
    mutable ns_model::Markers::Item m_itemData;
    mutable MarkersListEditor* m_currentEditor;
public:
    EditorDelegate(QObject* parent = nullptr);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    virtual bool editorEvent(QEvent *e, QAbstractItemModel *m, const QStyleOptionViewItem &opt, const QModelIndex &index) override;
    MarkersListEditor* currentEditor() const;
    virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
public slots:
    void reset();

    // QStyledItemDelegate interface
protected:
    virtual void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;

};

/********************************************************************************************************/
/***************************************  DelegateForCompactView  *****************************************/
/********************************************************************************************************/
class NOTES_DLL DelegateForCompactView : public QItemDelegate
{
    Q_OBJECT
public:
    DelegateForCompactView(QObject* parent = nullptr);

public:
    virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
protected:
    virtual void drawDisplay(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QString &text) const override;
    virtual void drawDecoration(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QPixmap &pix) const override;

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
    virtual void destroyEditor(QWidget *editor, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
};

/********************************************************************************************************/
/***************************************  DelegateForPagesView  *****************************************/
/********************************************************************************************************/

class NOTES_DLL DelegateForPageView : public DelegateForCompactView
{
    Q_OBJECT
    mutable bool m_editorOpened;
public:
    DelegateForPageView(QObject* parent = nullptr);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
    virtual void destroyEditor(QWidget *editor, const QModelIndex &index) const override;
    virtual void paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const override;
    virtual void drawDecoration(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QPixmap &pix) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
signals:
    void editingStarted() const;
    void editingFinished() const;
};
