#include "delegates.h"
#include "noteslisteditor.h"

#include <qabstractbutton.h>
#include <qbuttongroup.h>
#include <qpainter.h>
#include <qdebug.h>
#include <qpropertyanimation.h>

namespace
{
    template <typename EnumType>
    bool setButtonChecked(QButtonGroup* group, EnumType enumValue) {
        if (QAbstractButton* bttn = group->button(underlying_cast(enumValue))) {
            bttn->setChecked(true);
            return true;
        }
        return false;
    }
}

Delegate::Delegate(QObject *parent) : QStyledItemDelegate(parent) {}

void Delegate::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    if (index.row() == 0) {
        QStyledItemDelegate::paint(p, opt, index);
    } else if (index.column() == 1) {
        QStyledItemDelegate::paint(p, opt, index);
        static const QPixmap normal(":/textures/Common/icons/settings/icon_edit_tag.png");
        static const QPixmap selected(":/textures/Common/icons/settings/icon_edit_tag_hover.png");
        QRect rc(opt.rect.center() - QPoint(5, 5), QSize(12, 12));
        if (opt.state & (QStyle::State_Selected | QStyle::State_MouseOver)) {
            p->drawPixmap(rc, selected);
        } else {
            p->drawPixmap(rc, normal);
        }
    } else if (index.column() == 2) {
        auto opt2 = opt;
        opt2.decorationPosition = QStyleOptionViewItem::Left;
        opt2.decorationAlignment = Qt::AlignLeft | Qt::AlignVCenter;
        opt2.rect.adjust(-2, 0, 0, 0);
        QStyledItemDelegate::paint(p, opt2, index);
    }
}

EditorDelegate::EditorDelegate(QObject *parent) : QStyledItemDelegate(parent), m_currentEditor(nullptr)
{

}

MarkersListEditor *EditorDelegate::currentEditor() const { return m_currentEditor; }

void EditorDelegate::reset()
{
    if (currentEditor() != nullptr) {
        emit closeEditor(currentEditor(), QAbstractItemDelegate::SubmitModelCache);
        m_currentEditor = nullptr;
    }
}

QWidget *EditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    m_currentEditor = new MarkersListEditor(parent->parentWidget());
    m_currentEditor->setFixedSize(option.rect.size() + QSize(70, 0));

    connect(this, &EditorDelegate::closeEditor, this, [this](QWidget* editor, QAbstractItemDelegate::EndEditHint hint){
        if (m_currentEditor == editor) {
            m_currentEditor = nullptr;
            if (hint == QAbstractItemDelegate::SubmitModelCache) {
            }
        }
    });

    return m_currentEditor;
}

void EditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (MarkersListEditor* item = qobject_cast<MarkersListEditor*>(editor)) {
        m_itemData = index.data(UserType::RowDataRole).value<ns_model::Markers::Item>();
        item->setColor(m_itemData.color());
        item->setText(m_itemData.name);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void EditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (MarkersListEditor* item = qobject_cast<MarkersListEditor*>(editor)) {
        m_itemData.setColor(item->color());
        m_itemData.name = item->text();
        model->setData(index, QVariant::fromValue(m_itemData), Qt::EditRole);
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

bool EditorDelegate::editorEvent(QEvent *e, QAbstractItemModel *m, const QStyleOptionViewItem &opt, const QModelIndex &index) {
    return QStyledItemDelegate::editorEvent(e, m, opt, index);
}


void EditorDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
//    QPixmap pix(option->decorationSize);
//    pix.fill(index.data(Qt::DecorationRole).value<QColor>());
//    QPainter p(&pix);
//    p.setPen(QColor(0x191919));
//    p.drawRect(pix.rect().adjusted(0, 0, -1, -1));
//    option->icon = QIcon(pix);
}


void EditorDelegate::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
//    QStyleOptionViewItem opt2 = opt;
//    opt2.decorationSize.setWidth(opt2.decorationSize.width()-1);
    QStyledItemDelegate::paint(p, opt, index);
    QRect rc = QRect(opt.rect.topLeft(), opt.decorationSize).adjusted(24, 4, 24, 4);
    if (index.row() == 0) {
        p->fillRect(rc, QBrush(QColor(0x37474f), Qt::BDiagPattern));
    }
    p->setPen(QColor(0x191919));
    p->drawRect(rc);
}

/********************************************************************************************************/
/***************************************  DelegateForCompactView  *****************************************/
/********************************************************************************************************/

DelegateForCompactView::DelegateForCompactView(QObject *parent): QItemDelegate(parent) {}

void DelegateForCompactView::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    QItemDelegate::paint(p, opt, index);
    if (index.column() == 1) {
        static const QPixmap icoEdit(":/textures/Common/icons/settings/icon_edit_player.png");
        static const QPixmap icoEditHover(":/textures/Common/icons/settings/icon_edit_player_hover.png");
        static const QPixmap icoDelete(":/textures/Common/icons/settings/icon_delete_player.png");


        if (index.sibling(index.row(), 0).flags() & Qt::ItemIsEditable) {
            const QPixmap* target = &icoEdit;
            if (opt.state & QStyle::State_Selected) {
                target = &icoEditHover;
                //qDebug() << opt.state;
            }

            p->drawPixmap(/*opt.rect.x() + opt.rect.width() - 42*/ opt.widget->width() - 56, opt.rect.center().y() - target->height()/2 + 1, target->width(), target->height(), *target);
        }
        QRect targetArea(opt.widget->width() - 32, opt.rect.center().y() - icoDelete.height()/2 + 1, icoDelete.width(), icoDelete.height());
        p->drawPixmap(targetArea, icoDelete);
    }
}

void DelegateForCompactView::drawDisplay(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QString &text) const {
    auto opt2 = opt;
    if (opt.state & QStyle::State_Selected) {
        QPalette pal = opt.palette;
        pal.setColor(QPalette::HighlightedText, QColor(0x191919));
        opt2.palette = pal;
    }
    QItemDelegate::drawDisplay(p, opt2, rc.adjusted(21, -1, 21, -1), text);
}

void DelegateForCompactView::drawDecoration(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QPixmap &pix) const {
    if (pix.isNull()) {
        return;
    }
    QRect rc2 = rc.adjusted(23, 0, 22, -1);
    QItemDelegate::drawDecoration(p, opt, rc2, pix.scaled(rc2.size()));
    p->setPen(QColor(0x191919));
    p->drawRect(rc2);
}

QWidget *DelegateForCompactView::createEditor(QWidget *parent, const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    if (NotesListEditor* editor = parent->window()->findChild<NotesListEditor*>()) {
        editor->setFocusPolicy(Qt::StrongFocus);
        editor->setRespondable(true);
        editor->setButtonsVisible(true);
        editor->setVisible(true);
        DelegateForCompactView* that = (DelegateForCompactView*)this;
        connect(editor, &NotesListEditor::editingFinished, that, [that, editor](bool accepted){
            if (accepted)
                emit that->commitData(editor);
            emit that->closeEditor(editor, accepted ? QAbstractItemDelegate::SubmitModelCache : QAbstractItemDelegate::RevertModelCache);
        });

        return editor;
    }
    return QItemDelegate::createEditor(parent, opt, index);
}

void DelegateForCompactView::destroyEditor(QWidget *editor, const QModelIndex &index) const {
    if (NotesListEditor* e = qobject_cast<NotesListEditor*>(editor)) {
        editor->setFocusPolicy(Qt::NoFocus);
        e->setRespondable(false);
        e->setButtonsVisible(false);
        e->setVisible(true);
        connect(e->changeHeightTo(e->heightForDisplaying()), &QPropertyAnimation::finished, [e]{
            e->setHidden(e->hideOnEditingFinished());
        });
    } else {
        QItemDelegate::destroyEditor(editor, index);
    }
}

void DelegateForCompactView::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (NotesListEditor* e = qobject_cast<NotesListEditor*>(editor)) {
        e->editNote(index);
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void DelegateForCompactView::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    if (NotesListEditor* e = qobject_cast<NotesListEditor*>(editor)) {
        ns_model::Notes::ItemEditData d(e->markerId(), e->noteText());

        if (model->setData(index, QVariant::fromValue(d), Qt::EditRole)) {
            if (auto m = qobject_cast<ns_model::Notes*>(model)) {
                m->write();
            }
        }
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void DelegateForCompactView::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    if (NotesListEditor* e = qobject_cast<NotesListEditor*>(editor)) {
        e->updateEditorGeometry(opt, index);
    } else {
        QItemDelegate::updateEditorGeometry(editor, opt, index);
    }
}

/********************************************************************************************************/
/***************************************  DelegateForPagesView  *****************************************/
/********************************************************************************************************/
DelegateForPageView::DelegateForPageView(QObject *parent)
    : DelegateForCompactView(parent)
{}

QWidget *DelegateForPageView::createEditor(QWidget *parent, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if (NotesListEditor* editor = parent->window()->findChild<NotesListEditor*>()) {
        DelegateForPageView* that = (DelegateForPageView*)this;
        connect(editor, &NotesListEditor::editingFinished, that, [that, editor](bool accepted){
            if (accepted)
                emit that->commitData(editor);
            emit that->closeEditor(editor, accepted ? QAbstractItemDelegate::SubmitModelCache : QAbstractItemDelegate::RevertModelCache);
        });

        editor->setHeightForDisplaying(418);
        editor->setHeightForEditing(418);
        editor->setTextEditorAdjustedToContents(false);

        emit editingStarted();
        m_editorOpened = true;
        return editor;
    }
    return QItemDelegate::createEditor(parent, opt, index);
}

void DelegateForPageView::destroyEditor(QWidget *editor, const QModelIndex &/*index*/) const
{
    editor->setVisible(true);
    emit editingFinished();
    m_editorOpened = false;
}

void DelegateForPageView::drawDecoration(QPainter *p, const QStyleOptionViewItem &opt, const QRect &rc, const QPixmap &pix) const
{
    DelegateForCompactView::drawDecoration(p, opt, rc, pix);
//    if (!m_editorOpened) {
//    }
}


void DelegateForPageView::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (NotesListEditor* e = qobject_cast<NotesListEditor*>(editor))
    {
        e->setFixedHeight(418);
        e->setTextEditorAvailableHeight(192, 192);
    }
}

void DelegateForPageView::paint(QPainter *p, const QStyleOptionViewItem &opt, const QModelIndex &index) const {
    QItemDelegate::paint(p, opt, index);
    if (!m_editorOpened && index.column() == 1) {
        static const QPixmap icoEdit(":/textures/Common/icons/settings/icon_edit_player.png");
        static const QPixmap icoEditHover(":/textures/Common/icons/settings/icon_edit_player_hover.png");
        static const QPixmap icoDelete(":/textures/Common/icons/settings/icon_delete_player.png");


        if (index.sibling(index.row(), 0).flags() & Qt::ItemIsEditable) {
            const QPixmap* target = &icoEdit;
            if (opt.state & QStyle::State_Selected) {
                target = &icoEditHover;
                //qDebug() << opt.state;
            }

            p->drawPixmap(/*opt.rect.x() + opt.rect.width() - 42*/ opt.widget->width() - 56, opt.rect.center().y() - target->height()/2 + 1, target->width(), target->height(), *target);
        }
        QRect targetArea(opt.widget->width() - 32, opt.rect.center().y() - icoDelete.height()/2 + 1, icoDelete.width(), icoDelete.height());
        p->drawPixmap(targetArea, icoDelete);
    }
}
