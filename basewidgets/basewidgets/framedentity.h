#pragma once

#include "basewidgets_global.h"
#include "entitywidget.h"
//#include "util.h"
#include <QPainter>

struct FramedEntityData;

class BASEWIDGETS_DLL FramedEntity : public EntityWidget
{
    Q_OBJECT

    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)
    Q_PROPERTY(int frameIndex READ frameIndex WRITE setFrameIndex NOTIFY frameIndexChanged)
    Q_PROPERTY(QSize sizeOfFrame READ sizeOfFrame WRITE setSizeOfFrame NOTIFY sizeOfFrameChanged)
    Q_PROPERTY(int widthOfFrame READ widthOfFrame WRITE setWidthOfFrame STORED false DESIGNABLE false)
    Q_PROPERTY(int heightOfFrame READ heightOfFrame WRITE setHeightOfFrame STORED false DESIGNABLE false)
    Q_PROPERTY(QSize sizeInFrames READ sizeInFrames WRITE setSizeInFrames NOTIFY sizeInFramesChanged)
    Q_PROPERTY(int cols READ cols WRITE setCols STORED false DESIGNABLE false)
    Q_PROPERTY(int rows READ rows WRITE setRows STORED false DESIGNABLE false)
    Q_PROPERTY(int maxFrameIndex READ maxFrameIndex WRITE setMaxFrameIndex RESET unsetMaxFrameIndex)
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(uint loops READ loops WRITE setLoops NOTIFY loopsChanged)
    Q_PROPERTY(bool autoplay READ autoplay WRITE setAutoplay)
    Q_PROPERTY(bool playing READ isPlaying WRITE setPlaying DESIGNABLE false)
    Q_PROPERTY(Qt::AspectRatioMode aspectRatioMode READ aspectRatioMode WRITE setAspectRatioMode)

    static const uint infiniteLoop = -1;
private:
    FramedEntityData* m_dataPtr;
    QTimer *m_timer;

    FramedEntity(const FramedEntityData &data, QWidget* parent=0);
    void registerEvents();

public:
    Q_INVOKABLE FramedEntity(QWidget* parent=0);
    FramedEntity(const QPixmap* pixmap, QWidget* parent=0);
    virtual ~FramedEntity();
    virtual void init() override;

    const QPixmap& pixmap() const;
    bool pixmapValid() const;
    int frameIndex() const;

    virtual QSize sizeHint() const override;
    QSize sizeOfFrame() const;
    QSize sizeInFrames() const;

    inline int widthOfFrame()  const;
    inline int heightOfFrame() const;
    inline int cols() const;
    inline int rows() const;
    int maxFrameIndex() const;
    int fps() const;
    uint loops() const;
    bool autoplay() const;

    bool isPlaying() const;
    uint loopsLeft() const;

    virtual QSize minimumSizeHint() const override;
    virtual int heightForWidth(int) const override;
    virtual bool hasHeightForWidth() const override;

    FramedEntity* clone() const;
    void setSizeOfFrame(const QSize& sizeOfFrame);
    inline void setSizeOfFrame(int w, int h);
    inline void setWidthOfFrame(int w);
    inline void setHeightOfFrame(int h);

    void setSizeInFrames(const QSize& sizeInFrames);
    inline void setSizeInFrames(int cols, int rows);
    inline void setCols(int cols);
    inline void setRows(int rows);

    void setMaxFrameIndex(int maxFramesIndex);
    void unsetMaxFrameIndex();
    void adjustProperties();
    Qt::AspectRatioMode aspectRatioMode() const;

    void setFps(int fps);
    void setAutoplay(bool autoplay);
    void setLoops(uint loops);
    void setAspectRatioMode(Qt::AspectRatioMode aspectRatioMode);

public slots:
    virtual void setVisible(bool visible) override;

    void setPixmap(const QPixmap& pixmap);
    void setFrameIndex(int frameIndex);

    void nextFrame();
    void prevFrame();
    void setPlaying(bool playing = true);
    void stop();
    void togglePlaying();
    void pause();
    void resume();
    void togglePause();
signals:
    void pixmapChanged(const QPixmap& pixmap);
    void frameIndexChanged(int frameIndex);
    void sizeOfFrameChanged(QSize frameSize);
    void sizeInFramesChanged(QSize sizeInFrames);
    void fpsChanged(int fps);

    void started();
    void finished();
    void paused();
    void resumed();
    void loopsChanged(uint loops);

protected:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void showEvent(QShowEvent *e) override;

    virtual void initPainter(QPainter *p) const override;
};

inline int FramedEntity::widthOfFrame() const {
    return sizeOfFrame().width();
}

inline int FramedEntity::heightOfFrame() const {
    return sizeOfFrame().height();
}

inline void FramedEntity::setSizeOfFrame(int w, int h) {
    setSizeOfFrame(QSize(w, h));
}

inline void FramedEntity::setWidthOfFrame(int w) {
    setSizeOfFrame(w, sizeOfFrame().height());
}

inline int FramedEntity::cols() const {
    return sizeInFrames().width();
}

inline int FramedEntity::rows() const {
    return sizeInFrames().height();
}

inline void FramedEntity::setHeightOfFrame(int h) {
    setSizeOfFrame(sizeOfFrame().width(), h);
}


inline void FramedEntity::setSizeInFrames(int cols, int rows) {
    setSizeInFrames(QSize(cols, rows));
}

inline void FramedEntity::setCols(int cols) {
    setSizeInFrames(cols, rows());
}

inline void FramedEntity::setRows(int rows) {
    setSizeInFrames(cols(), rows);
}

