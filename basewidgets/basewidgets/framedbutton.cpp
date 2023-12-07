//#ifdef NOT_A_PLUGIN
//    #include <precompiled.h>
//#endif // NOT_A_PLUGIN
#include "framedbutton.h"

#include <QEvent>
#include <QGraphicsEffect>
#include <QResizeEvent>
#include <QLayout>
#include "util.h"

namespace ns_util {

    qreal clamp(qreal min, qreal max, qreal value)
    {
        if (value < min)
            return min;
        return (value > max ? max : value);
    }
    QPointF clamp(qreal min, qreal max, QPointF point)
    {
        return QPointF(clamp(min, max, point.x()), clamp(min, max, point.y()));
    }
}

bool FramedButton::pixmapValid() const
{
    return !m_pixmap.isNull();
}

FramedButton::FramedButton(QWidget *parent)
    : QPushButton(parent)
    , m_pixmap()
    , m_logic(nullptr)
    , m_textRegion(0, 0, width(), height())
    , m_sizeOfFrame(0, 0)
    , m_sizeInFrames(0, 0)
    , m_frameIndex(0)
    , m_maxFrameIndex(0)
    , m_backgroundColor(Qt::transparent)
    , m_blending(E::Normal)
    , m_aspectRatioMode(Qt::KeepAspectRatio)
    , m_state(0)
    , m_textAlignment(Qt::AlignCenter)
{
    connect(this, &FramedButton::toggled, this, [this](bool checked) {setState(E::ButtonState::CHECKED, checked);});
    connect(this, &FramedButton::pixmapChanged, this, &FramedButton::adjustProperties);
    connect(this, &FramedButton::sizeOfFrameChanged, this, &FramedButton::adjustProperties);
    connect(this, &FramedButton::textMoved, this, (void(FramedButton::*)(void))&FramedButton::repaint);
    connect(this, &FramedButton::stateChanged, this, (void(FramedButton::*)(void))&FramedButton::update, Qt::DirectConnection);
    connect(this, &FramedButton::frameIndexChanged, this, (void(FramedButton::*)(void))&FramedButton::repaint, Qt::DirectConnection);
    connect(this, &FramedButton::backgroundColorChanged, this, (void(FramedButton::*)(void))&FramedButton::repaint, Qt::DirectConnection);

//    setLogic(new DefaultFrameChangeLogic(this));
}

FramedButton::~FramedButton()
{
    if (m_logic != nullptr)
    {
        delete m_logic;
        m_logic = nullptr;
    }
}

void FramedButton::setLogic(Logic *logic)
{
    if (m_logic == logic) {
        return;
    }
    if (m_logic != nullptr) {
        disconnect(this, &FramedButton::stateChanged, m_logic, &Logic::onStateChanged);
        delete m_logic;
    }
    m_logic = logic;
    if (m_logic != nullptr) {
        connect(this, &FramedButton::stateChanged, m_logic, &Logic::onStateChanged);
    }
}

void FramedButton::setPixmap(const QPixmap &pixmap)
{
//    const QPixmap* pix = ns_util::PixmapCache::instance().cached(pixmap);
    if (m_pixmap.cacheKey() == pixmap.cacheKey())
        return;

    m_pixmap = pixmap;
    emit pixmapChanged(pixmap);
}

void FramedButton::setEnabled(bool enabled) {
    QPushButton::setEnabled(enabled);
    setState(E::ButtonState::DISABLED, !isEnabled());
}

void FramedButton::setDisabled(bool disabled)
{
    QPushButton::setDisabled(disabled);
    setState(E::ButtonState::DISABLED, disabled);
}

void FramedButton::setVisible(bool visible) {
    QPushButton::setVisible(visible);
}

void FramedButton::setSizeOfFrame(const QSize &sizeOfFrame)
{
    if (m_sizeOfFrame == sizeOfFrame)
            return;

    m_sizeOfFrame = sizeOfFrame;
    emit sizeOfFrameChanged(sizeOfFrame);
}

void FramedButton::setSizeInFrames(const QSize &sizeInFrames)
{
    if (m_sizeInFrames == sizeInFrames)
            return;

        m_sizeInFrames = sizeInFrames;
        emit sizeInFramesChanged(sizeInFrames);
}

void FramedButton::setMaxFrameIndex(int maxFrameIndex) {
    int validFrameIndex = ns_util::clamp(0, m_sizeInFrames.width() * m_sizeInFrames.height() - 1, maxFrameIndex);
    if (validFrameIndex == m_maxFrameIndex)
        return;

    m_maxFrameIndex = validFrameIndex;
}

void FramedButton::unsetMaxFrameIndex() {
    m_maxFrameIndex = 0;
}

QColor FramedButton::backgroundColor() const
{
    return m_backgroundColor;
}

Qt::Alignment FramedButton::textAlignment() const
{
    return m_textAlignment;
}

void FramedButton::moveText(const QRect &region)
{
    if (m_textRegion == region) {
        return;
    }

    m_textRegion = region;
    emit textMoved(m_textRegion);
}

void FramedButton::setBlending(E::Blending blending)
{
    m_blending = blending;
}

void FramedButton::setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode)
{
    m_aspectRatioMode = aspectRatioMode;
}

void FramedButton::setFrameIndex(int frameIndex)
{
    if (m_frameIndex == frameIndex)
        return;

    m_frameIndex = frameIndex;
    emit frameIndexChanged(frameIndex);
}

void FramedButton::setBackgroundColor(QColor color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged(color);
    }
}

void FramedButton::setTextAlignment(Qt::Alignment textAlignment)
{
    if (m_textAlignment == textAlignment) {
        return;
    }
    m_textAlignment = textAlignment;
    update();
}

void FramedButton::setState(E::ButtonFlag state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(state);
}

void FramedButton::setState(E::ButtonState state, bool enabled) {
    if (enabled == hasState(state))
        return;

    m_state = (enabled) ? (m_state | state) : (m_state ^ state);
    emit stateChanged(m_state);
}

bool FramedButton::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::Enter: if (isEnabled()) emit entered(); setState(E::ButtonState::HOVER, true);    break;
    case QEvent::Leave: if (isEnabled()) emit leaved();  setState(E::ButtonState::HOVER, false);   break;
    case QEvent::MouseButtonPress:      setState(E::ButtonState::PRESSED, true);  break;
    case QEvent::MouseButtonRelease:    setState(E::ButtonState::PRESSED, false); break;
    default:
        break;
    }
    return QPushButton::event(e);
}

int FramedButton::maxFrameIndex() const
{
    return (m_maxFrameIndex > 0) ? m_maxFrameIndex : (cols() * rows() - 1);
}

void FramedButton::adjustProperties()
{
    if ( !m_pixmap.isNull() )
    {
        int w = width();
        int h = height();
        if (!m_sizeOfFrame.isNull() && m_sizeOfFrame.isValid() && !m_sizeOfFrame.isEmpty())
        {
            w = m_sizeOfFrame.width();
            h = m_sizeOfFrame.height();
            resize(w, h);
        }
        else
        {
            setSizeOfFrame(size());
        }
        int cols = std::max(1, m_pixmap.width() / w);
        int rows = std::max(1, m_pixmap.height()/ h);
        setSizeInFrames(cols, rows);
    }
    repaint();
}

void FramedButton::paintEvent(QPaintEvent *event)
{
    if (!pixmapValid() && (m_backgroundColor.alpha() <= 1))
    {
        QPushButton::paintEvent(event);
    }
    else
    {
        QPainter p(this);
        ns_util::setBlendingOption(blending(), p);

        QRect targetRect = rect();

        if (widthOfFrame() > 0 && heightOfFrame() > 0)
        {
            if (cols() > 0 && rows() > 0) {
                targetRect.setTopLeft(QPoint((frameIndex() % cols()) * widthOfFrame()
                                           , (frameIndex() / cols()) * heightOfFrame()));
            }
            targetRect.setSize(m_sizeOfFrame);
        }

        if (m_backgroundColor.alpha() > 1) {
            p.fillRect(targetRect, m_backgroundColor);
        }
        if (pixmapValid()) {
            if (aspectRatioMode() != Qt::IgnoreAspectRatio)
            {
                qreal aspectRatio = static_cast<qreal>(width())/height();
                qreal frameAspectRatio = static_cast<qreal>(widthOfFrame()) / heightOfFrame();
                if ((aspectRatioMode() == Qt::KeepAspectRatio) == (aspectRatio < frameAspectRatio)) {
                        p.drawPixmap(QRect(0, 0, width(), (width() / frameAspectRatio)), m_pixmap, targetRect);
                } else {
                    p.drawPixmap(QRect(0, 0, height()*frameAspectRatio, height()), m_pixmap, targetRect);
                }
            }
            else
            {
                p.drawPixmap(rect(), m_pixmap, targetRect);
            }
        }
        p.drawText(m_textRegion, m_textAlignment | Qt::TextWordWrap, text());
        event->accept();
    }

    // QPointF absolutePos(m_textRelativePosition.x()*(width() - rc.width()), m_textRelativePosition.y()*(height() + rc.height()/2));
    // p.drawText(absolutePos, m_text);
}


QSize FramedButton::sizeHint() const
{
    if (layout() != nullptr) {
        return layout()->sizeHint();
    }
    return (pixmapValid() && sizeOfFrame().isValid()) ? sizeOfFrame() : QPushButton::sizeHint();
}

const QPixmap& FramedButton::pixmap() const
{
    return m_pixmap;
}

bool FramedButton::isEnabled() const {
    return QPushButton::isEnabled();
}

bool FramedButton::isVisible() const {
    return QPushButton::isVisible();
}

E::ButtonFlag FramedButton::state() const {
    return m_state;
}

bool FramedButton::hasState(E::ButtonState state) const {
    return m_state & state;
}

bool FramedButton::hasState(E::ButtonFlag state) const {
    return m_state & state;
}

QSize FramedButton::sizeOfFrame() const {
    return m_sizeOfFrame;
}

QSize FramedButton::sizeInFrames() const {
    return m_sizeInFrames;
}

QRect FramedButton::textRegion() const {
    return m_textRegion;
}

int FramedButton::frameIndex() const
{
    return (maxFrameIndex() >= m_frameIndex) ? m_frameIndex : 0;
}

E::Blending FramedButton::blending() const
{
    return m_blending;
}

Qt::AspectRatioMode FramedButton::aspectRatioMode() const
{
    return m_aspectRatioMode;
}

void FramedButton::resizeEvent(QResizeEvent *e)
{
    QPushButton::resizeEvent(e);
}

