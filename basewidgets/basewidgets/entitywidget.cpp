#include "entitywidget.h"
#include <qcoreevent.h>
#include <qevent.h>
#include <qpainter.h>
#include <QStyleOption>
#include "util.h"

bool isScrOpacityDesignable()
{
    return false;
}

void updateScrOpacity(QObject* object, qreal parentScrOpacity = 1.0)
{
    bool ok = false;
    qreal opacity = object->property("opacity").toReal(&ok);

    if (!ok) {
        opacity = 1.0;
        object->setProperty("opacity", 1.0);
    }
    qreal scrOpacity = opacity * parentScrOpacity;
    object->setProperty("scrOpacity", scrOpacity);

    {
        for (QObject* obj : object->children()) {
            updateScrOpacity(obj, scrOpacity);
        }
    }
}

void updateScrOpacity(EntityWidget* entity, qreal parentScrOpacity = 1.0)
{
    qreal opacity = entity->opacity();
    qreal scrOpacity = opacity * parentScrOpacity;
    entity->setScrOpacity(scrOpacity);

    for (QObject* obj : entity->children()) {
        updateScrOpacity(obj, scrOpacity);
    }
}



EntityWidget::EntityWidget(QWidget *parent)
    : QWidget(parent)
    , m_bgColor(0, 0, 0, 0)
    , m_opacity(1.0)
    , m_scrOpacity(1.0)
    , m_blending(E::Normal)
    , m_shape(false)
    , m_rendering(true)
    , m_parentPainting(E::ParentPainting::PaintParentBefore)
    , m_fixedAspectRatio(false)
    , m_aspectRatio(1.)
{
     connect(this, SIGNAL(renderingChanged(bool)), this, SLOT(update()));
}


bool EntityWidget::event(QEvent *event)
{
    switch  (event->type()) {
    case QEvent::MouseButtonPress: emit pressed(); break;
    case QEvent::MouseButtonRelease:  emit released();  break;
    case QEvent::FocusIn: emit focused();  break;
    case QEvent::FocusOut:  emit unfocused(); break;
    case QEvent::Enter: emit entered();    break;
    case QEvent::Leave: emit leaved();    break;
    case QEvent::Show: emit shown(); break;
    case QEvent::Hide: emit hidden(); break;
    default:
        break;
    }
    return QWidget::event(event);
}

bool EntityWidget::isRespondable() const {
    return !testAttribute(Qt::WA_TransparentForMouseEvents);
}

void EntityWidget::setBackgroundColor(const QColor& color)
{
    if (m_bgColor == color)
            return;
    m_bgColor = color;
    update();
    emit backgroundColorChanged(color);
}

void EntityWidget::setBlending(E::Blending blending)
{
    if (m_blending != blending) {
        m_blending = blending;
        update();
    }
}

void EntityWidget::setRendering(bool rendering)
{
    if (m_rendering == rendering)
        return;

    m_rendering = rendering;
    emit renderingChanged(rendering);
}

void EntityWidget::setOpacity(qreal opacity)
{
    opacity = qBound(0.0, opacity, 1.0);
    if (!qFuzzyCompare(m_opacity, opacity)) {
        m_opacity = opacity;
        bool ok = false;
        qreal scrOp = (!parentWidget()) ? 1.0 : parentWidget()->property("scrOpacity").toReal(&ok);
        updateScrOpacity(this, (ok ? scrOp : 1.0));
//        update();
//        setScrOpacity(scrOp*);
    }
}

void EntityWidget::setScrOpacity(qreal opacity)
{
    if (!qFuzzyCompare(m_scrOpacity, opacity)) {
        m_scrOpacity = opacity;
        update();
    }
}

void EntityWidget::setShape(bool shape)
{
    m_shape = shape;
#ifdef DESIGNER
    update();
#endif // DESIGNER
}

bool EntityWidget::isRendering() const {
    return m_rendering;
}

qreal EntityWidget::scrOpacity() const
{
//    bool ok = false;
//    if (parentWidget()) {
//        qreal parentScrOpacity = parentWidget()->property("scrOpacity").toReal(&ok);
//        if (ok) {
//            return parentScrOpacity * m_opacity;
//        }
//    }
    return m_scrOpacity;
}

E::ParentPainting EntityWidget::parentPainting() const
{
    return m_parentPainting;
}

void EntityWidget::setParentPainting(E::ParentPainting parentPainting)
{
    if (m_parentPainting != parentPainting) {
        m_parentPainting = parentPainting;
        update();
    }
}

QColor EntityWidget::backgroundColor() const
{
    return m_bgColor;
}

qreal EntityWidget::opacity() const {
    return m_opacity;
}

E::Blending EntityWidget::blending() const
{
    return m_blending;
}

bool EntityWidget::hasShape() const
{
    return m_shape;
}

void EntityWidget::setRespondable(bool respondable)
{
    if (isRespondable() == respondable)
        return;

    setAttribute(Qt::WA_TransparentForMouseEvents, !respondable);
    emit respondableToggled(respondable);
}


void EntityWidget::paintEvent(QPaintEvent *e)
{
    if (!m_rendering) {
        e->ignore();
        return;
    }
    if (m_parentPainting == E::ParentPainting::PaintParentBefore) {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
    if (true/*this->metaObject()->className() == QStringLiteral("EntityWidget")*/)
    {
        QPainter p(this);
        QRect rc(0, 0, width()-1, height()-1);
        p.fillRect(rc, m_bgColor);
        if (m_shape)
        {
            p.setPen(Qt::black);
            p.drawRect(rc);
        }
    }

    if (m_parentPainting == E::ParentPainting::PaintParentAfter) {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
}




void EntityWidget::initPainter(QPainter *p) const
{
    p->setOpacity(scrOpacity());
    ns_util::setBlendingOption(blending(), *p);
}

int EntityWidget::heightForWidth(int w) const
{
    return w/std::max(1., m_aspectRatio);
}

bool EntityWidget::hasHeightForWidth() const
{
    return m_fixedAspectRatio;
}

qreal EntityWidget::aspectRatio() const
{
    return m_aspectRatio;
}
