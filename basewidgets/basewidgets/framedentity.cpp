//#ifdef NOT_A_PLUGIN
//    #include <precompiled.h>
//#endif // NOT_A_PLUGIN
#include "framedentity.h"
#include <QEvent>
#include <QTimer>
#include <qevent.h>
#include "util.h"

struct FramedEntityData
{
    const QPixmap*            m_pixmap;
    QSize                     m_sizeOfFrame;
    QSize                     m_sizeInFrames;
    int                       m_frameIndex;
    int                       m_maxFrameIndex;
    int                       m_fps;
    uint                      m_loops;
    uint                      m_loopsLeft;
    bool                      m_autoplay;
    bool                      m_isPlaying;
    Qt::AspectRatioMode       m_aspectRatioMode;

    FramedEntityData(const QPixmap* pixmap)
        : m_pixmap(pixmap)
        , m_sizeOfFrame(0, 0)
        , m_sizeInFrames(1, 1)
        , m_frameIndex(0)
        , m_maxFrameIndex(0)
        , m_fps(0)
        , m_loops(0)
        , m_loopsLeft(0)
        , m_autoplay(false)
        , m_isPlaying(false)
        , m_aspectRatioMode(Qt::KeepAspectRatio)
    {}

    FramedEntityData(const FramedEntityData& other)
        : m_pixmap(other.m_pixmap)
        , m_sizeOfFrame(other.m_sizeOfFrame)
        , m_sizeInFrames(other.m_sizeInFrames)
        , m_frameIndex(other.m_frameIndex)
        , m_maxFrameIndex(other.m_maxFrameIndex)
        , m_fps(other.m_fps)
        , m_loops(other.m_loops)
        , m_loopsLeft(other.m_loopsLeft)
        , m_autoplay(other.m_autoplay)
        , m_isPlaying(other.m_isPlaying)
        , m_aspectRatioMode(Qt::KeepAspectRatio)
    {}
};


FramedEntity::FramedEntity(const FramedEntityData &data, QWidget *parent)
    : EntityWidget(parent)
    , m_dataPtr(new FramedEntityData(data))
    , m_timer(new QTimer(this))
{
    registerEvents();
}

void FramedEntity::registerEvents()
{
    connect(this, &FramedEntity::pixmapChanged, this, &FramedEntity::adjustProperties);
    connect(this, &FramedEntity::sizeOfFrameChanged, this, &FramedEntity::adjustProperties);
    connect(this, &FramedEntity::frameIndexChanged, this, (void(FramedEntity::*)(void))&FramedEntity::repaint);
    connect(m_timer, &QTimer::timeout, this, &FramedEntity::nextFrame);
    setPlaying(m_dataPtr->m_autoplay);
}

FramedEntity::FramedEntity(QWidget *parent)
    : EntityWidget(parent)
    , m_dataPtr(new FramedEntityData(nullptr))
    , m_timer(new QTimer(this))
{
    registerEvents();
}
FramedEntity::FramedEntity(const QPixmap *pixmap, QWidget *parent)
    : EntityWidget(parent)
    , m_dataPtr(new FramedEntityData(pixmap))
    , m_timer(new QTimer(this))
{
    registerEvents();
}

FramedEntity::~FramedEntity()
{
    delete m_timer;
    delete m_dataPtr;
}

void FramedEntity::init()
{

}

const QPixmap &FramedEntity::pixmap() const {
    static const QPixmap empty = QPixmap();
    return (m_dataPtr->m_pixmap == nullptr ? empty : *m_dataPtr->m_pixmap);
}

bool FramedEntity::pixmapValid() const
{
    return (m_dataPtr->m_pixmap != nullptr && !m_dataPtr->m_pixmap->isNull() && !m_dataPtr->m_pixmap->size().isEmpty());
}

int FramedEntity::frameIndex() const
{
    return m_dataPtr->m_frameIndex;
}

QSize FramedEntity::sizeOfFrame() const
{
    return m_dataPtr->m_sizeOfFrame;
}

void FramedEntity::adjustProperties()
{
    if (m_dataPtr->m_pixmap != nullptr)
    {
        int w = width();
        int h = height();
        if (!m_dataPtr->m_sizeOfFrame.isNull() && m_dataPtr->m_sizeOfFrame.isValid() && !m_dataPtr->m_sizeOfFrame.isEmpty())
        {
            w = m_dataPtr->m_sizeOfFrame.width();
            h = m_dataPtr->m_sizeOfFrame.height();
            resize(w, h);
        }
        else
        {
            setSizeOfFrame(size());
        }
        int cols = std::max(1, m_dataPtr->m_pixmap->width() / w);
        int rows = std::max(1, m_dataPtr->m_pixmap->height()/ h);
        setSizeInFrames(cols, rows);
    }
}

void FramedEntity::unsetMaxFrameIndex()
{
    m_dataPtr->m_maxFrameIndex = 0;
}

void FramedEntity::setPlaying(bool playing)
{
    if (playing == m_dataPtr->m_isPlaying)
        return;

    if (playing)
    {
        if (m_dataPtr->m_fps == 0)
            setFps(30);
        setFrameIndex(0);
        m_dataPtr->m_loopsLeft = m_dataPtr->m_loops;
        m_dataPtr->m_isPlaying = true;
        m_timer->start(1000/m_dataPtr->m_fps);

        emit started();
    }
    else
    {
        stop();
    }
}

void FramedEntity::stop()
{
    m_timer->stop();
    setFrameIndex(m_dataPtr->m_maxFrameIndex);
    m_dataPtr->m_loopsLeft = 0;
    m_dataPtr->m_isPlaying = false;
    emit finished();
}

void FramedEntity::togglePlaying()
{
    if (m_dataPtr->m_isPlaying) {
        stop();
    } else {
        setPlaying();
    }
}

void FramedEntity::pause()
{
    m_timer->stop();
    m_dataPtr->m_isPlaying = false;

    emit paused();
}

void FramedEntity::resume()
{
    if (m_dataPtr->m_fps == 0)
        setFps(30);
    m_dataPtr->m_isPlaying = true;
    m_timer->start(1000/m_dataPtr->m_fps);

    emit resumed();
}

void FramedEntity::togglePause()
{
    if (m_dataPtr->m_isPlaying) {
        pause();
    } else {
        resume();
    }
}

void FramedEntity::setFps(int fps)
{
    if (m_dataPtr->m_fps == fps)
        return;

    m_dataPtr->m_fps = fps;
    emit fpsChanged(fps);
}

void FramedEntity::setLoops(uint loops)
{
    if (m_dataPtr->m_loops == loops)
        return;

    m_dataPtr->m_loops = loops;
    emit loopsChanged(loops);
}

void FramedEntity::setAutoplay(bool autoplay) {
    m_dataPtr->m_autoplay = autoplay;
}

void FramedEntity::setSizeInFrames(const QSize &sizeInFrames) {
    if (m_dataPtr->m_sizeInFrames == sizeInFrames)
        return;

    m_dataPtr->m_sizeInFrames = sizeInFrames;
    emit sizeInFramesChanged(sizeInFrames);
}

int FramedEntity::maxFrameIndex() const {
    return (m_dataPtr->m_maxFrameIndex > 0) ? m_dataPtr->m_maxFrameIndex : (cols() * rows() - 1);
}

int FramedEntity::fps() const {
    return m_dataPtr->m_fps;
}

uint FramedEntity::loops() const
{
    return m_dataPtr->m_loops;
}

bool FramedEntity::autoplay() const
{
    return m_dataPtr->m_autoplay;
}

bool FramedEntity::isPlaying() const {
    return m_dataPtr->m_isPlaying;
}

QSize FramedEntity::sizeInFrames() const {
    return m_dataPtr->m_sizeInFrames;
}

void FramedEntity::setPixmap(const QPixmap &pixmap) {

    if (m_dataPtr->m_pixmap && m_dataPtr->m_pixmap->cacheKey() == pixmap.cacheKey())
        return;
    if (m_dataPtr->m_pixmap)
        delete m_dataPtr->m_pixmap;

    m_dataPtr->m_pixmap = new QPixmap(pixmap);
    emit pixmapChanged(pixmap);
}

void FramedEntity::setFrameIndex(int frameIndex)
{
    if (m_dataPtr->m_frameIndex == frameIndex)
        return;

    m_dataPtr->m_frameIndex = frameIndex;
    emit frameIndexChanged(frameIndex);
}

void FramedEntity::setSizeOfFrame(const QSize &sizeOfFrame)
{
    if (m_dataPtr->m_sizeOfFrame == sizeOfFrame)
        return;

    m_dataPtr->m_sizeOfFrame = sizeOfFrame;
    emit sizeOfFrameChanged(sizeOfFrame);
}

void FramedEntity::setMaxFrameIndex(int maxFrameIndex)
{
    int validFrameIndex = qBound(0, maxFrameIndex, m_dataPtr->m_sizeInFrames.width() * m_dataPtr->m_sizeInFrames.height() - 1);
    if (validFrameIndex == m_dataPtr->m_maxFrameIndex)
        return;

    m_dataPtr->m_maxFrameIndex = validFrameIndex;
}

void FramedEntity::nextFrame()
{
    if (m_dataPtr->m_frameIndex < maxFrameIndex()) {
        setFrameIndex(m_dataPtr->m_frameIndex+1);
    } else {
        if (m_dataPtr->m_loopsLeft > 0) {
            m_dataPtr->m_loopsLeft -= 1;
            setFrameIndex(0);
        } else {
            stop();
        }
    }
}

void FramedEntity::prevFrame()
{
    if (m_dataPtr->m_frameIndex > 0) {
        setFrameIndex(m_dataPtr->m_frameIndex-1);
    } else {
        if (m_dataPtr->m_loopsLeft > 0) {
            m_dataPtr->m_loopsLeft -= 1;
            setFrameIndex(maxFrameIndex());
        } else {
            stop();
        }
    }
}

void FramedEntity::paintEvent(QPaintEvent *e)
{
    if (!isRendering()) {
        e->ignore();
        return;
    }
    EntityWidget::paintEvent(e);
    if (m_dataPtr->m_pixmap != nullptr)
    {
        QPainter p(this);

        QPoint framePos((m_dataPtr->m_frameIndex % m_dataPtr->m_sizeInFrames.width()) * m_dataPtr->m_sizeOfFrame.width()
                      , (m_dataPtr->m_frameIndex / m_dataPtr->m_sizeInFrames.width()) * m_dataPtr->m_sizeOfFrame.height());


        QPixmap pix2 = m_dataPtr->m_pixmap->copy(QRect(framePos, m_dataPtr->m_sizeOfFrame)).scaled(size(), aspectRatioMode(), Qt::SmoothTransformation);
        p.drawPixmap(rect(), pix2);
    }
}

void FramedEntity::setVisible(bool visible)
{
    if (!this->isVisible() && visible && m_dataPtr->m_autoplay)
    {
        setPlaying();
    }
    QWidget::setVisible(visible);
}

void FramedEntity::setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode)
{
    if (m_dataPtr->m_aspectRatioMode != aspectRatioMode)
    {
        m_dataPtr->m_aspectRatioMode = aspectRatioMode;
        update();
    }
}

QSize FramedEntity::minimumSizeHint() const
{
    return m_dataPtr->m_sizeOfFrame;
}

int FramedEntity::heightForWidth(int w) const {
    return w * m_dataPtr->m_sizeOfFrame.width()/m_dataPtr->m_sizeOfFrame.height();
}

bool FramedEntity::hasHeightForWidth() const {
    return m_dataPtr->m_pixmap != nullptr && !m_dataPtr->m_sizeOfFrame.isNull() && !m_dataPtr->m_sizeOfFrame.isEmpty();
}

FramedEntity *FramedEntity::clone() const {
    return new FramedEntity(*m_dataPtr, parentWidget());
}

QSize FramedEntity::sizeHint() const {
    return sizeOfFrame();
}

Qt::AspectRatioMode FramedEntity::aspectRatioMode() const {
    return m_dataPtr->m_aspectRatioMode;
}


void FramedEntity::showEvent(QShowEvent *e)
{
    EntityWidget::showEvent(e);
    setPlaying(autoplay());
}


void FramedEntity::initPainter(QPainter *p) const
{
    EntityWidget::initPainter(p);
    auto hints = QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing;
    p->setRenderHints(hints);
}
