//#include <precompiled.h>
#include "ui_noteslisteditor.h"
#include <qsortfilterproxymodel.h>
#include <qpropertyanimation.h>
#include <qdebug.h>
#include <qevent.h>
#include "noteslisteditor.h"
#include "model_markers.h"
#include "model_notes.h"

namespace
{

}

TextBrowser::TextBrowser(QWidget *parent)
    : QTextBrowser(parent)
    , m_minimumHeight(44)
    , m_maximumHeight(260)
    , m_adjustedToContents(true)
{
    connect(this, &TextBrowser::textChanged, this, [this]{
        if (m_adjustedToContents)
        {
            QSize contentsSize = fontMetrics().boundingRect(rect().adjusted(-3, -3, 3, 3), Qt::TextWordWrap | Qt::AlignTop | Qt::AlignLeft, toPlainText()).size();

            int h = qBound(minimumSizeHint().height(), contentsSize.height(), maximumSizeHint().height());
            emit heightDeltaUpdated(h);
        }
    });
}

bool TextBrowser::event(QEvent *e)
{
    if (e->type() == QEvent::FocusIn) {
        emit focused();
    } else if (e->type() == QEvent::FocusOut) {
        emit unfocused();
    }
    return QTextBrowser::event(e);
}

QSize TextBrowser::sizeHint() const
{
//    int dw1, dw2, dh1, dh2;
//    getContentsMargins(&dw1, &dh1, &dw2, &dh2);
//    return fontMetrics().size(QTextOption::WrapAtWordBoundaryOrAnywhere, toPlainText()) + QSize(dw1+dw2, dh1+dh2);

    return QTextBrowser::sizeHint();// : QSize(QTextBrowser::sizeHint().width(), fontMetrics().size(QTextOption::WrapAtWordBoundaryOrAnywhere, toPlainText()).height());
}

QSize TextBrowser::minimumSizeHint() const {
    return QSize(QTextBrowser::minimumSizeHint().width(), 44);
}
QSize TextBrowser::maximumSizeHint() const {
    return QSize(QTextBrowser::sizeHint().width(), m_maximumHeight);
}

int TextBrowser::actualHeight() const
{
    QSize contentsSize = fontMetrics().boundingRect(rect().adjusted(-3, -3, 3, 3), Qt::TextWordWrap | Qt::AlignTop | Qt::AlignLeft, toPlainText()).size();
    //qDebug() << contentsSize;
    int h = qBound(m_minimumHeight, contentsSize.height() + 12, m_maximumHeight);
    return qBound(minimumSizeHint().height(), h, maximumSizeHint().height());
}

int TextBrowser::setAvailableHeight(int minimumHeight, int maximumHeight)
{
    if (m_minimumHeight != minimumHeight || m_maximumHeight != maximumHeight) {
        m_minimumHeight = minimumHeight;
        m_maximumHeight = maximumHeight;
    }
    int h = actualHeight();
    setFixedHeight(h);
    return h;
}

bool TextBrowser::isAdjustedToContents() const {
    return m_adjustedToContents;
}

void TextBrowser::setAdjustToContents(bool val) {
    m_adjustedToContents = val;
}

void TextBrowser::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::CTRL) {
        QPoint numDegrees = e->angleDelta() / 8;
        int val = (numDegrees.x() == 0 ? numDegrees.y() : numDegrees.x()) / 15;

        if (val > 0) {
            zoomIn();
        } else {
            zoomOut();
        }
        e->accept();
    }
    else
    {
        QTextBrowser::wheelEvent(e);
    }
}

// --------------------------

class SortFilterProxyModel : public QSortFilterProxyModel
{
public:
    SortFilterProxyModel(QObject* parent = nullptr): QSortFilterProxyModel(parent) {}

    virtual QVariant data(const QModelIndex &index, int role) const override {
        if (role == Qt::DisplayRole) {
            return QVariant();
        } else if (role == Qt::ToolTipRole) {
            return QSortFilterProxyModel::data(index, Qt::DisplayRole);
        } /*else if (role == Qt::DecorationRole) {
            QVariant d = QSortFilterProxyModel::data(index, Qt::DecorationRole);
            if (d.canConvert<QPixmap>()) {
                QPixmap p = d.value<QPixmap>();
                return p.scaled(12, 12);
            }
        }*/
        return QSortFilterProxyModel::data(index, role);
    }
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override {
        return QSortFilterProxyModel::flags(index) | Qt::ItemIsSelectable;
    }
};

int NotesListEditor::heightForDisplaying() const {
    return m_heightForDisplaying;
}

void NotesListEditor::setHeightForEditing(int h)
{
    if (m_heightForEditing != h) {
        m_heightForEditing = h;
        updateGeometry();
    }
}

void NotesListEditor::setHeightForDisplaying(int h) {
    if (m_heightForDisplaying != h) {
        m_heightForDisplaying = h;
        updateGeometry();
    }
}

NotesListEditor::NotesListEditor(QWidget *parent)
    : EntityWidget(parent)
    , m_ui(new Ui::NotesListEditor)
    , m_proxyMarkersModel(new SortFilterProxyModel)
    , m_notesModel(nullptr)
    , m_heightForEditing(200)
    , m_heightForDisplaying(106)
    , m_hideOnEditingFinished(false)
{
    m_ui->setupUi(this);
    m_ui->listView->setModel(m_proxyMarkersModel.data());

    m_ui->buttons->hide();

    auto updateOkBttnEnabled = [this]{
        if (m_ui->buttons->isVisible()) {
            m_ui->bttn_apply->setEnabled(m_sourceData.markerId != markerId() || m_sourceData.note != noteText());
        }
    };

    connect(m_ui->listView, (void(MarkersListView::*)(const QModelIndex&))&MarkersListView::currentChanged, this, updateOkBttnEnabled);
    connect(m_ui->textBrowser, &TextBrowser::textChanged, this, updateOkBttnEnabled);
//    connect(m_ui->textBrowser, &TextBrowser::textChanged, this, adjustGeometry);

    connect(m_ui->textBrowser, &TextBrowser::heightDeltaUpdated, this, [this](int h){
//        if (abs(h - m_ui->textBrowser->height()) > 20) {
//            int delta = h - m_ui->textBrowser->height();
//            qDebug() << delta;
//            int reserved = 54 + (m_ui->buttons->isHidden() ? 0 : 52);
//            h = setTextEditorAvailableHeight(44, std::min(h, maximumHeight() - reserved));

            changeHeightTo(m_heightForEditing);
//        }
    });
}

NotesListEditor::~NotesListEditor()
{
    delete m_ui;
}

void NotesListEditor::setMarkersModel(ns_model::Markers *model) {
    m_proxyMarkersModel->setSourceModel(model);
}

void NotesListEditor::setNotesModel(ns_model::Notes *model)
{
    if (m_notesModel != model) {
        m_notesModel = model;
    }
    if (m_notesModel && m_notesModel->sourceModel() != m_proxyMarkersModel->sourceModel()) {
        setMarkersModel(m_notesModel->sourceModel());
    }
}


QModelIndex NotesListEditor::currentMarkerIndex() const {
    return m_ui->listView->currentIndex();
}

QString NotesListEditor::noteText() const {
    return m_ui->textBrowser->toPlainText();
}

QColor NotesListEditor::markerColor() const {
    return currentMarkerIndex().data(Qt::DecorationRole).value<QColor>();
}
uint NotesListEditor::markerId() const {
    return currentMarkerIndex().data(UserType::RowItemIdRole).value<uint>();
}

int NotesListEditor::textEditorHeight() const {
    return m_ui->textBrowser->height();
}

int NotesListEditor::heightForEditing() const {
    return m_heightForEditing;
}

void NotesListEditor::setCurrentMarker(const QModelIndex &idx) {
    m_ui->listView->setCurrentIndex(idx);
}

void NotesListEditor::displayNote(const QModelIndex &index)
{
    m_sourceData = index.data(Qt::EditRole).value<ns_model::Notes::ItemEditData>();
    m_ui->textBrowser->setText(m_sourceData.note);
    QModelIndex idx = m_notesModel->sourceModel()->indexOf(m_sourceData.markerId);
    if (idx.isValid()) {
        m_ui->listView->setCurrentIndex(m_proxyMarkersModel->mapFromSource(idx));
    }
}


void NotesListEditor::editNote(const QModelIndex &index)
{
    displayNote(index);
    setButtonsVisible();
    m_ui->textBrowser->setFocus();
    m_ui->textBrowser->selectAll();

    setWindowModified(true);

    emit editingStarted();
}

void NotesListEditor::submit() {
    emit editingFinished(true);
}

void NotesListEditor::revert() {
    emit editingFinished(false);
}

void NotesListEditor::setButtonsVisible(bool visible)
{
    m_ui->buttons->setVisible(visible);
}

void NotesListEditor::saveState(const char* stateName)
{
    QByteArray value;
    QDataStream stream(&value, QIODevice::WriteOnly);
    stream << saveGeometry();
    stream << isHidden();
    stream << isRespondable();
    stream << isEnabled();
    QWidgetList lst = findChildren<QWidget*>();
    stream << lst.size();
    for (QWidget* child: lst) {
        stream << child->metaObject()->className() << child->objectName() << child->isHidden() << child->saveGeometry();
    }
    setProperty(QString("state_%0").arg(stateName).toLatin1(), value);
}

void NotesListEditor::restoreState(const char *stateName)
{
    QVariant uncheckedValue = property(QString("state_%0").arg(stateName).toLatin1());
    if (uncheckedValue.canConvert(QVariant::ByteArray)) {
        QByteArray value = uncheckedValue.toByteArray();
        QDataStream stream(&value, QIODevice::ReadOnly);
        QByteArray geometry;
        stream >> geometry;
        restoreGeometry(geometry);

        bool hidden, respondable, enabled;
        stream >> hidden >> respondable >> enabled;
        setHidden(hidden);
        setRespondable(respondable);
        setEnabled(enabled);

        int childrenCount;
        stream >> childrenCount;

        QByteArray clsName;
        QString objName;
        bool isHidden;
//        QByteArray geometry;
        for (int i = 0; i < childrenCount; i++) {
            stream >> clsName >> objName >> isHidden >> geometry;
            if (QWidget* w = findChild<QWidget*>(objName)) {
                if (clsName == w->metaObject()->className()) {
                    w->setHidden(isHidden);
                    w->restoreGeometry(geometry);
                }
            }
        }
    }
}

void NotesListEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NotesListEditor::resizeEvent(QResizeEvent *e)
{
//    if (QWidget* parent = parentWidget()) {
//        move(0, parent->height() - e->size().height());
//    }
    if (e && e->size().isValid() && e->oldSize().isValid()) {
        QSize diff = e->size() - e->oldSize();
        move(pos() - QPoint(diff.width(), diff.height()));
//        int reservedSpace = 54 + (m_ui->buttons->isHidden() ? 0 : 52);
//        setTextEditorAvailableHeight(44, e->size().height() - reservedSpace);
    }

    EntityWidget::resizeEvent(e);
}

void NotesListEditor::setHeight(int h)
{
    if (height() != h) {
        resize(width(), h);
//        int diff = h - sizeHint().height();
//        setTextEditorHeight(textEditorHeight()+diff);
//        adjustSize();
        emit heightChanged(h);
    }
}

QPropertyAnimation *NotesListEditor::changeHeightTo(int h, int msec)
{
    int reservedSpace = 54 + (m_ui->buttons->isHidden() ? 0 : 52);
    int hintHeight = h - reservedSpace;
    int delta = hintHeight - setTextEditorAvailableHeight(44, hintHeight);
    h -= delta;

    QPropertyAnimation *anim = new QPropertyAnimation(this, "height", nullptr);
    anim->setDuration(msec);
//    auto g = geometry();
    anim->setStartValue(height());
    anim->setEndValue(h);//QRect(g.x(), g.y()+height()-h, g.width(), h));
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    setMinimumHeight(std::min(height(), h) - std::abs(h-height())/2);
    setMaximumHeight(std::max(height(), h) + std::abs(h-height())/2);
    connect(anim, &QPropertyAnimation::finished, this, [this, h]{ resize(width(), h);});
    return anim;
}

int NotesListEditor::setTextEditorAvailableHeight(int minimum, int maximum) {
    return m_ui->textBrowser->setAvailableHeight(minimum, maximum);
}


QSize NotesListEditor::sizeHint() const
{
    return minimumSizeHint() + m_ui->textBrowser->sizeHint();
}

QSize NotesListEditor::minimumSizeHint() const
{
    return QSize(EntityWidget::minimumSizeHint().width(), 54 + (m_ui->buttons->isHidden() ? 0 : 52) + m_ui->textBrowser->minimumSizeHint().height());
}

void NotesListEditor::updateEditorGeometry(const QStyleOptionViewItem &opt, const QModelIndex &index)
{
//    qDebug() << "updateEditorGeometry  " << index;
//    qDebug() << "version               " << opt.version;
//    qDebug() << "type                  " << opt.type;
//    qDebug() << "state                 " << opt.state;
//    qDebug() << "direction             " << opt.direction;
//    qDebug() << "rect                  " << opt.rect;
////    qDebug() << "fontMetrics           " << opt.fontMetrics;
//    qDebug() << "palette               " << opt.palette;
//    qDebug() << "styleObject           " << opt.styleObject;
//    qDebug() << "displayAlignment      " << opt.displayAlignment;
//    qDebug() << "decorationAlignment   " << opt.decorationAlignment;
//    qDebug() << "textElideMode         " << opt.textElideMode;
//    qDebug() << "decorationPosition    " << opt.decorationPosition;
//    qDebug() << "decorationSize        " << opt.decorationSize;
//    qDebug() << "font                  " << opt.font;
//    qDebug() << "showDecorationSelected" << opt.showDecorationSelected;

//    qDebug() << "features              " << opt.features;
//    qDebug() << "locale                " << opt.locale;
//    qDebug() << "widget                " << opt.widget;
//    qDebug() << "index                 " << opt.index;
//    qDebug() << "checkState            " << opt.checkState;
//    qDebug() << "icon                  " << opt.icon;
//    qDebug() << "text                  " << opt.text;
//    qDebug() << "viewItemPosition      " << opt.viewItemPosition;
//    qDebug() << "backgroundBrush       " << opt.backgroundBrush;

    changeHeightTo(heightForEditing());
}

bool NotesListEditor::hideOnEditingFinished() const {
    return m_hideOnEditingFinished;
}

bool NotesListEditor::isTextEditorAdjustedToContents() const {
    return m_ui->textBrowser->isAdjustedToContents();
}

void NotesListEditor::setTextEditorAdjustedToContents(bool val)
{
    m_ui->textBrowser->setAdjustToContents(val);
}
void NotesListEditor::setHideOnEditingFinished(bool v)
{
    m_hideOnEditingFinished = v;
}


